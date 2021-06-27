#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "default_vga_driver.h"
#include "interrupt_types.h"

#include "scheduler.h"


void set_data_context(void* new_context);
void set_process_keystroke_implementation(void(*new_implementation)(void*, unsigned char));
void enable_keyboard(void);
