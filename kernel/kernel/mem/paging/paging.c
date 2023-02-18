#include "paging.h"

extern void load_page_directory(uint32_t *page_directory);
extern void enable_paging(void);

extern void enable_ring0_write_protect(void);

/*
Relevant Intel manual section on 32 bit paging:
https://web.archive.org/web/20151025081259if_/http://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-system-programming-manual-325384.pdf#G9.81100
*/

extern uint32_t boot_page_directory;
extern uint32_t boot_page_table;

static uint32_t *kernel_address_space;

static uint32_t kernel_page_directory[1024] __attribute__((aligned(PAGE_SIZE)));
static uint32_t kernel_page_table[1024] __attribute__((aligned(PAGE_SIZE)));
static uint32_t mapping_page_table[1024] __attribute__((aligned(PAGE_SIZE)));
#define ARBITRARY_MAPPING_REGION_START (1023*1024*PAGE_SIZE)

bool paging_init(void) {
    kernel_address_space = kernel_page_directory;

    const uint32_t pages_in_kernel = (((get_endkernel() - get_immutable_data_start()) / PAGE_SIZE) + ((get_endkernel() % PAGE_SIZE) > 0u));
    kprintf("get_endkernel() - get_immutable_data_start(): %X\n", (get_endkernel() - get_immutable_data_start()));
    kprintf("get_immutable_data_end()-get_immutable_data_start(): %X\n", (get_mutable_data_start()-get_immutable_data_start()));
    kprintf("get_mutable_data_end()-get_mutable_data_start(): %X\n", (get_endkernel()-get_mutable_data_start()));
    kassert((pages_in_kernel + (KERNEL_START / PAGE_SIZE)) <= 1024u, false);

    const uint32_t immutable_pages_in_kernel = ((get_mutable_data_start()-get_immutable_data_start()) / PAGE_SIZE) + (((get_mutable_data_start()-get_immutable_data_start()) % PAGE_SIZE) > 0u);
    const uint32_t mutable_pages_in_kernel = ((get_endkernel()-get_mutable_data_start()) / PAGE_SIZE) + (((get_mutable_data_end()-get_mutable_data_start()) % PAGE_SIZE) > 0u);
    kprintf("pages_in_kernel: %u, immutable_pages_in_kernel: %u, mutable_pages_in_kernel: %u", pages_in_kernel, immutable_pages_in_kernel, mutable_pages_in_kernel);
    kassert((immutable_pages_in_kernel + mutable_pages_in_kernel) == pages_in_kernel, false);

    kmemset(kernel_page_table, 0, sizeof(kernel_page_table));
    for(uint32_t i = 0u; i < immutable_pages_in_kernel; ++i) {
        kernel_page_table[i + (V2P(get_immutable_data_start()) / PAGE_SIZE)] = (V2P(get_immutable_data_start()) + (i * PAGE_SIZE)) | PT_PRESENT;
    }
    for(uint32_t i = 0u; i < mutable_pages_in_kernel; ++i) {
        kernel_page_table[i + (V2P(get_mutable_data_start()) / PAGE_SIZE)] = (V2P(get_mutable_data_start()) + (i * PAGE_SIZE)) | PT_PRESENT | PT_RW;
    }

    kmemset(kernel_page_directory, 0, sizeof(kernel_page_directory));
    kernel_page_directory[768] = V2P(&kernel_page_table) | PD_PRESENT | PD_RW;

    kmemset(mapping_page_table, 0, sizeof(mapping_page_table));
    // sets up a region of memory which can be used for arbitrary short term mappings,
    //  mostly useful for editing/writing to physical memory addresses
    kernel_page_directory[1023] = V2P(&mapping_page_table) | PD_PRESENT | PD_RW;

    load_and_turn_on_paging();

    kprintf("after load_and_turn_on_paging\n");

    return true;
}

void load_and_turn_on_paging(void) {
    kprintf("kernel_page_directory: %p\n", &kernel_page_directory[0]);

    load_page_directory(V2P(&kernel_page_directory[0]));

    serial_writestring("after load_page_directory\n");


    enable_ring0_write_protect();


    enable_paging();

    kprintf("after enable_paging\n");
}

