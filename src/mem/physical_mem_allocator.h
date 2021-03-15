#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "kstring.h"
#include "kstdio.h"
#include "bitset.h"


void allocate_init(void);
void* allocate_page(void);
void free_page(const void* page);
