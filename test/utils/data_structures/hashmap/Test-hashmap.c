#include "test_kernel_early.h"

#include "unity.h"


void setUp(void) {}
void tearDown(void) {}


void kernel_main(void) {
    serial_init();
    allocate_init();
    paging_init();
    kdynamic_memory_init();

    UNITY_BEGIN();





    UNITY_END();

    outb(0xf4, 0x10);
}
