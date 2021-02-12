#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "cstring.h"

/* Hardware text mode color constants. */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

void terminal_initialize(volatile uint16_t *terminal_buffer_address);

void terminal_setcolor(enum vga_color color);

void terminal_putentryat(char c, enum vga_color color, size_t x, size_t y);

void terminal_putchar(char c);

void terminal_write(const char *data, size_t size);

void terminal_writestring(const char *data);

void terminal_write_color(const char *text, enum vga_color color);
