#include "scheduler.h"


//Provided by task_switch.s
extern void save_current_task(volatile struct syscall_regs* current_task);
extern void init_task_switch(volatile struct syscall_regs* current_task); //__attribute__((noreturn));
extern void resume_task(volatile struct syscall_regs* current_task);

#define STACK_ALIGNMENT 16     ///< In bytes
#define ROUND_ROBIN_QUANTUM 25 ///< In milliseconds

//The Process Control Block
typedef struct process_control_t pcb_t[MAX_PROCESS];

static pcb_t pcb;

//Define one run queue for each priority level
static vector_type(pid_t) run_queues[PRIORITY_LEVELS];

static volatile bool started = false;

static volatile size_t rr_quantum = 0;

static volatile size_t current_pid;
static volatile size_t next_pid = 0;

static size_t gc_pid = 0;
static size_t idle_pid = 0;
static size_t init_pid = 0;
static size_t post_init_pid = 0;

static vector_type(pid_t) run_queue(const size_t priority) {
    return run_queues[priority - MIN_PRIORITY];
}

static void idle_task(void) {
    kprintf("idle_task()\n");
    while(true) {
        asm volatile("hlt");

        kprintf("idle_task called with current_pid: %u\n", current_pid);

        //If we go out of 'hlt', there have been an IRQ
        //There is probably someone ready, let's yield
        scheduler_yield();
    }
}

const size_t virtual_paging_start = 0u;
const size_t physical_memory_pages = 1 + 1024; // page directory + 1024 page tables

static void gc_task(void) {
    kprintf("gc_task()\n");
    while(true) {
        //Wait until there is something to do
        scheduler_block_process(scheduler_get_pid());

        //2. Clean up each killed process

        for(size_t i = 0u; i < MAX_PROCESS; ++i) {
            struct process_control_t *const process = &pcb[i];

            if(process->state == PROCESS_KILLED) {
                struct process_t *const desc = &process->process;
                //const pid_t prev_pid = desc->pid;

                // 0. Notify parent if still waiting
                const pid_t ppid = desc->ppid;
                for(size_t j = 0u; j < MAX_PROCESS; ++j) {
                    struct process_control_t *const parent_process = &pcb[j];

                    if(parent_process->process.pid == ppid && parent_process->state == PROCESS_WAITING) {
                        scheduler_unblock_process(parent_process->process.pid);
                    }
                }

                // 1. Release physical memory of page tables (if not system task)

                if(!desc->system) {
                    global_phys_allocator_free_page((void*)desc->physical_cr3);
                }

                // 2. Release physical stacks (if dynamically allocated)

                if(desc->physical_kernel_stack) {
                    global_phys_allocator_free_pages((void*)desc->physical_kernel_stack, kernel_stack_size / PAGE_SIZE);
                }

                if(desc->physical_user_stack) {
                    global_phys_allocator_free_pages((void*)desc->physical_user_stack, user_stack_size / PAGE_SIZE);
                }

                // kernel processes can either use dynamic memory or static memory

                if(desc->system) {
                    if(((size_t)desc->user_stack) >= virtual_paging_start + (physical_memory_pages * PAGE_SIZE)) {
                        kfree(desc->user_stack);
                    }

                    if(((size_t)desc->kernel_stack) >= virtual_paging_start + (physical_memory_pages * PAGE_SIZE)) {
                        kfree(desc->kernel_stack);
                    }
                }

                // 3. Release segment's physical memory

                for(size_t j = 0u; j < size(desc->segments); ++j) {
                    struct segment_t *const segment = at(desc->segments, j);

                    global_phys_allocator_free_pages((void*)segment->physical, segment->size / PAGE_SIZE);
                }
                clear(desc->segments);

                // 4. Release virtual kernel stack

                if(desc->virtual_kernel_stack) {
                    global_phys_allocator_free_pages((void*)desc->virtual_kernel_stack, kernel_stack_size / PAGE_SIZE);
                    //TODO: double check that the kernel virtual stack for a process should be unmapped from the kernel address space and not some other address space
                    unmap_pages_in_kernel_addr((void*)desc->virtual_kernel_stack, kernel_stack_size / PAGE_SIZE);
                }

                // 5. Remove process from run queue

                {
                    // Move only write to the list with a lock
                    struct int_lock queue_lock;
                    int_lock(&queue_lock);

                    for(size_t index = 0; index < size(run_queue(desc->priority)); ++index) {
                        if((*(pid_t*)at(run_queue(desc->priority), index)) == desc->pid) {
                            erase(run_queue(desc->priority), index);
                            break;
                        }
                    }

                    int_unlock(&queue_lock);
                }

                // 6. Clean process

                desc->pid = 0;
                desc->ppid = 0;
                desc->system = false;
                desc->physical_cr3 = 0;
                desc->physical_user_stack = 0;
                desc->physical_kernel_stack = 0;
                desc->virtual_kernel_stack = 0;
                desc->paging_size = 0;
                desc->context = NULL;
                desc->brk_start = desc->brk_end = 0;

                // 7. Release the PCB slot
                process->state = PROCESS_EMPTY;
            }
        }
    }
}

