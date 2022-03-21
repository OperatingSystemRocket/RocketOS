#pragma once


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include <drivers/vga_driver/vga_driver.h>


void terminal_context_initialize(void);
void terminal_context_clear(void);
void terminal_context_scroll_clear(void);
void terminal_context_setcolor(uint8_t color);
void terminal_context_resetcolor(void);
void terminal_context_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_context_putchar(char c);
void terminal_context_putchar_color(char c, uint8_t color);
void terminal_context_swapchar(char c);
void terminal_context_swapchar_color(char c, uint8_t color);
void terminal_context_write(const char* text, size_t size);
void terminal_context_writestring(const char* text);
void terminal_context_write_color(const char* text, size_t size, uint8_t color);
void terminal_context_writestring_color(const char* text, uint8_t color);
void terminal_context_scroll_down(void);
void terminal_context_scroll_up(void);
void terminal_context_cursor_up(void);
void terminal_context_cursor_left(void);
void terminal_context_cursor_right(void);
void terminal_context_cursor_down(void);
void terminal_context_backspace(void);
void terminal_context_updatecursor(void);


//unfortunate escape hatch to the global needed by the terminal system
//(unless the user substitutes in or uses a custom vga context)
struct vga_driver_context* get_default_vga_context(void);
