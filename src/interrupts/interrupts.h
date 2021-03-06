#pragma once

#include <stdint.h>

#include "hardware_io.h"
#include "kstdio.h"
#include "default_vga_driver.h"
#include "interrupt_types.h"

#include "time.h"

#include "system_call_implementations.h"


void pic_init(void);
void isr_install(void);

void pic_send_eoi(uint8_t no);
void pic_irq_enable(uint8_t no);
void idt_register_handler(uint8_t interrupt, uint32_t address);
