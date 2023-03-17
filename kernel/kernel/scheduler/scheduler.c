#include "scheduler.h"


extern void arch_enter_tasklet(volatile struct process_context*);
extern __attribute__((noreturn)) void arch_restore_context(volatile struct process_context*);
extern __attribute__((returns_twice)) void arch_save_context(volatile struct process_context*);

#define STACK_ALIGNMENT 16     ///< In bytes
#define ROUND_ROBIN_QUANTUM 25 ///< In milliseconds

//The Process Control Block
typedef struct process_control_t pcb_t[MAX_PROCESS];

static pcb_t pcb;

//Define one run queue for each priority level
static vector_type(pid_t) run_queues[PRIORITY_LEVELS];

static volatile bool started = false;

static volatile size_t rr_quantum = 0;

static volatile pid_t current_pid;
static volatile pid_t next_pid = 0;

static vector_type(void(*)(void)) init_tasks;

static pid_t idle_pid = 0;
static pid_t init_pid = 0;
static pid_t post_init_pid = 0;
//static pid_t gc_pid = 0;

#define likely(x)    __builtin_expect (!!(x), 1)
#define unlikely(x)  __builtin_expect (!!(x), 0)


static size_t ceil_divide(const size_t base, const size_t divisor) {
    return (base + divisor - 1) / divisor;
}
static bool is_page_aligned(const uint32_t mem_addr) {
    return mem_addr % PAGE_SIZE == 0;
}

