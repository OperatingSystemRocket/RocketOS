#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "kassert.h"


//TODO: merge and consolidate with `bitutils.h`

//index = which number bit you want (0-indexed)
//size = number of uint32_t's, NOT number of bits
bool at(uint32_t index, const void* bitset);

void set_at(uint32_t index, void* bitset, bool value);
