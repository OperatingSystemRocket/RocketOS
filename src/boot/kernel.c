#include "kstdio.h"
#include "kmath.h"
#include "interrupts.h"


void kernel_main(void) {
    terminal_initialize();

    pic_init();
    isr_install();

    kprintf("Hello %c %i %s\n", 'c', 5, "Hello String");
    terminal_writestring("this is a test of scrolling\n");


    for(int32_t i = 0; i < 3; ++i){
        volatile int32_t n = 0;
        volatile int32_t y = 3;
        volatile int32_t r = y/n;
    }
    terminal_writestring("Divide by Zero for loop has completed\n");
    {
        volatile int32_t n = 0;
        volatile int32_t y = 3;
        volatile int32_t r = y/n;
    }
    {
        volatile int32_t n = 0;
        volatile int32_t y = 3;
        volatile int32_t r = y/n;
    }
    {
        volatile int32_t n = 0;
        volatile int32_t y = 3;
        volatile int32_t r = y/n;
    }

//    for(int32_t i = 0; i < 3; ++i) {
//        volatile int32_t n = 0;
//        volatile int32_t y = 3;
//        volatile int32_t r = y/n;
//    }
}
