#pragma once

#include <stdint.h>


extern uint32_t get_faulting_address(void);

//TODO: add `outw`
extern void __attribute__((regparm(2))) outb(uint16_t port, uint8_t value);
extern void __attribute__((regparm(2))) outl(uint16_t port, uint32_t value);

//TODO: add `inw`
extern uint8_t __attribute__((regparm(1))) inb(uint16_t port);
extern uint32_t __attribute__((regparm(1))) inl(uint16_t port);

void io_wait(void);

void flush_tlb_single_page(uint32_t addr);
