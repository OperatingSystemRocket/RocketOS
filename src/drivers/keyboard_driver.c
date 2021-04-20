#include "keyboard_driver.h"

void process_keystroke(const char keycode) {
    char final;
    switch(keycode) {
        case 14:
            //backspace
            terminal_backspace();
            break;
        case 15:
            //tab
            if(terminal_row * 80 + terminal_column >= start_of_command) {
                terminal_writestring("    ");
                terminal_end();
            }
        case 42:
            //left shift
            lshift = true;
            break;
        case 54:
            //right shift
            rshift = true;
            break;
        case 58:
            //caps lock
            caps_lock = !caps_lock;
            break;
        case 72:
            //up arrow
            terminal_cursor_up();
            break;
        case 75:
            //left
            terminal_cursor_left();
            break;
        case 77:
            //right
            terminal_cursor_right();
            break;
        case 80:
            //down
            terminal_cursor_down();
            break;
        default:
            final = keyboard_map[keycode];
            if(caps_lock && final >= 97 && final <= 122 && !lshift && !rshift) {
                final -= 32;
            } else if(lshift || rshift) {
                if(final >= 97 && final <= 122 && !caps_lock) {
                    final -= 32;
                } else if (final == '`') {
                    final = '~';
                } else if (final == '1') {
                    final = '!';
                } else if (final == '2') {
                    final = '@';
                } else if (final == '3') {
                    final = '#';
                } else if (final == '4') {
                    final = '$';
                } else if (final == '5') {
                    final = '%';
                } else if (final == '6') {
                    final = '^';
                } else if (final == '7') {
                    final = '&';
                } else if (final == '8') {
                    final = '*';
                } else if (final == '9') {
                    final = '(';
                } else if (final == '0') {
                    final = ')';
                } else if (final == '-') {
                    final = '_';
                } else if (final == '=') {
                    final = '+';
                } else if (final == '[') {
                    final = '{';
                } else if (final == ']') {
                    final = '}';
                } else if (final == '\\') {
                    final = '|';
                } else if (final == ';') {
                    final = ':';
                } else if (final == '\'') {
                    final = '\"';
                } else if (final == ',') {
                    final = '<';
                } else if (final == '.') {
                    final = '>';
                } else if (final == '/') {
                    final = '?';
                }
            }
            if(final > 0 && terminal_row * 80 + terminal_column >= start_of_command) {
                terminal_putchar(final);
                terminal_end();
            }
            break;
    }
}
