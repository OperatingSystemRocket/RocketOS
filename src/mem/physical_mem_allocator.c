#include "physical_mem_allocator.h"

//The address of this variable is set in the linker script. It has the ending address of the kernel.
//We just need it for the address, it stores no value.
extern uint32_t endkernel;

//32 bit = 4 GiB
//1 GiB = 1,048,576 KiB
//4 GiB = 4,294,967,296 bytes
//1 page = 4 KiB
//I cap the amount of pages at 1024 so that I use a page for bookkeeping.
//This maxes the heap out at 32*4*1024*1024 (134,217,728 bytes), much less than 4 GiB, but no one seems to use all of the 4 GiB from my research.
//I think this array would need to have 32,768 (32*1024) entries to map all of the 4 GiB
uint32_t page_bitmap_table[1024u] __attribute__((aligned(4096))); //each bit in this buffer is a page

bool has_filled_freed_pages_list = false;
uint32_t most_recently_freed_pages[20u]; //store a cache of 20 pages so that we only have to do expensive allocation every 20 pages


#define PAGE_SIZE 4096


void allocate_init(void) {
    kmemset(page_bitmap_table, 0, sizeof(uint32_t) * 1024u);
    kmemset(most_recently_freed_pages, 0, sizeof(uint32_t) * 20u);
    const uintptr_t end_address = (uintptr_t)&endkernel;
    size_t number_of_pages_used = end_address / PAGE_SIZE;
    if(number_of_pages_used % PAGE_SIZE) {
        ++number_of_pages_used;
    }
    kassert_void(number_of_pages_used > 0);

    //Marks all pages that are used by the kernel and below as used in the page table cache and never frees them.
    //This also protects some mmio addresses/ports.
    for(int32_t i = 0; i < number_of_pages_used/32u; ++i) {
        page_bitmap_table[i] = 0xFFFFFFFF;
    }
    for(uint8_t i = 0u; i < number_of_pages_used%32u; ++i) {
        set_at(number_of_pages_used-i-1u, page_bitmap_table, 1);
    }
}

void* allocate_page(void) {
    for(int32_t i = 0; i < 20; ++i) {
        const uint32_t current_entry = most_recently_freed_pages[i];
        if(current_entry != 0) {
            most_recently_freed_pages[i] = 0u;
            page_bitmap_table[current_entry/32u] |= 1u << (current_entry%32u);
            has_filled_freed_pages_list = false;
            kprintf("allocated index: %i\n", current_entry);
            return (void*)(current_entry*PAGE_SIZE);
        }
    }


    //we have already tried to allocate pages and failed
    if(has_filled_freed_pages_list) {
        has_filled_freed_pages_list = false; //we will try to allocate next time this function is called
        return NULL;
    }

    //page table cache is empty, must do expensive allocation:
    for(int32_t i = 0, number_of_free_pages = 0; i < 1024 && number_of_free_pages < 20; ++i) {
        const uint32_t current_pages = page_bitmap_table[i];
        //at least one bit is 0
        if(current_pages != 0xFFFFFFFF) {
            for(uint8_t j = 0u; j < 32u; ++j) {
                if(at((i*32u)+j, page_bitmap_table) == 0) {
                    most_recently_freed_pages[number_of_free_pages++] = (i*32u)+j;
                }
            }
        }
    }
    //we store this to be able to detect if we are out of memory
    has_filled_freed_pages_list = true; //it might not have filled the page table if there is no free memory or not enough freed memory

    return allocate_page(); //grabs the first page from most_recently_freed_pages and triggers the early return.
}

void free_page(const void *const page) {
    kprintf("freed page: %i\n", ((uintptr_t)page)/PAGE_SIZE);
    set_at(((uintptr_t)page)/PAGE_SIZE, page_bitmap_table, 0u);
}
