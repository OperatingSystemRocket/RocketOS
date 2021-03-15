#pragma once

#include "vga_driver.h"

#define PROMPT "> "


void terminal_start(void);
void terminal_end(void);
void terminal_process_command(void);
void terminal_shift(void);

void get_command(char final[]);
