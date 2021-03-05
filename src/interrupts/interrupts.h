#pragma once

#include <stdint.h>

#include "vga_driver.h"
#include "keyboard_driver.h"


void idt_init(void);

struct IDT_entry{
    uint16_t offset_lowerbits;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_higherbits;
} __attribute__((packed));

void irq0_handler(void);

void irq1_handler(void);
