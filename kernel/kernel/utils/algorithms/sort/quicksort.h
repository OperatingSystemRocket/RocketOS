#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <utils/algorithms/swap.h>


//TODO: implement these two interfaces and remove the current one
//void kqsort(void* ptr, size_t count, size_t size, int (*comp)(const void*, const void*));
//void kqsort_range(void* ptr, uint32_t lo_index, uint32_t hi_index, size_t size, int (*comp)(const void*, const void*));

void quicksort_array(uint32_t* array, uint32_t lo_index, uint32_t hi_index);
