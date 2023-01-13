#include "heap_virt_mem_allocator.h"


static uint32_t num_of_pages_allocated = 0u;

void* kernel_virt_mem_allocate_page(void) {
    return kernel_virt_mem_allocate_pages(1u);
}
void* kernel_virt_mem_allocate_pages(const size_t num_of_pages) {
    if(num_of_pages_allocated+num_of_pages > MAX_KERNEL_HEAP_SIZE) {
        return NULL;
    }

    void *const page_virt_addr = ADD_TWO_PTRS(HEAP_START_ADDR, num_of_pages_allocated*PAGE_SIZE);

    num_of_pages_allocated += num_of_pages;

    return page_virt_addr;
}

