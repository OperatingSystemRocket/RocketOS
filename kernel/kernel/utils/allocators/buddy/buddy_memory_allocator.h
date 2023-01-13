#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <kstring.h>
#include <bitmap_allocator.h>


bool binary_buddy_memory_allocator_init(uint32_t* allocator, size_t allocator_type_size);
// `num_of_lowest_order_blocks` is the number of bits for the entry for the lowest size memory blocks
/* expected layout of `allocator` type:
struct alloc {
    uint32_t highest_mem_size[num_of_lowest_mem_size_in_allocator/4]; // index 2
    uint32_t medium_mem_size[num_of_lowest_mem_size_in_allocator/2]; // index 1
    uint32_t lowest_mem_size[num_of_lowest_mem_size_in_allocator]; // index 0
};
*/
bool binary_buddy_memory_allocator_reserve(uint32_t* allocator, uint32_t start_index, uint32_t end_index, const uint32_t num_of_elements_in_level, uint32_t level_in_allocator, uint32_t number_of_levels);
void* binary_buddy_memory_allocator_allocate(uint32_t* allocator, uint32_t lowest_mem_size, uint32_t num_of_lowest_mem_size_in_allocator, uint32_t number_of_levels, uint32_t amount_to_allocate); // `amount_to_allocate` is in terms of `lowest_mem_size`
bool binary_buddy_memory_allocator_free(uint32_t* allocator, uint32_t lowest_mem_size, uint32_t num_of_lowest_mem_size_in_allocator, uint32_t number_of_levels, void* address_to_free, uint32_t amount_to_free); // `amount_to_free` is in terms of `lowest_mem_size`

