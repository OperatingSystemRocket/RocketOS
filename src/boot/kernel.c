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


    if(serial_init()) { //fails if serial is faulty
        serial_writestring("hello, this is \n a test \n of serial strings \n containing \n newlines\n");
    }

    char *const ptr = zeroed_out_kmalloc(100);
    uint16_t *const ptr2 = zeroed_out_kmalloc(37);
    uint8_t *const ptr3 = zeroed_out_kmalloc(43);
    kfree(ptr);
    kfree(ptr2);


    {
        kprintf("freelist\n");
        uint32_t* current_block = get_head();
        if(get_head() != NULL) {
            kprintf("head is not NULL\n");
        } else {
            kprintf("head is NULL\n");
        }
        while(current_block != NULL) {
            kprintf("first word size: %u, first word allocated bit: %u, second word: %u, third word: %u, last word size: %u, last word allocated bit: %u\n",
            get_size(current_block[0]), get_allocated_bit(current_block[0]), current_block[1], current_block[2], get_size(current_block[get_size(current_block[0])-1]), get_allocated_bit(current_block[get_size(current_block[0])-1]));
            current_block = current_block[2];
        }
    }


    //terminal_start();

    char dest_buf[512] = "You shouldn't be seeing this text\n"; /// prints page fault to qemu terminal when buf is >= 408 elements
    char src_buf[512] = "EPIC VICTORY\n";
    terminal_writestring(dest_buf);
    time_sleep_ticks(1);

    kprintf("%s\n" "before ide_initialize:\n");
    ide_print_register_debug_info();
    kprintf("%s", "\n\n\n");

    ide_initialize(0x1F0, 0x3F4, 0x170, 0x374, 0x000);

    kprintf("%s\n" "after ide_initialize:\n");

    //ide_print_register_debug_info();

    ide_write_sectors((unsigned char)3,1,0,0,(unsigned int)src_buf);

    ide_read_sectors((unsigned char)3,1,0,0,(unsigned int)dest_buf);

    kprintf("%s", "error package: ");
    kprintf("%c", get_error_package());




    terminal_writestring(dest_buf);


    for(;;) {
        asm volatile("hlt");
    }
}
