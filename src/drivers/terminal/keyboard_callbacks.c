#include "keyboard_callbacks.h"


void catch_keycode(void *const context, struct GET_EVENT_TYPENAME(key_message) event) {
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    if(event.event_type == KEYBOARD_EVENT) {
        if(event.data.key_type == NORMAL_KEY) {
            if(event.data.keycode == '\b') {
                terminal_backspace();
            }
            else if(event.data.keycode == '\t' && terminal_row * 80 + terminal_column >= terminal_context_ptr->start_of_command) {
                terminal_writestring("    ");
                run_terminal_end(context);
            } else if(event.data.keycode > 0 && terminal_row * 80 + terminal_column >= terminal_context_ptr->start_of_command) {
                terminal_putchar(event.data.keycode);
                run_terminal_end(context);
            }
        }
        else if(event.data.key_type == UP_ARROW) {
            terminal_cursor_up();
        }
        else if(event.data.key_type == LEFT_ARROW) {
            terminal_cursor_left();
        }
        else if(event.data.key_type == RIGHT_ARROW) {
            terminal_cursor_right();
        }
        else if(event.data.key_type == DOWN_ARROW) {
            terminal_cursor_down();
        }
    }
}
