#include "buddy_memory_allocator.h"


bool binary_buddy_memory_allocator_init(uint32_t *const allocator, const size_t allocator_type_size) {
    return kmemset(allocator, 0, allocator_type_size) != NULL;
}

// TODO: pull out into another file and make public/externally linked
static uint32_t log2(uint32_t x) {
    uint32_t result = 0;
    while (x >>= 1) result++;
    return result;
}

bool binary_buddy_memory_allocator_reserve(uint32_t *const allocator, const uint32_t start_index, const uint32_t end_index, const uint32_t num_of_elements_in_level, const uint32_t level_in_allocator, const uint32_t number_of_levels) {
    if(level_in_allocator >= number_of_levels) {
        return false;
    }

    const uint32_t num_of_lowest_mem_size_in_allocator = num_of_elements_in_level << level_in_allocator;

    const uint32_t number_of_highest_mem_size_in_allocator = num_of_lowest_mem_size_in_allocator >> (number_of_levels-1u);
    const uint32_t allocation_level_start_index = ((1u << (number_of_levels - level_in_allocator - 1u)) - 1u)*number_of_highest_mem_size_in_allocator; // index in bits


    const uint32_t num_of_entries_to_set_in_level = (end_index-start_index)+1u;
    for(uint32_t i = start_index; i <= end_index; ++i) {
        bitset_set_at(allocation_level_start_index+i, allocator, true);
    }
    serial_writestring("after num_of_entries_to_set_in_level\n");
    uint32_t num_of_entries_to_set_higher = num_of_elements_in_level/2u;
    for(uint32_t i = number_of_levels-1u; i > level_in_allocator; --i) {
        const uint32_t current_allocation_level_start_index = ((1u << (number_of_levels - i - 1u)) - 1u)*number_of_highest_mem_size_in_allocator;
        const uint32_t current_allocation_index = start_index >> (i - level_in_allocator);
        for(uint32_t j = 0u; j < num_of_entries_to_set_higher; ++j) {
            bitset_set_at(current_allocation_level_start_index+current_allocation_index+j, allocator, true);
            //serial_writestring("iteration of inner for loop done\n");
        }
        num_of_entries_to_set_higher /= 2;
        if(num_of_entries_to_set_higher == 0u) {
            num_of_entries_to_set_higher = 1u;
        }
        //serial_writestring("iteration of outer for loop done\n");
    }
    //serial_writestring("after num_of_entries_to_set_higher\n");
    //kprintf("num_of_entries_to_set_in_level: %u\n", num_of_entries_to_set_in_level);
    uint32_t num_of_entries_to_set_lower = num_of_entries_to_set_in_level*2u;
    for(uint32_t i = 0u; i < level_in_allocator; ++i) {
        const uint32_t current_allocation_level_start_index = ((1u << (number_of_levels - i - 1u)) - 1u)*number_of_highest_mem_size_in_allocator;
        const uint32_t first_current_allocation_index = start_index << (level_in_allocator - i);
        for(uint32_t j = 0u; j < num_of_entries_to_set_lower; ++j) {
            bitset_set_at(current_allocation_level_start_index+first_current_allocation_index+j, allocator, true);
            //kprintf("second iteration of inner for loop done with j: %u, num_of_entries_to_set_lower: %u\n", j, num_of_entries_to_set_lower);
        }
        num_of_entries_to_set_lower *= 2u;
        //serial_writestring("second iteration of outer for loop done\n");
    }

    return true;
}

