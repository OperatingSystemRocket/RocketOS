#include <subsystems/terminal/keyboard_callbacks.h>


bool is_turned_on = false;

void catch_keycode(void *const context, struct GET_EVENT_TYPENAME(key_message) event) {
    kprintf("catch_keycode: %c\n", event.data.keycode);
    struct default_terminal_context *const terminal_context_ptr = (struct default_terminal_context*) context;

    if(event.event_type == KEYBOARD_EVENT) {
        if(event.data.key_type == NORMAL_KEY) {
            if(event.data.keycode == '\b') {
                terminal_backspace(terminal_context_ptr->vga_context);
                return;
            } else if(terminal_context_ptr->vga_context->terminal_column == 0) {
                default_init_context(terminal_context_ptr);

                terminal_writestring(terminal_context_ptr->vga_context, terminal_context_ptr->prompt_symbol);

                terminal_context_ptr->start_of_command += kstrlen(terminal_context_ptr->prompt_symbol);
            }

            if(event.data.keycode == '\t' && terminal_context_ptr->vga_context->terminal_row * 80 + terminal_context_ptr->vga_context->terminal_column >= terminal_context_ptr->start_of_command) {
                terminal_writestring(terminal_context_ptr->vga_context, "    ");
                run_terminal_end(context);
            } else if(event.data.keycode == '\n') {
                run_terminal_end(context);
                terminal_putchar(terminal_context_ptr->vga_context, event.data.keycode);
                run_terminal_process_command(context);
                run_terminal_end(context);
            } else if(event.data.keycode > 0 && terminal_context_ptr->vga_context->terminal_row * 80 + terminal_context_ptr->vga_context->terminal_column >= terminal_context_ptr->start_of_command) {
                terminal_putchar(terminal_context_ptr->vga_context, event.data.keycode);
                run_terminal_end(context);
            }
        }
        else if(event.data.key_type == UP_ARROW) {
            terminal_cursor_up(terminal_context_ptr->vga_context);
        }
        else if(event.data.key_type == LEFT_ARROW) {
            terminal_cursor_left(terminal_context_ptr->vga_context);
        }
        else if(event.data.key_type == RIGHT_ARROW) {
            terminal_cursor_right(terminal_context_ptr->vga_context);
        }
        else if(event.data.key_type == DOWN_ARROW) {
            terminal_cursor_down(terminal_context_ptr->vga_context);
        }

        if(event.data.keycode == '1') {
            init_pit(1000, PIT_CHANNEL_0, ACCESS_MODE_LOBYTE_HIBYTE, PIT_MODE_SQUARE_WAVE_GENERATOR);
        } else if (event.data.keycode == '2') {
            init_pit(18, PIT_CHANNEL_0, ACCESS_MODE_LOBYTE_HIBYTE, PIT_MODE_SQUARE_WAVE_GENERATOR);
        } else if(event.data.keycode == '3') {
            is_turned_on = true;
            play_sound(1000);
        } else if(event.data.keycode == '4') {
            is_turned_on = true;
            play_sound(18);
        } else if(event.data.keycode == '5') {
            is_turned_on = true;
            play_sound(500);
        } else if(event.data.keycode == '6') {
            is_turned_on = false;
            no_sound();
            restore_timer();
        }
    }
}
