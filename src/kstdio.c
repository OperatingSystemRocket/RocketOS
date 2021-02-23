#include "kstdio.h"



//For now just cover the none cases. Deal with length modifiers later
static int32_t conversion_specifier(const char *const format, const size_t format_size, uint32_t *const index, va_list* variadic_args) {
    kassert((*index) < format_size, -1);

    if(format[*index] == '%') {
        if(((*index)+1) < format_size) {
            //max amount of digits I will allow is 127 (since null terminator is last byte of number)
            //wack switch case rules described here: https://stackoverflow.com/questions/3757445/switch-case-declaration-with-initialization-declaration-and-then-assignment
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
                    *index += 1u;
                    return 2;
                case 'd':
                case 'i':
                    number = va_arg(*variadic_args, int);
                    kint_to_string(number, str, 128);
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

int kprintf(const char *const format, ...) {
    va_list pargs;

    va_start(pargs, format);

    const size_t format_len = kstrlen(format);
    for(uint32_t i = 0u; i < format_len; ++i) {
        conversion_specifier(format, format_len, &i, &pargs);
    }

    va_end(pargs);

    return 0; //@todo implement return value
}
