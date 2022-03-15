#include <mem/global_virt_allocator.h>


static struct osi_memory_allocator global_virt_allocator;

void global_virt_allocator_init(void) {
    osi_memory_allocator_init(&global_virt_allocator, get_global_virt_allocator_start(), get_global_virt_allocator_size());
}

void* global_virt_allocator_allocate(const uint32_t num_of_pages) {
    return osi_memory_allocator_allocate(&global_virt_allocator, num_of_pages);
}

void global_virt_allocator_free(const void *const ptr, const uint32_t num_of_pages) {
    osi_memory_allocator_free(&global_virt_allocator, ptr, num_of_pages);
}


void* global_allocate_phys_virt_mem(const uint32_t num_of_pages) {
    void *const virt_mem = global_virt_allocator_allocate(num_of_pages);
    if (virt_mem == NULL) {
        return NULL;
    }
    const uint32_t virtual_page_address = (uint32_t)virt_mem;
    for(uint32_t i = 0; i < num_of_pages; ++i) {
        map_page((void*)(virtual_page_address+(i*PAGE_SIZE)), allocate_page(USER_USE), PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);
    }
    return virt_mem;
}
void global_free_phys_virt_mem(const void *const ptr, const uint32_t num_of_pages) {
    const uint32_t virtual_page_address = (uint32_t)ptr;
    for(uint32_t i = 0; i < num_of_pages; ++i) {
        const uint32_t physical_page = get_physical_address(virtual_page_address+(i*PAGE_SIZE));
        unmap_page((void*)(virtual_page_address+(i*PAGE_SIZE)));
        free_page(USER_USE, physical_page);
    }
    global_free_phys_virt_mem(ptr, num_of_pages);
}
