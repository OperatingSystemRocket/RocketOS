#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h>

#include "paging.h"
#include "initialize_kernel_memory.h"
#include "kstring.h"


inline uint32_t bytes_to_words(const uint32_t bytes) {
    return (bytes/4u) + (bytes%4u > 0);
}

inline uint32_t round_up_to_nearest_n_power(const uint32_t num_to_round, const uint32_t multiple) {
    if(multiple == 0u) return num_to_round;

    uint32_t remainder = num_to_round % multiple;
    if(remainder == 0u) {
        return num_to_round;
    }

    return num_to_round + multiple - remainder;
}

inline uint32_t get_size(const uint32_t size_word) {
    return size_word & 0x7fffffff;
}
inline bool get_allocated_bit(const uint32_t size_word) {
    return (size_word & 0x80000000) > 0;
}


void kdynamic_memory_init(void);
void* kmalloc(size_t size);
void* zeroed_out_kmalloc(size_t size);
void kfree(const void* payload_ptr);
void* krealloc(void* ptr, size_t new_size);
void* zeroed_out_krealloc(void* ptr, size_t new_size);
void* kcalloc(size_t num, size_t size);
void* uninitialized_kcalloc(size_t num, size_t size);

uint32_t* get_head(void);
