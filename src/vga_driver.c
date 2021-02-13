#include "vga_driver.h"


static inline uint8_t vga_entry_color(const enum vga_color fg, const enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(const unsigned char uc, const enum vga_color color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}


#define VGA_WIDTH 80
#define VGA_HEIGHT 25


static size_t terminal_row;
static size_t terminal_column;
static enum vga_color terminal_color;
static volatile uint16_t* terminal_buffer;


void terminal_initialize(void) {
    terminal_initialize_test((uint16_t*)0xB8000);
}

void terminal_initialize_test(volatile uint16_t *const terminal_buffer_address) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = terminal_buffer_address;
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

void terminal_putentryat(const char c, const enum vga_color color, const size_t x, const size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(const char c) {
	if (c == '\n') {
		if(++terminal_row == VGA_HEIGHT) {
			terminal_row = 0;
		}
		terminal_column = 0;
		return;
	}
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

void terminal_write(const char *const data, const size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char *const data) {
    terminal_write(data, cstrlen(data));
}

void terminal_write_color(const char *const text, const enum vga_color color) {
    const enum vga_color old_color = terminal_color;
    terminal_setcolor(color);
    terminal_write(text, cstrlen(text));
    terminal_setcolor(old_color);
}
