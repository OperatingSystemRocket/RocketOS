#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "mem_constants.h"
#include "physical_mem_allocator.h"
#include "paging.h"


void* get_heap_range_start(void);
size_t get_max_heap_size(void); //in pages
void* get_acpica_start(void);
size_t get_acpica_size(void); //in pages
void* get_global_virt_allocator_start(void);
size_t get_global_virt_allocator_size(void); //in pages
void initialize_kernel_memory(void);
