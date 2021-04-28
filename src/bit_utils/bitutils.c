
#include "bitutils.h"
#include "kstdio.h"

void print_bits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j, pos;

    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;

            terminal_putchar(kitoc(byte));

            pos = i*8 + j;
            if(pos == 63 || pos == 52) {
                terminal_putchar(' ');
            }
        }
    }
    //kprintf("%c", '\n');
}

void print_bits_newline(const size_t size, void const * const ptr) {
  print_bits(size, ptr);
  terminal_putchar('\n');
}

void print_double_bits(const double* const ptr) {

    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j, pos;

    size_t size = 8;

    for (i = 0; i < size; ++i) {
        for (j = 0; j < 8; ++j) {
            byte = (b[i] >> j) & 1;
            terminal_putchar(kitoc(byte));

            pos = i*8 + j;
            if(pos == 62 || pos == 51) {
                terminal_putchar(' ');
            }
        }
    }
    //kprintf("%c", '\n');
}


uint8_t get_bit_at(size_t position, const void* const ptr) {
    uint64_t *b = (uint64_t*) ptr;

    return (((*b) >> position) & 1);
}



void set_bit_at(size_t position, uint8_t value, void* const ptr) {
    uint8_t *b = (uint8_t*)ptr;
    uint8_t one = 1;
    uint8_t all = ~0;


    char or_mask = one << (7-position);
    char and_mask = ~or_mask;

    if(value == 1) {
        *b = *b | or_mask;
    }
    else {
        *b = *b & and_mask;
    }


}

void set_bit_at_far(size_t position, uint8_t value, void* const ptr) {
    int64_t byte_number = position/8;
    char* byte_ptr = (char*)(ptr);
    set_bit_at(7-(position-(byte_number*8)),value, ptr + byte_number); /// should probably use the byte_ptr
}

void set_bit_at_far_LTR(size_t position, uint8_t value, void* const ptr) {
  int64_t byte_number = position/8;
  char* byte_ptr = (char*)(ptr);
  set_bit_at(position-(byte_number*8),value, ptr + byte_number); /// should probably use the byte_ptr
}



uint8_t get_bit_at_far(size_t position, void* const ptr) {
    int64_t byte_number = position/8;
    char* byte_ptr = (char*)(ptr);
    return get_bit_at((position-(byte_number*8)), byte_ptr + byte_number);

}

uint8_t get_bit_at_far_LTR(size_t position, void* const ptr) {
  int64_t byte_number = position/8;
  char* byte_ptr = (char*)(ptr);
  get_bit_at(7-(position-(byte_number*8)), byte_ptr + byte_number);

}


void set_bits_in_range(size_t start_bit, size_t end_bit, uint8_t value, void* const location) {
    char* const ptr = (char* const)location;
    size_t width_in_bits = end_bit - start_bit;

    for(int i = 0; i < end_bit; ++i) {
        int byte_number = i/8;
        if(i >= start_bit) {
            set_bit_at(7-(i-(byte_number*8)),value, location + byte_number);
        }
    }



/*
    for(int i = (end_bit/8) + ksign(kmod(end_bit,8)) - 1; i >= 0; --i) {
        for(int j = 7; j >= 0; --j) {
            if((i*8 + j >= start_bit) && (i*8 + j <= end_bit)) {
                set_bit_positive_at(7-j, ptr+i);
            }
        }

    }*/


}


void mirror_bits_in_range(size_t start, size_t end, const void* ptr) {

  uint32_t number_of_zeros =0;
  uint32_t number_of_ones=0;
  uint32_t number_of_errors=0;

  uint32_t number_of_iterations = 0;
    for(size_t i = start; i < (end / 2); ++i) {

        //kprintf("%i\n", i);
         size_t x_position = i;
         size_t y_position = end - i;

         uint8_t x = get_bit_at_far(x_position, ptr);
         uint8_t y = get_bit_at_far(y_position, ptr);

        if(x == 0 || y == 0) {
          ++number_of_zeros;
        }
        if(x == 1 || y == 1) {
          ++number_of_ones;
        }
        if (x > 1 || y > 1) {
          ++number_of_errors;
        }

        set_bit_at_far(x_position, y, ptr);
        set_bit_at_far(y_position, x, ptr);

        ++number_of_iterations;
    }

    size_t x_position = end/2;
    size_t y_position = end - (end/2);

    uint8_t x = get_bit_at_far(end/2, ptr);
    uint8_t y = get_bit_at_far(end - (end/2), ptr);

  set_bit_at_far(x_position, y, ptr);
  set_bit_at_far(y_position, x, ptr);


  //set_bit_at_far(end, x, ptr);

/*
  terminal_putchar('\n');
  terminal_putchar('\n');
  terminal_putchar('\n');
  kprintf("%i\n", number_of_zeros);
  kprintf("%i\n", number_of_ones);
  kprintf("%i\n", number_of_errors);
  kprintf("%i\n", number_of_iterations);

  terminal_putchar('\n');
  terminal_putchar('\n');
  */

}
