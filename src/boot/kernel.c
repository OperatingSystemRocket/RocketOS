#include <stdbool.h>

#include "kstdio.h"
#include "kmath.h"
#include "interrupts.h"
#include "time.h"
#include "serial_driver.h"
#include "kassert.h"
#include "terminal_driver.h"

#include "physical_mem_allocator.h"


void kernel_main(void) {
    terminal_initialize();

    pic_init();
    isr_install();

    //enable_time();
    enable_keyboard();

    /*
    volatile int32_t n = 0;
    volatile int32_t y = 3;
    volatile int32_t r = y/n;
    */
    //kprintf("%s", "Line 1\n");
    //time_sleep_ticks(5);
    //kprintf("%s", "Line 2\n");
    //time_sleep_ticks(18);
    //kprintf("%s", "Line 3\n");
    //time_sleep_ticks(36);
    //kprintf("%s", "Line 4\n");


    allocate_init();
    void* allocated_page = allocate_page();


    free_page(allocated_page);



    terminal_start();

    kassert_void(serial_init()); //fails if serial is faulty

    serial_writestring("hello, this is \n a test \n of serial strings \n containing \n newlines\n");

    kprintf("%i\n", -152234);




    for(volatile uint32_t i = 0u; ; ++i);
}
