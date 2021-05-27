#pragma once

#include "vga_driver.h"
#include "elf_loader.h"

#define PROMPT "> "

extern size_t start_of_command;
extern size_t end_of_command;

void terminal_start(void);
void terminal_end(void);
void terminal_process_command(void);
void terminal_shift(void);

void get_command(char* final);
