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
        terminal_start();
        return;
    }
    const char* command_string = get_command();
    terminal_writestring(command_string);
    terminal_start();
}

const char* get_command(void) {
    char* final;
    char* temp;
    for(size_t i = start_of_command; i <= end_of_command; i++) {
        *temp = (char)terminal_buffer[i];
        *(temp + 1) = '\0';
        kstrcat(final, temp);
    }
    return final;
}
