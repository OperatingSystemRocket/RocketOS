#include "scheduler.h"


#define UCODE_START 0x600000




struct registers {
    uint32_t EAX;
    uint32_t EBX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t ESI;
    uint32_t EDI;
    uint32_t EBP;
    uint32_t ESP;
    uint32_t EIP;
};

struct process_struct {
    struct registers registers;
    struct process_struct* next;
};


struct process_struct* process_queue;
struct process_struct* current_process;


volatile void switch_task(struct registers *const regs) {
    /* copy the saved registers into the current_proc structure */
    kmemcpy(&current_process->registers, regs, sizeof(struct registers));

    /* now go onto the next task - if there isn't one, go back to the start
      of the queue. */
    if (current_process->next != NULL) current_process = current_process->next;
    else current_process = process_queue;

    /* now hack the registers! */
    kmemcpy(regs, &current_process->registers, sizeof(struct registers));
    //switch_page_directory(get_cr3()); //this shouldn't be necessary since cr3 is always the same after paging is enabled, will change in future
};

