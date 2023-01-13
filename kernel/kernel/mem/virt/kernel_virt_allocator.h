#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <kstring.h>
#include <mem_constants.h>
#include <higher_half_utils.h>
#include <bitset.h>
#include <buddy_memory_allocator.h>


bool kernel_virt_allocator_init(void);
bool kernel_virt_allocator_reserve_page(void* page_virt_addr);
bool kernel_virt_allocator_reserve_pages(void* page_virt_addr, size_t num_of_pages);
void* kernel_virt_allocator_allocate_page(void);
void* kernel_virt_allocator_allocate_pages(size_t num_of_pages);
bool kernel_virt_allocator_free_page(void* page_virt_addr);
bool kernel_virt_allocator_free_pages(void* page_virt_addr, size_t num_of_pages);
