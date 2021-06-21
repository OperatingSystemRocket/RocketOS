#pragma once

#include <stdint.h>
#include <stddef.h>

#include "default_vga_driver.h"


extern void trigger_interrupt(void);

void print(const char* str);
