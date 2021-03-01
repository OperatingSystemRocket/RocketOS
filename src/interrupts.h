#pragma once
#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val);

void idt_init(void);

struct IDT_entry{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};
