#include "default_terminal_functions.h"


#define DEFAULT_PROMPT "> "


void default_init_context(struct default_terminal_context *const terminal_context_ptr) {
    terminal_context_ptr->start_of_command = terminal_row * 80 + terminal_column;
    terminal_context_ptr->end_of_command = terminal_context_ptr->start_of_command;
    terminal_context_ptr->prompt_symbol = DEFAULT_PROMPT;
}

void set_default_functions(void) {
    set_terminal_start(default_terminal_start);
    set_terminal_end(default_terminal_end);
    set_terminal_process_command(default_terminal_process_command);
    set_terminal_shift(default_terminal_shift);
}

void default_terminal_start(void *const context) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    default_init_context(terminal_context_ptr);

    terminal_on = true;

    terminal_writestring(terminal_context_ptr->prompt_symbol);

    terminal_context_ptr->start_of_command += kstrlen(terminal_context_ptr->prompt_symbol);
}

void default_terminal_end(void *const context) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    terminal_context_ptr->end_of_command = terminal_row * 80 + terminal_column;
}

void default_terminal_process_command(void *const context) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    terminal_on = false;
    if(terminal_context_ptr->end_of_command - terminal_context_ptr->start_of_command <= 0) {
        kprintf("Invalid command! Try 'help'\n");
        run_terminal_start(context);
        return;
    } else if(terminal_context_ptr->end_of_command - terminal_context_ptr->start_of_command > 79) {
        kprintf("Command is too long! Limit of 79 characters! Cutting down to length...\n");
        terminal_context_ptr->end_of_command = terminal_context_ptr->start_of_command + 79;
    }
    char command_string[80];
    default_get_command(context, command_string);
    default_run_command(context, command_string);
    run_terminal_start(context);
}

void default_terminal_shift(void *const context) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    terminal_context_ptr->start_of_command -= 80;
    terminal_context_ptr->end_of_command -= 80;
}

















int32_t get_char_location(const char *const src, const char c, const size_t start, const size_t end) {
    for(size_t i = start; i < kmin(end, kstrlen(src)); ++i) {
        if(src[i] == c) {
            return i;
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
    const size_t start = get_char_location(src, open, 0, src_len-1);
    const size_t end = get_char_location(src, close, start+1, src_len-1);

    get_string_slice(src, dest, start+1, end);
}

void default_parse_command_args(void *const context, const char *const args) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    size_t arg_index = 0u;
    for(size_t i = 0u; i < kstrlen(args); ++i) {
        if(args[i] == ' ') {
            if(args[i+1] == '\"') {
                const int32_t end_location = get_char_location(args, '\"', i+2, kstrlen(args));
                if(end_location != -1) {
                    get_string_slice(args, terminal_context_ptr->command_arguments[arg_index], i+2, end_location);
                    i = end_location;
                }
            }
            else {
                const int32_t end_location = get_char_location(args, ' ', i+1, kstrlen(args));
                if(end_location != -1) {
                    get_string_slice(args, terminal_context_ptr->command_arguments[arg_index], i+1, end_location);
                    i = end_location-1;
                }
            }
            ++arg_index;
        }
    }
}


// puts the rest of a string after the first occurrence of search_term into dest. Returns 0 for success, -1 for string not found
int8_t get_string_section_after(const char *const src, char *const dest, const char *const search_term) {
    size_t src_len = kstrlen(src);
    size_t search_term_len = kstrlen(search_term);
    for(size_t i = 0; i < src_len; ++i) {
        if(src[i] == search_term[0]) {
            //kprintf("%s", "found search term first char at location: ");
            //kprintf("%i\n", i);
            int32_t cmp_status = kstrncmp(src+i, search_term, search_term_len);
            if(cmp_status == 0) {
                //kprintf("%s %i\n", "found entire search term at location: ", i);
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
    default_parse_command_args(context, command);

    if(kstrncmp(command, "echo", 4) == 0) {
        if(kstrlen(command) > 5) {
            kprintf(kstrcat(command + 5, "\n"));
        } else {
            kprintf("'echo' requires one argument!\n");
        }
    } else if(kstrncmp(command, "help", 4) == 0) {
        kprintf("List of available commands:\n");
        kprintf("echo\n");
        kprintf("help\n");
    }
    else {
        kprintf("Invalid command! Try 'help'\n");
    }
}

void default_get_command(void *const context, char *final) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    final[0] = (char)terminal_buffer[terminal_context_ptr->start_of_command];
    final[1] = '\0';
    char temp[2];
    for(size_t i = terminal_context_ptr->start_of_command + 1u; i < terminal_context_ptr->end_of_command; i++) {
        temp[0] = (char)terminal_buffer[i];
        temp[1] = '\0';
        kstrcat(final, temp);
    }
}

