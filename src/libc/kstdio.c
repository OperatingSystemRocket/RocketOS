#include "kstdio.h"

int32_t color_setting = -1;

//@todo replace with ansi escape codes: https://en.wikipedia.org/wiki/ANSI_escape_code
//possible good idea: have zsh type color formatting codes
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


//For now just cover the none cases. Deal with length modifiers later
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
                    //for some reason (maybe integer promotion), cppreference says to retrieve the character via as an int
                    if(color_setting == -1) {
                        terminal_putchar(va_arg(*variadic_args, int));
                    } else {
                        terminal_putchar_color(va_arg(*variadic_args, int), color_setting);
                    }
                    *index += 1u;
                    return 1;
                case 's':
                    if(color_setting == -1) {
                        terminal_writestring(va_arg(*variadic_args, char*));
                    } else {
                        terminal_writestring_color(va_arg(*variadic_args, char*), color_setting);
                    }
                    *index += 1u;
                    return 2;
                case 'd':
                case 'i':
                    number = va_arg(*variadic_args, int);
                    kint_to_string(number, str, 128u);
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

    va_start(pargs, format);

    const size_t format_len = kstrlen(format);
    for(uint32_t i = 0u; i < format_len; ++i) {
        conversion_specifier(format, format_len, &i, &pargs);
    }

    va_end(pargs);

    color_setting = -1;

    return 0; //@todo implement return value
}
