#include "scheduler.h"


volatile struct process* current_process; //start
volatile struct process* ready_queue; //end

volatile char* new_stack;


static volatile bool first_task_switch;
bool volatile should_switch_task;


void create_process(void) {
    kassert_void(current_process != NULL);
    struct process* process = current_process;
    for(;;) {
        if(process->next != NULL) {
            current_process = process->next;
        } else {
            process->next = zeroed_out_kmalloc(sizeof(struct process));
            struct process *const next_process = process->next;

            next_process->previously_loaded = false;
            next_process->stack = zeroed_out_kmalloc(4096); //TODO: make 16KiB and be page aligned
            next_process->id = 0; //TODO: make this an actual value

            next_process->register_states.ebp = (uint32_t) next_process->stack;
            next_process->register_states.esp = ((uint32_t) next_process->stack)+4096;
            next_process->register_states.eip = (uint32_t) example_function_task; //today make this passed in

            next_process->page_directory = get_page_directory();
            next_process->next = NULL;
        }
    }
}


void scheduler_init(void) {
    first_task_switch = true;
    should_switch_task = false;

    //temporarily don't use these as I'm just testing task switching and creation itself
    current_process = NULL;
    ready_queue = NULL;

    new_stack = zeroed_out_kmalloc(4096);
    kprintf("new_stack: %p\n", new_stack);
}

//TODO: replace __attribute__((interrupt)) as it is garbage
__attribute__((interrupt)) static void timer_irq(struct interrupt_frame *const frame) {
    increment_time();
    set_time_in_seconds();
    kprintf("time: %u\n", get_time_in_ticks());


    if(first_task_switch && should_switch_task) {
        kprintf("task switch\n");

        first_task_switch = false;
        should_switch_task = false;

        current_process = zeroed_out_kmalloc(sizeof(struct process));

        current_process->previously_loaded = false;
        current_process->id = 0;

        current_process->register_states.ebp = (uint32_t) new_stack;
        current_process->register_states.esp = ((uint32_t) new_stack)+4096;
        current_process->register_states.eip = (uint32_t) example_function_task;

        kprintf("{\n\tebp: %u,\n\tesp: %u,\n\teip: %u\n}\n", current_process->register_states.ebp, current_process->register_states.esp, current_process->register_states.eip);

        current_process->page_directory = get_page_directory();
        current_process->next = zeroed_out_kmalloc(sizeof(struct process));


        struct process *const next_process = current_process->next;
        next_process->previously_loaded = true;
        next_process->id = 1;
        next_process->page_directory = get_page_directory();
        next_process->next = NULL;

        save_current_task(&next_process->register_states);

        kprintf("load task\n");

        load_task(&current_process->register_states);

        //unreachable, but this is here for debugging purposes:
        kprintf("timer_irq finished\n");
        current_process->previously_loaded = true;
    }
    if((!first_task_switch && should_switch_task) || (current_process != NULL && current_process->previously_loaded)) {
        kprintf("load_old_task\n");

        should_switch_task = false;

        if(!current_process->previously_loaded) {
            struct process* temp = current_process->next;
            temp->next = current_process;
            current_process = temp;
            current_process->next->next = NULL;
            temp = NULL;

            save_current_task(&current_process->next->register_states);

            kprintf("{\n\tebp: %u,\n\tesp: %u,\n\teip: %u\n}\n", current_process->register_states.ebp, current_process->register_states.esp, current_process->register_states.eip);

            resume_task(&current_process->register_states);
        } else {
            current_process->previously_loaded = false;
        }

        //unreachable, but this is here for debugging purposes:
        kprintf("else if taken\n");
    }


    pic_send_eoi(1);
}

void enable_timer(void) {
    pic_irq_enable(0);
    idt_register_handler(32, (uint32_t)timer_irq);
}


void example_function_task(void) {
    uint32_t count = 0u;

    for(;;) {
        kprintf("example_function_task called with count: %u\n", count++);
        asm volatile("hlt");
    }
}
