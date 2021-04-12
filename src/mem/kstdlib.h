#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "paging.h"
#include "kstring.h"


void dynamic_memory_init(void);
void* malloc(size_t size);
