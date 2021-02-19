#pragma once

#include <stddef.h>

#include "cassert.h"

size_t cstrlen(const char* str);

int cstrcmp(const char* lhs, const char* rhs);

void cint_to_string(ssize_t input, char* string_ret, size_t ret_size);
