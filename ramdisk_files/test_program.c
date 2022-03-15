#include <stdint.h>
#include <stddef.h>


size_t kstrlen(const char *const str) {
    size_t len = 0;
    while (str != NULL && str[len]) {
        len++;
    }
    return len;
}


uint8_t inb(const uint16_t port) {
    unsigned char v;

    asm volatile ("inb %w1,%0":"=a" (v):"Nd" (port));
    return v;
}

void outb(const uint16_t port, const uint8_t value) {
    asm volatile ("outb %b0,%w1": :"a" (value), "Nd" (port));
}


#define COM1 0x3F8

uint8_t is_transmit_empty(void) {
    return inb(COM1 + 5) & 0x20u;
}

void serial_putchar(const char c) {
    while (is_transmit_empty() == 0);

    outb(COM1, (uint8_t)c);
}

void serial_write(const char *const text, const size_t size) {
    for(size_t i = 0u; i < size; ++i) {
        serial_putchar(text[i]);
    }
}

void serial_writestring(const char *const text) {
    serial_write(text, kstrlen(text));
}


void simple_program(void) {
    serial_writestring("simple_program called\n");
}

void _start(void) {
    simple_program();
}