static vector_type(void(*)(void)) init_tasks;

static void post_init_task(void) {
    for(int32_t i = 0; i < 10; ++i) {
        kprintf("post_init_task()\n");
    }
}

static void init_task(void) {
    for(int32_t i = 0; i < 10; ++i) {
        kprintf("init_task()\n");
    }
}

#define likely(x)    __builtin_expect (!!(x), 1)
#define unlikely(x)  __builtin_expect (!!(x), 0)

void suspend_kernel(void) {
    asm volatile("cli; hlt");
    __builtin_unreachable();
}

static pid_t get_free_pid(void) {
    kprintf("get_free_pid()\n");
    // Normally, the next pid should be empty
    if(likely(pcb[next_pid].state == PROCESS_EMPTY)) {
        size_t pid = next_pid;
        next_pid = (next_pid + 1) % MAX_PROCESS;
        return pid;
    }

    // If the next pid is not available, iterate until one is empty

    size_t pid = (next_pid + 1) % MAX_PROCESS;
    size_t i = 0;

    while(pcb[pid].state != PROCESS_EMPTY && i < MAX_PROCESS) {
        pid = (pid + 1) % MAX_PROCESS;
        ++i;
    }

    // Make sure there was one free
    if(unlikely(i == MAX_PROCESS)) {
        kprintf("Ran out of processes");
        suspend_kernel();
    }

    // Set the next pid
    next_pid = (pid + 1) % MAX_PROCESS;

    return pid;
}

static struct process_t* scheduler_create_new_process(void) {
    const pid_t pid = get_free_pid();

    struct process_control_t *const process = &pcb[pid];

    process->process.system = false;
    process->process.pid = pid;
    process->process.ppid = current_pid;
    process->process.priority = DEFAULT_PRIORITY;
    process->state = PROCESS_NEW;

    process->process.brk_start = 0;
    process->process.brk_end = 0;

    process->process.wait.pid = pid;
    process->process.wait.next = NULL;

    return &process->process;
}

static void queue_process(const pid_t pid) {
    kassert_message_void(pid < MAX_PROCESS, "pid out of bounds");

    struct process_control_t *const process = &pcb[pid];

    kassert_message_void(process->process.priority <= MAX_PRIORITY, "Invalid priority");
    kassert_message_void(process->process.priority >= MIN_PRIORITY, "Invalid priority");

    process->state = PROCESS_READY;

    // Move only write to the list with a lock
    struct int_lock queue_lock;
    int_lock(&queue_lock);
    push_back(run_queue(process->process.priority), &pid);
    int_unlock(&queue_lock);
}


