#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "bitmap_allocator.h"
#include "initialize_kernel_memory.h"
#include "kstdlib.h"


struct memory_bookkeeping_node {
    struct memory_bookkeeping_node* prev;
    struct memory_bookkeeping_node* next;
    uint32_t page_index; //which page is it in the virtual memory range with `start_of_virt_mem` as index `0`
    uint32_t num_of_pages;
};

struct osi_memory_allocator {
    uint32_t* start_of_virt_mem;
    uint32_t number_of_pages;

    struct memory_bookkeeping_node* allocated_list_head;
    struct memory_bookkeeping_node* allocated_list_tail;
    struct memory_bookkeeping_node* free_list_head;
    struct memory_bookkeeping_node* free_list_tail;
};


void osi_memory_allocator_init(struct osi_memory_allocator* allocator, uint32_t* start_of_virt_mem, uint32_t number_of_pages);
void* osi_memory_allocator_allocate(struct osi_memory_allocator* allocator, uint32_t num_of_pages);
void osi_memory_allocator_free(struct osi_memory_allocator* allocator, const void* ptr, uint32_t num_of_pages);
