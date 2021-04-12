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


//TODO: remove all 64 bit integer types as they are bigger than a word size



void kernel_early(const uint32_t mboot_magic, const multiboot_info_t *const mboot_header) {
    terminal_initialize();
    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        terminal_writestring_color("Invalid Multiboot Magic!\n", VGA_COLOR_RED);
    } else {
        terminal_writestring("The multiboot structure was loaded properly\n");
    }
}

void kernel_main(void) {
    pic_init();
    isr_install();

    //enable_time();
    enable_keyboard();


    allocate_init();
    paging_init();
    dynamic_memory_init();

    uint32_t *const ptr = zeroed_out_malloc(5);
    uint32_t *const ptr2 = zeroed_out_malloc(7);

    for(int32_t i = -3; i < 3; ++i) {
        kprintf("ptr: %u\n", ptr[i] & 0x7fffffff);
        kprintf("ptr2: %u\n", ptr2[i] & 0x7fffffff);
    }



    terminal_start();

    if(serial_init()) { //fails if serial is faulty
        serial_writestring("hello, this is \n a test \n of serial strings \n containing \n newlines\n");
    }


    for(;;) {
        asm volatile("hlt");
    }
}
