#include "keyboard_driver.h"

void process_keystroke(char keycode) {
    char final;
    switch(keycode) {
        case 14:
           //backspace
            terminal_backspace();
           break;
        case 15:
           //tab
           terminal_writestring("    ");
        case 42:
           //left shift
           break;
        case 54:
           //right shift
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
            if(caps_lock) {
                final = keyboard_map[keycode] - 32;
            } else {
                final = keyboard_map[keycode];
            }
            if(final > 0) {
                terminal_putchar(final);
            }
           break;
    }
}
