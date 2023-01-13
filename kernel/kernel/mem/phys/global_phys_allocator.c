#include "global_phys_allocator.h"


struct global_phys_allocator {
    uint32_t pages_1024[32]; // in 4MiB blocks
    uint32_t pages_512[64];
    uint32_t pages_256[128];
    uint32_t pages_128[256];
    uint32_t pages_64[512];
    uint32_t pages_32[1024];
    uint32_t pages_16[2048];
    uint32_t oct_pages[4096];
    uint32_t quad_pages[8192];
    uint32_t double_pages[16384];
    uint32_t pages[32768]; // in individual pages
};

static struct global_phys_allocator global_phys_allocator;


bool global_phys_allocator_init(void) {
    const bool ret0 = binary_buddy_memory_allocator_init(&global_phys_allocator, sizeof(struct global_phys_allocator));
    serial_writestring("after binary_buddy_memory_allocator_init\n");
    if(!ret0) {
        return false;
    }

#if 0
    const uint32_t val = ((V2P(get_endkernel())/PAGE_SIZE) + ((V2P(get_endkernel())%PAGE_SIZE)>0u));
    kprintf("((V2P(get_endkernel())/PAGE_SIZE) + ((V2P(get_endkernel()) PAGE_SIZE)>0u)): %u\n", val);

    const bool ret1 = binary_buddy_memory_allocator_reserve(&global_phys_allocator, 0, ((V2P(get_endkernel())/PAGE_SIZE) + ((V2P(get_endkernel())%PAGE_SIZE)>0u)), 32768, 0, 11);
    serial_writestring("after binary_buddy_memory_allocator_reserve\n");
#endif

    return true; //ret1;
}

void* global_phys_allocator_allocate_page(void) {
    return global_phys_allocator_allocate_pages(1);
}
void* global_phys_allocator_allocate_pages(const size_t num_of_pages) {
    return binary_buddy_memory_allocator_allocate(&global_phys_allocator, PAGE_SIZE, 32768, 11, num_of_pages);
}

bool global_phys_allocator_free_page(void *const page_phys_addr) {
    return global_phys_allocator_free_pages(page_phys_addr, 1);
}
bool global_phys_allocator_free_pages(void *const page_phys_addr, const size_t num_of_pages) {
    return binary_buddy_memory_allocator_free(&global_phys_allocator, PAGE_SIZE, 32768, 11, page_phys_addr, num_of_pages);
}



















//TODO: move to more apprioriate file

//The address of this variable is set in the linker script. It has the ending address of the kernel.
//We just need it for the address, it stores no value.
#if 0
extern uint32_t text_start;
extern uint32_t text_end;
extern uint32_t rodata_start;
extern uint32_t rodata_end;
extern uint32_t mutable_data_start;
extern uint32_t mutable_data_end;

uint32_t get_text_start(void) {
    return (uint32_t)&text_start;
}
uint32_t get_text_end(void) {
    return (uint32_t)&text_end;
}
uint32_t get_rodata_start(void) {
    return (uint32_t)&rodata_start;
}
uint32_t get_rodata_end(void) {
    return (uint32_t)&rodata_end;
}
uint32_t get_mutable_data_start(void) {
    return (uint32_t)&mutable_data_start;
}
uint32_t get_mutable_data_end(void) {
    return (uint32_t)&mutable_data_end;
}
#endif


extern uint32_t endkernel;

uint32_t get_endkernel(void) {
    return (uint32_t)&endkernel;
}

