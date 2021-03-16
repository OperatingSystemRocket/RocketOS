#pragma once

#include <stdint.h>

void outb(uint16_t port, uint8_t val);

uint8_t inb(uint16_t port);

void io_wait(void);

void flush_tlb_single_page(uint64_t addr);
