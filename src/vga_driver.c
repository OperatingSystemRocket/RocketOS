#include "vga_driver.h"


static inline uint8_t vga_entry_color(const enum vga_color fg, const enum vga_color bg) {
    return fg | bg << 4u;
}

static inline uint16_t vga_entry(const char uc, const enum vga_color color) {
    return (uint16_t) uc | (uint16_t) color << 8u;
}


#define VGA_WIDTH 80
#define VGA_HEIGHT 25


static size_t terminal_row;
static size_t terminal_column;
static enum vga_color terminal_color;
static volatile uint16_t* terminal_buffer;
static volatile uint16_t terminal_upward_history[VGA_WIDTH * VGA_HEIGHT];
static volatile uint16_t terminal_downward_history[VGA_WIDTH * VGA_HEIGHT];
static size_t terminal_upward_history_size;
static size_t terminal_downward_history_size;


void terminal_initialize(void) {
    terminal_initialize_test((uint16_t*)0xB8000);
}

void terminal_initialize_test(uint16_t *const terminal_buffer_address) {
    terminal_resetcolor();
    terminal_buffer = terminal_buffer_address;
    terminal_upward_history_size = 0;
    terminal_downward_history_size = 0;
    terminal_clear();
}

void terminal_clear(void) {
    terminal_row = 0;
    terminal_column = 0;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(const enum vga_color color) {
    terminal_color = color;
}

void terminal_resetcolor(void) {
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

void terminal_putentryat(const char c, const enum vga_color color, const size_t x, const size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(const char c) {
	if (c == '\n') {
		if(++terminal_row == VGA_HEIGHT) {
			terminal_scroll_down();
		}
		terminal_column = 0;
		return;
	}
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
			terminal_scroll_down();
        }
    }
}

void terminal_write(const char *const text, const size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(text[i]);
    }
}

void terminal_writestring(const char *const text) {
    terminal_write(text, cstrlen(text));
}

void terminal_write_color(const char *const text, const size_t size, const enum vga_color color) {
    const enum vga_color old_color = terminal_color;
    terminal_setcolor(color);
    terminal_write(text, size);
    terminal_setcolor(old_color);
}

void terminal_writestring_color(const char *const text, const enum vga_color color) {
    terminal_write_color(text, cstrlen(text), color);
}

void terminal_scroll_down(void) {
    for(size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_upward_history[terminal_upward_history_size++] = terminal_buffer[x];
    }
	for(size_t y = 0; y < VGA_HEIGHT - 1; y++) {
		for(size_t x = 0; x < VGA_WIDTH; x++) {
			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
		}
	}
    if (terminal_downward_history_size == 0) {
	    for(size_t x = 0; x < VGA_WIDTH; x++) {
	    	terminal_putentryat(' ', terminal_color, x, VGA_HEIGHT - 1);
	    }
    } else {
        for(size_t x = terminal_downward_history_size - 1; x > 0; x--) {
            terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = terminal_downward_history[--terminal_downward_history_size];
        }
    }
	terminal_row = VGA_HEIGHT - 1;
	terminal_column = 0;
}

void terminal_scroll_up(void) {
    if(terminal_upward_history_size == 0) return;
    for(size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_downward_history[terminal_downward_history_size++] = terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x];
    }
	for(size_t y = VGA_HEIGHT - 1; y > 0; y--) {
		for(size_t x = 0; x < VGA_WIDTH; x++) {
			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y - 1) * VGA_WIDTH + x];
		}
	}
    for(size_t x = 1; x < VGA_WIDTH + 1; x++) {
        terminal_buffer[VGA_WIDTH - x] = terminal_upward_history[--terminal_downward_history_size];
    }
	terminal_row = VGA_HEIGHT - 1;
	terminal_column = 0;
}
