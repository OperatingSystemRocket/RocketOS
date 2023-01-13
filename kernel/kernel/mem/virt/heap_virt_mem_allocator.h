#pragma once


#include <stdint.h>
#include <stddef.h>

#include <mem_constants.h>
#include <higher_half_utils.h>


void* kernel_virt_mem_allocate_page(void);
void* kernel_virt_mem_allocate_pages(size_t num_of_pages);

