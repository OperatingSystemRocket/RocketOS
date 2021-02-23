#include "cstdio.h"


void kernel_main(void) {
    terminal_initialize();

    //cassert_void(1==2);

    cprintf("Hello %c %i %s\n", 'c', 5, "Hello String");
    terminal_writestring("this is a test of scrolling\n");
}
