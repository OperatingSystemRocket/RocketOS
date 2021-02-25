# 1 "src/vga_driver.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/vga_driver.c"
# 1 "src/vga_driver.h" 1
       

# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdbool.h" 1 3 4
# 4 "src/vga_driver.h" 2
# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stddef.h" 1 3 4
# 143 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stddef.h" 3 4

# 143 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stddef.h" 3 4
typedef long int ptrdiff_t;
# 209 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 321 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stddef.h" 3 4
typedef int wchar_t;
# 415 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stddef.h" 3 4
typedef struct {
  long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
  long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));






  __float128 __max_align_f128 __attribute__((__aligned__(__alignof(__float128))));

} max_align_t;
# 5 "src/vga_driver.h" 2
# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint.h" 1 3 4
# 11 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint.h" 3 4
# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint-gcc.h" 1 3 4
# 34 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint-gcc.h" 3 4
typedef signed char int8_t;


typedef short int int16_t;


typedef long int int32_t;


typedef long long int int64_t;


typedef unsigned char uint8_t;


typedef short unsigned int uint16_t;


typedef long unsigned int uint32_t;


typedef long long unsigned int uint64_t;




typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef long int int_least32_t;
typedef long long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef short unsigned int uint_least16_t;
typedef long unsigned int uint_least32_t;
typedef long long unsigned int uint_least64_t;



typedef int int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long long int int_fast64_t;
typedef unsigned int uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef long long unsigned int uint_fast64_t;




typedef long int intptr_t;


typedef long unsigned int uintptr_t;




typedef long long int intmax_t;
typedef long long unsigned int uintmax_t;
# 12 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint.h" 2 3 4
# 6 "src/vga_driver.h" 2

# 1 "src/kstring.h" 1
       

# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stddef.h" 1 3 4
# 4 "src/kstring.h" 2


# 1 "src/kassert.h" 1
       

# 1 "src/kstdio.h" 1
       

# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stddef.h" 1 3 4
# 4 "src/kstdio.h" 2

# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdarg.h" 1 3 4
# 40 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 99 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdarg.h" 3 4
typedef __gnuc_va_list va_list;
# 6 "src/kstdio.h" 2







# 12 "src/kstdio.h"
int kprintf(const char *format, ...);
# 4 "src/kassert.h" 2
# 7 "src/kstring.h" 2

size_t kstrlen(const char* str);

int32_t kstrcmp(const char* lhs, const char* rhs);

void kint_to_string(int64_t input, char* string_ret, size_t ret_size);
# 8 "src/vga_driver.h" 2


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


void terminal_initialize(void);


void terminal_initialize_test(uint16_t *terminal_buffer_address);

void terminal_clear(void);

void terminal_setcolor(enum vga_color color);

void terminal_resetcolor(void);

void terminal_putentryat(char c, enum vga_color color, size_t x, size_t y);

void terminal_putchar(char c);

void terminal_putchar_color(char c, enum vga_color color);

void terminal_write(const char* text, size_t size);

void terminal_writestring(const char* text);

void terminal_write_color(const char *const text, const size_t size, const enum vga_color color);

void terminal_writestring_color(const char *const text, const enum vga_color color);

void terminal_scroll_down(void);

void terminal_scroll_up(void);
# 2 "src/vga_driver.c" 2


static inline uint8_t vga_entry_color(const enum vga_color fg, const enum vga_color bg) {
    return fg | bg << 4u;
}

static inline uint16_t vga_entry(const char uc, const enum vga_color color) {
    return (uint16_t) uc | (uint16_t) color << 8u;
}






static size_t terminal_row;
static size_t terminal_column;
static enum vga_color terminal_color;
static volatile uint16_t* terminal_buffer;
static volatile uint16_t terminal_upward_history[80 * 25];
static volatile uint16_t terminal_downward_history[80 * 25];
static size_t terminal_upward_history_size;
static size_t terminal_downward_history_size;


void terminal_initialize(void) {
    terminal_initialize_test((uint16_t*)0xB8000u);
}

void terminal_initialize_test(uint16_t *const terminal_buffer_address) {
    terminal_resetcolor();
    terminal_buffer = terminal_buffer_address;
    terminal_upward_history_size = 0;
    terminal_downward_history_size = 0;
    terminal_clear();
}

void terminal_clear(void) {
    terminal_row = 0u;
    terminal_column = 0u;

    for (size_t y = 0u; y < 25; y++) {
        for (size_t x = 0u; x < 80; x++) {
            const size_t index = y * 80 + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(const enum vga_color color) {
    terminal_color = color;
}

void terminal_resetcolor(void) {
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void terminal_putentryat(const char c, const enum vga_color color, const size_t x, const size_t y) {
    const size_t index = y * 80 + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(const char c) {
 if (c == '\n') {
  if(++terminal_row == 25) {
   terminal_scroll_down();
  }
  terminal_column = 0;
  return;
 }
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == 80) {
        terminal_column = 0;
        if (++terminal_row == 25) {
   terminal_scroll_down();
        }
    }
}

void terminal_putchar_color(const char c, const enum vga_color color) {
    const enum vga_color old_color = terminal_color;
    terminal_setcolor(color);
    terminal_putchar(c);
    terminal_setcolor(old_color);
}

void terminal_write(const char *const text, const size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(text[i]);
    }
}

void terminal_writestring(const char *const text) {
    terminal_write(text, kstrlen(text));
}

void terminal_write_color(const char *const text, const size_t size, const enum vga_color color) {
    const enum vga_color old_color = terminal_color;
    terminal_setcolor(color);
    terminal_write(text, size);
    terminal_setcolor(old_color);
}

void terminal_writestring_color(const char *const text, const enum vga_color color) {
    terminal_write_color(text, kstrlen(text), color);
}

void terminal_scroll_down(void) {
    for(size_t x = 0u; x < 80; x++) {
        terminal_upward_history[terminal_upward_history_size++] = terminal_buffer[x];
    }
 for(size_t y = 0u; y < 25 - 1; y++) {
  for(size_t x = 0u; x < 80; x++) {
   terminal_buffer[y * 80 + x] = terminal_buffer[(y + 1) * 80 + x];
  }
 }
    if (terminal_downward_history_size == 0u) {
     for(size_t x = 0u; x < 80; x++) {
      terminal_putentryat(' ', terminal_color, x, 25 - 1);
     }
    } else {
        for(size_t x = terminal_downward_history_size - 1; x > 0u; x--) {
            terminal_buffer[(25 - 1) * 80 + x] = terminal_downward_history[--terminal_downward_history_size];
        }
    }
 terminal_row = 25 - 1;
 terminal_column = 0u;
}

void terminal_scroll_up(void) {
    if(terminal_upward_history_size == 0u) return;
    for(size_t x = 0u; x < 80; x++) {
        terminal_downward_history[terminal_downward_history_size++] = terminal_buffer[(25 - 1) * 80 + x];
    }
 for(size_t y = 25 - 1; y > 0u; y--) {
  for(size_t x = 0; x < 80; x++) {
   terminal_buffer[y * 80 + x] = terminal_buffer[(y - 1) * 80 + x];
  }
 }
    for(size_t x = 1u; x < 80 + 1; x++) {
        terminal_buffer[80 - x] = terminal_upward_history[--terminal_downward_history_size];
    }
 terminal_row = 25 - 1;
 terminal_column = 0u;
}
