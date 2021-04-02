#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "kstring.h"
#include "kstdio.h"
#include "bitset.h"


#define PAGE_SIZE 4096

enum memory_type {
    CRITICAL_KERNEL_USE = 0,
    KERNEL_USE = 1,
    USER_USE = 2
};

void allocate_init(void);
void* allocate_page(enum memory_type allocation_type);
void free_page(const enum memory_type allocation_type, const void *const page);
void* get_first_nonreserved_address(void);
size_t get_amount_of_nonreserved_memory(void);

void* get_end_kernel(void);
