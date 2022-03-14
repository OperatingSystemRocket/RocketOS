#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <drivers/vga_driver/default_vga_driver.h>
#include <interrupts/interrupt_types.h>
#include <interrupts/interrupts.h>

#include <scheduler/scheduler.h>


void set_data_context(void* new_context);
void set_process_keystroke_implementation(void(*new_implementation)(void*, unsigned char));
void enable_keyboard(void);
