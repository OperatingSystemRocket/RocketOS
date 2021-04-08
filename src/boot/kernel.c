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


    kassert_void(serial_init()); //fails if serial is faulty


    allocate_init();
    paging_init();
    //dynamic_memory_init();


    uint32_t *const allocated_page = allocate_page(CRITICAL_KERNEL_USE);
    kprintf("%i\n", (uint32_t)allocated_page);
    *allocated_page = 5;
    free_page(CRITICAL_KERNEL_USE, allocated_page);

    #define VIRTUAL_ADDRESS_TEST 0xfffe000

    kprintf("Decimal number test: %x\n", 0xdeadbeefu);


    terminal_start();

    if(serial_init()) { //fails if serial is faulty
        serial_writestring("hello, this is \n a test \n of serial strings \n containing \n newlines\n");
    }
    kprintf("first_nonreserved_address: %i, address: %i\n", (uint32_t)get_first_nonreserved_address(), (uint32_t)VIRTUAL_ADDRESS_TEST);

    const uint32_t test_pt_flags = PT_PRESENT | PT_RW;
    const uint32_t test_pd_flags = PD_PRESENT | PD_RW;

    const uint32_t physical_page_address = map_virtual_page((void*)VIRTUAL_ADDRESS_TEST, test_pt_flags, test_pd_flags);
    map_page(VIRTUAL_ADDRESS_TEST+PAGE_SIZE, physical_page_address, test_pt_flags, test_pd_flags);
    map_page(VIRTUAL_ADDRESS_TEST+(PAGE_SIZE*2), physical_page_address, test_pt_flags, test_pd_flags);

    uint32_t *const virtual_mapped_page_address1 = (uint32_t*)VIRTUAL_ADDRESS_TEST;
    *virtual_mapped_page_address1 = 8;
    kprintf("first mapped page: %i\n", virtual_mapped_page_address1[0]);

    uint32_t *const virtual_mapped_page_address2 = (uint32_t*)(VIRTUAL_ADDRESS_TEST+PAGE_SIZE);
    kprintf("second mapped page: %i\n", virtual_mapped_page_address2[0]);
    *virtual_mapped_page_address2 = 17;

    uint32_t *const virtual_mapped_page_address3 = (uint32_t*)(VIRTUAL_ADDRESS_TEST+(PAGE_SIZE*2));
    kprintf("third mapped page: %i\n", virtual_mapped_page_address3[0]);
    *virtual_mapped_page_address3 = 92;

    kprintf("first mapped page again: %i\n", virtual_mapped_page_address1[0]);


    //terminal_start();

    //kassert_void(serial_init()); //fails if serial is faulty

    //serial_writestring("hello, this is \n a test \n of serial strings \n containing \n newlines\n");



    for(;;) {
        asm volatile("hlt");
    }
}
