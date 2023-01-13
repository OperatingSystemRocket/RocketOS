#include "initialize_kernel_memory.h"


bool initialize_kernel_memory(void) {
    //TODO: reserve the space used by the kernel itself (for both the physical and virtual memory allocators)
    if(!global_phys_allocator_init()) {
        serial_writestring("Failed to initialize global physical memory allocator\n");
        return false;
    }
    serial_writestring("Initialized global physical memory allocator\n");
    if(!kernel_virt_allocator_init()) { //TODO: reserve the max amount of kernel space in the virtual memory allocator
        serial_writestring("Failed to initialize kernel virtual memory allocator\n");
        return false;
    }
    serial_writestring("Initialized kernel virtual memory allocator\n");

    if(!paging_init()) {
        serial_writestring("Failed to initialize paging\n");
        return false;
    }
    serial_writestring("Initialized paging\n");

    return true;
}
