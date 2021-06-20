#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "kstring.h"
#include "hardware_io.h"


//TODO: put these in an enum with a `serial_` prefix on the names
#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8


//returns whether or not the serial is faulty. true = serial works properly. false = serial is faulty.
bool serial_init(void);


//TODO: maybe take the performance hit and make these functions externally linked instead for maintainability reasons

inline int8_t serial_received(void) {
    return inb(COM1 + 5) & 1u;
}

inline char serial_read(void) {
    while (serial_received() == 0);

    return inb(COM1);
}

inline int8_t is_transmit_empty(void) {
    return inb(COM1 + 5) & 0x20u;
}

inline void serial_putchar(const char a) {
    while (is_transmit_empty() == 0);

    outb(COM1, a);
}


size_t kstrlen(const char* str);

inline void serial_write(const char *const text, const size_t size) {
    for(size_t i = 0u; i < size; ++i) {
        serial_putchar(text[i]);
    }
}


inline void serial_writestring(const char *const text) {
    serial_write(text, kstrlen(text));
}
