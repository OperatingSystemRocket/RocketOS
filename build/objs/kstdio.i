# 1 "src/kstdio.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/kstdio.c"
# 1 "src/kstdio.h" 1
       

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
# 4 "src/kstdio.h" 2
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
# 5 "src/kstdio.h" 2
# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdarg.h" 1 3 4
# 40 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 99 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdarg.h" 3 4
typedef __gnuc_va_list va_list;
# 6 "src/kstdio.h" 2
# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdbool.h" 1 3 4
# 7 "src/kstdio.h" 2

# 1 "src/kassert.h" 1
       


# 1 "src/vga_driver.h" 1
       


# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stddef.h" 1 3 4
# 5 "src/vga_driver.h" 2


# 1 "src/kstring.h" 1
       

# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stddef.h" 1 3 4
# 4 "src/kstring.h" 2





# 8 "src/kstring.h"
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
# 5 "src/kassert.h" 2
# 9 "src/kstdio.h" 2



int kprintf(const char *format, ...);
# 2 "src/kstdio.c" 2

int32_t color_setting = -1;

static int32_t has_color(const char *const format, const size_t format_size) {
    if(format_size < 2u) return -1;

    if(format[0u] == '%') {
        switch(format[1u]) {
            case '0':
                return VGA_COLOR_BLACK;
            case '1':
                return VGA_COLOR_BLUE;
            case '2':
                return VGA_COLOR_GREEN;
            case '3':
                return VGA_COLOR_CYAN;
            case '4':
                return VGA_COLOR_RED;
            case '5':
                return VGA_COLOR_MAGENTA;
            case '6':
                return VGA_COLOR_BROWN;
            case '7':
                return VGA_COLOR_LIGHT_GREY;
            case '8':
                return VGA_COLOR_DARK_GREY;
            case '9':
                return VGA_COLOR_LIGHT_BLUE;
            case 'A':
                return VGA_COLOR_LIGHT_GREEN;
            case 'B':
                return VGA_COLOR_LIGHT_CYAN;
            case 'C':
                return VGA_COLOR_LIGHT_RED;
            case 'D':
                return VGA_COLOR_LIGHT_MAGENTA;
            case 'E':
                return VGA_COLOR_LIGHT_BROWN;
            case 'F':
                return VGA_COLOR_WHITE;
        }
    }

    return -1;
}



static int32_t conversion_specifier(const char *const format, const size_t format_size, uint32_t *const index, va_list* variadic_args) {
    do { if(!((*index) < format_size)) { kprintf("%4[file: %s :: line: %i]: ", "src/kstdio.c", 51); terminal_writestring_color("Condition Failed: ", VGA_COLOR_RED); terminal_writestring_color("(*index) < format_size", VGA_COLOR_RED); terminal_putchar_color('\n', VGA_COLOR_RED); return -1; } } while(0);

    if(color_setting == -1) {
        color_setting = has_color(format, format_size);
        if(color_setting != -1) {
            *index += 2u;
        }
    }

    if(format[*index] == '%') {
        if(((*index)+1) < format_size) {


            char str[128];
            int64_t number;

            switch (format[(*index) + 1]) {
                case 'c':

                    if(color_setting == -1) {
                        terminal_putchar(
# 71 "src/kstdio.c" 3 4
                                        __builtin_va_arg(
# 71 "src/kstdio.c"
                                        *variadic_args
# 71 "src/kstdio.c" 3 4
                                        ,
# 71 "src/kstdio.c"
                                        int
# 71 "src/kstdio.c" 3 4
                                        )
# 71 "src/kstdio.c"
                                                                   );
                    } else {
                        terminal_putchar_color(
# 73 "src/kstdio.c" 3 4
                                              __builtin_va_arg(
# 73 "src/kstdio.c"
                                              *variadic_args
# 73 "src/kstdio.c" 3 4
                                              ,
# 73 "src/kstdio.c"
                                              int
# 73 "src/kstdio.c" 3 4
                                              )
# 73 "src/kstdio.c"
                                                                         , color_setting);
                    }
                    *index += 1u;
                    return 1;
                case 's':
                    if(color_setting == -1) {
                        terminal_writestring(
# 79 "src/kstdio.c" 3 4
                                            __builtin_va_arg(
# 79 "src/kstdio.c"
                                            *variadic_args
# 79 "src/kstdio.c" 3 4
                                            ,
# 79 "src/kstdio.c"
                                            char*
# 79 "src/kstdio.c" 3 4
                                            )
# 79 "src/kstdio.c"
                                                                         );
                    } else {
                        terminal_writestring_color(
# 81 "src/kstdio.c" 3 4
                                                  __builtin_va_arg(
# 81 "src/kstdio.c"
                                                  *variadic_args
# 81 "src/kstdio.c" 3 4
                                                  ,
# 81 "src/kstdio.c"
                                                  char*
# 81 "src/kstdio.c" 3 4
                                                  )
# 81 "src/kstdio.c"
                                                                               , color_setting);
                    }
                    *index += 1u;
                    return 2;
                case 'd':
                case 'i':
                    number = 
# 87 "src/kstdio.c" 3 4
                            __builtin_va_arg(
# 87 "src/kstdio.c"
                            *variadic_args
# 87 "src/kstdio.c" 3 4
                            ,
# 87 "src/kstdio.c"
                            int
# 87 "src/kstdio.c" 3 4
                            )
# 87 "src/kstdio.c"
                                                       ;
                    kint_to_string(number, str, 128);
                    if(color_setting == -1) {
                        terminal_writestring(str);
                    } else {
                        terminal_writestring_color(str, color_setting);
                    }
                    *index += 1u;
                    return 3;
                case 'o':
                    return 4;
                case 'x':
                case 'X':
                    return 5;
                case 'u':
                    return 6;
                case 'f':
                case 'F':
                    return 7;
                case 'e':
                case 'E':
                    return 8;
                case 'a':
                case 'A':
                    return 9;
                case 'g':
                case 'G':
                    return 10;
                case 'n':
                    return 11;
                case 'p':
                    return 12;
            }
        }
    } else {
        if(color_setting == -1) {
            terminal_putchar(format[*index]);
        } else {
            terminal_putchar_color(format[*index], color_setting);
        }
    }
    return 0;
}

int kprintf(const char *const format, ...) {
    va_list pargs;

    
# 134 "src/kstdio.c" 3 4
   __builtin_va_start(
# 134 "src/kstdio.c"
   pargs
# 134 "src/kstdio.c" 3 4
   ,
# 134 "src/kstdio.c"
   format
# 134 "src/kstdio.c" 3 4
   )
# 134 "src/kstdio.c"
                          ;

    const size_t format_len = kstrlen(format);
    for(uint32_t i = 0u; i < format_len; ++i) {
        conversion_specifier(format, format_len, &i, &pargs);
    }

    
# 141 "src/kstdio.c" 3 4
   __builtin_va_end(
# 141 "src/kstdio.c"
   pargs
# 141 "src/kstdio.c" 3 4
   )
# 141 "src/kstdio.c"
                ;

    color_setting = -1;

    return 0;
}