void* binary_buddy_memory_allocator_allocate(uint32_t *const allocator, const uint32_t lowest_mem_size, const uint32_t num_of_lowest_mem_size_in_allocator, const uint32_t number_of_levels, const uint32_t amount_to_allocate) { // `amount_to_allocate` is in terms of `lowest_mem_size`
    const uint32_t level_of_allocation = log2(amount_to_allocate);
    if (level_of_allocation >= number_of_levels) {
        return NULL;
    }

    const uint32_t number_of_highest_mem_size_in_allocator = num_of_lowest_mem_size_in_allocator >> (number_of_levels-1u);
    const uint32_t allocation_level_start_index = ((1u << (number_of_levels - level_of_allocation - 1u)) - 1u)*number_of_highest_mem_size_in_allocator; // index in bits
    const uint32_t allocation_level_n = num_of_lowest_mem_size_in_allocator >> level_of_allocation;

    const uint32_t allocation_index = bitmap_find_first_zero_bit(&allocator[allocation_level_start_index/32], allocation_level_n);
    if (allocation_index == allocation_level_n) {
        return NULL;
    }

    const uint32_t size_of_allocation = amount_to_allocate*lowest_mem_size;
    const uint32_t ret_addr = allocation_index*size_of_allocation;


    bitset_set_at(allocation_level_start_index+allocation_index, allocator, true);
    for(uint32_t i = number_of_levels-1u; i > level_of_allocation; --i) {
        const uint32_t current_allocation_level_start_index = ((1u << (number_of_levels - i - 1u)) - 1u)*number_of_highest_mem_size_in_allocator;
        const uint32_t current_allocation_index = allocation_index >> (i - level_of_allocation);
        bitset_set_at(current_allocation_level_start_index+current_allocation_index, allocator, true);
    }
    uint32_t num_of_entries_to_set = 2u;
    for(uint32_t i = 0u; i < level_of_allocation; ++i) {
        const uint32_t current_allocation_level_start_index = ((1u << (number_of_levels - i - 1u)) - 1u)*number_of_highest_mem_size_in_allocator;
        const uint32_t first_current_allocation_index = allocation_index << (level_of_allocation - i);
        for(uint32_t j = 0u; j < num_of_entries_to_set; ++j) {
            bitset_set_at(current_allocation_level_start_index+first_current_allocation_index+j, allocator, true);
        }
        num_of_entries_to_set *= 2u;
    }


    return (void*)ret_addr;
}

bool binary_buddy_memory_allocator_free(uint32_t *const allocator, const uint32_t lowest_mem_size, const uint32_t num_of_lowest_mem_size_in_allocator, const uint32_t number_of_levels, void *const address_to_free, const uint32_t amount_to_free) { // `amount_to_free` is in terms of `lowest_mem_size`
    const uint32_t level_of_allocation = log2(amount_to_free);
    if (level_of_allocation >= number_of_levels) {
        return false;
    }

    const uint32_t number_of_highest_mem_size_in_allocator = num_of_lowest_mem_size_in_allocator >> (number_of_levels-1u);
    const uint32_t allocation_level_start_index = ((1u << (number_of_levels - level_of_allocation - 1u)) - 1u)*number_of_highest_mem_size_in_allocator; // index in bits
    const uint32_t allocation_level_n = num_of_lowest_mem_size_in_allocator >> level_of_allocation;

    const uint32_t allocation_index = (uint32_t)address_to_free/(amount_to_free*lowest_mem_size);
    if (allocation_index >= allocation_level_n) {
        return false;
    }

    bitset_set_at(allocation_level_start_index+allocation_index, allocator, false);
    for(uint32_t i = level_of_allocation; i < number_of_levels; ++i) {
        const uint32_t current_allocation_level_start_index = ((1u << (number_of_levels - i - 1u)) - 1u)*number_of_highest_mem_size_in_allocator;
        const uint32_t current_allocation_index = allocation_index >> (i - level_of_allocation);
        const uint32_t pair_index = (current_allocation_index % 2u == 0u) ? current_allocation_index+1 : current_allocation_index-1;
        if(bitset_at(current_allocation_level_start_index+pair_index, allocator) == true) {
            break;
        }
        bitset_set_at(current_allocation_level_start_index+current_allocation_index, allocator, false);
    }
    uint32_t num_of_entries_to_set = 2u;
    for(uint32_t i = 0u; i < level_of_allocation; ++i) {
        const uint32_t current_allocation_level_start_index = ((1u << (number_of_levels - i - 1u)) - 1u)*number_of_highest_mem_size_in_allocator;
        const uint32_t first_current_allocation_index = allocation_index << (level_of_allocation - i);
        for(uint32_t j = 0u; j < num_of_entries_to_set; ++j) {
            bitset_set_at(current_allocation_level_start_index+first_current_allocation_index+j, allocator, false);
        }
        num_of_entries_to_set *= 2u;
    }

    return true;
}
