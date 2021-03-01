# 1 "src/kstring.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/kstring.c"
# 1 "src/kstring.h" 1
       

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
# 4 "src/kstring.h" 2
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
# 5 "src/kstring.h" 2

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
# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdbool.h" 1 3 4
# 7 "src/kstdio.h" 2


# 1 "src/vga_driver.h" 1
       


# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stddef.h" 1 3 4
# 5 "src/vga_driver.h" 2






# 10 "src/vga_driver.h"
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
# 10 "src/kstdio.h" 2


int kprintf(const char *format, ...);
# 4 "src/kassert.h" 2
# 7 "src/kstring.h" 2

size_t kstrlen(const char* str);

int32_t kstrcmp(const char* lhs, const char* rhs);

void kint_to_string(int64_t input, char* string_ret, size_t ret_size);
# 2 "src/kstring.c" 2

size_t kstrlen(const char *const str) {
    do { if(!(str != 
# 4 "src/kstring.c" 3 4
   ((void *)0)
# 4 "src/kstring.c"
   )) { kprintf("%4[file: %s :: line: %i]: ", "src/kstring.c", 4); terminal_writestring_color("Condition Failed: ", VGA_COLOR_RED); terminal_writestring_color("str != NULL", VGA_COLOR_RED); terminal_putchar_color('\n', VGA_COLOR_RED); return 0; } } while(0);

    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

int32_t kstrcmp(const char *const lhs, const char *const rhs) {
    do { if(!(lhs != 
# 13 "src/kstring.c" 3 4
   ((void *)0) 
# 13 "src/kstring.c"
   && rhs != 
# 13 "src/kstring.c" 3 4
   ((void *)0)
# 13 "src/kstring.c"
   )) { kprintf("%4[file: %s :: line: %i]: ", "src/kstring.c", 13); terminal_writestring_color("Condition Failed: ", VGA_COLOR_RED); terminal_writestring_color("lhs != NULL && rhs != NULL", VGA_COLOR_RED); terminal_putchar_color('\n', VGA_COLOR_RED); return 0; } } while(0);

 size_t index = 0u;
 while (lhs[index]) {
  if (lhs[index] == rhs[index]) {
   index++;
  } else if (lhs[index] < rhs[index]) {
   return -1;
  } else {
   return 1;
  }
 }
 return 0;
}

void kint_to_string(int64_t input, char *const string_ret, const size_t ret_size) {
    size_t index = 0u;
    if(0u < ret_size && input < 0u) {
        string_ret[index++] = '-';
        input *= -1;
    }
    do {
        size_t current_input = input % 10u;
        input /= 10u;
        const char current_char = current_input + 48u;
        if(index < ret_size) {
            string_ret[index++] = current_char;
        }
    } while(input);
    if(index < ret_size) {
        string_ret[index] = '\0';
    }
    const size_t start_index = (string_ret[0u] == '-') ? 1u : 0u;
    for(size_t i = 0u; i < index/2u; ++i) {
        const char temp = string_ret[i+start_index];
        string_ret[i+start_index] = string_ret[index-i-1u];
        string_ret[index-i-1u] = temp;
    }
}
