#pragma once

#include <stdint.h>

#include "hardware_io.h"
#include "kstdio.h"
#include "default_vga_driver.h"
#include "interrupt_types.h"

#include "system_call_implementations.h"


void pic_init(void);
void isr_install(void);

void enable_timer(void);
