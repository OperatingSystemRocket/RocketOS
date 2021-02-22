//@todo implement scrolling when the screen fills up

#include "vga_driver.h"
#include "cassert.h"
#include "cstdio.h"


void kernel_main(void) {
    terminal_initialize();

    /*terminal_writestring("Hello, kernel World!\n");
    terminal_writestring_color("Hello, kernel World!\n", VGA_COLOR_RED);
    terminal_writestring("Second line\nThird!\n");
    //terminal_clear();
    terminal_writestring_color("Hello, kernel World!\n", VGA_COLOR_RED);
    terminal_setcolor(VGA_COLOR_CYAN);
    terminal_writestring("This\nis\ndifficult to read\n");
    for(char i = 0; i < 50; i++) {
        terminal_writestring("Test\n");
    }*/

    //terminal_writestring("Test\n");
    //for(int32_t i = 0; i < 23; ++i) {
    //    terminal_writestring("this is a test of scrolling\n");
    //}
    //terminal_writestring("Testing this buckaroo\n");

    //for(int32_t i = 0; i < 24; ++i) {
    //    terminal_writestring("this is a test of scrolling\n");
    //}
    //terminal_writestring("this is a test of scrolling\n");

    cprintf("%c %i", 'c', 5);
}
