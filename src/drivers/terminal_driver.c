#include <terminal_driver.h>

void terminal_start(void) {
    terminal_on = true;
    terminal_writestring(PROMPT);
    start_of_command = terminal_row * 80 + terminal_column;
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
    }
    char command_string[80];
    get_command(command_string);
    kprintf(kstrcat(command_string, "\n"));
    terminal_start();
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