static void create_idle_task(void) {
    struct process_t *const idle_process = scheduler_create_kernel_task("idle", zeroed_out_kmalloc(4096), zeroed_out_kmalloc(4096), &idle_task);

    idle_process->ppid = 0;
    idle_process->priority = MIN_PRIORITY;

    idle_pid = idle_process->pid;

    kprintf("idle_process->pid: %u\n", idle_process->pid);

    scheduler_queue_system_process(idle_process->pid);
}

static void create_init_tasks(void) {
    struct process_t *const init_process = scheduler_create_kernel_task("init", zeroed_out_kmalloc(4096), zeroed_out_kmalloc(4096), &init_task);

    init_process->ppid = 0;
    init_process->priority = MIN_PRIORITY + 1;

    init_pid = init_process->pid;

    scheduler_queue_system_process(init_process->pid);
}

static void create_gc_task(void) {
    struct process_t *const gc_process = scheduler_create_kernel_task("gc", zeroed_out_kmalloc(4096), zeroed_out_kmalloc(4096), &gc_task);

    gc_process->ppid = 0;
    gc_process->priority = MIN_PRIORITY + 1;

    gc_pid = gc_process->pid;

    scheduler_queue_system_process(gc_process->pid);
}

static void create_post_init_task(void) {
    struct process_t *const post_init_process = scheduler_create_kernel_task("post_init", zeroed_out_kmalloc(4096), zeroed_out_kmalloc(4096), &post_init_task);

    post_init_process->ppid = 0;
    post_init_process->priority = MAX_PRIORITY;

    post_init_pid = post_init_process->pid;

    scheduler_queue_system_process(post_init_process->pid);
}

static void switch_to_process_with_lock(const size_t new_pid) {
    kprintf("switch_to_process_with_lock()\n");
    pid_t old_pid = current_pid;

    // It is possible that preemption occured and that the process is already
    // running, in which case, there is no need to do anything
    if(old_pid == new_pid) {
        return;
    }

    current_pid = new_pid;
    kprintf("current_pid: %u, old_pid: %u, new_pid: %u\n", current_pid, old_pid, new_pid);

    struct process_control_t *const process = &pcb[new_pid];
    process->state = PROCESS_RUNNING;

    tss_entry.esp0 = process->process.kernel_esp;

    kprintf("old_pid: %u, current_pid: %u\n", old_pid, current_pid);

    save_current_task(pcb[old_pid].process.context);
    resume_task(pcb[current_pid].process.context);
    kprintf("task_switch done\n");
}

static void switch_to_process(const size_t new_pid) {
    struct int_lock lock;
    int_lock(&lock);

    switch_to_process_with_lock(new_pid);

    int_unlock(&lock);
}

/*!
 * \brief Select the next process to run.
 *
 * This function assume that an int_lock is already owned.
 */
static size_t select_next_process_with_lock(void) {
    kprintf("select_next_process_with_lock()\n");
    size_t current_priority = pcb[current_pid].process.priority;

    //1. Run a process of higher priority, if any
    for(size_t p = MAX_PRIORITY; p > current_priority; --p) {
        for(size_t j = 0u; j < size(run_queue(p)); ++j) {
            pid_t pid = *(pid_t*)at(run_queue(p), j);
            if(pcb[pid].state == PROCESS_READY || pcb[pid].state == PROCESS_RUNNING) {
                kprintf("p: %u, pid: %u\n", p, pid);
                return pid;
            }
        }
    }

    //2. Run the next process of the same priority

    {
        vector_type(pid_t) current_run_queue = run_queue(current_priority);

        size_t next_index = 0;
        for(size_t i = 0; i < size(current_run_queue); ++i) {
            if((*(pid_t*)at(current_run_queue, i)) == current_pid) {
                next_index = (i + 1) % size(current_run_queue);
                break;
            }
        }

        for(size_t i = 0; i < size(current_run_queue); ++i) {
            size_t index = (next_index + i) % size(current_run_queue);
            pid_t pid = *(pid_t*)at(current_run_queue, index);

            if(pcb[pid].state == PROCESS_READY || pcb[pid].state == PROCESS_RUNNING) {
                kprintf("step 2 return\n");
                if(pcb[pid].state == PROCESS_READY) {
                    kputs("PROCESS_READY");
                } else {
                    kassert(pcb[pid].state == PROCESS_RUNNING, 0u);
                    kputs("PROCESS_RUNNING");
                }
                return pid;
            }
        }
    }

    kassert_message(current_priority > 0, "The idle task should always be ready", 0u);

    //3. Run a process of lower priority

    for(size_t p = current_priority - 1; p >= MIN_PRIORITY; --p) {
        for(size_t j = 0u; j < size(run_queue(p)); ++j) {
            pid_t pid = *(pid_t*)at(run_queue(p), j);

            if(pcb[pid].state == PROCESS_READY || pcb[pid].state == PROCESS_RUNNING) {
                kprintf("step 3 return\n");
                return pid;
            }
        }
    }

    //thor_unreachable("No process is READY");
    kassert_message(false, "No process is READY", 0u);
}

