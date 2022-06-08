#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <kstring.h>
#include <bitset.h>


//TODO: consider whether or not to have a `;` on the end of the struct declaration **inside** of the macro
#define GENERATE_BITMAP(NAMESPACE, N, ALIGNMENT) \
struct NAMESPACE##_bitmap_allocator { \
    uint32_t bitset[N/32u] __attribute__((aligned(ALIGNMENT))); \
    bool has_filled_bitset_cache; \
    int32_t bitset_cache[20u]; \
};

//TODO: consider whether or not to put `struct` **inside** of the macro
#define BITMAP_TYPENAME(NAMESPACE) NAMESPACE##_bitmap_allocator


void bitmap_allocator_init(uint32_t* bitset, uint32_t n, int32_t* bitset_cache, uint32_t cache_n, bool* has_filled_bitset_cache);
//returns index into bitmap allocator
uint32_t bitmap_allocate(uint32_t* bitset, uint32_t n, int32_t* bitset_cache, uint32_t cache_n, bool* has_filled_bitset_cache);
void bitmap_free(uint32_t* bitset, uint32_t n, uint32_t index);


//returns index just like `bitmap_allocate`
uint32_t bitmap_find_first_zero_bit(uint32_t* bitset, uint32_t n);
