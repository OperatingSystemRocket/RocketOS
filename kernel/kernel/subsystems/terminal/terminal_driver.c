#include <subsystems/terminal/terminal_driver.h>



static void (*terminal_start)(void*);
static void (*terminal_end)(void*);
static void (*terminal_process_command)(void*);
static void (*terminal_shift)(void*);


void set_terminal_start(void (*const new_terminal_start)(void*)) {
    terminal_start = new_terminal_start;
}
void set_terminal_end(void (*const new_terminal_end)(void*)) {
    terminal_end = new_terminal_end;
}
void set_terminal_process_command(void (*const new_terminal_process_command)(void*)) {
    terminal_process_command = new_terminal_process_command;
}
void set_terminal_shift(void (*const new_terminal_shift)(void*)) {
    terminal_shift = new_terminal_shift;
}



void run_terminal_start(void *const context) {
    terminal_start(context);
}
void run_terminal_end(void *const context) {
    terminal_end(context);
}
void run_terminal_process_command(void *const context) {
    terminal_process_command(context);
}
void run_terminal_shift(void *const context) {
    terminal_shift(context);
}


