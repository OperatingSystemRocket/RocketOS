#include "kernel_virt_allocator.h"


struct kernel_virt_allocator { // owns 1GiB of the address space (the upper GiB)
    uint32_t pages_256[32];
    uint32_t pages_128[64];
    uint32_t pages_64[128];
    uint32_t pages_32[256];
    uint32_t pages_16[512];
    uint32_t oct_pages[1024];
    uint32_t quad_pages[2048];
    uint32_t double_pages[4096];
    uint32_t pages[8192]; // in individual pages
};

static struct kernel_virt_allocator kernel_virt_allocator;


bool kernel_virt_allocator_init(void) {
    const bool ret0 = binary_buddy_memory_allocator_init(&kernel_virt_allocator, sizeof(struct kernel_virt_allocator));
    if(!ret0) {
        return false;
    }

    const uint32_t val = ((V2P(get_endkernel())/PAGE_SIZE) + ((V2P(get_endkernel())%PAGE_SIZE)>0u));
    kprintf("((V2P(get_endkernel())/PAGE_SIZE) + ((V2P(get_endkernel()) PAGE_SIZE)>0u)): %u\n", val);

    //TODO: double check the -1u later in the end_index param
    const bool ret1 = binary_buddy_memory_allocator_reserve(&kernel_virt_allocator, 0, ((V2P(get_endkernel())/PAGE_SIZE) + ((V2P(get_endkernel())%PAGE_SIZE)>0u))-1u, 8192, 0, 9);
    if(!ret1) {
        return false;
    }
    kprintf("Reserved %u pages for kernel\n", val);

    // 0 --- 7, 8 --- 15, 16 --- 23, 24 --- 31
    // 16 = start index, increments of `uint32_t`, do not allow indexes between
    // 23 = end index, increments of `uint32_t`, do not allow indexes between
    //32 = num of elements in `uint32_t`'s in the level
    // 8 = level of allocator index, 8 is the highest level, 0 is lowest
    // 9 = number of allocator levels
    const bool ret2 = binary_buddy_memory_allocator_reserve(&kernel_virt_allocator, 16, 23, 32, 8, 9);
    kprintf("Reserved %u pages for heap\n", val);
    return ret2;
}

bool kernel_virt_allocator_reserve_page(void *const page_virt_addr) {
    return kernel_virt_allocator_reserve_pages(page_virt_addr, 1);
}
bool kernel_virt_allocator_reserve_pages(void *const page_virt_addr, const size_t num_of_pages) {
    //TODO: double check the -1u later in the start_index and end_index params
    return binary_buddy_memory_allocator_reserve(&kernel_virt_allocator, ((uint32_t)V2P(page_virt_addr))/PAGE_SIZE-1u, (((uint32_t)V2P(page_virt_addr))/PAGE_SIZE-1u)+num_of_pages, 8192, 0, 9);
}

void* kernel_virt_allocator_allocate_page(void) {
    return kernel_virt_allocator_allocate_pages(1);
}
void* kernel_virt_allocator_allocate_pages(const size_t num_of_pages) {
    return (void*)P2V(binary_buddy_memory_allocator_allocate(&kernel_virt_allocator, PAGE_SIZE, 8192, 9, num_of_pages));
}

bool kernel_virt_allocator_free_page(void *const page_virt_addr) {
    return kernel_virt_allocator_free_pages(page_virt_addr, 1);
}
bool kernel_virt_allocator_free_pages(void *const page_virt_addr, const size_t num_of_pages) {
    return binary_buddy_memory_allocator_free(&kernel_virt_allocator, PAGE_SIZE, 8192, 9, (void*)V2P(page_virt_addr), num_of_pages);
}
