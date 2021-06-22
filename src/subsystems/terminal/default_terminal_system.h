#pragma once


#include "default_terminal_functions.h"


void default_context_init_context(void);

void default_context_terminal_start(void);
void default_context_terminal_end(void);
void default_terminal_context_process_command(void);
void default_terminal_context_shift(void);


void default_context_parse_command_args(const char* args);
void default_context_run_command(char* command);
void default_context_get_command(char* final);
