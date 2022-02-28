#pragma once

#include "default_vga_driver.h"
#include "kstdlib.h"


void set_terminal_start(void (*new_terminal_start)(void*));
void set_terminal_end(void (*new_terminal_end)(void*));
void set_terminal_process_command(void (*new_terminal_process_command)(void*));
void set_terminal_shift(void (*new_terminal_shift)(void*));


void run_terminal_start(void* context);
void run_terminal_end(void* context);
void run_terminal_process_command(void* context);
void run_terminal_shift(void* context);
