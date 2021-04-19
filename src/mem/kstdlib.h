#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "paging.h"
#include "kstring.h"


void kdynamic_memory_init(void);
void* kmalloc(size_t size);
void* zeroed_out_kmalloc(size_t size);
void kfree(const void* payload_ptr);
