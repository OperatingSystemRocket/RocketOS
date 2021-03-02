#pragma once
#include "keyboard_driver.h"

void process_keystroke(unsigned char status, char keycode) {
   terminal_putchar(keyboard_map[keycode]); 
}
