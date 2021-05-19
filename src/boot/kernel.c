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



    freelist_dump(true);

    char *const ptr = zeroed_out_kmalloc(100);
    uint16_t *const ptr2 = zeroed_out_kmalloc(37);
    kfree(ptr);
    freelist_dump(true);
    uint8_t *const ptr3 = zeroed_out_kmalloc(43);
    uint32_t *const ptr4 = zeroed_out_kmalloc(1000);
    kfree(ptr4);
    freelist_dump(true);
    uint32_t *const ptr5 = zeroed_out_kmalloc(5);
    uint32_t *const ptr6 = zeroed_out_kmalloc(19);
    kfree(ptr2);
    freelist_dump(true);
    kfree(ptr3);
    freelist_dump(true);
    kfree(ptr5);
    freelist_dump(true);
    kfree(ptr6);

    freelist_dump(true);

    char *const ptr7 = zeroed_out_kmalloc(100);
    uint16_t *const ptr8 = zeroed_out_kmalloc(37);
    kfree(ptr7);
    freelist_dump(true);
    kfree(ptr8);
    freelist_dump(true);



    uint32_t *const big_ptr = zeroed_out_kmalloc(4197);
    kfree(big_ptr);

    freelist_dump(true);

    heap_dump(2u);



    uint32_t* realloc_ptr = zeroed_out_kmalloc(576);
    kprintf("ptr: %p, (((uint32_t*)get_first_nonreserved_address())+3): %p\n", ptr, (((uint32_t*)get_first_nonreserved_address())+3));
    kassert_void(ptr == (((uint32_t*)get_first_nonreserved_address())+3));
    kassert_void(realloc_ptr == ptr);
    kprintf("before pointer: %p\n", realloc_ptr);
    kprintf("before size: %u\n", bytes_to_words(576));
    //for(int32_t i = -3; i <= bytes_to_words(576); ++i) {
    //    if(i >= 0 && i < bytes_to_words(576)) {
    //        realloc_ptr[i] = 1;
    //    }
    //    kprintf("%u\n", (realloc_ptr[i]&0x7fffffffu));
    //}
    realloc_ptr = zeroed_out_krealloc(realloc_ptr, 87);
    kprintf("after pointer: %p\n", realloc_ptr);
    kprintf("after size: %u\n", bytes_to_words(87));
    //for(int32_t i = -3; i <= bytes_to_words(87); ++i) {
    //    kprintf("%u\n", (realloc_ptr[i]&0x7fffffffu));
    //}
    //kprintf("memory block dump of first 1004 words\n");
    //for(int32_t i = -3; i <= 643; ++i) {
    //    kprintf("%u\n", (realloc_ptr[i]&0x7fffffffu));
    //}
    kfree(realloc_ptr);

    freelist_dump(true);
    heap_dump(2u);



    //terminal_start();


    for(;;) {
        asm volatile("hlt");
    }
}
