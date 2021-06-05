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

void kdynamic_memory_init(void);
void* kmalloc(size_t size);
void* zeroed_out_kmalloc(size_t size);
void kfree(const void* payload_ptr);
void* krealloc(void* ptr, size_t new_size);
void* zeroed_out_krealloc(void* ptr, size_t new_size);

uint32_t* get_head(void);


//TODO: put this in kstring.h instead
/// Doesn't do anything with endptr at the moment
int64_t kstrtol(const char* src, char** endptr, int8_t base);



inline int32_t bytes_to_words(const uint32_t bytes) {
    return (bytes/4) + (bytes%4 > 0);
}


int kprintf(const char *format, ...);
void* get_first_nonreserved_address(void);

inline void print_freelist(void) {
    kprintf("freelist\n");
    uint32_t* current_block = get_head();
    if(get_head() != NULL) {
        kprintf("head is not NULL\n");
    } else {
        kprintf("head is NULL\n");
    }
    while(current_block != NULL) {
        kprintf("first word size: %u, first word allocated bit: %u, second word: %u, third word: %u, last word size: %u, last word allocated bit: %u\n",
        get_size(current_block[0]), get_allocated_bit(current_block[0]), current_block[1], current_block[2], get_size(current_block[get_size(current_block[0])-1]), get_allocated_bit(current_block[get_size(current_block[0])-1]));
        current_block = current_block[2];
    }
}


inline void dump_heap(void) {
    kprintf("full heap dump\n");
    for(int32_t i = 0; i < 1024; ++i) {
        const uint32_t *const start_of_heap = get_first_nonreserved_address();
        kprintf("size: %u, is_allocated: %u\n", get_size(start_of_heap[i]), get_allocated_bit(start_of_heap[i]));
    }
}
