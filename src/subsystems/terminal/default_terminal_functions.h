#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "default_vga_driver.h"
#include "observer.h"
#include "kmath.h"


//TODO: consider making this user tweakable

#define MAX_NUMBER_OF_ARGS 10
#define MAX_ARGUMENT_SIZE 64

struct default_terminal_context {
    size_t start_of_command;
    size_t end_of_command;
    const char* prompt_symbol;
    char command_arguments[MAX_NUMBER_OF_ARGS][MAX_ARGUMENT_SIZE];

    struct vga_driver_context* vga_context;
};


void default_init_context(struct default_terminal_context* terminal_context_ptr);

void set_default_functions(void);

void default_terminal_start(void* context);
void default_terminal_end(void* context);
void default_terminal_process_command(void* context);
void default_terminal_shift(void* context);


//utility functions that don't require a context
//TODO: possibly put the context-less functions into their own header and source file
int32_t get_char_location(const char* src, char c, size_t start, size_t end);
void get_string_slice(const char* src, char* dest, size_t start, size_t end);
void get_string_between_chars(const char* src, char* dest, char open, char close);
int8_t get_string_section_after(const char* src, char* dest, const char* search_term);

bool default_parse_command_args(void* context, const char* args);
void default_run_command(void* context, char* command);
void default_get_command(void* context, char* final, size_t number_of_elements);

