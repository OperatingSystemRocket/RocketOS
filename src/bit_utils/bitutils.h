#pragma once

#include <stddef.h>
#include <stdint.h>

#include "kmath.h"
#include "kstdio.h"


//TODO: Delete this UB nonsense in the near future
#define BIT_CAST(OLD_V, NEW_T) *((NEW_T*)(&OLD_V))


void print_bits(size_t size, const void* ptr);
void print_bits_newline(size_t size, const void* ptr);
void print_double_bits(const double* ptr);
uint8_t get_bit_at(size_t position, const void* ptr);
void set_bit_at(size_t position, uint8_t value, void* ptr);
void set_bit_at_far(size_t position, uint8_t value, void* ptr);
void set_bit_at_far_LTR(size_t position, uint8_t value, void* ptr);
uint8_t get_bit_at_far(size_t position, void* ptr);
uint8_t get_bit_at_far_LTR(size_t position, void* ptr);
void set_bits_in_range(size_t start_bit, size_t end_bit, uint8_t value, void* location);
void mirror_bits_in_range(size_t start, size_t end, const void* ptr);