static size_t select_next_process(void) {
    struct int_lock lock;
    int_lock(&lock);

    const size_t ret_pid = select_next_process_with_lock();

    int_unlock(&lock);

    return ret_pid;
}

static size_t page_align(const size_t addr) {
    return (addr / PAGE_SIZE) * PAGE_SIZE;
}

static size_t ceil_divide(const size_t base, const size_t divisor) {
    return (base + divisor - 1) / divisor;
}

static size_t pages(const size_t size) {
    return ceil_divide(size, PAGE_SIZE);
}
static bool is_page_aligned(const uint32_t mem_addr) {
    return mem_addr % PAGE_SIZE == 0;
}

static bool allocate_user_memory(struct process_t *const process, const size_t address, const size_t size, size_t *const ref) {

}

static void clear_physical_memory(size_t memory, size_t pages) {

}

static bool create_paging(char* buffer, struct process_t *const process) {

}

static void init_context(struct process_t *const process, const char *const buffer, const struct string file, const vector_type(struct string) params) {

}


void scheduler_init(void) {
    kprintf("scheduler_init()\n");
    //Create all the kernel tasks
    for(uint32_t i = 0u; i < PRIORITY_LEVELS; ++i) {
        run_queues[i] = create_vector(sizeof(pid_t));
    }

    init_tasks = create_vector(sizeof(void(*)(void)));

    create_idle_task();
    create_init_tasks();
    create_gc_task();
    create_post_init_task();
}

void scheduler_start(void) {
    kprintf("scheduler_start()\n");
    // TODO The current_pid should be set dynamically to the task in the list
    // with highest priority

    //Run the post init task by default (maximum priority)
    current_pid = post_init_pid;
    pcb[current_pid].state = PROCESS_RUNNING;

    started = true;

    kprintf("Scheduler started with current_pid: %u\n", current_pid);

    kputs("right before init_task_switch");
    init_task_switch(pcb[current_pid].process.context);
    kputs("init_task_switch ended");
}

bool scheduler_is_started(void) {
    return started;
}

//TODO: load from ramdisk
struct OPTIONAL_NAME(pid_t) scheduler_exec(struct string file, const vector_type(struct string) params) {
    struct OPTIONAL_NAME(uint32_t) file_entry_point = parse_elf_file(file.data);

    if(!file_entry_point.has_value) {
        return (struct OPTIONAL_NAME(pid_t)){0u, false};
    }

    const uint32_t entry_point_address = file_entry_point.data;
    void (*entry_start)(void) = (void (*)(void))(entry_point_address);

    struct process_t *const entry_point_process = scheduler_create_kernel_task("test_program", zeroed_out_kmalloc(4096), zeroed_out_kmalloc(4096), entry_start);
    entry_point_process->ppid = 0;
    entry_point_process->priority = MIN_PRIORITY;

    scheduler_queue_system_process(entry_point_process->pid);

    return (struct OPTIONAL_NAME(pid_t)) {entry_point_process->pid, true};
}

void scheduler_sbrk(const size_t inc) {

}

void scheduler_await_termination(const pid_t pid) {

}

