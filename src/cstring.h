#pragma once

#include <stddef.h>
#include <stdint.h>

#include "cassert.h"

size_t cstrlen(const char* str);

int cstrcmp(const char* lhs, const char* rhs);

void cint_to_string(int64_t input, char* string_ret, size_t ret_size);
