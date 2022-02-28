#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "kstring.h"
#include "kstdio.h"
#include "bitset.h"
#include "mem_constants.h"
#include "bitmap_allocator.h"


void allocate_init(void);
void reserve_physical_address(uint32_t physical_address, size_t num_of_pages, enum memory_type type);
void* allocate_page(enum memory_type allocation_type);
void free_page(const enum memory_type allocation_type, const void *const page);
size_t get_number_of_permanently_reserved_pages(void);
size_t get_number_of_pages_used(void);
void* get_first_nonreserved_address(void);
size_t get_amount_of_nonreserved_memory(void);
