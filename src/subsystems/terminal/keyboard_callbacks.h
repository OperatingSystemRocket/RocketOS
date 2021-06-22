#pragma once


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "kstdio.h"

#include "observer.h"
#include "default_vga_driver.h"
#include "default_keyboard_logic.h"
#include "default_terminal_functions.h"


void catch_keycode(void* context, struct GET_EVENT_TYPENAME(key_message) event);

