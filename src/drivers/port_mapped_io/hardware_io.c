#include "hardware_io.h"

void outb(const uint16_t port, const uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
    /*There's an outb %al, $imm8  encoding, for compile-time constant port numbers
    * that fit in 8b.  (N constraint). Wider immediate constants would be
    * truncated at assemble-time (e.g. "i" constraint). The  outb  %al, %dx
    * encoding is the only option for all other cases.
    * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we
    * had the port number a wider C type */
}

uint8_t inb(const uint16_t port) {
	uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void io_wait(void) {
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

void flush_tlb_single_page(const uint32_t addr) {
    asm volatile("invlpg (%0)" : : "r" (addr) : "memory");
}
