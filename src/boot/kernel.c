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
    kdynamic_memory_init();


    if(serial_init()) { //fails if serial is faulty
        serial_writestring("hello, this is \n a test \n of serial strings \n containing \n newlines\n");
    }

    char *const ptr = zeroed_out_kmalloc(100);
    uint16_t *const ptr2 = zeroed_out_kmalloc(37);
    kfree(ptr);
    uint8_t *const ptr3 = zeroed_out_kmalloc(43);
    uint32_t *const ptr4 = zeroed_out_kmalloc(1000);
    kfree(ptr4);
    uint32_t *const ptr5 = zeroed_out_kmalloc(5);
    uint32_t *const ptr6 = zeroed_out_kmalloc(19);
    kfree(ptr2);
    kfree(ptr3);
    kfree(ptr5);
    kfree(ptr6);
    char *const ptr7 = zeroed_out_kmalloc(100);
    uint16_t *const ptr8 = zeroed_out_kmalloc(37);
    kfree(ptr7);
    kfree(ptr8);

    uint32_t *const big_ptr = kmalloc(4197);
    kfree(big_ptr);


    kprintf("foo\n");


    terminal_start();


    for(;;) {
        asm volatile("hlt");
    }
}
