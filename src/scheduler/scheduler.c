#include "scheduler.h"


volatile struct process* current_process;
volatile struct process* process_queue_begin; //"beginning" of circular linked list
volatile struct process* process_queue_end; //"end" of circular linked list, used to speed up insertion into list


#define TIME_QUANTUM_VALUE 10u


void create_process(void (*const entry_point)(void)) {
    if(process_queue_begin == NULL) return;

    kassert_void(process_queue_end != NULL);

    process_queue_end->next = zeroed_out_kmalloc(sizeof(struct process));
    process_queue_end = process_queue_end->next;

    process_queue_end->stack = zeroed_out_kmalloc(4096);

    process_queue_end->previously_loaded = false;
    process_queue_end->id = 1;
    process_queue_end->time_quantum = TIME_QUANTUM_VALUE;

    process_queue_end->register_states.ebp = (uint32_t) process_queue_end->stack;
    process_queue_end->register_states.esp = ((uint32_t) process_queue_end->stack)+4096u;
    process_queue_end->register_states.eip = (uint32_t) entry_point;

    process_queue_end->page_directory = get_default_page_directory();
    process_queue_end->next = process_queue_begin;

    if(process_queue_begin->next == NULL) {
        process_queue_begin->next = process_queue_end;
    }
}


void scheduler_init(void) {
    current_process = zeroed_out_kmalloc(sizeof(struct process));

    current_process->previously_loaded = true;
    current_process->stack = NULL;
    current_process->id = 0;
    current_process->time_quantum = TIME_QUANTUM_VALUE;
    current_process->page_directory = get_default_page_directory();
    current_process->next = NULL;

    process_queue_begin = current_process;
    process_queue_end = current_process;
}

//TODO: replace __attribute__((interrupt)) as it is garbage
__attribute__((interrupt)) static void timer_irq(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons


    increment_ticks();
    increment_timer();


    if(current_process != NULL && current_process->next != NULL && (--current_process->time_quantum == 0)) {
        save_current_task(&current_process->register_states);
        current_process->time_quantum = TIME_QUANTUM_VALUE;

        kassert_void(current_process->next != current_process);

        current_process = current_process->next;

        if(current_process->previously_loaded) {
            resume_task(&current_process->register_states);
        } else {
            current_process->previously_loaded = true;
            load_task(&current_process->register_states);
        }
    }


    pic_send_eoi(1);
}

void enable_timer(void) {
    pic_irq_enable(0);
    idt_register_handler(32, (uint32_t)timer_irq);
}


void example_function_task(void) {
    //uint32_t count = 0u;

    for(;;) {
        //kprintf("example_function_task called with count: %u\n", count++);
        asm volatile("hlt");
    }
}


void foo_function_task(void) {
    //uint32_t count = 0u;

    for(;;) {
        //kprintf("foo_function_task called with count: %u\n", count++);
        asm volatile("hlt");
    }
}
