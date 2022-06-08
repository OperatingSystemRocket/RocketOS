#pragma once

#include <utils/allocators/osi_memory_allocator.h>
#include <mem/initialize_kernel_memory.h>
#include <buddy_memory_allocator.h>
#include <mem/paging.h>

void global_virt_allocator_init(void);
void* global_virt_allocator_allocate(uint32_t num_of_pages);
void global_virt_allocator_free(const void* ptr, uint32_t num_of_pages);

void* global_allocate_phys_virt_mem(uint32_t num_of_pages);
void global_free_phys_virt_mem(const void* ptr, uint32_t num_of_pages);
