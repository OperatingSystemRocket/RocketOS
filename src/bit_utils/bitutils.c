#include "bitutils.h"


void print_bits(const size_t size, const void *const ptr) {
    unsigned char *const b = (unsigned char*) ptr;

    for(int32_t i = size-1; i >= 0; i--) {
        for(int32_t j = 7; j >= 0; j--) {
            const unsigned char byte = (b[i] >> j) & 1;
            terminal_context_putchar(kint_to_char(byte));

            const int32_t pos = i*8 + j;
            if(pos == 63 || pos == 52) {
                terminal_context_putchar(' ');
            }
        }
    }
}

void print_bits_newline(const size_t size, const void *const ptr) {
    print_bits(size, ptr);
    terminal_context_putchar('\n');
}

void print_double_bits(const double *const ptr) {
    unsigned char *const b = (unsigned char*) ptr;

    for(int32_t i = 0; i < sizeof(double); ++i) {
        for(int32_t j = 0; j < 8; ++j) {
            const unsigned char byte = (b[i] >> j) & 1;
            terminal_context_putchar(kint_to_char(byte));

            const int32_t pos = i*8 + j;
            if(pos == 62 || pos == 51) {
                terminal_context_putchar(' ');
            }
        }
    }
}

uint8_t get_bit_at(const size_t position, const void *const ptr) {
    const uint64_t *const b = (uint64_t*) ptr;

    return (((*b) >> position) & 1);
}

void set_bit_at(const size_t position, const uint8_t value, void *const ptr) {
    uint8_t *const b = (uint8_t*)ptr;

    const unsigned char or_mask = 1u << (7u-position);
    const unsigned char and_mask = ~or_mask;

    if(value == 1) {
        *b = *b | or_mask;
    }
    else {
        *b = *b & and_mask;
    }
}

void set_bit_at_far(const size_t position, const uint8_t value, void *const ptr) {
    const uint64_t byte_number = position/8u;
    char *const byte_ptr = (char*)ptr;

    set_bit_at(7u-(position-(byte_number*8u)), value, byte_ptr + byte_number);
}

void set_bit_at_far_LTR(const size_t position, const uint8_t value, void *const ptr) {
    const uint64_t byte_number = position/8u;
    char *const byte_ptr = (char*)ptr;

    set_bit_at(position-(byte_number*8u), value, byte_ptr + byte_number);
}

uint8_t get_bit_at_far(const size_t position, void *const ptr) {
    const uint64_t byte_number = position/8u;
    char *const byte_ptr = (char*)ptr;

    return get_bit_at(position-(byte_number*8u), byte_ptr + byte_number);

}

uint8_t get_bit_at_far_LTR(const size_t position, void *const ptr) {
    const uint64_t byte_number = position/8u;
    char *const byte_ptr = (char*)ptr;

    get_bit_at(7u-(position-(byte_number*8u)), byte_ptr + byte_number);

}

void set_bits_in_range(const size_t start_bit, const size_t end_bit, const uint8_t value, void *const location) {
    char *const ptr = (char*) location;
    const size_t width_in_bits = end_bit - start_bit;

    for(int32_t i = 0; i < end_bit; ++i) {
        const int32_t byte_number = i/8;
        if(i >= start_bit) {
            set_bit_at(7u-(i-(byte_number*8u)), value, ptr + byte_number);
        }
    }
}

void mirror_bits_in_range(const size_t start, const size_t end, const void *const ptr) {
    uint32_t number_of_zeros = 0;
    uint32_t number_of_ones = 0;
    uint32_t number_of_errors = 0;

    for(size_t i = start; i < (end/2u); ++i) {
        const size_t x_position = i;
        const size_t y_position = end - i;

        const uint8_t x = get_bit_at_far(x_position, ptr);
        const uint8_t y = get_bit_at_far(y_position, ptr);

        if(x == 0u || y == 0u) {
            ++number_of_zeros;
        }
        if(x == 1u || y == 1u) {
            ++number_of_ones;
        }
        if (x > 1u || y > 1u) {
            ++number_of_errors;
        }

        set_bit_at_far(x_position, y, ptr);
        set_bit_at_far(y_position, x, ptr);
    }

    const size_t x_position = end/2u;
    const size_t y_position = end - (end/2u);

    const uint8_t x = get_bit_at_far(end/2u, ptr);
    const uint8_t y = get_bit_at_far(end - (end/2u), ptr);

    set_bit_at_far(x_position, y, ptr);
    set_bit_at_far(y_position, x, ptr);
}
