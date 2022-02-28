#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kstring.h"
#include "hardware_io.h"
#include "terminal_driver.h"


#define VGA_WIDTH 80
#define VGA_HEIGHT 25


//hardware text mode color constants
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


struct vga_driver_context {
    size_t terminal_row;
    size_t terminal_column;
    enum vga_color terminal_color;
    volatile uint16_t* terminal_buffer;
    bool terminal_on;

    uint16_t terminal_upward_history[VGA_WIDTH * VGA_HEIGHT];
    uint16_t terminal_downward_history[VGA_WIDTH * VGA_HEIGHT];
    size_t terminal_upward_history_size;
    size_t terminal_downward_history_size;
};


//these two functions are correct by inspection:
inline uint8_t vga_entry_color(const enum vga_color fg, const enum vga_color bg) {
    return (uint8_t) (fg | bg << 4u);
}

inline uint16_t vga_entry(const char uc, const enum vga_color color) {
    return (uint16_t) ((uint16_t) uc | (uint16_t) color << 8u);
}


void terminal_initialize(struct vga_driver_context* context);
void terminal_clear(struct vga_driver_context* context);
void terminal_scroll_clear(struct vga_driver_context* context);
void terminal_setcolor(struct vga_driver_context* context, enum vga_color color);
void terminal_resetcolor(struct vga_driver_context* context);
void terminal_putentryat(struct vga_driver_context* context, char c, enum vga_color color, size_t x, size_t y);
void terminal_putchar(struct vga_driver_context* context, char c);
void terminal_putchar_color(struct vga_driver_context* context, char c, enum vga_color color);
void terminal_swapchar(struct vga_driver_context* context, char c);
void terminal_swapchar_color(struct vga_driver_context* context, char c, enum vga_color color);
void terminal_write(struct vga_driver_context* context, const char* text, size_t size);
void terminal_writestring(struct vga_driver_context* context, const char* text);
void terminal_write_color(struct vga_driver_context* context, const char* text, size_t size, enum vga_color color);
void terminal_writestring_color(struct vga_driver_context* context, const char* text, enum vga_color color);
void terminal_scroll_down(struct vga_driver_context* context);
void terminal_scroll_up(struct vga_driver_context* context);
void terminal_cursor_up(struct vga_driver_context* context);
void terminal_cursor_left(struct vga_driver_context* context);
void terminal_cursor_right(struct vga_driver_context* context);
void terminal_cursor_down(struct vga_driver_context* context);
void terminal_backspace(struct vga_driver_context* context);
void terminal_updatecursor(struct vga_driver_context* context);
