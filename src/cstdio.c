#include "cstdio.h"

//For now just cover the none cases. Deal with length modifiers later
static int32_t conversion_specifier(const char *const format, const size_t format_size, size_t *const index, va_list* variadic_args) {
    cassert((*index) < format_size, -1);

    if(format[*index] == '%') {
        if(((*index)+1) < format_size) {
            //max amount of digits I will allow is 127 (since null terminator is last byte of number)
            //wack switch case rules
            char str[128];
            int64_t number;

            switch (format[(*index) + 1]) {
                case 'c':
                    //for some reason (maybe integer promotion), cppreference says to retrieve the character via as an int
                    terminal_putchar(va_arg(*variadic_args, int));
                    *index += 1u;
                    return 1;
                case 's':
                    terminal_writestring(va_arg(*variadic_args, char*));
                    return 2;
                case 'd':
                case 'i':
                    number = va_arg(*variadic_args, int);
                    cint_to_string(number, str, 128);
                    terminal_writestring(str);
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
        terminal_putchar(format[*index]);
    }
    return 0;
}

/*
static int32_t length_modifier(const char *const format, const size_t format_size, const size_t index, va_list* variadic_args) {
    cassert(index < format_size, -1);

    switch(format[index]) {
        case 'h':
            if((index+1) < format_size && format[index+1] == 'h') {
                return 1;
            } else {
                return 2;
            }
        case 'l':
            if((index+1) < format_size && format[index+1] == 'l') {
                return 3;
            } else {
                return 4;
            }
        case 'j':
            return 5;
        case 'z':
            return 6;
        case 't':
            return 7;
        case 'L':
            return 8;
    }
    return 0;
}
*/

int cprintf(const char *const format, ...) {
    va_list pargs;

    va_start(pargs, format);

    //const uint32_t value = va_arg(pargs, int32_t);

    for(uint32_t i = 0u; i < cstrlen(format); ++i) {
        conversion_specifier(format, cstrlen(format), &i, &pargs);
    }

    va_end(pargs);


}