void scheduler_kill_current_process(void) {
    const current_current_pid = current_pid;
    kprintf("scheduler_kill_current_process called\n");
    {
        struct int_lock lock;
        int_lock(&lock);

        // The process is now considered killed
        pcb[current_pid].state = PROCESS_KILLED;
        kprintf("Process %u killed\n", current_pid);

        //Notify parent if waiting
        const pid_t ppid = pcb[current_pid].process.ppid;
        for(size_t i = 0u; i < MAX_PROCESS; ++i) {
            struct process_control_t *const process = &pcb[i];

            if(process->process.pid == ppid && process->state == PROCESS_WAITING) {
                kprintf("scheduler_unblock_process(process->process.pid);: %u\n", process->process.pid);
                scheduler_unblock_process(process->process.pid);
            }
        }

        //The GC thread will clean the resources eventually
        if(pcb[gc_pid].state == PROCESS_BLOCKED) {
            kprintf("scheduler_unblock_process(gc_pid);\n");
            scheduler_unblock_process(gc_pid);
        }

        int_unlock(&lock);
    }

    //Run another process
    scheduler_reschedule();

    if(pcb[current_current_pid].state == PROCESS_KILLED) {
        kprintf("PROCESS_KILLED: %u\n", current_current_pid);
    }

    //thor_unreachable("A killed process has been run!");
    //kassert_message_void(false, "A killed process has been run!");
    kprintf("A killed process has been run!\n");
    __builtin_unreachable();
}

void scheduler_tick(void) {
    if(!started) {
        //kprintf("!started\n");
        return;
    }

    kprintf("scheduler_tick()\n");

    // Update sleep timeouts
    for(size_t i = 0u; i < MAX_PROCESS; ++i) {
        struct process_control_t *const process = &pcb[i];

        if(process->state == PROCESS_SLEEPING || process->state == PROCESS_BLOCKED_TIMEOUT) {
            --process->sleep_timeout;

            if(process->sleep_timeout == 0) {
                kputs("if(process->sleep_timeout == 0)");
                process->state = PROCESS_READY;
            }
        }
    }

    struct process_control_t *const process = &pcb[current_pid];

    kprintf("current_pid: %u\n", current_pid);

    if(process->rounds == rr_quantum) {
        kprintf("if(process->rounds == rr_quantum) with process->rounds: %u, and rr_quantum: %u\n", process->rounds, rr_quantum);
        process->rounds = 0;

        const enum process_state previous_state = process->state;

        // Change to Ready if it was not blocked
        // If it was blocked, we still prempt and it will end up in reschedule
        // later but with a full time quanta
        if(previous_state != PROCESS_BLOCKED && previous_state != PROCESS_BLOCKED_TIMEOUT && previous_state != PROCESS_KILLED) {
            kputs("if(previous_state != PROCESS_BLOCKED && previous_state != PROCESS_BLOCKED_TIMEOUT) {");
            process->state = PROCESS_READY;
        }

        const size_t pid = select_next_process();
        kprintf("next process pid: %u, current_pid: %u\n", pid, current_pid);

        //If it is the same, no need to go to the switching process
        if(pid == current_pid) {
            process->state = previous_state;
            return;
        }

        switch_to_process(pid);
    } else {
        kprintf("++process->rounds;\n");
        ++process->rounds;
    }

    //At this point we just have to return to the current process
    kprintf("//At this point we just have to return to the current process\n");
}

void scheduler_yield(void) {
    kprintf("scheduler_yield()\n");
    kassert_message_void(started, "No interest in yielding before start");
    kassert_message_void(pcb[current_pid].state == PROCESS_RUNNING, "Can only yield() running processes");

    pcb[current_pid].state = PROCESS_READY;

    struct int_lock lock;
    int_lock(&lock);

    const size_t pid = select_next_process_with_lock();

    kprintf("current_pid: %u, pid: %u\n", current_pid, pid);

    if(pid != current_pid) {
        kprintf("pid != current_pid\n");
        switch_to_process_with_lock(pid);
    } else {
        kprintf("pid == current_pid\n");
        pcb[current_pid].state = PROCESS_RUNNING;
    }

    int_unlock(&lock);
}

