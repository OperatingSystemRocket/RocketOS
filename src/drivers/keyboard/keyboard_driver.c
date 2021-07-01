#include "keyboard_driver.h"


//data context (containing keyboard map)
static void* data_context;

//params: `data_context`, scancode
static void(*current_process_keystroke_implementation)(void*, unsigned char);


void set_data_context(void *const new_context) {
    data_context = new_context;
}

void set_process_keystroke_implementation(const void(*new_implementation)(void*, unsigned char)) {
    current_process_keystroke_implementation = new_implementation;
}


void pic_send_eoi(uint8_t no);

__attribute__((interrupt)) static void keyboard_irq(struct interrupt_frame *const frame) {
    const unsigned char scancode = inb(0x60);

    if(!(scancode & 128u)) {
        kprintf("keyboard_irq triggered\n");
        should_switch_task = true;
    }

    current_process_keystroke_implementation(data_context, scancode);

	pic_send_eoi(1);
}


void pic_irq_enable(uint8_t no);

void enable_keyboard(void) {
    pic_irq_enable(1);
    idt_register_handler(33, (uint32_t)keyboard_irq);
}
