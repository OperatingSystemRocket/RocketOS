#include "terminal_driver.h"

#define MAX_NUMBER_OF_ARGS 5
#define MAX_ARGUMENT_SIZE 64

char test_file[16] = "\0ELF\0\0\0\0\0\0\0\0\0\0\0\0";
size_t start_of_command;
size_t end_of_command;

char disk_src_buf[512];
char disk_dest_buf[512] = "don't want this";
char command_arguments[MAX_NUMBER_OF_ARGS][MAX_ARGUMENT_SIZE];

/// Expects just arguments. Use kstrstr_end to remove the command
void parse_command_args(const char* args) {
    size_t args_len = kstrlen(args);

    uint8_t arg_index = 0;
    size_t current_arg_char_index = 0;
    size_t current_arg_start = 0;

    for(size_t i = 0; i < args_len; ++i) {
        current_arg_char_index = i - current_arg_start;
        command_arguments[arg_index][current_arg_char_index] = args[i];

        if(args[i] == ' ' && i != args_len-1) {
            if(args[i+1] == '\"') {
                i+=2;
                current_arg_start = i;
                while(args[i] != '\"') {
                    current_arg_char_index = i - current_arg_start;

                    ++i;
                  }
                  kmemcpy(command_arguments[arg_index+1], args + current_arg_start, i-current_arg_start);
                  i+=2;
                }
            else {
                kmemcpy(command_arguments[arg_index], args + current_arg_start, i-current_arg_start);
                current_arg_start = i+1;
                }
                command_arguments[arg_index][i] = '\0';
            ++arg_index;
        }
    }
}

// puts the rest of a string after the first occurrence of search_term into dest. Returns 0 for success, -1 for string not found
int8_t get_string_section_after(const char* const src, char* const dest, const char* const search_term) {
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

void run_command(char *const command) {
    if(kstrncmp(command, "echo", 4) == 0) {
        if(kstrlen(command) > 5) {
            kprintf(kstrcat(command + 5, "\n"));
        } else {
            kprintf("'echo' requires one argument!\n");
        }
    } else if(kstrncmp(command, "load", 4) == 0) {
        if(kstrlen(command) > 5) {
            kprintf("Loading file...");
            load_elf_file(test_file); 
            kprintf("Done!\n");
        } else {
            kprintf("'load' requires one argument!\n");
        }
    } else if(kstrncmp(command, "help", 4) == 0) {
        kprintf("List of available commands:\n");
        kprintf("echo\n");
        kprintf("load\n");
        kprintf("help\n");
    } else if(kstrncmp(command, "wrtdsk", 6) == 0) {
        if(kstrlen(command) <= 7) {
            kprintf("%s\n", "wrtdsk requires arguments");
        }
        else {
            kstrcpy(disk_src_buf, kstrstr_end(command, "wrtdsk "));
            ide_write_sectors(0, 1, 0, 0, (unsigned int) disk_src_buf);
        }
    } else if(kstrncmp(command, "prntdsk", 6) == 0) {
        ide_read_sectors(0,1,0,0, (unsigned int)disk_dest_buf);
        terminal_writestring(disk_dest_buf);
        terminal_putchar('\n');
    }
    else if(kstrncmp(command, "test", 4) == 0) {
        parse_command_args(kstrstr_end(command, "test "));
        kprintf("%s\n", "Command args: ");
        kprintf("%s\n%s\n%s\n", command_arguments[0], command_arguments[1], command_arguments[2]);
    }
    else {
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
