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
#include "src/storage/storage.h"



//TODO: remove all 64 bit integer types as they are bigger than a word size


void kernel_early(const uint32_t mboot_magic, const multiboot_info_t *const mboot_header) {
    kassert_void(serial_init());
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

    enable_time();
    enable_keyboard();


    allocate_init();
    paging_init();
    kdynamic_memory_init();



    //freelist_dump(true);

    char *const ptr = zeroed_out_kmalloc(100);
    uint16_t *const ptr2 = zeroed_out_kmalloc(37);
    kfree(ptr);
    //freelist_dump(true);
    uint8_t *const ptr3 = zeroed_out_kmalloc(43);
    uint32_t *const ptr4 = zeroed_out_kmalloc(1000);
    kfree(ptr4);
    //freelist_dump(true);
    uint32_t *const ptr5 = zeroed_out_kmalloc(5);
    uint32_t *const ptr6 = zeroed_out_kmalloc(19);
    kfree(ptr2);
    //freelist_dump(true);
    kfree(ptr3);
    //freelist_dump(true);
    kfree(ptr5);
    //freelist_dump(true);
    kfree(ptr6);

    //freelist_dump(true);

    char *const ptr7 = zeroed_out_kmalloc(100);
    uint16_t *const ptr8 = zeroed_out_kmalloc(37);
    kfree(ptr7);
    //freelist_dump(true);
    kfree(ptr8);
    //freelist_dump(true);

    //terminal_start();


    freelist_dump(true);
    uint32_t *const big_ptr = zeroed_out_kmalloc(4197);
    kfree(big_ptr);

    //freelist_dump(true);

    //heap_dump(2u);


    kprintf("\n\n\n");


    uint32_t* realloc_ptr = zeroed_out_kmalloc(576);
    kprintf("malloc'ed size: %u\n", bytes_to_words(576)+4);

    uint32_t *const malloced_ptr = zeroed_out_kmalloc(1000);

    kprintf("realloc_ptr: %X, malloced_ptr: %X\n", realloc_ptr, malloced_ptr);
    freelist_dump(true);
    //heap_dump(2u);

    realloc_ptr = zeroed_out_krealloc(realloc_ptr, 87);
    kprintf("realloc'ed size: %u\n", bytes_to_words(87)+4);

    kprintf("realloc_ptr: %X, malloced_ptr: %X\n", realloc_ptr, malloced_ptr);
    freelist_dump(true);
    //heap_dump(2u);

    kfree(realloc_ptr);
    kfree(malloced_ptr);

    freelist_dump(true);
    heap_dump(2u);

    //ide_print_register_debug_info();

    char dest_buf[512] = "You shouldn't be seeing this text\n";
    char src_buf[512] = "EPIC STORAGE SUCCESS\n";
    terminal_writestring(dest_buf);
    time_sleep_ticks(1);

    kprintf("%s\n" "before ide_initialize:\n");
    ide_print_register_debug_info();
    kprintf("%s", "\n\n\n");

    ide_initialize(0x1F0, 0x3F4, 0x170, 0x374, 0x000);

    kprintf("%s\n" "after ide_initialize:\n");


    ide_write_sectors((unsigned char)3,1,0,0,(unsigned int)src_buf);


    ide_read_sectors((unsigned char)3,1,0,0,(unsigned int)dest_buf);


    terminal_writestring(dest_buf);


    for(;;) {
        asm volatile("hlt");
    }
}
