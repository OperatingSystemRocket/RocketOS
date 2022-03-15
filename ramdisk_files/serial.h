#pragma once

#include <stdint.h>

#include "strlen.h"
#include "port_io.h"


#define COM1 0x3F8

inline uint8_t is_transmit_empty(void) {
    return inb(COM1 + 5) & 0x20u;
}

inline void serial_putchar(const char c) {
    while (is_transmit_empty() == 0);

    outb(COM1, (uint8_t)c);
}

inline void serial_write(const char *const text, const size_t size) {
    for(size_t i = 0u; i < size; ++i) {
        serial_putchar(text[i]);
    }
}

inline void serial_writestring(const char *const text) {
    serial_write(text, kstrlen(text));
}
