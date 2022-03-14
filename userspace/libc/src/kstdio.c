#include "kstdio.h"

#include <drivers/vga_driver/default_vga_driver.h>
#include <drivers/serial/serial_driver.h>


static int32_t color_setting = -1;

//TODO: replace with ansi escape codes: https://en.wikipedia.org/wiki/ANSI_escape_code
//possibly good idea: have zsh type color formatting codes
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


static void print_char(const char c) {
    terminal_context_putchar(c);
    serial_putchar(c);
    //serial_putchar(c);
}

static void print_char_color(const char c, const enum vga_color color) {
    terminal_context_putchar_color(c, color);
}

static void print_string(const char *const str) {
    terminal_context_writestring(str);
    serial_writestring(str);
    //serial_writestring(str);
}

static void print_string_color(const char *const str, const enum vga_color color) {
    terminal_context_writestring_color(str, color);
}



//TODO: Deal with length modifiers later
static int32_t conversion_specifier(const char *const format, const size_t format_size, uint32_t *const index, va_list* variadic_args) {
    kassert((*index) < format_size, -1);

    if(color_setting == -1) {
        color_setting = has_color(format, format_size);
        if(color_setting != -1) {
            *index += 2u;
        }
    }

    if(format[*index] == '%') {
        if(((*index)+1) < format_size) {
            //max amount of digits I will allow is 127 (since null terminator is last byte of number)
            //wack switch case rules described here: https://stackoverflow.com/questions/3757445/switch-case-declaration-with-initialization-declaration-and-then-assignment
            char str[128];
            int64_t number;

            switch (format[(*index) + 1]) {
                case 'c':
                    if(color_setting == -1) {
                        print_char((char) va_arg(*variadic_args, int32_t));
                    } else {
                        print_char_color((char) va_arg(*variadic_args, int32_t), color_setting);
                    }
                    *index += 1u;
                    return 1;
                case 's':
                    if(color_setting == -1) {
                        print_string(va_arg(*variadic_args, const char*));
                    } else {
                        print_string_color(va_arg(*variadic_args, const char*), color_setting);
                    }
                    *index += 1u;
                    return 2;
                case 'd':
                case 'i':
                    number = va_arg(*variadic_args, int32_t);
                    kint_to_string(number, str, 128u, 10u, false);
                    if(color_setting == -1) {
                        print_string(str);
                    } else {
                        print_string_color(str, color_setting);
                    }
                    *index += 1u;
                    return 3;
                case 'o':
                    number = va_arg(*variadic_args, uint32_t);
                    kint_to_string(number, str, 128u, 8u, false);
                    if(color_setting == -1) {
                        print_string(str);
                    } else {
                        print_string_color(str, color_setting);
                    }
                    *index += 1u;
                    return 4;
                case 'x':
                    number = va_arg(*variadic_args, uint32_t);
                    kint_to_string(number, str, 128u, 16u, true);
                    if(color_setting == -1) {
                        print_string(str);
                    } else {
                        print_string_color(str, color_setting);
                    }
                    *index += 1u;
                    return 5;
                case 'X':
                    number = va_arg(*variadic_args, uint32_t);
                    kint_to_string(number, str, 128u, 16u, false);
                    if(color_setting == -1) {
                        print_string(str);
                    } else {
                        print_string_color(str, color_setting);
                    }
                    *index += 1u;
                    return 6;
                case 'u':
                    number = va_arg(*variadic_args, uint32_t);
                    kint_to_string(number, str, 128u, 10u, false);
                    if(color_setting == -1) {
                        print_string(str);
                    } else {
                        print_string_color(str, color_setting);
                    }
                    *index += 1u;
                    return 7;
                case 'f':
                case 'F':
                    return 8;
                case 'e':
                case 'E':
                    return 9;
                case 'a':
                case 'A':
                    return 10;
                case 'g':
                case 'G':
                    return 11;
                case 'n':
                    return 12;
                case 'p':
                    number = (uint32_t)va_arg(*variadic_args, void*);
                    kint_to_string(number, str, 128u, 16u, false);
                    if(color_setting == -1) {
                        print_string(str);
                    } else {
                        print_string_color(str, color_setting);
                    }
                    *index += 1u;
                    return 13;
                /*non-standard:*/
                case 'b':
                case 'B':
                    number = va_arg(*variadic_args, uint32_t);
                    kint_to_string(number, str, 128u, 2u, false);
                    if(color_setting == -1) {
                        print_string(str);
                    } else {
                        print_string_color(str, color_setting);
                    }
                    *index += 1u;
                    return 14;
            }
        }
    } else {
        if(color_setting == -1) {
            print_char(format[*index]);
        } else {
            print_char_color(format[*index], color_setting);
        }
    }
    return 0;
}

int32_t kprintf_implementation(const char *const format, va_list* pargs) {
    const size_t format_len = kstrlen(format);
    for(uint32_t i = 0u; i < format_len; ++i) {
        conversion_specifier(format, format_len, &i, pargs);
    }

    color_setting = -1;

    return 0; //TODO: implement return value
}

int32_t kprintf(const char *const format, ...) {
    va_list pargs;

    va_start(pargs, format);

    (void) kprintf_implementation(format, &pargs);

    va_end(pargs);

    return 0; //TODO: implement return value
}
