#include "default_terminal_functions.h"


#define DEFAULT_PROMPT "> "


void default_init_context(struct default_terminal_context *const terminal_context_ptr) {    
    terminal_context_ptr->vga_context = get_default_vga_context();

    terminal_context_ptr->start_of_command = terminal_context_ptr->vga_context->terminal_row * 80 + terminal_context_ptr->vga_context->terminal_column;
    terminal_context_ptr->end_of_command = terminal_context_ptr->start_of_command;
    terminal_context_ptr->prompt_symbol = DEFAULT_PROMPT;
}

void set_default_functions(void) {
    set_terminal_start(default_terminal_start);
    set_terminal_end(default_terminal_end);
    set_terminal_process_command(default_terminal_process_command);
    set_terminal_shift(default_terminal_shift);
}


//TODO: Refactor this wrapper function out of existance maybe. Though this might be useful to keep in the future possibly.
void default_terminal_start(void *const context) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    default_init_context(terminal_context_ptr);
}

void default_terminal_end(void *const context) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    terminal_context_ptr->end_of_command = terminal_context_ptr->vga_context->terminal_row * 80 + terminal_context_ptr->vga_context->terminal_column;
}

void default_terminal_process_command(void *const context) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    if(terminal_context_ptr->end_of_command - terminal_context_ptr->start_of_command <= 0) {
        terminal_writestring(terminal_context_ptr->vga_context, terminal_context_ptr->prompt_symbol);

        run_terminal_start(context);
        terminal_context_ptr->vga_context->terminal_on = true;
        return;
    } else if(terminal_context_ptr->end_of_command - terminal_context_ptr->start_of_command > 79) {
        kprintf("Command is too long! Limit of 79 characters! Cutting down to length...\n");
        terminal_context_ptr->end_of_command = terminal_context_ptr->start_of_command + 79;
    }
    char command_string[80];
    default_get_command(context, command_string, 80u);
    default_run_command(context, command_string);
    run_terminal_start(context);
}

void default_terminal_shift(void *const context) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    terminal_context_ptr->start_of_command -= 80;
    terminal_context_ptr->end_of_command -= 80;

    terminal_context_ptr->vga_context->terminal_on = false;
}

















int32_t get_char_location(const char *const src, const char c, const size_t start, const size_t end) {
    for(size_t i = start; i < kmin(end, kstrlen(src)); ++i) {
        if(src[i] == c) {
            return (int32_t) i;
        }
    }
    return -1; //failure
}

void get_string_slice(const char *const src, char *const dest, const size_t start, const size_t end) {
    kmemcpy(dest, src+start, end-start);
    dest[end-start] = '\0';
}

void get_string_between_chars(const char *const src, char *const dest, const char open, const char close) {
    const size_t src_len = kstrlen(src);

    const int32_t start = get_char_location(src, open, 0, src_len-1);
    kassert_void(start != -1);

    const int32_t end = get_char_location(src, close, (size_t)(start+1), src_len-1u);
    kassert_void(end != -1);

    get_string_slice(src, dest, (size_t)(start+1), (size_t)end);
}

bool default_parse_command_args(void *const context, const char *const args) {
    (void) context;

    bool found_space = false;

    for(size_t i = 0u; i < kstrlen(args); ++i) {
        if(args[i] == ' ') {
            found_space = true;
        }
    }

    return found_space;
}


// puts the rest of a string after the first occurrence of search_term into dest. Returns 0 for success, -1 for string not found
int8_t get_string_section_after(const char *const src, char *const dest, const char *const search_term) {
    size_t src_len = kstrlen(src);
    size_t search_term_len = kstrlen(search_term);
    for(size_t i = 0; i < src_len; ++i) {
        if(src[i] == search_term[0]) {
            if(kstrncmp(src+i, search_term, search_term_len) == 0) {
                for(size_t j = 0; j < src_len-i; ++j) {
                    dest[j] = src[i+j+search_term_len];
                }
                dest[src_len] = '\0';
                return 0;
            }
        }
    }
    return -1;
}

void default_run_command(void *const context, char *const command) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    const bool has_space = default_parse_command_args(context, command);

    if(kstrncmp(command, "echo", 4) == 0) {
        if(!has_space) {
            terminal_writestring_color(terminal_context_ptr->vga_context, "missing space between 'echo' and argument\n", VGA_COLOR_RED);
        } else {
            if(kstrlen(command) > 5) {
                terminal_writestring(terminal_context_ptr->vga_context, kstrcat(command + 5, "\n"));
            } else {
                terminal_writestring_color(terminal_context_ptr->vga_context, "'echo' requires one argument!\n", VGA_COLOR_RED);
            }
        }
    } else if(kstrncmp(command, "help", 4) == 0) {
        terminal_writestring(terminal_context_ptr->vga_context, "List of available commands:\n");
        terminal_writestring(terminal_context_ptr->vga_context, "echo\n");
        terminal_writestring(terminal_context_ptr->vga_context, "help\n");
    }
    else {
        terminal_writestring_color(terminal_context_ptr->vga_context, "Invalid command! Try 'help'\n", VGA_COLOR_RED);
    }

    terminal_context_ptr->vga_context->terminal_on = true;
    terminal_writestring(terminal_context_ptr->vga_context, terminal_context_ptr->prompt_symbol);
}

void default_get_command(void *const context, char *const final, const size_t number_of_elements) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    final[0] = (char)terminal_context_ptr->vga_context->terminal_buffer[terminal_context_ptr->start_of_command];
    final[1] = '\0';
    char temp[2];
    for(size_t i = terminal_context_ptr->start_of_command + 1u; i < terminal_context_ptr->end_of_command; ++i) {
        temp[0] = (char)terminal_context_ptr->vga_context->terminal_buffer[i];
        temp[1] = '\0';
        kstrncat(final, temp, number_of_elements);
    }
}

