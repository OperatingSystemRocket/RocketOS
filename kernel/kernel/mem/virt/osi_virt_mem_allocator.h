#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <mem_constants.h>
#include <kstring.h>
#include <kernel_virt_allocator.h>


bool osi_virt_mem_allocator_init(void);
void* osi_virt_mem_allocator_allocate_page(void);
void* osi_virt_mem_allocator_allocate_pages(size_t num_of_pages);
bool osi_virt_mem_allocator_free_page(void* page_virt_addr);
bool osi_virt_mem_allocator_free_pages(void* page_virt_addr, size_t num_of_pages);
