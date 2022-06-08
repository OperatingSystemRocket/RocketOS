#include "scheduler.h"


//Provided by task_switch.s
extern void task_switch(size_t current, size_t next);
extern void init_task_switch(size_t current) __attribute__((noreturn));

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

static vector_type(pid_t) run_queue(size_t priority) {
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

                // 1. Release physical memory of PML4T (if not system task)

                if(!desc->system) {
                    global_free_page(USER_USE, (void*)desc->physical_cr3);
                }

                // 2. Release physical stacks (if dynamically allocated)

                if(desc->physical_kernel_stack) {
                    global_free_pages(USER_USE, (void*)desc->physical_kernel_stack, kernel_stack_size / PAGE_SIZE);
                }

                if(desc->physical_user_stack) {
                    global_free_pages(USER_USE, (void*)desc->physical_user_stack, user_stack_size / PAGE_SIZE);
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

                    global_free_pages(USER_USE, (void*)segment->physical, segment->size / PAGE_SIZE);
                }
                clear(desc->segments);

                // 4. Release virtual kernel stack

                if(desc->virtual_kernel_stack) {
                    global_free_pages(USER_USE, (void*)desc->virtual_kernel_stack, kernel_stack_size / PAGE_SIZE);
                    unmap_pages((void*)desc->virtual_kernel_stack, kernel_stack_size / PAGE_SIZE);
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
    kprintf("post_init_task()\n");
    for(size_t i = 0u; i < size(init_tasks); ++i) {
        uint32_t *const func_addr = at(init_tasks, i);
        void (*func)(void) = *func_addr;
        func();
    }

    scheduler_kill_current_process();
}

//TODO tsh should be configured somewhere
static void init_task(void) {
    kprintf("init_task\n");

    while(true) {
        kprintf("while(true) in init_task\n");

        vector_type(struct string) params = create_vector(sizeof(struct string));
        struct string first_str = string_new("foo");
        struct string second_str = string_new("bar");
        push_back(params, &first_str);
        push_back(params, &second_str);

        struct OPTIONAL_NAME(pid_t) pid = scheduler_exec(string_new("test_program"), params);

        if(!pid.has_value) {
            kprintf("!pid.has_value\n");
            return;
        }

        scheduler_await_termination(pid.data);
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
    if(likely(pcb[next_pid].state == PROCESS_EMPTY)){
        size_t pid = next_pid;
        next_pid = (next_pid + 1) % MAX_PROCESS;
        return pid;
    }

    // If the next pid is not available, iterate until one is empty

    size_t pid = (next_pid + 1) % MAX_PROCESS;
    size_t i = 0;

    while(pcb[pid].state != PROCESS_EMPTY && i < MAX_PROCESS){
        pid = (pid + 1) % MAX_PROCESS;
        ++i;
    }

    // Make sure there was one free
    if(unlikely(i == MAX_PROCESS)){
        kprintf("Ran out of processes");
        suspend_kernel();
    }

    // Set the next pid
    next_pid = (pid + 1) % MAX_PROCESS;

    return pid;
}

static struct process_t* new_process(void) {
    kprintf("new_process()\n");
    pid_t pid = get_free_pid();

    struct process_control_t *const process = &pcb[pid];

    process->process.system = false;
    process->process.pid = pid;
    process->process.ppid = current_pid;
    process->process.priority = DEFAULT_PRIORITY;
    process->state = PROCESS_NEW;
    process->process.tty = pcb[current_pid].process.tty;

    process->process.brk_start = 0;
    process->process.brk_end = 0;

    process->process.wait.pid = pid;
    process->process.wait.next = NULL;

    return &process->process;
}

static void queue_process(pid_t pid) {
    kprintf("queue_process called\n");
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

static void foo_func(void) {
    while(true) {
        kprintf("foo_func with current_pid: %u\n", current_pid);
        asm("hlt");
    }
}

static void create_idle_task(void) {
    kprintf("create_idle_task()\n");
    struct process_t *const idle_process = scheduler_create_kernel_task("idle", kmalloc(user_stack_size), kmalloc(kernel_stack_size), &idle_task);

    idle_process->ppid = 0;
    idle_process->priority = MIN_PRIORITY;

    scheduler_queue_system_process(idle_process->pid);

    idle_pid = idle_process->pid;
}

//TODO: actually implement this and rewrite the terminal and vga code, this is just a simple mock for now
size_t terminals_count(void) {
    kprintf("terminals_count()\n");
    return 1;
}

static void create_init_tasks(void) {
    kprintf("create_init_tasks()\n");
    for(size_t i = 0; i < terminals_count(); ++i){
        struct process_t *const init_process = scheduler_create_kernel_task("init", kmalloc(user_stack_size), kmalloc(kernel_stack_size), &init_task);

        init_process->ppid = 0;
        init_process->priority = MIN_PRIORITY + 1;

        init_process->tty = i;

        pid_t pid = init_process->pid;
        if(i == 0){
            init_pid = pid;
        }
/*
        char tty_name[512] = "/dev/tty";
        char integer_str_buf[512];
        kint_to_string(i, integer_str_buf, sizeof(integer_str_buf), 10, false);
        kstrncat(tty_name, integer_str_buf, sizeof(tty_name));

        struct string tty = string_new(tty_name);
*/
        scheduler_queue_system_process(pid);
    }
}

static void create_gc_task(void) {
    kprintf("create_gc_task()\n");
    struct process_t *const gc_process = scheduler_create_kernel_task("gc", kmalloc(user_stack_size), kmalloc(kernel_stack_size), &gc_task);

    gc_process->ppid = 0;
    gc_process->priority = MIN_PRIORITY + 1;

    scheduler_queue_system_process(gc_process->pid);

    gc_pid = gc_process->pid;
}

static void create_post_init_task(void) {
    kprintf("create_post_init_task()\n");
    struct process_t *const post_init_process = scheduler_create_kernel_task("post_init", kmalloc(user_stack_size), kmalloc(kernel_stack_size), &post_init_task);

    post_init_process->ppid = 0;
    post_init_process->priority = MAX_PRIORITY;

    post_init_pid = post_init_process->pid;

    scheduler_queue_system_process(post_init_pid);
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
    //current_pid = old_pid; //new_pid;
    kprintf("current_pid: %u, old_pid: %u, new_pid: %u\n", current_pid, old_pid, new_pid);

    struct process_control_t *const process = &pcb[new_pid];
    //struct process_control_t *const process = &pcb[old_pid]; //&pcb[new_pid];
    process->state = PROCESS_RUNNING;

    tss_entry.esp0 = process->process.kernel_esp;
    //gdt::tss().rsp0_low = process.process.kernel_esp & 0xFFFFFFFF;
    //gdt::tss().rsp0_high = process.process.kernel_esp >> 32;

    kprintf("old_pid: %u, current_pid: %u\n", old_pid, current_pid);

    //task_switch(old_pid, old_pid);
    task_switch(old_pid, current_pid);
    kprintf("task_switch done\n");
    //task_switch(old_pid, current_pid);
}

static void switch_to_process(const size_t new_pid) {
    kprintf("switch_to_process()\n");
    // This should never be interrupted
    struct int_lock l;
    int_lock(&l);

    switch_to_process_with_lock(new_pid);

    int_unlock(&l);
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
    kprintf("select_next_process()\n");
    // Cannot be interrupted
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

static size_t pages(const size_t size){
    return ceil_divide(size, PAGE_SIZE);
}
static bool is_page_aligned(const uint32_t mem_addr) {
    return mem_addr % PAGE_SIZE == 0;
}

static bool allocate_user_memory(struct process_t *const process, const size_t address, const size_t size, size_t *const ref) {
    kprintf("allocate_user_memory()\n");
    //1. Calculate some stuff
    const size_t first_page = page_align(address);
    const size_t left_padding = address - first_page;

    const size_t bytes = left_padding + size;
    const size_t pages_var = pages(bytes);

    //2. Get enough physical memory
    const uint32_t physical_memory = global_allocate_pages(USER_USE, pages_var);

    if(!physical_memory) {
        return false;
    }

    //3. Find a start of a page inside the physical memory

    const uint32_t aligned_physical_memory = is_page_aligned(physical_memory) ? physical_memory :
            (physical_memory / PAGE_SIZE + 1) * PAGE_SIZE;

    //4. Map physical allocated memory to the necessary virtual memory
    if(!user_map_pages(process, first_page, aligned_physical_memory, pages_var)) {
        return false;
    }

    *ref = physical_memory;

    kprintf("allocate_user_memory()\n");

    return true;
}

static void clear_physical_memory(size_t memory, size_t pages) {
    kprintf("clear_physical_memory()\n");
    struct physical_pointer phys_ptr = create_physical_pointer(memory, pages);

    uint32_t *const it = (uint32_t*)phys_ptr.virt;
    kmemset(it, 0, pages * PAGE_SIZE);
}

static bool create_paging(char* buffer, struct process_t *const process) {
    kprintf("create_paging\n");
    //1. Prepare page directory

    //Get memory for cr3
    process->physical_cr3 = global_allocate_page(USER_USE);
    process->paging_size = PAGE_SIZE;

    clear_physical_memory(process->physical_cr3, 1);

    //Map the kernel pages inside the user memory space
    map_kernel_inside_user(process);

    //2. Create all the other necessary structures

    //2.1 Allocate user stack
    allocate_user_memory(process, user_stack_start, user_stack_size, &process->physical_user_stack);

    //2.2 Allocate all user segments

    struct Elf32_Ehdr *const header = (struct Elf32_Ehdr*)buffer;
    struct Elf32_Phdr *const program_header_table = (struct Elf32_Phdr*)(buffer + header->e_phoff);

    for(size_t p = 0; p < header->e_phnum; ++p) {
        struct Elf32_Phdr *const p_header = &program_header_table[p];

        if(p_header->p_type == 1) {
            Elf32_Addr first_page = p_header->p_vaddr;
            Elf32_Addr left_padding = p_header->p_vaddr - first_page;

            uint32_t bytes = left_padding + p_header->p_memsz;

            //Make sure only complete pages are allocated
            if(bytes % PAGE_SIZE != 0) {
                bytes += PAGE_SIZE - (bytes % PAGE_SIZE);
            }

            uint32_t pages = bytes / PAGE_SIZE;

            struct segment_t segment;
            segment.size = bytes;

            allocate_user_memory(process, first_page, bytes, &segment.physical);

            //kprintf("create_paging\n");
            push_back(process->segments, &segment);

            //Copy the code into memory

            struct physical_pointer phys_ptr = create_physical_pointer(segment.physical, pages);

            auto memory_start = phys_ptr.virt + left_padding;

            //In the case of the BSS segment, the segment must be
            //filled with zero
            if(p_header->p_filesz != p_header->p_memsz) {
                kmemcpy(memory_start, buffer + p_header->p_offset, p_header->p_filesz);
                kmemset(memory_start + p_header->p_filesz, 0, p_header->p_memsz - p_header->p_filesz);
            } else {
                kmemcpy(memory_start, buffer + p_header->p_offset, p_header->p_memsz);
            }
        }
    }

    //2.3 Allocate kernel stack
    //TODO: STOP USING THIS ALLOCATOR FOR VIRTUAL MEMORY ON PROCESS CREATION OR GENERAL ADDRESS SPACE VIRTUAL MEMORY ALLOCATION, it should only be used for memory pools (like for OSI), use binary buddy (like with physical pages) instead
    void *const virtual_kernel_stack = osi_memory_allocator_allocate(get_default_virt_allocator(), kernel_stack_size / PAGE_SIZE);

    void *const physical_kernel_stack = global_allocate_pages(USER_USE, kernel_stack_size / PAGE_SIZE);

    //auto virtual_kernel_stack = virtual_allocator::allocate(kernel_stack_size / PAGE_SIZE);
    //auto physical_kernel_stack = physical_allocator::allocate(kernel_stack_size / PAGE_SIZE);


    if(!map_pages(virtual_kernel_stack, physical_kernel_stack, kernel_stack_size / PAGE_SIZE, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW)) {
        return false;
    }

    process->physical_kernel_stack = physical_kernel_stack;
    process->virtual_kernel_stack = virtual_kernel_stack;
    process->kernel_esp = virtual_kernel_stack + (user_stack_size - 8);

    //3. Clear stacks
    clear_physical_memory(process->physical_user_stack, user_stack_size / PAGE_SIZE);
    clear_physical_memory(process->physical_kernel_stack, kernel_stack_size / PAGE_SIZE);

    return true;
}

static void init_context(struct process_t *const process, const char *const buffer, const struct string file, const vector_type(struct string) params) {
    kprintf("init_context()\n");
    struct Elf32_Ehdr *const header = (const struct Elf32_Ehdr*)buffer;

    size_t pages = user_stack_size / PAGE_SIZE;

    struct physical_pointer phys_ptr = create_physical_pointer(process->physical_user_stack, pages);

    //1. Compute the size of the arguments on the stack

    //One pointer for each args
    size_t args_size = sizeof(size_t) * (1 + size(params));

    //Add the size of the default argument (+ terminating char)
    args_size += file.length + 1;

    //Add the size of all the other arguments (+ terminating char)
    for(size_t i = 0; i < size(params); ++i) {
        kprintf("args_size += ((const struct string*)at_const(params, i))->length + 1;\n");
        args_size += ((const struct string*)at_const(params, i))->length + 1;
    }

    //Align the size on 16 bytes
    if(args_size % 16 != 0){
        args_size = ((args_size / 16) + 1) * 16;
    }

    //2. Modify the stack to add the args

    uintptr_t esp = phys_ptr.virt + user_stack_size - 8;
    size_t arrays_start = user_esp - sizeof(size_t) * (1 + size(params));

    //Add pointer to each string
    size_t acc = 0;
    for(int64_t i = size(params) - 1; i >= 0; --i) {
        kprintf("Add pointer to each string: const struct string *const param = at_const(params, i);\n");
        const struct string *const param = at_const(params, i);

        acc += param->length;

        *((uint32_t*)esp) = arrays_start - acc;
        esp -= sizeof(size_t);

        ++acc;
    }

    //Add pointer to the default argument string
    *((uint32_t*)esp) = arrays_start - acc - file.length;
    esp -= sizeof(size_t);

    //Add the strings of the arguments
    for(int64_t i = size(params) - 1; i >= 0; --i) {
        kprintf("Add the strings of the arguments: const struct string *const param = at_const(params, i);\n");
        const struct string *const param = at_const(params, i);

        *((char*)(esp--)) = '\0';
        for(int64_t j = param->length - 1; j >= 0; --j){
            *((char*)(esp--)) = param->data[j];
        }
    }

    //Add the the string of the default argument
    *((char*)(esp--)) = '\0';
    for(int64_t i = file.length - 1; i >= 0; --i){
        *((char*)(esp--)) = file.data[i];
    }

    //3. Modify the stack to configure the context

    esp = phys_ptr.virt + user_stack_size - sizeof(struct syscall_regs) - 8 - args_size;
    struct syscall_regs *const regs = (struct syscall_regs*)esp;

    regs->esp = user_esp - sizeof(struct syscall_regs) - args_size; //Not sure about that
    regs->ebp = 0;
    regs->eip = header->e_entry;
    //((void(*)(void))(header->e_entry))();
    regs->cs = 0x20 + 3;
    regs->ds = 0x28 + 3;
    regs->eflags = 0x200;

    regs->edi = 1 + size(params); //argc
    regs->esi = user_esp - size(params) * sizeof(size_t); //argv

    process->context = (struct syscall_regs*)(user_esp - sizeof(struct syscall_regs) - args_size);
}

//Provided for task_switch.s

extern void print_place(void) {
    kprintf("print_place called with current_pid: %u\n", current_pid);
}

extern uint32_t get_context_address(const uint32_t old_pid, const uint32_t pid) {
    kprintf("get_context_address called with pid value: %u, old_pid: %u\n", pid, old_pid);
    return (uint32_t)&pcb[pid].process.context;
}

extern void print_reg(const uint32_t reg) {
    kprintf("print_reg called with reg: %X\n", reg);
}

extern uint32_t get_process_cr3(const size_t old_pid, const uint32_t pid) {
    kprintf("get_process_cr3 called with pid value: %u, old_pid: %u\n", pid, old_pid);
    return pcb[pid].process.physical_cr3;
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

    init_task_switch(current_pid);
    kprintf("init_task_switch ended\n");
}

bool scheduler_is_started(void) {
    return started;
}

//TODO: load from ramdisk
struct OPTIONAL_NAME(pid_t) scheduler_exec(struct string file, const vector_type(struct string) params) {
    kprintf("scheduler_exec called with size(param): %u\n", size(params));
    struct file_header *const file_header = get_file_header(file.data);
    if(!file_header) {
        kprintf("if(!file_header) {\n");
        return (struct OPTIONAL_NAME(pid_t)){0, false};
    }

    const size_t string_len = file_header->size - sizeof(struct file_header);
    if(file_header->size == 0 || string_len == 0) {
        return (struct OPTIONAL_NAME(pid_t)){0, false};
    }
    struct string content = string_new_with_len(file_header->file, string_len);

    char *const buffer = content.data;

    if(!is_valid_elf_sig((struct Elf32_Ehdr*)buffer)){
        return (struct OPTIONAL_NAME(pid_t)){0, false};
    }

    struct process_t *const process = new_process();
    process->priority = DEFAULT_PRIORITY;
    process->segments = create_vector(sizeof(struct segment_t));

    process->name = file;

    if(!create_paging(buffer, process)) {
        return (struct OPTIONAL_NAME(pid_t)){0, false};
    }

    process->brk_start = program_break;
    process->brk_end = program_break;

    init_context(process, buffer, file, params);

    queue_process(process->pid);

    destroy_string(&content);

    kprintf("scheduler_exec: process->pid: %u\n", process->pid);

    return (struct OPTIONAL_NAME(pid_t)){process->pid, true};
}

void scheduler_sbrk(const size_t inc) {
    kprintf("scheduler_sbrk called with inc: %u\n", inc);
    struct process_t *const process = &pcb[current_pid].process;

    const size_t size = (inc + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    const size_t pages = size / PAGE_SIZE;

    //Get some physical memory
    void *const physical = global_allocate_pages(USER_USE, pages);

    if(!physical){
        return;
    }

    size_t virtual_start = process->brk_end;

    //Map the memory inside the process memory space
    if(!user_map_pages(process, virtual_start, physical, pages)){
        global_free_pages(USER_USE, physical, pages);
        return;
    }

    const struct segment_t segment = {physical, pages};
    //kprintf("scheduler_sbrk\n");
    push_back(process->segments, &segment);

    process->brk_end += size;
}

void scheduler_await_termination(const pid_t pid) {
    kprintf("scheduler_await_termination called with pid: %u\n", pid);
    while(true) {
        {
            struct int_lock lock;
            int_lock(&lock);

            bool found = false;
            for(size_t i = 0u; i < MAX_PROCESS; ++i) {
                struct process_control_t *const process = &pcb[i];

                if(process->process.ppid == current_pid && process->process.pid == pid) {
                    if(process->state == PROCESS_KILLED || process->state == PROCESS_EMPTY) {
                        return;
                    }

                    found = true;
                    break;
                }
            }

            // The process may have already been cleaned, we can simply return
            if(!found) {
                return;
            }

            pcb[current_pid].state = PROCESS_WAITING;

            int_unlock(&lock);
        }

        // Reschedule is out of the critical section
        scheduler_reschedule();
        kprintf("scheduler_await_termination: rescheduled\n");
    }
}

void scheduler_kill_current_process(void) {
    kprintf("scheduler_kill_current_process called\n");
    {
        struct int_lock lock;
        int_lock(&lock);

        // The process is now considered killed
        pcb[current_pid].state = PROCESS_KILLED;

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

    //thor_unreachable("A killed process has been run!");
    //kassert_message_void(false, "A killed process has been run!");
    kprintf("A killed process has been run!\n");
    //__builtin_unreachable();
}

void scheduler_tick(void) {
    if(!started) {
        kprintf("!started\n");
        return;
    }

    kprintf("scheduler_tick()\n");

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

    kprintf("current_pid: %u\n", current_pid);

    if(process->rounds == rr_quantum) {
        kprintf("if(process->rounds == rr_quantum) with process->rounds: %u, and rr_quantum: %u\n", process->rounds, rr_quantum);
        process->rounds = 0;

        const enum process_state previous_state = process->state;

        // Change to Ready if it was not blocked
        // If it was blocked, we still prempt and it will end up in reschedule
        // later but with a full time quanta
        if(previous_state != PROCESS_BLOCKED && previous_state != PROCESS_BLOCKED_TIMEOUT){
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
    kassert_message_void(pid < MAX_PROCESS, "pid out of bounds");

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

    if(state != PROCESS_RUNNING){
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
    kprintf("scheduler_create_kernel_task(name: %s, user_stack: %p, kernel_stack: %p, fun: %p)\n", name, user_stack, kernel_stack, fun);
    struct process_t *const process = new_process();

    process->system = true;
    process->physical_cr3 = get_default_page_directory(); //paging::get_physical_pml4t(); // TODO: double check to see if this is correct
    process->paging_size = 0;
    assign_string(&process->name, name); //process->name = name;

    // Directly uses memory of the executable
    process->physical_user_stack = 0;
    process->physical_kernel_stack = 0;
    process->virtual_kernel_stack = 0;

    process->user_stack = user_stack;
    process->user_stack = kernel_stack;

    char* esp = &user_stack[user_stack_size - STACK_ALIGNMENT];
    esp -= sizeof(struct syscall_regs);

    process->context = (struct syscall_regs*)esp;

    process->context->eflags = 0x200;
    process->context->eip = (size_t)fun;
    process->context->esp = (size_t)esp;
    process->context->cs = 0x08; //gdt::LONG_SELECTOR;
    process->context->ds = 0x10; //gdt::DATA_SELECTOR;

    process->kernel_esp = (size_t)&kernel_stack[kernel_stack_size - STACK_ALIGNMENT];

    //kprintf("process->context: %p, process->context modulo STACK_ALIGNMENT: %u\n", process->context, ((size_t)process->context - sizeof(struct syscall_regs)) % STACK_ALIGNMENT);
    kassert_message(((size_t)process->context - sizeof(struct syscall_regs)) % STACK_ALIGNMENT == 0, "Process context must be correctly aligned", NULL);
    kassert_message((process->context->esp - sizeof(struct syscall_regs)) % STACK_ALIGNMENT == 0, "Process user stack must be correctly aligned", NULL);
    kassert_message(process->kernel_esp % STACK_ALIGNMENT == 0, "Process kernel stack must be correctly aligned", NULL);

    return process;
}

struct process_t* scheduler_create_kernel_task_args(const char *const name, char *const user_stack, char *const kernel_stack, void (*const fun)(void*), void *const data){
    kprintf("scheduler_create_kernel_task_args(name: %s, user_stack: %p, kernel_stack: %p, fun: %p, data: %p)\n", name, user_stack, kernel_stack, fun, data);
    struct process_t *const process = scheduler_create_kernel_task(name, user_stack, kernel_stack, (void(*)(void))fun);

    // rdi is the first register used for integers parameter passing
    process->context->edi = (size_t)data;

    return process;
}

void scheduler_queue_system_process(pid_t pid) {
    kprintf("scheduler_queue_system_process(pid: %u)\n", pid);
    kassert_message_void(pid < MAX_PROCESS, "pid out of bounds");

    struct process_control_t *const process = &pcb[pid];

    kassert_message_void(process->process.priority <= MAX_PRIORITY, "Invalid priority");
    kassert_message_void(process->process.priority >= MIN_PRIORITY, "Invalid priority");

    process->state = PROCESS_READY;

    push_back(run_queue(process->process.priority), &pid);
}

void scheduler_queue_async_init_task(void (*fun)(void)) {
    kprintf("scheduler_queue_async_init_task(fun: %p)\n", fun);
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

        if(process->state == PROCESS_SLEEPING){
            process->sleep_timeout *= ratio;
        }
    }

    int_unlock(&lock);
}

void scheduler_fault(void) {
    scheduler_kill_current_process();
}
