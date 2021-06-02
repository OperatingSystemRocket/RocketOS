#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h>

#include "paging.h"
#include "kstring.h"


uint32_t get_size(uint32_t size_word);
bool get_allocated_bit(uint32_t size_word);

void heap_dump(size_t number_of_pages);
void freelist_dump(bool increment);

inline int32_t bytes_to_words(const uint32_t bytes) {
    return (bytes/4) + (bytes%4 > 0);
}

void kdynamic_memory_init(void);
void* kmalloc(size_t size);
void* zeroed_out_kmalloc(size_t size);
void kfree(const void* payload_ptr);
void* krealloc(void* ptr, size_t new_size);
void* zeroed_out_krealloc(void* ptr, size_t new_size);

uint32_t* get_head(void);

/// Doesn't do anything with endptr at the moment
int64_t kstrtol(const char* src, char** endptr, int8_t base);

