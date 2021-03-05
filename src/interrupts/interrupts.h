#pragma once

#include <stdint.h>

#include "vga_driver.h"
#include "keyboard_driver.h"


void idt_init(void);

struct IDT_entry{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

void irq0_handler(void);

void irq1_handler(void);
