#include <multiboot.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "unity.h"


void setUp(void) {}
void tearDown(void) {}


void test_cstrlen(void) {
    TEST_ASSERT_EQUAL_UINT32(1, kstrlen("f"));
    TEST_ASSERT_EQUAL_UINT32(5, kstrlen("hello"));
}


void kernel_early(const uint32_t mboot_magic, const multiboot_info_t *const mboot_header) {
    (void) mboot_header; //needed for calling convention reasons, but currently unused

    if(serial_init()) { //fails if serial is faulty
        serial_writestring("Serial driver works\n");
    }

    terminal_context_initialize();

    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        terminal_context_writestring_color("Invalid Multiboot Magic!\n", VGA_COLOR_RED);
    } else {
        terminal_context_writestring("The multiboot structure was loaded properly\n");
    }
}


void kernel_main(void) {
    serial_init();
    UNITY_BEGIN();
    RUN_TEST(test_cstrlen);
    UNITY_END();
    outb(0xf4, 0x10);
}