void scheduler_reschedule(void) {
    kprintf("scheduler_reschedule()\n");
    kassert_message_void(started, "No interest in rescheduling before start");

    struct process_control_t *const process = &pcb[current_pid];

    //The process just got blocked or put to sleep, choose another one
    if(process->state != PROCESS_RUNNING) {
        kprintf("process->state != PROCESS_RUNNING\n");
        struct int_lock lock;
        int_lock(&lock);

        const size_t index = select_next_process_with_lock();

        kprintf("current_pid: %u, index: %u\n", current_pid, index);

        switch_to_process_with_lock(index);

        kprintf("scheduler_reschedule after switch with current_pid: %u\n", current_pid);

        int_unlock(&lock);
    }

    //At this point we just have to return to the current process
}

pid_t scheduler_get_pid(void) {
    return current_pid;
}

struct process_t* scheduler_get_process(const pid_t pid) {
    kassert_message(pid < MAX_PROCESS, "pid out of bounds", NULL);

    return &pcb[pid].process;
}

enum process_state scheduler_get_process_state(const pid_t pid) {
    kassert_message(pid < MAX_PROCESS, "pid out of bounds", PROCESS_ERROR);

    return pcb[pid].state;
}

void scheduler_block_process_light(const pid_t pid) {
    kprintf("scheduler_block_process_light(pid: %u)\n", pid);
    kassert_message_void(pid < MAX_PROCESS, "pid out of bounds");

    pcb[pid].state = PROCESS_BLOCKED;
}

void scheduler_block_process_timeout_light(const pid_t pid, const size_t ms) {
    kprintf("scheduler_block_process_timeout_light(pid: %u, ms: %u)\n", pid, ms);
    kassert_message_void(pid < MAX_PROCESS, "pid out of bounds");

    // Compute the amount of ticks to sleep
    size_t sleep_ticks = ms * (timer_get_timer_frequency() / 1000);
    sleep_ticks = !sleep_ticks ? 1 : sleep_ticks;

    // Put the process to sleep
    pcb[pid].sleep_timeout = sleep_ticks;

    pcb[pid].state = PROCESS_BLOCKED_TIMEOUT;
}

void scheduler_block_process(const pid_t pid) {
    kprintf("scheduler_block_process(pid: %u)\n", pid);
    kassert_message_void(scheduler_is_started(), "The scheduler is not started");
    kassert_message_void(pid < MAX_PROCESS, "pid out of bounds");
    kassert_message_void(pid != idle_pid, "No reason to block the idle task");

    pcb[pid].state = PROCESS_BLOCKED;

    scheduler_reschedule();
}

void scheduler_unblock_process(const pid_t pid) {
    kprintf("scheduler_unblock_process(pid: %u)\n", pid);
    kassert_message_void(pid < MAX_PROCESS, "pid out of bounds");
    kassert_message_void(pid != idle_pid, "No reason to unblock the idle task");
    kassert_message_void(scheduler_is_started(), "The scheduler is not started");
    kassert_message_void(pcb[pid].state == PROCESS_BLOCKED || pcb[pid].state == PROCESS_BLOCKED_TIMEOUT || pcb[pid].state == PROCESS_WAITING, "Can only unblock BLOCKED/WAITING processes");

    pcb[pid].state = PROCESS_READY;
}

void scheduler_unblock_process_hint(const pid_t pid) {
    kprintf("scheduler_unblock_process_hint(pid: %u)\n", pid);
    kassert_message_void(pid < MAX_PROCESS, "pid out of bounds");
    kassert_message_void(pid != idle_pid, "No reason to unblock the idle task");
    kassert_message_void(scheduler_is_started(), "The scheduler is not started");

    const enum process_state state = pcb[pid].state;

    if(state != PROCESS_RUNNING) {
        pcb[pid].state = PROCESS_READY;
    }
}

