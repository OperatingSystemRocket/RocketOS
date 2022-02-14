#include "test_kernel_early.h"

#include "unity.h"

#include "default_hashmap_functions.h"
#include "hashmap.h"


void setUp(void) {}
void tearDown(void) {}


void kernel_main(void) {
    serial_init();

    init_gdt();
    gdt_load();

    pic_init();
    isr_install();

    write_tss();

    initialize_kernel_memory();
    kdynamic_memory_init();

    UNITY_BEGIN();





    UNITY_END();

    outb(0xf4, 0x10);
}
