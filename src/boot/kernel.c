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
    //kfree(ptr);
    uint8_t *const ptr3 = zeroed_out_kmalloc(43);
    //uint32_t *const ptr4 = zeroed_out_kmalloc(1000);
    //kfree(ptr4);
    //uint32_t *const ptr5 = zeroed_out_kmalloc(5);
    //uint32_t *const ptr6 = zeroed_out_kmalloc(19);
    kfree(ptr);
    kfree(ptr2);
    //kfree(ptr3);
    //kfree(ptr5);
    //kfree(ptr6);

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

    //char *const ptr7 = zeroed_out_kmalloc(100);
    //uint16_t *const ptr8 = zeroed_out_kmalloc(37);
    //kfree(ptr7);
    //kfree(ptr8);



    //uint32_t *const big_ptr = zeroed_out_kmalloc(4197);
    //kfree(big_ptr);



/*
    uint32_t* realloc_ptr = zeroed_out_kmalloc(576);
    kassert_void(ptr == get_first_nonreserved_address());
    kassert_void(realloc_ptr == ptr);
    kprintf("before pointer: %p\n", realloc_ptr);
    kprintf("before size: %u\n", bytes_to_words(576));
    for(int32_t i = -3; i <= bytes_to_words(576); ++i) {
        if(i >= 0 && i < bytes_to_words(576)) {
            realloc_ptr[i] = 1;
        }
        kprintf("%u\n", (realloc_ptr[i]&0x7fffffffu));
    }
    realloc_ptr = zeroed_out_krealloc(realloc_ptr, 87);
    kprintf("after pointer: %p\n", realloc_ptr);
    kprintf("after size: %u\n", bytes_to_words(87));
    for(int32_t i = -3; i <= bytes_to_words(87); ++i) {
        kprintf("%u\n", (realloc_ptr[i]&0x7fffffffu));
    }
    kprintf("memory block dump of first 1004 words\n");
    for(int32_t i = -3; i <= 643; ++i) {
        kprintf("%u\n", (realloc_ptr[i]&0x7fffffffu));
    }
    kfree(realloc_ptr);
*/

    kprintf("full heap dump\n");
    for(int32_t i = 0; i < 1024; ++i) {
        const uint32_t *const start_of_heap = get_first_nonreserved_address();
        kprintf("size: %u, is_allocated: %u\n", get_size(start_of_heap[i]), get_allocated_bit(start_of_heap[i]));
    }
    {
        kprintf("freelist\n");
        uint32_t* current_block = get_head();
        if(get_head() != NULL) {
            kprintf("head is not NULL\n");
        } else {
            kprintf("head is NULL\n");
        }
        while(current_block != NULL) {
            kprintf("memory address: %u, first word size: %u, first word allocated bit: %u, second word: %u, third word: %u, last word size: %u, last word allocated bit: %u\n",
            current_block, get_size(current_block[0]), get_allocated_bit(current_block[0]), current_block[1], current_block[2], get_size(current_block[get_size(current_block[0])-1]), get_allocated_bit(current_block[get_size(current_block[0])-1]));
            current_block = current_block[2];
        }
    }



    //terminal_start();

    char dest_buf[512] = "foobar\n"; /// prints page fault to qemu terminal when buf is >= 408 elements
    char src_buf[512] = "write this to the disk\n";
    terminal_writestring(dest_buf);
    time_sleep_ticks(1);

    ide_initialize(0x1F0, 0x3F4, 0x170, 0x374, 0x000);
    ide_read_sectors(0,1,0,1,dest_buf);

    ide_read_sectors(0,1,0,1, dest_buf);

    terminal_writestring(dest_buf);


    for(;;) {
        asm volatile("hlt");
    }
}
