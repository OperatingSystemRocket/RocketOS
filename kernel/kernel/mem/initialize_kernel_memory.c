#include <mem/initialize_kernel_memory.h>


static uint32_t malloc_heap_start;

static uint32_t acpica_start;

static uint32_t global_virt_allocator_start;

void* get_heap_range_start(void) {
    return (void*)(malloc_heap_start*PAGE_SIZE);
}
size_t get_max_heap_size(void) {
    return MAX_SIZE_OF_HEAP;
}

void* get_acpica_start(void) {
    return (void*)(acpica_start*PAGE_SIZE);
}
size_t get_acpica_size(void) {
    return ACPICA_SIZE;
}

void* get_global_virt_allocator_start(void) {
    return (void*)(global_virt_allocator_start*PAGE_SIZE);
}
size_t get_global_virt_allocator_size(void) {
    return NUMBER_OF_PAGES_IN_GLOBAL_VIRT_ALLOCATOR;
}

void initialize_kernel_memory(void) {
    serial_writestring("initialize_kernel_memory() called\n");
    global_allocator_init();
    serial_writestring("after global_allocator_init() called\n");
    paging_init();
    serial_writestring("after paging_init() called\n");


    //kernel itself
    global_reserve_physical_address(0x0, get_number_of_permanently_reserved_pages(), CRITICAL_KERNEL_USE);
    reserve_virtual_address(0x0, get_number_of_permanently_reserved_pages(), CRITICAL_KERNEL_USE);

    //kernel heap
    const uint32_t kernel_heap_start = get_number_of_permanently_reserved_pages()+1u;
    const size_t kernel_heap_size = get_number_of_pages_used()-get_number_of_permanently_reserved_pages(); //in pages
    global_reserve_physical_address(kernel_heap_start, kernel_heap_size, KERNEL_USE);
    reserve_virtual_address(kernel_heap_start, kernel_heap_size, KERNEL_USE);

    //acpica virtual memory range
    acpica_start = get_number_of_pages_used()+1u;
    reserve_virtual_address(acpica_start, ACPICA_SIZE, KERNEL_USE);


    for(uint32_t i = 0u; i < (uint32_t) get_first_nonreserved_address(); i += PAGE_SIZE) {
        identity_map_page((uint32_t)get_default_page_directory(), i, PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);
    }


    load_and_turn_on_paging();

    global_virt_allocator_start = (acpica_start+ACPICA_SIZE)+1u;
    reserve_virtual_address(global_virt_allocator_start, NUMBER_OF_PAGES_IN_GLOBAL_VIRT_ALLOCATOR, USER_USE);

    malloc_heap_start = (global_virt_allocator_start+NUMBER_OF_PAGES_IN_GLOBAL_VIRT_ALLOCATOR)+1u;
    reserve_virtual_address(malloc_heap_start, MAX_SIZE_OF_HEAP, USER_USE);
}
