#include <stdbool.h>

#include "kstdio.h"
#include "kmath.h"
#include "interrupts.h"
#include "time.h"




void kernel_main(void) {
    terminal_initialize();

    pic_init();
    isr_install();

    kprintf("Hello %c %i %s\n", 'c', 5, "Hello String");
    terminal_writestring("this is a test of scrolling\n");


    enable_time(); // calls enable_timer()
    enable_keyboard();

    /*
    volatile int32_t n = 0;
    volatile int32_t y = 3;
    volatile int32_t r = y/n;
*/
    kprintf("%s", "Line 1\n");
    time_sleep_ticks(5);
    kprintf("%s", "Line 2\n");
    time_sleep_ticks(18);
    kprintf("%s", "Line 3\n");
    time_sleep_ticks(36);
    kprintf("%s", "Line 4\n");





    for(volatile uint32_t i = 0u; ; ++i);
}
