#include "default_vga_driver.h"


static struct vga_driver_context default_vga_driver_context;


void terminal_context_initialize(void) {
    terminal_initialize(&default_vga_driver_context);
}
void terminal_context_clear(void) {
    terminal_clear(&default_vga_driver_context);
}
void terminal_context_scroll_clear(void) {
    terminal_scroll_clear(&default_vga_driver_context);
}
void terminal_context_setcolor(const uint8_t color) {
    terminal_setcolor(&default_vga_driver_context, color);
}
void terminal_context_resetcolor(void) {
    terminal_resetcolor(&default_vga_driver_context);
}
void terminal_context_putentryat(const char c, const uint8_t color, const size_t x, const size_t y) {
    terminal_putentryat(&default_vga_driver_context, c, color, x, y);
}
void terminal_context_putchar(const char c) {
    terminal_putchar(&default_vga_driver_context, c);
}
void terminal_context_putchar_color(const char c, const uint8_t color) {
    terminal_putchar_color(&default_vga_driver_context, c, color);
}
void terminal_context_swapchar(const char c) {
    terminal_swapchar(&default_vga_driver_context, c);
}
void terminal_context_swapchar_color(const char c, const uint8_t color) {
    terminal_swapchar_color(&default_vga_driver_context, c, color);
}
void terminal_context_write(const char *const text, const size_t size) {
    terminal_write(&default_vga_driver_context, text, size);
}
void terminal_context_writestring(const char *const text) {
    terminal_writestring(&default_vga_driver_context, text);
}
void terminal_context_write_color(const char *const text, const size_t size, const uint8_t color) {
    terminal_write_color(&default_vga_driver_context, text, size, color);
}
void terminal_context_writestring_color(const char *const text, const uint8_t color) {
    terminal_writestring_color(&default_vga_driver_context, text, color);
}
void terminal_context_scroll_down(void) {
    terminal_scroll_down(&default_vga_driver_context);
}
void terminal_context_scroll_up(void) {
    terminal_scroll_up(&default_vga_driver_context);
}
void terminal_context_cursor_up(void) {
    terminal_cursor_up(&default_vga_driver_context);
}
void terminal_context_cursor_left(void) {
    terminal_cursor_left(&default_vga_driver_context);
}
void terminal_context_cursor_right(void) {
    terminal_cursor_right(&default_vga_driver_context);
}
void terminal_context_cursor_down(void) {
    terminal_cursor_down(&default_vga_driver_context);
}
void terminal_context_backspace(void) {
    terminal_backspace(&default_vga_driver_context);
}
void terminal_context_updatecursor(void) {
    terminal_updatecursor(&default_vga_driver_context);
}


struct vga_driver_context* get_default_vga_context(void) {
    return &default_vga_driver_context;
}

