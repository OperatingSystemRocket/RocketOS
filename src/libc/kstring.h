#pragma once

#include <stddef.h>
#include <stdint.h>

#include "kassert.h"

size_t kstrlen(const char* str);

int32_t kstrcmp(const char* lhs, const char* rhs);

int32_t kstrncmp(const char* lhs, const char* rhs, const size_t sz);

char* kstrcat(char* destination, char* source);

void kint_to_string(int64_t input, char* string_ret, size_t ret_size);
