#pragma once

#include <stdint.h>


extern uint32_t get_faulting_address(void);

//TODO: add `outw`
extern void __attribute__((regparm(2))) outb(uint16_t port, uint8_t value);
inline void outw(uint16_t _port, uint16_t _data) {
    asm volatile ("outw %[data],  %[port]"
        :  /* No outputs */
        : [port] "dN" (_port), [data] "a" (_data));
}
extern void __attribute__((regparm(2))) outl(uint16_t port, uint32_t value);

//TODO: add `inw`
extern uint8_t __attribute__((regparm(1))) inb(uint16_t port);
inline uint16_t inw(uint16_t _port) {
    uint16_t rv;

    asm volatile ("inw %[port], %[data]"
        : [data] "=a" (rv)
        : [port] "dN" (_port));

    return rv;
}
extern uint32_t __attribute__((regparm(1))) inl(uint16_t port);

inline void io_wait(void) {
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

inline void flush_tlb_single_page(const uint32_t addr) {
    asm volatile("invlpg (%0)" : : "r" (addr) : "memory");
    //asm volatile("invlpg (%0)" : : "m" (addr));
}
