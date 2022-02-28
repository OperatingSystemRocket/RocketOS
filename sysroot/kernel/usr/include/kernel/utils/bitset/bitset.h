#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "kassert.h"


//index = which number bit you want (0-indexed)
//size = number of uint32_t's, NOT number of bits
bool bitset_at(uint32_t index, const void* bitset);

void bitset_set_at(uint32_t index, void* bitset, bool value);
