#include "kstdio.h"
#include "kmath.h"
#include "interrupts.h"


void kernel_main(void) {
    terminal_initialize();

    kprintf("Hello %c %i %s\n", 'c', 5, "Hello String");
    terminal_writestring("this is a test of scrolling\n");

    idt_init();

}
