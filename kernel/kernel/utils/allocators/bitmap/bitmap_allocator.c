#include "bitmap_allocator.h"


void bitmap_allocator_init(uint32_t *const bitset, const uint32_t n, int32_t *const bitset_cache, const uint32_t cache_n, bool *const has_filled_bitset_cache) {
    kmemset(bitset, 0u, n/32 * sizeof(uint32_t));
    *has_filled_bitset_cache = false;
    for(int32_t i = 0; i < cache_n; i++) {
        bitset_cache[i] = -1;
    }
}

static int32_t allocate_impl(uint32_t *const bookkeeping_bitset, const size_t number_of_entries, int32_t *const bitset_cache, const uint32_t num_of_cache_entries, bool *const has_searched_cache) {
    for(int32_t i = 0; i < num_of_cache_entries; ++i) {
        const int32_t current_entry = bitset_cache[i];
        if(current_entry != -1) {
            bitset_cache[i] = -1;
            bookkeeping_bitset[current_entry/32] |= 1u << (current_entry%32);
            *has_searched_cache = false;
            //kprintf("allocated index: %i\n", current_entry);
            return current_entry;
        }
    }


    //we have already tried to allocate entries and failed
    if(*has_searched_cache) {
        *has_searched_cache = false; //we will try to allocate next time this function is called
        return -1;
    }

    //bitset table cache is empty, must do expensive allocation:
    for(uint32_t i = 0u, number_of_free_entries = 0u; i < number_of_entries && number_of_free_entries < num_of_cache_entries; ++i) {
        const uint32_t current_entries = bookkeeping_bitset[i];
        //at least one bit is 0
        if(current_entries != 0xFFFFFFFF) {
            for(uint8_t j = 0u; j < 32u && number_of_free_entries < num_of_cache_entries; ++j) {
                if(!bitset_at((i*32u)+j, bookkeeping_bitset)) {
                    bitset_cache[number_of_free_entries++] = (int32_t) ((i*32u)+j);
                }
            }
        }
    }
    //we store this to be able to detect if we are out of memory
    *has_searched_cache = true; //it might not have filled the bitset cache if there is no free memory or not enough freed memory


    return allocate_impl(bookkeeping_bitset, number_of_entries, bitset_cache, num_of_cache_entries, has_searched_cache); //grabs the first entry from bitset_cache and triggers the early return.
}

uint32_t bitmap_allocate(uint32_t *const bitset, const uint32_t n, int32_t *const bitset_cache, const uint32_t cache_n, bool *const has_filled_bitset_cache) {
    const int32_t allocated_page = allocate_impl(bitset, n, bitset_cache, cache_n, has_filled_bitset_cache);
    //TODO: don't use signed integers at all. This -1 to 0 conversion is also broken for other reasons.
    return (allocated_page == -1) ? 0u : allocated_page; //turn -1 into NULL
}

void bitmap_free(uint32_t *const bitset, const uint32_t n, const uint32_t index) {
    if(n*sizeof(uint32_t) > index) { //make sure we don't index out of bounds
        bitset_set_at(index, bitset, 0u);
    }
}



uint32_t bitmap_find_first_zero_bit(uint32_t *const bitset, const uint32_t n) {
    for(uint32_t i = 0u; i < n; ++i) {
        const uint32_t current_entries = bitset[i];
        //at least one bit is 0
        if(current_entries != 0xFFFFFFFF) {
            for(uint8_t j = 0u; j < 32u; ++j) {
                if(!bitset_at((i*32u)+j, bitset)) {
                    return ((i*32u)+j);
                }
            }
        }
    }
    return n;
}
