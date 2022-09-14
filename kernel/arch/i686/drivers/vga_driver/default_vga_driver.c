#include <drivers/vga_driver/default_vga_driver.h>


static struct vga_driver_context default_vga_driver_context;

static mutex_t global_vga_context_spinlock = false;


void terminal_context_initialize(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_initialize(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_clear(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_clear(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_scroll_clear(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_scroll_clear(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_setcolor(const uint8_t color) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_setcolor(&default_vga_driver_context, color);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_resetcolor(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_resetcolor(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_putentryat(const char c, const uint8_t color, const size_t x, const size_t y) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_putentryat(&default_vga_driver_context, c, color, x, y);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_putchar(const char c) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_putchar(&default_vga_driver_context, c);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_putchar_color(const char c, const uint8_t color) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_putchar_color(&default_vga_driver_context, c, color);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_swapchar(const char c) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_swapchar(&default_vga_driver_context, c);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_swapchar_color(const char c, const uint8_t color) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_swapchar_color(&default_vga_driver_context, c, color);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_write(const char *const text, const size_t size) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_write(&default_vga_driver_context, text, size);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_writestring(const char *const text) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_writestring(&default_vga_driver_context, text);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_write_color(const char *const text, const size_t size, const uint8_t color) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_write_color(&default_vga_driver_context, text, size, color);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_writestring_color(const char *const text, const uint8_t color) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_writestring_color(&default_vga_driver_context, text, color);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_scroll_down(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_scroll_down(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_scroll_up(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_scroll_up(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_cursor_up(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_cursor_up(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_cursor_left(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_cursor_left(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_cursor_right(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_cursor_right(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_cursor_down(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_cursor_down(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_backspace(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_backspace(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}
void terminal_context_updatecursor(void) {
    acquire_spinlock(&global_vga_context_spinlock);
    terminal_updatecursor(&default_vga_driver_context);
    release_spinlock(&global_vga_context_spinlock);
}


struct vga_driver_context* get_default_vga_context(void) {
    return &default_vga_driver_context;
}

