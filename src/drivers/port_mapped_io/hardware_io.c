#include "hardware_io.h"


void io_wait(void) {
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

void flush_tlb_single_page(const uint32_t addr) {
    asm volatile("invlpg (%0)" : : "r" (addr) : "memory");
}