static pid_t get_free_pid(void) {
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


void suspend_kernel(void) {
    asm volatile("cli; hlt");
    __builtin_unreachable();
}


void scheduler_kill_current_process(void) {
    kputs("scheduler_kill_current_process");
    const pid_t pid_to_kill = current_pid;

    {
        struct int_lock lock;
        int_lock(&lock);

        pcb[current_pid].state = PROCESS_KILLED;

#if 0
        //The GC thread will clean the resources eventually
        if(pcb[gc_pid].state == PROCESS_BLOCKED) {
            kprintf("scheduler_unblock_process(gc_pid);\n");
            scheduler_unblock_process(gc_pid);
        }

#endif
        int_unlock(&lock);
    }

    kputs("before scheduler_reschedule is called");

    //Run another process
    scheduler_reschedule();

    // a killed process is being run
    suspend_kernel();
}

void scheduler_fault(void) {
    scheduler_kill_current_process();
}

static vector_type(pid_t) run_queue(const size_t priority) {
    return run_queues[priority - MIN_PRIORITY];
}

static struct process_t* scheduler_create_new_process(void) {
    const pid_t pid = get_free_pid();

    struct process_control_t *const process = &pcb[pid];

    process->process.pid = pid;
    //process->process.ppid = current_pid;
    process->process.system = false;
    process->process.priority = DEFAULT_PRIORITY;
    process->state = PROCESS_NEW;

    return &process->process;
}


static pid_t select_next_process_with_lock(void) {
    kprintf("select_next_process_with_lock() with current_pid: %u\n", current_pid);
    const size_t current_priority = pcb[current_pid].process.priority;

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
                //kprintf("step 2 return\n");
                kprintf("new pid: %u\n", pid);
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

    kassert_message(false, "No process is READY", 0u);
}

static pid_t select_next_process(void) {
    //struct int_lock lock;
    //int_lock(&lock);
    disable_interrupts();

    const pid_t ret_pid = select_next_process_with_lock();

    enable_interrupts();
    //int_unlock(&lock);

    return ret_pid;
}


extern void load_page_directory(uint32_t *page_directory);

void scheduler_set_page_directory(const pid_t pid) {
    //struct int_lock lock;
    //int_lock(&lock);
    disable_interrupts();

    struct process_control_t *const process = &pcb[pid];

    const uint32_t page_directory_phys_addr = process->process.physical_cr3;

    kprintf("for pid: %u\n", pid);
    for(uint32_t i = 0; i < 1024; ++i) {
        kprintf("virtual_cr3[%u]: %X\n", i, ((uint32_t*)process->process.virtual_cr3)[i]);
    }

    //int_unlock(&lock);
    enable_interrupts();

    load_page_directory((uint32_t*)page_directory_phys_addr);
}


static void switch_to_process_with_lock(const pid_t new_pid) {
    kassert_void(scheduler_is_started());



    // TODO: double check that it is safe to only lock for this portion and to do the process switch and page directory change without locking
    //struct int_lock lock;
    //int_lock(&lock);
    disable_interrupts();

    const pid_t old_pid = current_pid;

    if(old_pid == new_pid) {
        kputs("(old_pid == new_pid)");
        return;
    }

    kprintf("old_pid: %u, new_pid: %u\n", old_pid, new_pid);

    arch_save_context(&pcb[old_pid].process.context);

    struct process_control_t *const process = &pcb[new_pid];
    process->state = PROCESS_RUNNING;

    current_pid = new_pid;

    //int_unlock(&lock);


    kputs("before setting page directory");

    scheduler_set_page_directory(new_pid);
    tss_entry.esp0 = process->process.context.esp;

    kputs("after setting page directory");

    kprintf("esp: %X\n", process->process.context.esp);
    kprintf("eip: %X\n", process->process.context.eip);


    //asm volatile ("" ::: "memory");

    enable_interrupts();

    arch_restore_context(&process->process.context);
}

static void switch_to_process(const size_t new_pid) {
    //struct int_lock lock;
    //int_lock(&lock);

    switch_to_process_with_lock(new_pid);

    //int_unlock(&lock);
}



void scheduler_queue_system_process(const pid_t pid) {
    kassert_message_void(pid < MAX_PROCESS, "pid out of bounds");

    struct process_control_t *const process = &pcb[pid];

    kassert_message_void(process->process.priority <= MAX_PRIORITY, "Process priority out of bounds");
    kassert_message_void(process->process.priority >= MIN_PRIORITY, "Process priority out of bounds");

    process->state = PROCESS_READY;

    push_back(run_queue(process->process.priority), &pid);
}

bool scheduler_is_started(void) {
    return started;
}

pid_t scheduler_get_pid(void) {
    return current_pid;
}

struct process_t* scheduler_get_process(const pid_t pid) {
    return &pcb[pid].process;
}

enum process_state scheduler_get_process_state(const pid_t pid) {
    return pcb[pid].state;
}

void scheduler_block_process(const pid_t pid) {
    kassert_void(scheduler_is_started());
    kassert_void(pid < MAX_PROCESS);
    kassert_void(pid != idle_pid);

    pcb[pid].state = PROCESS_BLOCKED;

    scheduler_reschedule();
}

void scheduler_unblock_process(const pid_t pid) {
    kassert_void(scheduler_is_started());
    kassert_void(pid < MAX_PROCESS);
    kassert_void(pid != idle_pid);
    kassert_message_void(pcb[pid].state == PROCESS_BLOCKED || pcb[pid].state == PROCESS_BLOCKED_TIMEOUT || pcb[pid].state == PROCESS_WAITING, "Can only unblock BLOCKED/WAITING processes");

    pcb[pid].state = PROCESS_READY;
}

void scheduler_unblock_process_hint(const pid_t pid) {
    kassert_void(scheduler_is_started());
    kassert_void(pid < MAX_PROCESS);
    kassert_void(pid != idle_pid);

    if(pcb[pid].state != PROCESS_RUNNING) {
        pcb[pid].state = PROCESS_READY;
    }
}

void scheduler_sleep_ms(const size_t time) {
    scheduler_proc_sleep_ms(current_pid, time);
}

void scheduler_proc_sleep_ms(const pid_t pid, const size_t time) {
    kassert_void(pid < MAX_PROCESS);
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

void scheduler_reschedule(void) {
    kputs("scheduler_reschedule");
    kassert_void(scheduler_is_started());

    //The process just got blocked or put to sleep, choose another one
    if(scheduler_get_process_state(current_pid) != PROCESS_RUNNING) {
        kputs("(scheduler_get_process_state(current_pid) != PROCESS_RUNNING)");
        struct int_lock lock;
        int_lock(&lock);

        const pid_t index = select_next_process_with_lock();

        int_unlock(&lock);

        switch_to_process_with_lock(index);
    }

    //At this point we just have to return to the current process
}

struct process_t* scheduler_create_kernel_task(const char *const name, void (*const fun)(void)) {
    return scheduler_create_kernel_task_args(name, fun, NULL);
}
struct process_t* scheduler_create_kernel_task_args(const char *const name, void (*const fun)(void), void *const data) {
    struct process_t *const new_process = scheduler_create_new_process();

    // TODO: support multi-page stacks
    char *const stack = kernel_virt_allocator_allocate_page();
    char *const physical_stack = global_phys_allocator_allocate_page();
    map_page_in_kernel_addr(stack, physical_stack, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);

    new_process->system = true;
    new_process->physical_cr3 = V2P(get_kernel_page_directory()); // use the same `cr3` as the kernel since this is a kernel task and not a user task
    new_process->virtual_cr3 = get_kernel_page_directory();
    new_process->physical_stack = physical_stack; // doesn't matter for kernel tasks
    new_process->name = string_new(name);

    volatile struct process_context *const new_process_context = &new_process->context;
    char *const esp = stack+STACK_SIZE;
    new_process_context->ebp = (uint32_t)stack;
    new_process_context->eip = (uint32_t)&arch_enter_tasklet;
    new_process_context->esp = esp;

    new_process_context-> esp -= sizeof(uint32_t);
    *((uint32_t*)new_process_context->esp) = (uint32_t)&scheduler_kill_current_process;
    new_process_context->esp -= sizeof(uint32_t);
    *((uint32_t*)new_process_context->esp) = (uint32_t)fun;
    new_process_context->esp -= sizeof(void*); // must equal `sizeof(uint32_t)` for now
    *((uint32_t*)new_process_context->esp) = data; // TODO: make it work with other sizes

    return new_process;
}

struct process_t* scheduler_load_process_in_new_address_space(const char *const program_filename) {
    kputs("scheduler_load_process_in_new_address_space");
    struct process_t *const new_process = scheduler_create_new_process();

    new_process->priority = DEFAULT_PRIORITY;

    const uint32_t new_page_directory_phys_addr = global_phys_allocator_allocate_page();
    clear_physical_page(new_page_directory_phys_addr);
    uint32_t *const new_page_directory_kernel_mapping = kernel_virt_allocator_allocate_page();
    map_page_in_kernel_addr(new_page_directory_kernel_mapping, new_page_directory_phys_addr, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    new_process->physical_cr3 = new_page_directory_phys_addr;
    new_process->virtual_cr3 = new_page_directory_kernel_mapping;

    const uint32_t process_stack = global_phys_allocator_allocate_page();
    new_process->physical_stack = process_stack;

    new_process->name = string_new(program_filename);

    kputs("before load_elf_file_in_process");
    load_elf_file_in_process(new_process, program_filename);
    kputs("before map_kernel_inside_user");
    map_kernel_inside_user(new_process);
    kputs("after map_kernel_inside_user");

    new_process->context.ebp = USER_STACK_TOP_ADDR-STACK_SIZE;
    new_process->context.esp = USER_STACK_TOP_ADDR;


    uint32_t temporary_esp_mapping = AcpiOsMapMemory(process_stack, STACK_SIZE);

    temporary_esp_mapping += STACK_SIZE;

    new_process->context.esp -= sizeof(uint32_t);
    temporary_esp_mapping -= sizeof(uint32_t);
    (*(uint32_t*)temporary_esp_mapping) = (uint32_t)&scheduler_kill_current_process;

    temporary_esp_mapping += sizeof(uint32_t);

    temporary_esp_mapping -= STACK_SIZE;

    AcpiOsUnmapMemory(temporary_esp_mapping, STACK_SIZE);


    scheduler_queue_system_process(new_process->pid);

    return new_process;
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

static void init_task(void) {
    for(int32_t i = 0; i < 10; ++i) {
        kprintf("init_task()\n");
    }
}


static void post_init_task(void) {
    for(int32_t i = 0; i < 10; ++i) {
        kprintf("post_init_task()\n");
    }
}

static void create_idle_task(void) {
    struct process_t *const idle_process = scheduler_create_kernel_task("idle", &idle_task);

    idle_process->priority = MIN_PRIORITY;

    idle_pid = idle_process->pid;

    scheduler_queue_system_process(idle_process->pid);
}

static void create_init_task(void) {
    struct process_t *const init_process = scheduler_create_kernel_task("init", &init_task);

    init_process->priority = DEFAULT_PRIORITY; //MIN_PRIORITY + 1;

    init_pid = init_process->pid;

    scheduler_queue_system_process(init_process->pid);
}

static void create_post_init_task(void) {
    struct process_t *const post_init_process = scheduler_create_kernel_task("post_init", &post_init_task);

    post_init_process->priority = MAX_PRIORITY;

    post_init_pid = post_init_process->pid;

    scheduler_queue_system_process(post_init_process->pid);
}


void scheduler_tick(void) {
    if(!scheduler_is_started()) return;

    // Update sleep timeouts
    for(size_t i = 0u; i < MAX_PROCESS; ++i) {
        struct process_control_t *const process = &pcb[i];

        if(process->state == PROCESS_SLEEPING || process->state == PROCESS_BLOCKED_TIMEOUT) {
            --process->sleep_timeout;

            if(process->sleep_timeout == 0) {
                process->state = PROCESS_READY;
            }
        }
    }

    struct process_control_t *const process = &pcb[current_pid];

    if(process->rounds == rr_quantum) {
        process->rounds = 0;

        const enum process_state previous_state = process->state;

        // Change to Ready if it was not blocked
        // If it was blocked, we still prempt and it will end up in reschedule
        // later but with a full time quanta
        if(previous_state != PROCESS_BLOCKED && previous_state != PROCESS_BLOCKED_TIMEOUT && previous_state != PROCESS_KILLED) {
            process->state = PROCESS_READY;
        }

        const size_t pid = select_next_process();

        //If it is the same, no need to go to the switching process
        if(pid == current_pid) {
            process->state = previous_state;
            return;
        }

        switch_to_process(pid);
    } else {
        ++process->rounds;
    }

    //At this point we just have to return to the current process
}

void scheduler_init(void) {
    //Create all the kernel tasks
    for(uint32_t i = 0u; i < PRIORITY_LEVELS; ++i) {
        run_queues[i] = create_vector(sizeof(pid_t));
    }

    init_tasks = create_vector(sizeof(void(*)(void)));

    create_idle_task();
    create_init_task();
    create_post_init_task();
    //gc_pid = (scheduler_tasklet_create(&gc_task, NULL))->pid;
}

void scheduler_start(void) {
    // TODO The current_pid should be set dynamically to the task in the list
    // with highest priority

    //Run the post init task by default (maximum priority)
    current_pid = post_init_pid;
    pcb[current_pid].state = PROCESS_RUNNING;

    started = true;

    arch_restore_context(&pcb[current_pid].process.context);
}

void scheduler_yield(void) {
    kputs("scheduler_yield");
    kassert_message_void(started, "No interest in yielding before start");
    kassert_message_void(pcb[current_pid].state == PROCESS_RUNNING, "Can only yield() running processes");

    pcb[current_pid].state = PROCESS_READY;

    {
        //struct int_lock lock;
        //int_lock(&lock);

        const size_t pid = select_next_process_with_lock();

        kprintf("current_pid: %u, pid: %u\n", current_pid, pid);

        if(pid != current_pid) {
            kprintf("pid != current_pid\n");
            switch_to_process_with_lock(pid);
        } else {
            kprintf("pid == current_pid\n");
            pcb[current_pid].state = PROCESS_RUNNING;
        }

        //int_unlock(&lock);
    }
}

void scheduler_frequency_updated(const uint64_t old_frequency, const uint64_t new_frequency) {
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



