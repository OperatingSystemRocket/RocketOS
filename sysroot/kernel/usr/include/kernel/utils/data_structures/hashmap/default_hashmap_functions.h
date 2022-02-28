#pragma once


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "kstring.h"


uint32_t str_hash_function(const char* str_to_hash);
bool str_comp(const char* str1, const char* str2);
