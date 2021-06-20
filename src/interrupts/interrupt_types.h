#pragma once

#include <stdint.h>


enum IDTFlags {
    IDT_PRESENT = 1 << 7,
    IDT_RING_0 = 0 << 5,
    IDT_RING_1 = 1 << 5,
    IDT_RING_2 = 2 << 5,
    IDT_RING_3 = 3 << 5,
    IDT_SS = 1 << 4,
    IDT_INTERRUPT = 0xE,
    IDT_TRAP = 0xF,
};

struct interrupt_frame {
    uint16_t error_code;
    uint16_t reserved1;
    uint32_t eip;
    uint16_t cs;
    uint16_t reserved2;
    uint32_t eflags;
    uint32_t esp;
    uint16_t ss;
    uint16_t reserved3;
    uint16_t es;
    uint16_t reserved4;
    uint16_t ds;
    uint16_t reserved5;
    uint16_t fs;
    uint16_t reserved6;
    uint16_t gs;
    uint16_t reserved7;
};

struct IDT_entry{
    uint16_t offset_lowerbits;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_higherbits;
};
