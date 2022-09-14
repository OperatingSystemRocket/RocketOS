#include "buddy_memory_allocator.h"

//The address of this variable is set in the linker script. It has the ending address of the kernel.
//We just need it for the address, it stores no value.
extern uint32_t endkernel;

static struct global_binary_buddy_memory_allocator global_physical_memory_bitmaps;

GENERATE_BITMAP(kernel_physical_mem_allocation, NUMBER_OF_PAGES_IN_KERNEL_HEAP, PAGE_SIZE)
static struct BITMAP_TYPENAME(kernel_physical_mem_allocation) kernel_heap_bitmap;

static size_t number_of_permanently_reserved_pages;
static size_t number_of_pages_used;

static uint32_t* first_nonreserved_address;


void global_binary_buddy_memory_allocator_init(struct global_binary_buddy_memory_allocator *const allocator) {
    serial_writestring("global_binary_buddy_memory_allocator_init called\n");
    kmemset(allocator, 0, sizeof(struct global_binary_buddy_memory_allocator));
}
struct array_level {
    uint32_t level;
    uint32_t* bitset;
    uint32_t num_of_elements;
};
static struct array_level block_level(struct global_binary_buddy_memory_allocator *const allocator, const uint32_t num_of_pages) {
    switch(num_of_pages) {
        case 1:
            return (struct array_level){20, allocator->level20, 32768};
        case 2:
            return (struct array_level){19, allocator->level19, 16384};
        case 4:
            return (struct array_level){18, allocator->level18, 8192};
        case 8:
            return (struct array_level){17, allocator->level17, 4096};
        case 16:
            return (struct array_level){16, allocator->level16, 2048};
        case 32:
            return (struct array_level){15, allocator->level15, 1024};
        case 64:
            return (struct array_level){14, allocator->level14, 512};
        case 128:
            return (struct array_level){13, allocator->level13, 256};
        case 256:
            return (struct array_level){12, allocator->level12, 128};
        case 512:
            return (struct array_level){11, allocator->level11, 64};
        case 1024:
            return (struct array_level){10, allocator->level10, 32};
        default:
            return (struct array_level){0, NULL, 0}; // you chose too big of a number or your number is not a power of 2
    }
}
void* global_binary_buddy_memory_allocator_allocate_pages(struct global_binary_buddy_memory_allocator *const allocator, const size_t num_of_pages) {
    struct array_level level = block_level(allocator, num_of_pages);
    if(level.level == 0) {
        return NULL;
    }
    const uint32_t index = bitmap_find_first_zero_bit(level.bitset, level.num_of_elements);
    //kprintf("index: %u\n", index);
    if(index == level.num_of_elements) {
        return NULL;
    }
    const uint32_t ret_phys_address = index * (1u << (12 + (20-level.level))); // (1u << (12 + (20-level.level))) = how many bytes each bit in that level represents
    uint32_t current_index = index;
    switch(level.level) {
        case 20:
            bitset_set_at(current_index, allocator->level20, true);
            current_index /= 2;
        case 19:
            bitset_set_at(current_index, allocator->level19, true);
            current_index /= 2;
        case 18:
            bitset_set_at(current_index, allocator->level18, true);
            current_index /= 2;
        case 17:
            bitset_set_at(current_index, allocator->level17, true);
            current_index /= 2;
        case 16:
            bitset_set_at(current_index, allocator->level16, true);
            current_index /= 2;
        case 15:
            bitset_set_at(current_index, allocator->level15, true);
            current_index /= 2;
        case 14:
            bitset_set_at(current_index, allocator->level14, true);
            current_index /= 2;
        case 13:
            bitset_set_at(current_index, allocator->level13, true);
            current_index /= 2;
        case 12:
            bitset_set_at(current_index, allocator->level12, true);
            current_index /= 2;
        case 11:
            bitset_set_at(current_index, allocator->level11, true);
            current_index /= 2;
        case 10:
            bitset_set_at(current_index, allocator->level10, true);
    }
    const uint32_t down_marking_index = index + 1;
    current_index = index*2;
    switch(level.level) {
        case 11:
            bitset_set_at(current_index, allocator->level11, true);
            bitset_set_at(current_index+1, allocator->level11, true);
            current_index *= 2;
        case 12:
            bitset_set_at(current_index, allocator->level12, true);
            bitset_set_at(current_index+1, allocator->level12, true);
            current_index *= 2;
        case 13:
            bitset_set_at(current_index, allocator->level13, true);
            bitset_set_at(current_index+1, allocator->level13, true);
            current_index *= 2;
        case 14:
            bitset_set_at(current_index, allocator->level14, true);
            bitset_set_at(current_index+1, allocator->level14, true);
            current_index *= 2;
        case 15:
            bitset_set_at(current_index, allocator->level15, true);
            bitset_set_at(current_index+1, allocator->level15, true);
            current_index *= 2;
        case 16:
            bitset_set_at(current_index, allocator->level16, true);
            bitset_set_at(current_index+1, allocator->level16, true);
            current_index *= 2;
        case 17:
            bitset_set_at(current_index, allocator->level17, true);
            bitset_set_at(current_index+1, allocator->level17, true);
            current_index *= 2;
        case 18:
            bitset_set_at(current_index, allocator->level18, true);
            bitset_set_at(current_index+1, allocator->level18, true);
            current_index *= 2;
        case 19:
            bitset_set_at(current_index, allocator->level19, true);
            bitset_set_at(current_index+1, allocator->level19, true);
            current_index *= 2;
        case 20:
            bitset_set_at(current_index, allocator->level20, true);
            bitset_set_at(current_index+1, allocator->level20, true);
        // no need to do anything for 21 since there is nothing below a page in this allocator
    }
    //kprintf("%u\n", ret_phys_address);
    return (void*)ret_phys_address;
}
void* global_binary_buddy_memory_allocator_allocate_page(struct global_binary_buddy_memory_allocator *const allocator) {
    return global_binary_buddy_memory_allocator_allocate_pages(allocator, 1);
}
void global_binary_buddy_memory_allocator_free_pages(struct global_binary_buddy_memory_allocator *const allocator, const void *const first_page, const size_t num_of_pages) {
    struct array_level level = block_level(allocator, num_of_pages);
    if(level.level == 0) {
        return NULL;
    }
    const uint32_t index = (uint32_t)first_page / (1u << (12 + (20-level.level)));
    uint32_t current_index = index;
    switch(level.level) {
        case 20:
            bitset_set_at(current_index, allocator->level20, false);
            if(current_index % 2 == 0) {
                if(bitset_at(current_index+1, allocator->level20)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            } else {
                if(bitset_at(current_index-1, allocator->level20)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            }
            current_index /= 2;
        case 19:
            bitset_set_at(current_index, allocator->level19, false);
            if(current_index % 2 == 0) {
                if(bitset_at(current_index+1, allocator->level19)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            } else {
                if(bitset_at(current_index-1, allocator->level19)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            }
            current_index /= 2;
        case 18:
            bitset_set_at(current_index, allocator->level18, false);
            if(current_index % 2 == 0) {
                if(bitset_at(current_index+1, allocator->level18)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            } else {
                if(bitset_at(current_index-1, allocator->level18)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            }
            current_index /= 2;
        case 17:
            bitset_set_at(current_index, allocator->level17, false);
            if(current_index % 2 == 0) {
                if(bitset_at(current_index+1, allocator->level17)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            } else {
                if(bitset_at(current_index-1, allocator->level17)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            }
            current_index /= 2;
        case 16:
            bitset_set_at(current_index, allocator->level16, false);
            if(current_index % 2 == 0) {
                if(bitset_at(current_index+1, allocator->level16)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            } else {
                if(bitset_at(current_index-1, allocator->level16)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            }
            current_index /= 2;
        case 15:
            bitset_set_at(current_index, allocator->level15, false);
            if(current_index % 2 == 0) {
                if(bitset_at(current_index+1, allocator->level15)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            } else {
                if(bitset_at(current_index-1, allocator->level15)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            }
            current_index /= 2;
        case 14:
            bitset_set_at(current_index, allocator->level14, false);
            if(current_index % 2 == 0) {
                if(bitset_at(current_index+1, allocator->level14)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            } else {
                if(bitset_at(current_index-1, allocator->level14)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            }
            current_index /= 2;
        case 13:
            bitset_set_at(current_index, allocator->level13, false);
            if(current_index % 2 == 0) {
                if(bitset_at(current_index+1, allocator->level13)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            } else {
                if(bitset_at(current_index-1, allocator->level13)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            }
            current_index /= 2;
        case 12:
            bitset_set_at(current_index, allocator->level12, false);
            if(current_index % 2 == 0) {
                if(bitset_at(current_index+1, allocator->level12)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            } else {
                if(bitset_at(current_index-1, allocator->level12)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            }
            current_index /= 2;
        case 11:
            bitset_set_at(current_index, allocator->level11, false);
            if(current_index % 2 == 0) {
                if(bitset_at(current_index+1, allocator->level11)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            } else {
                if(bitset_at(current_index-1, allocator->level11)) {
                    return; // the other buddy is allocated, so don't move up the hierarchy
                }
            }
            current_index /= 2;
        case 10:
            bitset_set_at(current_index, allocator->level10, false);
    }
    const uint32_t down_marking_index = index + 1;
    current_index = index*2;
    switch(level.level) {
        case 11:
            bitset_set_at(current_index, allocator->level11, false);
            bitset_set_at(current_index+1, allocator->level11, false);
            current_index *= 2;
        case 12:
            bitset_set_at(current_index, allocator->level12, false);
            bitset_set_at(current_index+1, allocator->level12, false);
            current_index *= 2;
        case 13:
            bitset_set_at(current_index, allocator->level13, false);
            bitset_set_at(current_index+1, allocator->level13, false);
            current_index *= 2;
        case 14:
            bitset_set_at(current_index, allocator->level14, false);
            bitset_set_at(current_index+1, allocator->level14, false);
            current_index *= 2;
        case 15:
            bitset_set_at(current_index, allocator->level15, false);
            bitset_set_at(current_index+1, allocator->level15, false);
            current_index *= 2;
        case 16:
            bitset_set_at(current_index, allocator->level16, false);
            bitset_set_at(current_index+1, allocator->level16, false);
            current_index *= 2;
        case 17:
            bitset_set_at(current_index, allocator->level17, false);
            bitset_set_at(current_index+1, allocator->level17, false);
            current_index *= 2;
        case 18:
            bitset_set_at(current_index, allocator->level18, false);
            bitset_set_at(current_index+1, allocator->level18, false);
            current_index *= 2;
        case 19:
            bitset_set_at(current_index, allocator->level19, false);
            bitset_set_at(current_index+1, allocator->level19, false);
            current_index *= 2;
        case 20:
            bitset_set_at(current_index, allocator->level20, false);
            bitset_set_at(current_index+1, allocator->level20, false);
        // no need to do anything for 21 since there is nothing below a page in this allocator
    }
}
void global_binary_buddy_memory_allocator_free_page(struct global_binary_buddy_memory_allocator *const allocator, const void *const page) {
    global_binary_buddy_memory_allocator_free_pages(allocator, page, 1);
}



void global_allocator_init(void) {
    serial_writestring("global_allocator_init() called\n");
    kprintf("&global_physical_memory_bitmaps: %p\n", &global_physical_memory_bitmaps);
    global_binary_buddy_memory_allocator_init(&global_physical_memory_bitmaps);
    serial_writestring("after global_binary_buddy_memory_allocator_init() called\n");

    //reserve the memory for the kernel heap so that it isn't allocated in the global heap
    bitmap_allocator_init(kernel_heap_bitmap.bitset, NUMBER_OF_PAGES_IN_KERNEL_HEAP, kernel_heap_bitmap.bitset_cache, CACHE_N, &kernel_heap_bitmap.has_filled_bitset_cache);
    serial_writestring("after bitmap_allocator_init() called\n");


    const uintptr_t end_address = (uintptr_t)&endkernel;
    number_of_pages_used = V2P(end_address) / PAGE_SIZE;
    if(number_of_pages_used % PAGE_SIZE) {
        ++number_of_pages_used;
    }
    //reserve an extra 4 MiB to make sure the multiboot structure is safe
    number_of_pages_used += 1024u;

    //CRITICAL_KERNEL_USE number
    number_of_permanently_reserved_pages = number_of_pages_used;

    number_of_pages_used += NUMBER_OF_PAGES_IN_KERNEL_HEAP;

    first_nonreserved_address = (void*)P2V(number_of_pages_used*PAGE_SIZE);
}

static void mark_level(uint32_t *const allocator_bitmap_level, const uint32_t start_page_index, const uint32_t end_page_index, const uint32_t layer_num) {
    uint32_t level_lower_index = start_page_index / (1u << (20-layer_num));
    uint32_t level_upper_index = end_page_index / (1u << (20-layer_num));

    kprintf("layer_num: %u\n", layer_num);
    kprintf("start_page_index: %u\n", start_page_index);
    kprintf("end_page_index: %u\n", end_page_index);
    kprintf("level_lower_index: %u\n", level_lower_index);
    kprintf("level_upper_index: %u\n", level_upper_index);

    for(uint32_t i = level_lower_index; i <= level_upper_index; ++i) {
        bitset_set_at(i, allocator_bitmap_level, true);
    }
}
// page index is 0 indexed
static void mark_memory(struct global_binary_buddy_memory_allocator *const allocator, const uint32_t start_page_index, const uint32_t num_of_pages) {
    const uint32_t end_page_index = start_page_index + num_of_pages;

    if(allocator == NULL || num_of_pages == 0 || start_page_index >= NUMBER_OF_PAGES || end_page_index >= NUMBER_OF_PAGES) return;

    // handle level 10
    mark_level(allocator->level10, start_page_index, end_page_index, 10);

    // handle level 11
    mark_level(allocator->level11, start_page_index, end_page_index, 11);

    // handle level 12
    mark_level(allocator->level12, start_page_index, end_page_index, 12);

    // handle level 13
    mark_level(allocator->level13, start_page_index, end_page_index, 13);

    // handle level 14
    mark_level(allocator->level14, start_page_index, end_page_index, 14);

    // handle level 15
    mark_level(allocator->level15, start_page_index, end_page_index, 15);

    // handle level 16
    mark_level(allocator->level16, start_page_index, end_page_index, 16);

    // handle level 17
    mark_level(allocator->level17, start_page_index, end_page_index, 17);

    // handle level 18
    mark_level(allocator->level18, start_page_index, end_page_index, 18);

    // handle level 19
    mark_level(allocator->level19, start_page_index, end_page_index, 19);

    // handle level 20
    mark_level(allocator->level20, start_page_index, end_page_index, 20);
}
void global_reserve_physical_address(const uint32_t physical_address, const size_t num_of_pages, const enum memory_type type) {
    const uint32_t rounded_physical_address = (physical_address / PAGE_SIZE) * PAGE_SIZE;
    const adjusted_num_of_pages = num_of_pages + (physical_address % PAGE_SIZE > 0);

    const uint32_t start_page_index = rounded_physical_address / PAGE_SIZE;

    mark_memory(&global_physical_memory_bitmaps, start_page_index, adjusted_num_of_pages);

    if(type == CRITICAL_KERNEL_USE) {
        //TODO: replace with call to kmemset at some point
        for(uint32_t i = 0u; i < num_of_pages/32u; ++i) {
            kernel_heap_bitmap.bitset[start_page_index/32] = 0xFFFFFFFFu;
        }
        for(uint32_t i = 0u; i < num_of_pages%32u; ++i) {
            bitset_set_at(start_page_index+i, kernel_heap_bitmap.bitset, true);
        }
    }
}
void* global_allocate_page(enum memory_type allocation_type) {
    if(allocation_type == USER_USE) {
        void *const allocated_page = global_binary_buddy_memory_allocator_allocate_page(&global_physical_memory_bitmaps);

        return allocated_page;
    }

    void *const allocated_page = (void*)(bitmap_allocate(kernel_heap_bitmap.bitset, NUMBER_OF_PAGES_IN_KERNEL_HEAP, kernel_heap_bitmap.bitset_cache, CACHE_N, &kernel_heap_bitmap.has_filled_bitset_cache)*PAGE_SIZE);
    //kprintf("KERNEL: allocated_page: %p\n", allocated_page);
    if(allocated_page != NULL) {
        //NULL is a valid page and not necessarily a failure for critical kernel allocations,
        //but the first page allocation (which will get NULL), happens in `paging_init` and the result is overwritten,
        //so calling `kmemcpy` is redundant and will produce a spurious error message from a `kassert`.
        kmemset(allocated_page, 0u, PAGE_SIZE);
    }
    return allocated_page;
}
void* global_allocate_pages(enum memory_type allocation_type, size_t num_of_pages) {
    if(allocation_type == USER_USE) {
        void *const allocated_pages = global_binary_buddy_memory_allocator_allocate_pages(&global_physical_memory_bitmaps, num_of_pages);

        return allocated_pages;
    }

    if(num_of_pages == 1) {
        return global_allocate_page(allocation_type);
    } else {
        kprintf("non-USER_USE allocations can not be bigger than one page\n");
        return NULL;
    }
}
void global_free_page(enum memory_type allocation_type, const void* page) {
    if(allocation_type == USER_USE) {
        global_binary_buddy_memory_allocator_free_page(&global_physical_memory_bitmaps, page);
    } else {
        bitmap_free(kernel_heap_bitmap.bitset, NUMBER_OF_PAGES_IN_KERNEL_HEAP, ((uintptr_t)page)/PAGE_SIZE);
    }
}
void global_free_pages(enum memory_type allocation_type, const void* page, size_t num_of_pages) {}
size_t get_number_of_permanently_reserved_pages(void) {
    return number_of_permanently_reserved_pages;
}
size_t get_number_of_pages_used(void) {
    return number_of_pages_used;
}
void* get_first_nonreserved_address(void) {
    return first_nonreserved_address;
}
//in pages, not bytes:
size_t get_amount_of_nonreserved_memory(void) {
    //the +1 accounts for the fact that it is the first non-reserved address instead of the last reserved address
    const size_t number_of_nonreserved_pages = NUMBER_OF_PAGES - ((uintptr_t)first_nonreserved_address)/PAGE_SIZE + 1u;
    return number_of_nonreserved_pages*PAGE_SIZE;
}
uint32_t get_endkernel(void) {
    return (uint32_t)&endkernel;
}
