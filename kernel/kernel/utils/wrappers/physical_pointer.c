#include "physical_pointer.h"


struct physical_pointer create_physical_pointer(const size_t phys_p, const size_t pages_p) {
    if(pages_p > 0) {
        void *const virt = osi_memory_allocator_allocate(get_default_virt_allocator(), pages_p);

        if(virt) {
            if(!map_pages(virt, phys_p, pages_p, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW)) {
                return (struct physical_pointer){ phys_p, pages_p, 0 };
            }
            return (struct physical_pointer){ phys_p, pages_p, (size_t)virt };
        }
        return (struct physical_pointer){ phys_p, pages_p, 0 };
    }
    return (struct physical_pointer){ phys_p, 0, 0 };
}



