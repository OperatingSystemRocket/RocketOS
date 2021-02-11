//@todo implement scrolling when the screen fills up and newline (especially this second one)

#include "vga_driver.h"

void kernel_main(void)
{
	/* Initialize terminal interface */
	terminal_initialize((uint16_t*)0xB8000);

	/* Newline support is left as an exercise. */
	terminal_writestring("Hello, kernel World!\n");
	terminal_writestring("Second line\nThird!\n");
}
