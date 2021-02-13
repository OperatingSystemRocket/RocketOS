//@todo implement scrolling when the screen fills up

#include "vga_driver.h"
#include "cassert.h"

void kernel_main(void) {
	terminal_initialize();

	terminal_writestring("Hello, kernel World!\n");
    terminal_write_color("Hello, kernel World!\n", VGA_COLOR_RED);
    terminal_writestring("Second line\nThird!\n");
}
