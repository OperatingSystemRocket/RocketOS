#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "kassert.h"


//index = which number bit you want (0-indexed)
//size = number of uint32_t's, NOT number of bits
bool at(uint32_t index, const uint32_t* bitset);

void set_at(uint32_t index, uint32_t* bitset, bool value);
