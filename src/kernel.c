#include "kstdio.h"
#include "kmath.h"


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


    kprintf("%i", algebraic_sign(0));
    terminal_putchar('\n');
    kprintf("%i", ksignbit(1));
    terminal_putchar('\n');
    kprintf("%i", ksignbit(2));
    terminal_putchar('\n');
    kprintf("%i", ksignbit(-3));
    terminal_putchar('\n');

    kprintf("%i", ksignbit(3));
    terminal_putchar('\n');

    kprintf("%i", ksignbit(13));
    terminal_putchar('\n');




    kassert_void(1==2);
}
