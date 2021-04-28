#pragma once
#include "bitutils.h"
#include "kstdio.h"

void extract_signbit_as_uint(double source, uint8_t* const destination);

int16_t get_exponent_as_int(double source);

uint8_t get_signbit(double* source);

uint64_t get_mantissa_as_uint(double source);

void double_to_str(double source, char* const destination);

