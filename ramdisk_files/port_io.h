#pragma once

#include <stdint.h>


inline uint8_t inb(const uint16_t port) {
    unsigned char v;

    asm volatile ("inb %w1,%0":"=a" (v):"Nd" (port));
    return v;
}

inline void outb(const uint16_t port, const uint8_t value) {
    asm volatile ("outb %b0,%w1": :"a" (value), "Nd" (port));
}
