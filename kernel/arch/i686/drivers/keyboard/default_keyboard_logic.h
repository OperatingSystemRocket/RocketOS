#pragma once

#include <stdbool.h>

#include <drivers/vga_driver/default_vga_driver.h>
#include <drivers/keyboard/keyboard_driver.h>
#include <utils/communication_and_events/observer.h>


//for keys which are just 0 in the default keymap
enum type_of_key {
    CONTROL = 0u,
    LEFT_SHIFT = 1u,
    RIGHT_SHIFT = 2u,
    ALT = 3u,
    CAPS_LOCK = 4u,
    F1 = 5u,
    F2 = 6u,
    F3 = 7u,
    F4 = 8u,
    F5 = 9u,
    F6 = 10u,
    F7 = 11u,
    F8 = 12u,
    F9 = 13u,
    F10 = 14u,
    NUMS_LOCK = 15u,
    SCROLL_LOCK = 16u,
    HOME_KEY = 17u,
    UP_ARROW = 18u,
    PAGE_UP = 19u,
    LEFT_ARROW = 20u,
    RIGHT_ARROW = 21u,
    END_KEY = 22u,
    DOWN_ARROW = 23u,
    PAGE_DOWN = 24u,
    INSERT_KEY = 25u,
    DELETE_KEY = 26u,
    F11 = 27u,
    F12 = 28u,

    NORMAL_KEY = 29u, //is a number, letter, or displayable symbol

    INVALID_KEY = 30u,
};

struct key_message {
    char keycode;
    size_t key_type; //by default uses the constants defined in `enum type_of_key`
};


GENERATE_SUBJECT_DECLARATION(128, key_message)


struct default_keyboard_map_state {
    bool rshift;
    bool lshift;
    bool caps_lock;
    const char* keyboard_map; //must be 128 entries
    struct GET_SUBJECT_TYPENAME(128, key_message) subject;
};

struct GET_SUBJECT_TYPENAME(128, key_message)* get_subject(void);


void process_keystroke(void* context, unsigned char keycode);
void default_keyboard_map_state_init(void);
