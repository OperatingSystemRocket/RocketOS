#include "terminal_driver.h"

size_t start_of_command;
size_t end_of_command;

void run_command(char *const command) {
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
    } else {
        kprintf("Invalid command! Try 'help'\n");
    }
}

void terminal_start(void) {
    terminal_on = true;
    terminal_writestring(PROMPT);
    start_of_command = terminal_row * 80 + terminal_column;
    end_of_command = start_of_command;
}

void terminal_end(void) {
    end_of_command = terminal_row * 80 + terminal_column;
}

void terminal_process_command(void) {
    terminal_on = false;
    if(end_of_command - start_of_command <= 0) {
        kprintf("Invalid command! Try 'help'\n");
        terminal_start();
        return;
    } else if(end_of_command - start_of_command > 79) {
        kprintf("Command is too long! Limit of 79 characters! Cutting down to length...\n");
        end_of_command = start_of_command + 79;
    }
    char command_string[80];
    get_command(command_string);
    run_command(command_string);
    terminal_start();
}

void terminal_shift(void) {
    start_of_command -= 80;
    end_of_command -= 80;
}

void get_command(char final[]) {
    final[0] = (char)terminal_buffer[start_of_command];
    final[1] = '\0';
    char temp[2];
    for(size_t i = start_of_command + 1; i < end_of_command; i++) {
        temp[0] = (char)terminal_buffer[i];
        temp[1] = '\0';
        kstrcat(final, temp);
    }
}
