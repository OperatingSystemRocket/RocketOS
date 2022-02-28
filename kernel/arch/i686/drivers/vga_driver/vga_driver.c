#include "vga_driver.h"


uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t vga_entry(char uc, enum vga_color color);


void terminal_initialize(struct vga_driver_context *const context) {
    terminal_resetcolor(context);
    context->terminal_buffer = (volatile uint16_t*)0xB8000u;
    context->terminal_upward_history_size = 0;
    context->terminal_downward_history_size = 0;
    terminal_clear(context);
}

void terminal_clear(struct vga_driver_context *const context) {
    context->terminal_row = 0u;
    context->terminal_column = 0u;

    for (size_t y = 0u; y < VGA_HEIGHT; y++) {
        for (size_t x = 0u; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            context->terminal_buffer[index] = vga_entry(' ', context->terminal_color);
        }
    }

    context->terminal_upward_history_size = 0;
    context->terminal_downward_history_size = 0;
}

void terminal_scroll_clear(struct vga_driver_context *const context) {
    for(int i = 0; i < 25; ++i) {
        terminal_writestring(context, "\n");
    }

    context->terminal_row = 0u;
    context->terminal_column = 0u;
}

void terminal_setcolor(struct vga_driver_context *const context, const enum vga_color color) {
    context->terminal_color = color;
}

void terminal_resetcolor(struct vga_driver_context *const context) {
    terminal_setcolor(context, vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void terminal_putentryat(struct vga_driver_context *const context, const char c, const enum vga_color color, const size_t x, const size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    context->terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(struct vga_driver_context *const context, const char c) {
	if (c == '\n') {
		if(++context->terminal_row == VGA_HEIGHT) {
			terminal_scroll_down(context);
		}
		context->terminal_column = 0u;
        terminal_updatecursor(context);
		return;
	}
    if(c == '\t') {
        return terminal_writestring(context, "    ");
    }
    terminal_putentryat(context, c, context->terminal_color, context->terminal_column, context->terminal_row);
    if (++context->terminal_column == VGA_WIDTH) {
        context->terminal_column = 0;
        if (++context->terminal_row == VGA_HEIGHT) {
			terminal_scroll_down(context);
        }
    }
    terminal_updatecursor(context);
}

void terminal_putchar_color(struct vga_driver_context *const context, const char c, const enum vga_color color) {
    const enum vga_color old_color = context->terminal_color;
    terminal_setcolor(context, color);
    terminal_putchar(context, c);
    terminal_setcolor(context, old_color);
}

void terminal_swapchar(struct vga_driver_context *const context, const char c) {
    terminal_putentryat(context, c, context->terminal_color, context->terminal_column, context->terminal_row);
}

void terminal_swapchar_color(struct vga_driver_context *const context, const char c, const enum vga_color color) {
    const enum vga_color old_color = context->terminal_color;
    terminal_setcolor(context, color);
    terminal_swapchar(context, c);
    terminal_setcolor(context, old_color);
}

void terminal_write(struct vga_driver_context *const context, const char *const text, const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        terminal_putchar(context, text[i]);
    }
}

void terminal_writestring(struct vga_driver_context *const context, const char *const text) {
    terminal_write(context, text, kstrlen(text));
}

void terminal_write_color(struct vga_driver_context *const context, const char *const text, const size_t size, const enum vga_color color) {
    const enum vga_color old_color = context->terminal_color;
    terminal_setcolor(context, color);
    terminal_write(context, text, size);
    terminal_setcolor(context, old_color);
}

void terminal_writestring_color(struct vga_driver_context *const context, const char *const text, const enum vga_color color) {
    terminal_write_color(context, text, kstrlen(text), color);
}

//TODO: fix this weird coupling
void default_terminal_context_shift(void);

void terminal_scroll_down(struct vga_driver_context *const context) {
    for(size_t x = 0u; x < VGA_WIDTH; x++) {
        if(context->terminal_upward_history_size >= VGA_WIDTH * VGA_HEIGHT) context->terminal_upward_history_size = 0;
        context->terminal_upward_history[context->terminal_upward_history_size++] = context->terminal_buffer[x];
    }
    for(size_t y = 0u; y < VGA_HEIGHT - 1; y++) {
        for(size_t x = 0u; x < VGA_WIDTH; x++) {
            context->terminal_buffer[y * VGA_WIDTH + x] = context->terminal_buffer[(y + 1) * VGA_WIDTH + x];
        }
    }
    if(context->terminal_downward_history_size == 0u) {
        for(size_t x = 0u; x < VGA_WIDTH; x++) {
            terminal_putentryat(context, ' ', VGA_COLOR_WHITE, x, VGA_HEIGHT - 1);
        }
    } else {
        for(size_t x = VGA_WIDTH; x > 0u; x--) {
            context->terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x - 1] = context->terminal_downward_history[--context->terminal_downward_history_size];
        }
    }
    context->terminal_row = VGA_HEIGHT - 1;
    context->terminal_column = 0u;

    //TODO: fix this weird coupling
    default_terminal_context_shift();
}

void terminal_scroll_up(struct vga_driver_context *const context) {
    if(context->terminal_upward_history_size == 0u) return;
    for(size_t x = 0u; x < VGA_WIDTH; x++) {
        if(context->terminal_downward_history_size >= VGA_WIDTH * VGA_HEIGHT) context->terminal_downward_history_size = 0;
        context->terminal_downward_history[context->terminal_downward_history_size++] = context->terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x];
    }
	for(size_t y = VGA_HEIGHT - 1; y > 0u; y--) {
		for(size_t x = 0; x < VGA_WIDTH; x++) {
			context->terminal_buffer[y * VGA_WIDTH + x] = context->terminal_buffer[(y - 1) * VGA_WIDTH + x];
		}
	}
    for(size_t x = 1u; x < VGA_WIDTH + 1; x++) {
        context->terminal_buffer[VGA_WIDTH - x] = context->terminal_upward_history[--context->terminal_upward_history_size];
    }
	context->terminal_row = 0u;
	context->terminal_column = 0u;
}

void terminal_cursor_up(struct vga_driver_context *const context) {
    if(context->terminal_row-- == 0) {
        terminal_scroll_up(context);
        context->terminal_row = 0;
    }
    terminal_updatecursor(context);
}

void terminal_cursor_left(struct vga_driver_context *const context) {
    if(context->terminal_column-- == 0) {
        context->terminal_column = 0;
    }
    terminal_updatecursor(context);
}

void terminal_cursor_right(struct vga_driver_context *const context) {
    if(++context->terminal_column > 79) {
        context->terminal_column = 79;
    }
    terminal_updatecursor(context);
}

void terminal_cursor_down(struct vga_driver_context *const context) {
    if(++context->terminal_row > 24) {
        if(context->terminal_downward_history_size > 0) terminal_scroll_down(context);
        context->terminal_row = 24;
    }
    terminal_updatecursor(context);
}

void terminal_backspace(struct vga_driver_context *const context) {
    if(context->terminal_column-- == 0) {
        context->terminal_column = 0;
    } else {
        terminal_swapchar(context, ' ');
    }
    terminal_updatecursor(context);
}

void terminal_updatecursor(struct vga_driver_context *const context) {
	const size_t pos = context->terminal_row * VGA_WIDTH + context->terminal_column;
	outb(0x3D4u, 14u);
	outb(0x3D5u, (uint8_t)(pos >> 8u));
	outb(0x3D4u, 15u);
	outb(0x3D5u, (uint8_t)pos);
}
