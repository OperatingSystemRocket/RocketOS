#include "osi_virt_mem_allocator.h"


#define MAX_ACPICA_SIZE_IN_UINT32 (MAX_ACPICA_SIZE/32)

struct osi_buddy_memory_allocator {
    uint32_t quad_pages[MAX_ACPICA_SIZE_IN_UINT32/4];
    uint32_t double_pages[MAX_ACPICA_SIZE_IN_UINT32/2];
    uint32_t pages[MAX_ACPICA_SIZE_IN_UINT32]; // MAX_ACPICA_SIZE_IN_UINT32 is 32
};

void* osi_virt_range_start;
static struct osi_buddy_memory_allocator osi_buddy_memory_allocator;

bool osi_virt_mem_allocator_init(void) {
    osi_virt_range_start = kernel_virt_allocator_allocate_pages(MAX_ACPICA_SIZE);
    if(osi_virt_range_start == NULL) {
        return false;
    }
    return kmemset(&osi_buddy_memory_allocator, 0, sizeof(struct osi_buddy_memory_allocator)) != NULL;
}

void* osi_virt_mem_allocator_allocate_page(void) {
    return osi_virt_mem_allocator_allocate_pages(1);
}
void* osi_virt_mem_allocator_allocate_pages(const size_t num_of_pages) {
    return ADD_TWO_PTRS(osi_virt_range_start, binary_buddy_memory_allocator_allocate(&osi_buddy_memory_allocator, PAGE_SIZE, MAX_ACPICA_SIZE, 3, num_of_pages));
}

bool osi_virt_mem_allocator_free_page(void *const page_virt_addr) {
    return osi_virt_mem_allocator_free_pages(page_virt_addr, 1);
}
bool osi_virt_mem_allocator_free_pages(void *const page_virt_addr, const size_t num_of_pages) {
    return binary_buddy_memory_allocator_free(&osi_buddy_memory_allocator, PAGE_SIZE, MAX_ACPICA_SIZE, 3, SUB_TWO_PTRS(page_virt_addr, osi_virt_range_start), num_of_pages);
}
