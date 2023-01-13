#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "system_call_implementations.h"

#include <kstring.h>
#include <kstdio.h>
#include <kstdlib.h>

#include <global_phys_allocator.h>
#include <kernel_virt_allocator.h>
#include <paging.h>


struct tss_entry_struct {
    uint32_t prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
    uint32_t esp0;     // The stack pointer to load when changing to kernel mode.
    uint32_t ss0;      // The stack segment to load when changing to kernel mode.
    // Everything below here is unused.
    uint32_t esp1; // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
};

extern struct tss_entry_struct tss_entry;


extern void gdt_load(void);
extern void jump_usermode(void);
extern void flush_tss(void);

void init_gdt(void);
void write_tss(void);
void set_kernel_stack(uint32_t stack);
