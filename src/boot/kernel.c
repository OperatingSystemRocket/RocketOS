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

    //jump_usermode();



    default_keyboard_map_state_init();

    set_default_functions();
    enable_keyboard();

    default_context_terminal_start();


    //jump_usermode();


    for(;;) {
        asm volatile("hlt");
        kprintf("kernel.c\n");
    }
}
