#include "keyboard_driver.h"

void process_keystroke(char keycode) {
   terminal_putchar(keyboard_map[keycode]); 
}
