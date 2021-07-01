#include "scheduler.h"


volatile struct process* current_process;
volatile struct process* ready_queue;

volatile char* new_stack;


static volatile bool first_task_switch;
bool volatile should_switch_task;


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

        current_process->id = 0;

        current_process->register_states.ebp = (uint32_t) new_stack;
        current_process->register_states.esp = ((uint32_t) new_stack)+4096;
        current_process->register_states.eip = (uint32_t) example_function_task;

        kprintf("{\n\tebp: %u,\n\tesp: %u,\n\teip: %u\n}\n", current_process->register_states.ebp, current_process->register_states.esp, current_process->register_states.eip);

        current_process->page_directory = get_page_directory();
        current_process->next = zeroed_out_kmalloc(sizeof(struct process));


        struct process *const next_process = current_process->next;
        next_process->id = 1;
        next_process->page_directory = get_page_directory();
        next_process->next = NULL;

        save_current_task(&next_process->register_states);

        kprintf("load task\n");

        load_task(&current_process->register_states);

        //unreachable, but this is here for debugging purposes:
        kprintf("timer_irq finished\n");
    } else if(should_switch_task) {
        kassert_void(should_switch_task == true);
        kprintf("load_old_task\n");

        should_switch_task = false;

        //save_current_task(&current_process->register_states);
        load_old_task(&current_process->next->register_states);

        //struct process* temp = current_process->next;
        //current_process->next = current_process;
        //current_process = temp;
        //temp = NULL;

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
    for(;;) {
        kprintf("example_function_task called\n");
        asm volatile("hlt");
    }
}
