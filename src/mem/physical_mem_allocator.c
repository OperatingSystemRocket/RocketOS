#include "physical_mem_allocator.h"

//The address of this variable is set in the linker script. It has the ending address of the kernel.
//We just need it for the address, it stores no value.
extern uint32_t endkernel;


GENERATE_BITMAP(physical_mem_allocation, NUMBER_OF_PAGES, PAGE_SIZE)
GENERATE_BITMAP(kernel_physical_mem_allocation, NUMBER_OF_PAGES_IN_KERNEL_HEAP, PAGE_SIZE)

static struct BITMAP_TYPENAME(physical_mem_allocation) global_physical_memory_bitmap;
static struct BITMAP_TYPENAME(kernel_physical_mem_allocation) kernel_heap_bitmap;

static size_t number_of_permanently_reserved_pages;
static size_t number_of_pages_used;

static uint32_t* first_nonreserved_address;


void allocate_init(void) {
    bitmap_allocator_init(global_physical_memory_bitmap.bitset, NUMBER_OF_PAGES, global_physical_memory_bitmap.bitset_cache, CACHE_N, &global_physical_memory_bitmap.has_filled_bitset_cache);

    //reserve the memory for the kernel heap so that it isn't allocated in the global heap
    bitmap_allocator_init(kernel_heap_bitmap.bitset, NUMBER_OF_PAGES_IN_KERNEL_HEAP, kernel_heap_bitmap.bitset_cache, CACHE_N, &kernel_heap_bitmap.has_filled_bitset_cache);


    const uintptr_t end_address = (uintptr_t)&endkernel;
    number_of_pages_used = end_address / PAGE_SIZE;
    if(number_of_pages_used % PAGE_SIZE) {
        ++number_of_pages_used;
    }
    //reserve an extra 4 MiB to make sure the multiboot structure is safe
    number_of_pages_used += 1024u;

    //CRITICAL_KERNEL_USE number
    number_of_permanently_reserved_pages = number_of_pages_used;

    number_of_pages_used += NUMBER_OF_PAGES_IN_KERNEL_HEAP;

    first_nonreserved_address = (void*)(number_of_pages_used*PAGE_SIZE);
}


void reserve_physical_address(const uint32_t physical_address, const size_t num_of_pages, const enum memory_type type) {
    const uint32_t start_page_index = physical_address / PAGE_SIZE;

    //TODO: replace with call to kmemset at some point
    for(uint32_t i = 0u; i < num_of_pages/32u; ++i) {
        global_physical_memory_bitmap.bitset[start_page_index+i] = 0xFFFFFFFFu;
    }
    for(uint32_t i = 0u; i < num_of_pages%32u; ++i) {
        bitset_set_at(start_page_index+i, global_physical_memory_bitmap.bitset, true);
    }

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


void* allocate_page(const enum memory_type allocation_type) {
    if(allocation_type == USER_USE) {
        void *const allocated_page = (void*)(bitmap_allocate(global_physical_memory_bitmap.bitset, NUMBER_OF_PAGES, global_physical_memory_bitmap.bitset_cache, CACHE_N, &global_physical_memory_bitmap.has_filled_bitset_cache)*PAGE_SIZE);
        //kprintf("USER: allocated_page: %p\n", allocated_page);

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

void free_page(const enum memory_type allocation_type, const void *const page) {
    if(allocation_type == USER_USE) {
        bitmap_free(global_physical_memory_bitmap.bitset, NUMBER_OF_PAGES, ((uintptr_t)page)/PAGE_SIZE);
    } else {
        bitmap_free(kernel_heap_bitmap.bitset, NUMBER_OF_PAGES_IN_KERNEL_HEAP, ((uintptr_t)page)/PAGE_SIZE);
    }
}

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
