#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <osi_memory_allocator.h>
#include <paging.h>


struct physical_pointer {
    const size_t phys; ///< The physical memory address
    const size_t pages; ///< The number of pages
    size_t virt; ///< The virtual memory
};

struct physical_pointer create_physical_pointer(size_t phys_p, size_t pages_p);


