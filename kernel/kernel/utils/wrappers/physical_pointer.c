#include "physical_pointer.h"


struct physical_pointer create_physical_pointer(const size_t phys_p, const size_t pages_p) {
    if(pages_p > 0) {
        void *const virt = kernel_virt_allocator_allocate_pages(pages_p);

        if(virt) {
            if(!map_pages_in_kernel_addr(virt, phys_p, pages_p, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW)) {
                return (struct physical_pointer){ phys_p, pages_p, 0 };
            }
            return (struct physical_pointer){ phys_p, pages_p, (size_t)virt };
        }
        return (struct physical_pointer){ phys_p, pages_p, 0 };
    }
    return (struct physical_pointer){ phys_p, 0, 0 };
}

bool free_physical_pointer(struct physical_pointer *const physical_pointer) {
    void *const virt = (void*)physical_pointer->virt;
    unmap_pages_in_kernel_addr(virt, physical_pointer->pages);
    if(!kernel_virt_allocator_free_pages(virt, physical_pointer->pages)) {
        return false;
    }

    physical_pointer->virt = 0;
    physical_pointer->pages = 0;

    return true;
}
