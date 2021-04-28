#pragma once
#include <stddef.h>
#include "kmath.h"

#define BIT_CAST(OLD_V, NEW_T) *((NEW_T*)(&OLD_V))




void print_bits(size_t const size, void const * const ptr);

uint8_t get_bit_at(size_t position, const void* const ptr);

uint8_t get_bit_at_far(size_t position, void* const ptr);

void set_bits_in_range(size_t start_bit, size_t end_bit, uint8_t value, void* const location);

void set_bit_at(size_t position, uint8_t value, void* const ptr);

void set_bit_at_far(size_t position, uint8_t value, void* const ptr);

void mirror_bits_in_range(size_t start, size_t end, const void* ptr);

void set_bit_at_far_LTR(size_t position, uint8_t value, void* const ptr);

uint8_t get_bit_at_far_LTR(size_t position, void* const ptr);