void scheduler_sleep_ms(const size_t time) {
    scheduler_proc_sleep_ms(current_pid, time);
}

void scheduler_proc_sleep_ms(const pid_t pid, const size_t time) {
    kassert_message_void(pid < MAX_PROCESS, "pid out of bounds");
    kassert_message_void(pcb[pid].state == PROCESS_RUNNING, "Only RUNNING processes can sleep");

    // Compute the amount of ticks to sleep
    size_t sleep_ticks = time * (timer_get_timer_frequency() / 1000);
    sleep_ticks = !sleep_ticks ? 1 : sleep_ticks;

    // Put the process to sleep
    pcb[pid].sleep_timeout = sleep_ticks;
    pcb[pid].state = PROCESS_SLEEPING;

    // Run another process
    scheduler_reschedule();
}

struct process_t* scheduler_create_kernel_task(const char *const name, char *const user_stack, char *const kernel_stack, void (*const fun)(void)) {
    struct process_t *const process = scheduler_create_new_process();

    // TODO: figure out how we're supposed to set all of these stack members up
    process->system = true;
    process->physical_cr3 = V2P(get_kernel_page_directory()); // use the same `cr3` as the kernel since this is a kernel task and not a user task
    process->paging_size = 0;
    process->name = string_new(name);

    // Directly uses memory of the executable
    process->physical_user_stack = 0;
    process->physical_kernel_stack = 0;
    process->virtual_kernel_stack = 0;

    process->user_stack = user_stack;
    process->kernel_stack = kernel_stack;

    // TODO: check if this is correct
    char *const esp = user_stack+4096;
    process->context = zeroed_out_kmalloc(sizeof(struct syscall_regs));
    process->context->eflags = 0x200;
    process->context->eip = (uint32_t)fun;
    process->context->esp = (uint32_t)esp;
    process->context->ebp = (uint32_t)user_stack;
    process->context->cr3 = process->physical_cr3;
    process->kernel_esp = kernel_stack+4096;

    process->context->esp -= 4;
    *((uint32_t*)process->context->esp) = (uint32_t)scheduler_kill_current_process;

    return process;
}

struct process_t* scheduler_create_kernel_task_args(const char *const name, char *const user_stack, char *const kernel_stack, void (*const fun)(void*), void *const data) {
    kputs("scheduler_create_kernel_task_args is not implemented yet!");
}

void scheduler_queue_system_process(const pid_t pid) {
    kassert_message_void(pid < MAX_PROCESS, "pid out of bounds");

    struct process_control_t *const process = &pcb[pid];

    kassert_message_void(process->process.priority <= MAX_PRIORITY, "Process priority out of bounds");
    kassert_message_void(process->process.priority >= MIN_PRIORITY, "Process priority out of bounds");

    process->state = PROCESS_READY;

    push_back(run_queue(process->process.priority), &pid);
}

void scheduler_queue_async_init_task(void (*const fun)(void)) {
    const uint32_t fun_addr = (uint32_t)fun;
    push_back(init_tasks, &fun_addr);
}

void scheduler_frequency_updated(const uint64_t old_frequency, const uint64_t new_frequency) {
    kprintf("scheduler_frequency_updated()\n");
    // Cannot be interrupted during frequency update
    struct int_lock lock;
    int_lock(&lock);

    rr_quantum = ROUND_ROBIN_QUANTUM * (new_frequency / 1000); //((double)new_frequency / (double)1000);

    kprintf("rr_quantum: %u\n", rr_quantum);

    /*double*/ uint64_t ratio = old_frequency / new_frequency; //(double)new_frequency;

    for(size_t i = 0; i < MAX_PROCESS; ++i) {
        struct process_control_t *const process = &pcb[i];

        if(process->state == PROCESS_SLEEPING) {
            process->sleep_timeout *= ratio;
        }
    }

    int_unlock(&lock);
}

void scheduler_fault(void) {
    scheduler_kill_current_process();
}
