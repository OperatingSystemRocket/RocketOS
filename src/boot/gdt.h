#pragma once

#include <stdint.h>

#include "kstring.h"
#include "kstdio.h"


extern void gdt_load(void);
extern jump_usermode(void);
extern flush_tss(void);

void init_gdt(void);
void write_tss(void);
void set_kernel_stack(uint32_t stack);
