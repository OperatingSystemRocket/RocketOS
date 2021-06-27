#include "scheduler.h"


volatile struct process* current_process;
volatile struct process* ready_queue;

volatile char* new_stack;


static bool first_task_switch;
bool should_switch_task;


void scheduler_init(void) {
    first_task_switch = true;
    should_switch_task = false;

    //temporarily don't use these as I'm just testing task switching and creation itself
    current_process = NULL;
    ready_queue = NULL;

    new_stack = zeroed_out_kmalloc(4096);
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

        switch_to_example_task();

        //unreachable, but this is here for debugging purposes:
        kprintf("timer_irq finished\n");
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
