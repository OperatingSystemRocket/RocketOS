#include <stdbool.h>

#include "kstdio.h"
#include "kmath.h"
#include "interrupts.h"


void kernel_main(void) {
    terminal_initialize();

    pic_init();
    isr_install();

    enable_timer();
    enable_keyboard();
    terminal_start();

    for(volatile uint32_t i = 0u; ; ++i);
}

