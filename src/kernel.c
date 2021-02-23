#include "kstdio.h"


void kernel_main(void) {
    terminal_initialize();

    kprintf("Hello %c %i %s\n", 'c', 5, "Hello String");
    terminal_writestring("this is a test of scrolling\n");
    //terminal_scroll_up();
    //terminal_scroll_down();
    //terminal_scroll_up();
    terminal_putchar_color('c', VGA_COLOR_CYAN);
    terminal_putchar('t');
    terminal_putchar('\n');

    kassert_message_void(1==2, "kernel_message_void test");
}
