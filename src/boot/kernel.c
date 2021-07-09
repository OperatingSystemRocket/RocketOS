#include <multiboot.h>


#include <stdbool.h>

#include "kstdio.h"
#include "kmath.h"
#include "interrupts.h"
#include "time.h"
#include "serial_driver.h"
#include "kassert.h"
#include "terminal_driver.h"

#include "physical_mem_allocator.h"
#include "paging.h"
#include "kstdlib.h"
#include "storage.h"
#include "gdt.h"
#include "default_keyboard_logic.h"

#include "terminal_driver.h"
#include "default_terminal_functions.h"
#include "default_terminal_system.h"
#include "keyboard_callbacks.h"


//TODO: remove all 64 bit integer types as they are bigger than a word size

void kernel_early(const uint32_t mboot_magic, const multiboot_info_t *const mboot_header) {
    if(serial_init()) { //fails if serial is faulty
        serial_writestring("Serial driver works\n");
    }

    terminal_context_initialize();

    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        terminal_context_writestring_color("Invalid Multiboot Magic!\n", VGA_COLOR_RED);
    } else {
        terminal_context_writestring("The multiboot structure was loaded properly\n");
    }
}

void kernel_main(void) {
    init_gdt();
    gdt_load();

    allocate_init();
    paging_init();
    kdynamic_memory_init();

    scheduler_init();

    pic_init();
    isr_install();

    write_tss();



    default_keyboard_map_state_init();

    set_default_functions();
    enable_keyboard();
    struct default_terminal_context *const data = zeroed_out_kmalloc(sizeof(struct default_terminal_context));
    kprintf("before vga_context: %p\n", data->vga_context);
    data->vga_context = get_default_vga_context();
    kprintf("after vga_context: %p\n", data->vga_context);
    struct GET_OBSERVER_TYPENAME(key_message) *const observer = sizeof(struct GET_OBSERVER_TYPENAME(key_message));
    *observer = (struct GET_OBSERVER_TYPENAME(key_message)){ data, catch_keycode };
    ADD_OBSERVER(128, key_message, get_subject(), observer);
    default_context_terminal_start();



    //jump_usermode();

    create_process(&example_function_task);
    create_process(&foo_function_task);


    uint32_t count = 0u;

    for(;;) {
        kprintf("kernel.c with count: %u\n", count++);
        asm volatile("hlt");
    }
}
