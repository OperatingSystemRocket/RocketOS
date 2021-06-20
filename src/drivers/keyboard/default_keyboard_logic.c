#include "default_keyboard_logic.h"


static struct default_keyboard_map_state keyboard_map_state;

static const char default_keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */

    '9', '0', '-', '=', '\b',	/* Backspace */

    '\t',			/* Tab */

    'q', 'w', 'e', 'r',	/* 19 */

    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */

    0,			/* 29   - Control */

    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */

    '\'', '`',   0,		/* Left shift */

    '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */

    'm', ',', '.', '/',   0,				/* Right shift */

    '*',

    0,	/* Alt */

    ' ',	/* Space bar */

    0,	/* Caps lock */

    0,	/* 59 - F1 key ... > */

    0,   0,   0,   0,   0,   0,   0,   0,

    0,	/* < ... F10 */

    0,	/* 69 - Num lock*/

    0,	/* Scroll Lock */

    0,	/* Home key */

    0,	/* Up Arrow */

    0,	/* Page Up */

    '-',

    0,	/* Left Arrow */

    0,

    0,	/* Right Arrow */

    '+',

    0,	/* 79 - End key*/

    0,	/* Down Arrow */

    0,	/* Page Down */

    0,	/* Insert Key */

    0,	/* Delete Key */

    0,   0,   0,

    0,	/* F11 Key */

    0,	/* F12 Key */

    0,	/* All other keys are undefined */
};


GENERATE_SUBJECT_DEFINITION(128, key_message)


struct GET_SUBJECT_TYPENAME(128, key_message)* get_subject(void) {
    return &keyboard_map_state.subject;
}


void pic_send_eoi(uint8_t no);

void process_keystroke(void *const context, const unsigned char keycode) {
    struct default_keyboard_map_state *const keyboard_map_context = (struct default_keyboard_map_state*) context;

    if(keycode & 128u) {
        // This is a release scancode, just ignore it
        //TODO: maybe add an enum entry and raise events for released keys
        if(keycode == 170) keyboard_map_context->lshift = false;
        if(keycode == 182) keyboard_map_context->rshift = false;
        return;
    }

    char final;
    struct GET_EVENT_TYPENAME(key_message) event;
    switch(keycode) {
        case 14:
            //backspace
            event = (struct GET_EVENT_TYPENAME(key_message)) {.event_type = KEYBOARD_EVENT, .data = (struct key_message){ .keycode = '\b', .key_type = NORMAL_KEY } };
            //terminal_backspace();
            break;
        case 15:
            //tab
            event = (struct GET_EVENT_TYPENAME(key_message)) {.event_type = KEYBOARD_EVENT, .data = (struct key_message){ .keycode = '\t', .key_type = NORMAL_KEY } };
            /*if(terminal_row * 80 + terminal_column >= start_of_command) {
                terminal_writestring("    ");
                terminal_end();
            }*/
            break;
        case 42:
            //left shift
            //TODO: throw event
            keyboard_map_context->lshift = true;
            break;
        case 54:
            //right shift
            //TODO: throw event
            keyboard_map_context->rshift = true;
            break;
        case 58:
            //caps lock
            //TODO: throw event
            keyboard_map_context->caps_lock = !keyboard_map_context->caps_lock;
            break;
        case 72:
            //up arrow
            event = (struct GET_EVENT_TYPENAME(key_message)) { .event_type = KEYBOARD_EVENT, .data = (struct key_message){ .keycode = 0, .key_type = UP_ARROW } };
            //terminal_cursor_up();
            break;
        case 75:
            //left
            event = (struct GET_EVENT_TYPENAME(key_message)) { .event_type = KEYBOARD_EVENT, .data = (struct key_message){ .keycode = 0, .key_type = LEFT_ARROW } };
            //terminal_cursor_left();
            break;
        case 77:
            //right
            event = (struct GET_EVENT_TYPENAME(key_message)) { .event_type = KEYBOARD_EVENT, .data = (struct key_message){ .keycode = 0, .key_type = RIGHT_ARROW } };
            //terminal_cursor_right();
            break;
        case 80:
            //down
            event = (struct GET_EVENT_TYPENAME(key_message)) { .event_type = KEYBOARD_EVENT, .data = (struct key_message){ .keycode = 0, .key_type = DOWN_ARROW } };
            //terminal_cursor_down();
            break;
        default:
            final = keyboard_map_context->keyboard_map[keycode];
            if(keyboard_map_context->caps_lock && final >= 97 && final <= 122 && !keyboard_map_context->lshift && !keyboard_map_context->rshift) {
                final -= 32;
            } else if(keyboard_map_context->lshift || keyboard_map_context->rshift) {
                if(final >= 97 && final <= 122 && !keyboard_map_context->caps_lock) {
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
            if(final > 0) {
                event = (struct GET_EVENT_TYPENAME(key_message)) {.event_type = KEYBOARD_EVENT, .data = (struct key_message){ .keycode = final, .key_type = NORMAL_KEY } };
            }
            /*if(final > 0 && terminal_row * 80 + terminal_column >= start_of_command) {
                terminal_putchar(final);
                terminal_end();
            }*/
            break;
    }
    NOTIFY(128, key_message, &keyboard_map_context->subject, event);
}

void default_keyboard_map_state_init(void) {
    keyboard_map_state.rshift = false;
    keyboard_map_state.lshift = false;

    keyboard_map_state.caps_lock = false;
    keyboard_map_state.keyboard_map = default_keyboard_map;

    keyboard_map_state.subject.num_of_active_observers = 0u;

    set_data_context(&keyboard_map_state);
    set_process_keystroke_implementation(process_keystroke);
}
