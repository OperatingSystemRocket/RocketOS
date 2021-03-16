#include "unity.h"
#include "hardware_io.h"

void setUp(void) {}
void tearDown(void) {}


void kernel_main(void) {
    UNITY_BEGIN();
    UNITY_END();
    outb(0xf4, 0x10);
}
