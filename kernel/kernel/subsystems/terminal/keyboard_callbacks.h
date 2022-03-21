#pragma once


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <kstdio.h>

#include <utils/communication_and_events/observer.h>
#include <drivers/vga_driver/default_vga_driver.h>
#include <drivers/keyboard/default_keyboard_logic.h>
#include <subsystems/terminal/default_terminal_functions.h>


//for testing purposes only
#include <drivers/pit/pit.h>


void catch_keycode(void* context, struct GET_EVENT_TYPENAME(key_message) event);