uint32_t get_physical_address_in_boot(const uint32_t page_directory, const void *const virtual_address) {
    kprintf("get_physical_address_in_boot: page_directory: %p, virtual_address: %p\n", page_directory, virtual_address);
    //kassert_message(is_readable(page_directory, virtual_address), "Virtual Address Not Present in Mapping Structure. Cannot Fetch Physical Address.", 0);
    //kprintf("after is_readable\n");

    const uint32_t page_index = (uint32_t)virtual_address / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t *const page_directory_ptr = (uint32_t*)page_directory;
    const uint32_t page_table_phys = return_page_address(page_directory_ptr[table_index]);
    uint32_t *const page_table_virt = P2V(page_table_phys);

    const uint32_t phys_ret = return_page_address(page_table_virt[page_index_in_table]);
    return phys_ret;
}

uint32_t get_physical_address(const uint32_t page_directory, const void *const virtual_address) {
    kprintf("get_physical_address: page_directory: %p, virtual_address: %p\n", page_directory, virtual_address);
    kassert_message(is_readable(page_directory, virtual_address), "Virtual Address Not Present in Mapping Structure. Cannot Fetch Physical Address.", 0);
    kprintf("after is_readable\n");

    const uint32_t page_index = (uint32_t)virtual_address / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t *const page_directory_ptr = (uint32_t*)page_directory;
    const uint32_t page_table_phys = return_page_address(page_directory_ptr[table_index]);
    uint32_t *const page_table_virt = map_to_arbitrary_kernel_virt_page(page_table_phys, 514);

    const uint32_t phys_ret = return_page_address(page_table_virt[page_index_in_table]);
    unmap_arbitrary_kernel_virt_page(514);
    return phys_ret;
}
uint32_t get_physical_address_in_kernel_addr(const void *const virtual_address) {
    return get_physical_address((uint32_t)kernel_address_space, virtual_address);
}

bool is_readable(uint32_t page_directory, const void *const virtual_address) {
    kprintf("is_readable: page_directory: %p, virtual_address: %p\n", page_directory, virtual_address);
    kassert(((uint32_t)virtual_address % PAGE_SIZE == 0), false);
    kprintf("after kassert\n");

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t *const virt_page_directory = (uint32_t *)page_directory;
    if ((virt_page_directory[table_index] & PD_PRESENT) != PD_PRESENT) {
        return false;
    }
    kprintf("after virt_page_directory[table_index] & PD_PRESENT\n");

    const uint32_t page_table_phys_frame = return_page_address(virt_page_directory[table_index]);
    const uint32_t *const virt_page_table = map_to_arbitrary_kernel_virt_page(page_table_phys_frame, 513);
    kprintf("virt_page_table: %p\n", virt_page_table);
    if ((virt_page_table[page_index_in_table] & PT_PRESENT) != PT_PRESENT) {
        kprintf("virt_page_table[page_index_in_table] & PT_PRESENT: %p\n", virt_page_table[page_index_in_table] & PT_PRESENT);
        unmap_arbitrary_kernel_virt_page(513);
        return false;
    }
    kprintf("after virt_page_table[page_index_in_table] & PT_PRESENT\n");
    unmap_arbitrary_kernel_virt_page(513);

    return true;
}
bool is_readable_in_kernel_addr(const void *const virtual_address) {
    return is_readable((uint32_t)kernel_address_space, virtual_address);
}
bool is_writable(const uint32_t page_directory, const void *const virtual_address) {
    if (!is_readable(page_directory, virtual_address)) {
        return false;
    }

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t *const virt_page_directory = (uint32_t *)page_directory;
    if ((virt_page_directory[table_index] & PD_RW) != PD_RW) {
        return false;
    }

    const uint32_t page_table_phys_frame = return_page_address(virt_page_directory[table_index]);
    const uint32_t *const virt_page_table = map_to_arbitrary_kernel_virt_page(page_table_phys_frame, 512);
    if ((virt_page_table[page_index_in_table] & PT_RW) != PT_RW) {
        unmap_arbitrary_kernel_virt_page(512);
        return false;
    }
    unmap_arbitrary_kernel_virt_page(512);

    return true;
}
bool is_writable_in_kernel_addr(const void *const virtual_address) {
    return is_writable((uint32_t)kernel_address_space, virtual_address);
}

