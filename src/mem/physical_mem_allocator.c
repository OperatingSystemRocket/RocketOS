#include "physical_mem_allocator.h"

//The address of this variable is set in the linker script. It has the ending address of the kernel.
//We just need it for the address, it stores no value.
extern uint32_t endkernel;


#define NUMBER_OF_PAGES (0x100000000u/PAGE_SIZE)


#define NUMBER_OF_PAGES_IN_KERNEL_HEAP (4096u)


uint32_t global_heap[NUMBER_OF_PAGES/32u] __attribute__((aligned(PAGE_SIZE))); //each bit in this buffer is a page
bool has_filled_freed_pages_list = false;
//global page cache
int32_t most_recently_freed_pages[20u]; //store a cache of 20 pages so that we only have to do expensive allocation every 20 pages


uint32_t internal_kernel_heap[NUMBER_OF_PAGES_IN_KERNEL_HEAP/32u]; __attribute__((aligned(PAGE_SIZE)));
bool kernel_page_cache_seached = false;
int32_t internal_kernel_free_page_cache[20u];


uint32_t* first_nonreserved_address;


void allocate_init(void) {
    kmemset(global_heap, 0, sizeof(uint32_t) * (NUMBER_OF_PAGES/32u));
    for(int32_t i = 0; i < 20; ++i) {
        most_recently_freed_pages[i] = -1;
    }

    const uintptr_t end_address = (uintptr_t)&endkernel;
    size_t number_of_pages_used = end_address / PAGE_SIZE;
    if(number_of_pages_used % PAGE_SIZE) {
        ++number_of_pages_used;
    }
    //reserve an extra 4 MiB to make sure the multiboot structure is safe
    number_of_pages_used += 1024u;

    number_of_pages_used += NUMBER_OF_PAGES_IN_KERNEL_HEAP; //reserve the memory for the kernel heap so that it isn't allocated in the global heap
    kmemset(internal_kernel_heap, 0, sizeof(uint32_t) * (NUMBER_OF_PAGES_IN_KERNEL_HEAP/32u));
    for(int32_t i = 0; i < 20; ++i) {
        internal_kernel_free_page_cache[i] = -1;
    }

    kassert_void(number_of_pages_used > 0);

    //Marks all pages that are used by the kernel and below as used in the page table cache and never frees them.
    //This also protects some mmio addresses/ports.
    //TODO: replace with call to kmemset at some point
    for(int32_t i = 0; i < number_of_pages_used/32u; ++i) {
        global_heap[i] = 0xFFFFFFFF;
    }
    for(uint8_t i = 0u; i < number_of_pages_used%32u; ++i) {
        set_at(number_of_pages_used-i-1u, global_heap, 1);
    }

    first_nonreserved_address = (void*)(number_of_pages_used*PAGE_SIZE);
}


void* allocate_page_impl(uint32_t *const bookkeeping_bitset, const size_t number_of_entries, int32_t *const page_cache, bool *const has_searched_cache) {
    for(int32_t i = 0; i < 20; ++i) {
        const int32_t current_entry = page_cache[i];
        if(current_entry != -1) {
            page_cache[i] = -1;
            bookkeeping_bitset[current_entry/32] |= 1u << (current_entry%32);
            *has_searched_cache = false;
            kprintf("allocated index: %i\n", current_entry);
            return (void*)(current_entry*PAGE_SIZE);
        }
    }


    //we have already tried to allocate pages and failed
    if(*has_searched_cache) {
        *has_searched_cache = false; //we will try to allocate next time this function is called
        return NULL;
    }

    //page table cache is empty, must do expensive allocation:
    for(int32_t i = 0, number_of_free_pages = 0; i < 1024 && number_of_free_pages < 20; ++i) {
        const uint32_t current_pages = bookkeeping_bitset[i];
        //at least one bit is 0
        if(current_pages != 0xFFFFFFFF) {
            for(uint8_t j = 0u; j < 32u && number_of_free_pages < 20; ++j) {
                if(at((i*32u)+j, bookkeeping_bitset) == 0) {
                    page_cache[number_of_free_pages++] = (i*32u)+j;
                }
            }
        }
    }
    //we store this to be able to detect if we are out of memory
    *has_searched_cache = true; //it might not have filled the page table if there is no free memory or not enough freed memory


    return allocate_page_impl(bookkeeping_bitset, number_of_entries, page_cache, has_searched_cache); //grabs the first page from most_recently_freed_pages and triggers the early return.
}


void* allocate_page(const enum memory_type allocation_type) {
    if(allocation_type == USER_USE) {
        return allocate_page_impl(global_heap, NUMBER_OF_PAGES/32u, most_recently_freed_pages, &has_filled_freed_pages_list);
    }
    return allocate_page_impl(internal_kernel_heap, NUMBER_OF_PAGES_IN_KERNEL_HEAP/32u, internal_kernel_free_page_cache, &kernel_page_cache_seached);
}


void free_page(const enum memory_type allocation_type, const void *const page) {
    kprintf("freed page: %i\n", ((uintptr_t)page)/PAGE_SIZE);

    if(allocation_type == USER_USE) {
        set_at(((uintptr_t)page)/PAGE_SIZE, global_heap, 0u);
    } else {
        set_at(((uintptr_t)page)/PAGE_SIZE, internal_kernel_heap, 0u);
    }
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
