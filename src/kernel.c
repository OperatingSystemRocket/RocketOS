#include "kstdio.h"


void kernel_main(void) {
    terminal_initialize();

    //kassert_void(1==2);

    kprintf("Hello %c %i %s\n", 'c', 5, "Hello String");
    terminal_writestring("this is a test of scrolling\n");
}