bool identity_map_page(const uint32_t page_directory, const uint32_t address, const uint32_t pt_flags, const uint32_t pd_flags) {
    const uint32_t page_index = address / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t page = address | pt_flags;

    // Check if this table is present and allocate it if not
    uint32_t *const pd_virt = (uint32_t *)page_directory;
    if ((pd_virt[table_index] & PD_PRESENT) != PD_PRESENT) {
        const uint32_t pt_phys = (uint32_t)global_phys_allocator_allocate_page();

        pd_virt[table_index] = pt_phys | pd_flags;
    }

    // Get a virtual pointer to the parent page table
    const uint32_t pt_phys_frame = return_page_address(pd_virt[table_index]);

    uint32_t *const pt_virt = map_to_arbitrary_kernel_virt_page(pt_phys_frame, 514);
    // Update page in page table
    pt_virt[page_index_in_table] = page;
    unmap_arbitrary_kernel_virt_page(514);

    return true;
}
bool identity_map_pages(const uint32_t page_directory, const uint32_t address, const uint32_t num_of_pages, const uint32_t const pt_flags, const uint32_t pd_flags) {
    kassert(address % PAGE_SIZE == 0, false);

    for (uint32_t i = 0u; i < num_of_pages; ++i) {
        if (!identity_map_page(page_directory, address + i * PAGE_SIZE, pt_flags, pd_flags)) {
            return false;
        }
    }

    return true;
}
bool identity_map_page_in_kernel_addr(const uint32_t address, const uint32_t pt_flags, const uint32_t pd_flags) {
    return identity_map_pages_in_kernel_addr(address, 1, pt_flags, pd_flags);
}
bool identity_map_pages_in_kernel_addr(const uint32_t address, const uint32_t num_of_pages, const uint32_t pt_flags, const uint32_t pd_flags) {
    return identity_map_pages((uint32_t)kernel_address_space, address, num_of_pages, pt_flags, pd_flags);
}

void* map_to_arbitrary_kernel_virt_page(const uint32_t phys_addr, const uint32_t page_index_in_table) {
    kassert(page_index_in_table <= 1023, NULL);

    mapping_page_table[page_index_in_table] = phys_addr | PT_PRESENT | PT_RW;

    kprintf("phys_addr: %X, (ARBITRARY_MAPPING_REGION_START+(page_index_in_table*PAGE_SIZE)): %X\n", phys_addr, (ARBITRARY_MAPPING_REGION_START+(page_index_in_table*PAGE_SIZE)));


    // TODO: for some reason, `flush_tlb_single_page` does NOT update the mappings
    //  and any code accessing the returned page address will page fault.
    //  I'm not sure why this is happening, but I'm going to leave it for now.
    //flush_tlb_single_page(ARBITRARY_MAPPING_REGION_START+(page_index_in_table*PAGE_SIZE));
    load_page_directory(V2P(&kernel_page_directory[0]));



    return (void*)(ARBITRARY_MAPPING_REGION_START+(page_index_in_table*PAGE_SIZE));
}

// `size` is number of pages
void* map_to_arbitrary_kernel_virt_pages(const uint32_t phys_addr, const uint32_t page_index_in_table, const uint32_t size) {
    for(uint32_t i = 0; i < size; ++i) {
        if(map_to_arbitrary_kernel_virt_page(phys_addr + (i * PAGE_SIZE),  i+page_index_in_table) == NULL) {
            return NULL;
        }
    }
    return (void*)(ARBITRARY_MAPPING_REGION_START+(page_index_in_table*PAGE_SIZE));
}

bool unmap_arbitrary_kernel_virt_page(const uint32_t page_index_in_table) {
    kassert(page_index_in_table <= 1023, false);

    mapping_page_table[page_index_in_table] = 0;

    //flush_tlb_single_page(ARBITRARY_MAPPING_REGION_START+(page_index_in_table*PAGE_SIZE));
    load_page_directory(V2P(&kernel_page_directory[0]));

    return true;
}

// `size` is number of pages
bool unmap_arbitrary_kernel_virt_pages(const uint32_t page_index_in_table, const uint32_t size) {
    for(uint32_t i = 0; i < size; ++i) {
        if(!unmap_arbitrary_kernel_virt_page(i+page_index_in_table)) {
            return false;
        }
    }
    return true;
}

