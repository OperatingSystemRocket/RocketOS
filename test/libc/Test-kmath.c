#include <multiboot.h>


#include "kmath.h"
#include "unity.h"
#include "hardware_io.h"
#include "vga_driver.h"


void setUp(void){}
void tearDown(void){}


void kernel_early(const uint32_t mboot_magic, const multiboot_info_t *const mboot_header) {
    terminal_initialize();
    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        terminal_writestring_color("Invalid Multiboot Magic!\n", VGA_COLOR_RED);
    } else {
        terminal_writestring("The multiboot structure was loaded properly\n");
    }
}

void kernel_main(void) {
    UNITY_BEGIN();
    UNITY_END();
    outb(0xf4, 0x10);
}
