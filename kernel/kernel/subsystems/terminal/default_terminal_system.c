#include "default_terminal_system.h"


static struct default_terminal_context terminal_context;


void default_context_init_context(void) {
    default_init_context(&terminal_context);
}

void default_context_terminal_start(void) {
    run_terminal_start(&terminal_context);
}
void default_context_terminal_end(void) {
    run_terminal_end(&terminal_context);
}
void default_terminal_context_process_command(void) {
    run_terminal_process_command(&terminal_context);
}
void default_terminal_context_shift(void) {
    run_terminal_shift(&terminal_context);
}


void default_context_parse_command_args(const char *const args) {
    default_parse_command_args(&terminal_context, args);
}
void default_context_run_command(char *const command) {
    default_run_command(&terminal_context, command);
}
void default_context_get_command(char *const final, const size_t number_of_elements) {
    default_get_command(&terminal_context, final, number_of_elements);
}


struct default_terminal_context* get_default_terminal_context(void) {
    return &terminal_context;
}
