#pragma once

#include <stdint.h>

#include <drivers/port_mapped_io/hardware_io.h>
#include "kstdio.h"
#include <drivers/vga_driver/default_vga_driver.h>
#include "interrupt_types.h"
#include <scheduler/scheduler.h>

#include <usermode/system_call_implementations.h>


void pic_init(void);
void isr_install(void);

void pic_send_eoi(uint8_t no);
void pic_irq_enable(uint8_t no);
void idt_register_handler(uint8_t interrupt, uint32_t address);

void enable_interrupts(void);
void disable_interrupts(void);


extern uint32_t __attribute__((naked)) __attribute__((regparm(2))) irq_common_handler(uint32_t eax, uint32_t edx);