bool map_page(const uint32_t page_directory, void *const virtual_address, const uint32_t phys_frame, const uint32_t pt_flags, const uint32_t pd_flags) {
    kprintf("map_page\n");
    kassert(((uint32_t)virtual_address % PAGE_SIZE == 0) && (phys_frame % PAGE_SIZE == 0), false);
    kprintf("passed kassert\n");

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    uint32_t *const virt_page_directory = (uint32_t*)page_directory;
    kprintf("virt_page_directory: %p\n", virt_page_directory);
    if ((virt_page_directory[table_index] & PD_PRESENT) != PD_PRESENT) {
        const uint32_t phys_page_table = (uint32_t)global_phys_allocator_allocate_page();
        kprintf("phys_page_table: %X\n", phys_page_table);

        clear_physical_page(phys_page_table);
        kprintf("cleared physical page\n");

        virt_page_directory[table_index] = phys_page_table | pd_flags;
        kprintf("virt_page_directory[table_index]: %X\n", virt_page_directory[table_index]);
    }

    const uint32_t page_table_phys_addr = return_page_address(virt_page_directory[table_index]);
    kprintf("page_table_phys_addr: %X\n", page_table_phys_addr);
    uint32_t *const virt_page_table = map_to_arbitrary_kernel_virt_page(page_table_phys_addr, 512);
    kprintf("virt_page_table: %p\n", virt_page_table);
    if ((virt_page_table[page_index_in_table] & PT_PRESENT) == PT_PRESENT) {
        kprintf("Woah, mapping an already mapped page. You should fix this.\n");
    }
    // TODO: properly handle the case of someone mapping an already mapped page, maybe return `false`?
    kprintf("after if statement\n");
    virt_page_table[page_index_in_table] = phys_frame | pt_flags;
    kprintf("virt_page_table[page_index_in_table]: %X\n", virt_page_table[page_index_in_table]);
    unmap_arbitrary_kernel_virt_page(512);

    //flush_tlb_single_page((uint32_t)virtual_address);
    load_page_directory(V2P(&kernel_page_directory[0]));

    return true;
}
bool map_pages(const uint32_t page_directory, void *const virtual_address, const uint32_t phys_frame, const size_t num_of_pages, const uint32_t pt_flags, const uint32_t pd_flags) {
    kprintf("map_pages\n");
    kassert(((uint32_t)virtual_address % PAGE_SIZE == 0) && (phys_frame % PAGE_SIZE == 0), false);
    kprintf("passed kassert\n");

    const uint32_t virtual_address_value = (uint32_t)virtual_address;

    for (uint32_t i = 0u; i < num_of_pages; ++i) {
        if (!map_page(page_directory, (void*)(virtual_address_value + i * PAGE_SIZE), phys_frame + i * PAGE_SIZE, pt_flags, pd_flags)) {
            return false;
        }
    }

    kprintf("finished map_pages\n");
    return true;
}
bool map_page_in_kernel_addr(void *const virtual_address, const uint32_t phys_frame, const uint32_t pt_flags, const uint32_t pd_flags) {
    kprintf("map_page_in_kernel_addr with virtual_address: %p, phys_frame: %X\n", virtual_address, phys_frame);
    return map_pages_in_kernel_addr(virtual_address, phys_frame, 1, pt_flags, pd_flags);
}
bool map_pages_in_kernel_addr(void *const virtual_address, const uint32_t phys_frame, const size_t num_of_pages, const uint32_t pt_flags, const uint32_t pd_flags) {
    kprintf("map_pages_in_kernel_addr with virtual_address: %p, phys_frame: %X, num_of_pages: %u\n", virtual_address, phys_frame, num_of_pages);
    return map_pages((uint32_t)kernel_address_space, virtual_address, phys_frame, num_of_pages, pt_flags, pd_flags);
}

