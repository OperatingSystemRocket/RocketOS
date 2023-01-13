#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kernel_virt_allocator.h>
#include <paging.h>


struct physical_pointer {
    size_t phys; ///< The physical memory address, DO NOT MODIFY YOURSELF (not marked `const` since it is mutated by the below functions)
    size_t pages; ///< The number of pages, DO NOT MODIFY YOURSELF (not marked `const` since it is mutated by the below functions)
    size_t virt; ///< The virtual memory
};


struct physical_pointer create_physical_pointer(size_t phys_p, size_t pages_p);

// frees resources allocated in `create_physical_pointer`, it does NOT free the physical memory
bool free_physical_pointer(struct physical_pointer* physical_pointer);
