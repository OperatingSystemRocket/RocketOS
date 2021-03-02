#include "kstdio.h"
#include "kmath.h"
#include "interrupts.h"


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


    idt_init();


    kprintf("%i\n", ksign(0));

    kprintf("%i\n", ksign(1));
    kprintf("%i\n", ksign(2));
    kprintf("%i\n", ksign(-3));

    kprintf("%i\n", ksign(3));

    kprintf("%i\n", kmax(13,15));
    kprintf("%i\n", kmin(13,15));





    kassert_void(1==2);

    kprintf("%i\n", ksignbit(13));
}
