#pragma once

#include <stddef.h>


inline void swap(void *const a, void *const b, const size_t size) {
    char *const a_ptr = a;
    char *const b_ptr = b;
    for(size_t i = 0u; i < size; ++i) {
        const char tmp = a_ptr[i];
        a_ptr[i] = b_ptr[i];
        b_ptr[i] = tmp;
    }
}
