#pragma once

#include "vga_driver.h"
#include "elf_loader.h"
#include "storage.h"
#include "kstdlib.h"

#define PROMPT "> "

extern size_t start_of_command;
extern size_t end_of_command;

void terminal_start(void);
void terminal_end(void);
void terminal_process_command(void);
void terminal_shift(void);

void get_command(char* final);
void parse_command_args(const char* args);

int8_t get_string_section_after(const char* const src, char* const dest, const char* const search_term);
