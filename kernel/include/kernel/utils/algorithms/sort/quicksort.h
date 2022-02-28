#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "swap.h"

#include "osi_memory_allocator.h"


//TODO: implement these two interfaces and remove the current one
//void kqsort(void* ptr, size_t count, size_t size, int (*comp)(const void*, const void*));
//void kqsort_range(void* ptr, uint32_t lo_index, uint32_t hi_index, size_t size, int (*comp)(const void*, const void*));

void quicksort_array(uint32_t* array, uint32_t lo_index, uint32_t hi_index);
void quicksort_linked_list_head(struct memory_bookkeeping_node* head);
void quicksort_linked_list_head_tail(struct memory_bookkeeping_node* head, struct memory_bookkeeping_node* tail);