uint32_t unmap_page(const uint32_t page_directory, const void *const virtual_address) {
    kassert((uint32_t)virtual_address % PAGE_SIZE == 0, 0);

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    uint32_t *const virt_page_directory = (uint32_t*)page_directory;

    const uint32_t page_table_phys_addr = return_page_address(virt_page_directory[table_index]);
    uint32_t *const virt_page_table = map_to_arbitrary_kernel_virt_page(page_table_phys_addr, 512);

    const uint32_t phys_frame = return_page_address(virt_page_table[page_index_in_table]);

    // TODO: consider iterating through page table to decide whether it can be freed and unmapped
    virt_page_table[page_index_in_table] = 0x0;
    unmap_arbitrary_kernel_virt_page(512);

    //flush_tlb_single_page((uint32_t)virtual_address);
    load_page_directory(V2P(&kernel_page_directory[0]));

    return phys_frame;
}
uint32_t unmap_pages(const uint32_t page_directory, const void *const virtual_address, const size_t num_of_pages) {
    if(num_of_pages == 0u) {
        return 0u;
    }

    const uint32_t virtual_address_page_start = return_page_address((uint32_t)virtual_address);
    const uint32_t virtual_address_page_offset = return_page_offset((uint32_t)virtual_address);

    const uint32_t ret = unmap_page(page_directory, (void*)virtual_address_page_start);
    for (size_t i = 1; i < num_of_pages; i++) {
        unmap_page(page_directory, (void *)(virtual_address_page_start + (i * PAGE_SIZE)));
    }
    return ret+virtual_address_page_offset;
}
uint32_t unmap_page_in_kernel_addr(const void *const virtual_address) {
    return unmap_pages_in_kernel_addr(virtual_address, 1);
}
uint32_t unmap_pages_in_kernel_addr(const void *const virtual_address, const size_t num_of_pages) {
    return unmap_pages((uint32_t)kernel_address_space, virtual_address, num_of_pages);
}

uint32_t return_page_address(uint32_t address);
uint32_t return_page_offset(uint32_t address);
uint32_t get_rounded_up_num_of_pages(uint32_t address);

void map_kernel_inside_user(struct process_t *const process) {
    uint32_t *const cr3_ptr = map_to_arbitrary_kernel_virt_page(process->physical_cr3, 511);

    // As we are ensuring that the first page table entries are reserved to the
    // kernel, it is enough to link these ones to the kernel ones

    uint32_t *const virt_page_directory = cr3_ptr;
    for (size_t i = 0; i < 1024; ++i) {
        virt_page_directory[i] = return_page_address(kernel_address_space[i]) | PD_PRESENT;
    }

    unmap_arbitrary_kernel_virt_page(511);
}
void clear_physical_page(const size_t physical) {
    uint32_t *const ptr = map_to_arbitrary_kernel_virt_page(physical, 510);

    kmemset((void*)ptr, 0, PAGE_SIZE);

    unmap_arbitrary_kernel_virt_page(510);
}
bool user_map(struct process_t *const process, const size_t virt, const size_t physical) {
    uint32_t *const cr3_ptr = map_to_arbitrary_kernel_virt_page(process->physical_cr3, 509);
    if (cr3_ptr == NULL) {
        return false;
    }

    const uint32_t page_index = ((uint32_t)cr3_ptr) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    uint32_t *const virt_page_directory = cr3_ptr;
    if ((virt_page_directory[table_index] & PD_PRESENT) != PD_PRESENT) {
        const uint32_t physical_pd = (uint32_t)global_phys_allocator_allocate_page();

        virt_page_directory[table_index] = physical_pd | PD_RW | PD_USER | PD_PRESENT;

        clear_physical_page(physical_pd);

        process->paging_size += PAGE_SIZE;
        const struct segment_t segment = {physical_pd, 1u};
        push_back(process->segments, &segment);
    }

    const uint32_t page_table_phys_addr = return_page_address(virt_page_directory[table_index]);
    uint32_t *const virt_page_table = map_to_arbitrary_kernel_virt_page(page_table_phys_addr, 508);

    virt_page_table[page_index_in_table] = physical | PT_RW | PT_USER | PT_PRESENT;

    unmap_arbitrary_kernel_virt_page(508);

    unmap_arbitrary_kernel_virt_page(509);

    return true;
}
bool user_map_pages(struct process_t *const process, const size_t virt, const size_t physical, const size_t pages) {
    // Map each page
    for (size_t page = 0; page < pages; ++page) {
        const size_t virt_addr = virt + page * PAGE_SIZE;
        const size_t phys_addr = physical + page * PAGE_SIZE;

        if (!user_map(process, virt_addr, phys_addr)) {
            return false;
        }
    }

    return true;
}

uint32_t* get_kernel_page_directory(void) {
    return kernel_address_space;
}
