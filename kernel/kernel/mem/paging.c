#include <mem/paging.h>


extern void load_page_directory(uint32_t* page_directory);
extern void enable_paging(void);

extern void enable_ring0_write_protect(void);


/*
Relevant Intel manual section on 32 bit paging:
https://web.archive.org/web/20151025081259if_/http://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-system-programming-manual-325384.pdf#G9.81100
*/


static uint32_t* default_page_directory;

GENERATE_BITMAP(virtual_memory, NUMBER_OF_PAGES, PAGE_SIZE)
static struct BITMAP_TYPENAME(virtual_memory) virtual_pages;
static uint32_t bitmap_page_permissions[NUMBER_OF_PAGES/32u]; //store 0 for kernel, 1 for user


void identity_map_page(const uint32_t page_directory, const uint32_t address, const uint32_t pt_flags, const uint32_t pd_flags) {
    const uint32_t page_index = address / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t page = address | pt_flags;


    // Check if this table is present and allocate it if not
    uint32_t *const pd_virt = (uint32_t*)page_directory;
    if ((pd_virt[table_index] & PD_PRESENT) != PD_PRESENT) {
        const uint32_t pt_phys = (uint32_t)global_allocate_page(CRITICAL_KERNEL_USE);

        pd_virt[table_index] = pt_phys | pd_flags;
    }

    // Get a virtual pointer to the parent page table
    uint32_t *const pt_virt = (uint32_t*) (pd_virt[table_index] & 0xFFFFF000u);

    // Update page in page table
    pt_virt[page_index_in_table] = page;
}

void paging_init(void) {
    kmemset(virtual_pages.bitset, 0u, sizeof(virtual_pages.bitset));
    virtual_pages.has_filled_bitset_cache = false;
    for(int32_t i = 0; i < 20; ++i) {
        virtual_pages.bitset_cache[i] = -1;
    }

    kmemset(bitmap_page_permissions, 1u, sizeof(bitmap_page_permissions));


    default_page_directory = global_allocate_page(CRITICAL_KERNEL_USE); //page frame allocator returns page aligned blocks of 4KiB of memory

    for(int32_t i = 0; i < 1024; ++i) {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        default_page_directory[i] = 0x00000002u;
    }


    uint32_t *const first_page_table = global_allocate_page(CRITICAL_KERNEL_USE);
    for(uint32_t i = 0u; i < 1024u; ++i) {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes
        first_page_table[i] = (i * 4096u) | (PT_PRESENT | PT_RW | PT_USER); // attributes: user level, read/write, present.
    }


    // attributes: user level, read/write, present
    default_page_directory[0] = ((uint32_t)first_page_table) | (PD_PRESENT | PD_RW | PD_USER);
}

void load_and_turn_on_paging(void) {
    load_page_directory(default_page_directory);

    //TODO: add flag to decide if this is done or not
    enable_ring0_write_protect();

    enable_paging();
}

void reserve_virtual_address(const uint32_t virtual_address, const size_t num_of_pages, const enum memory_type type) {
    const uint32_t start_page_index = virtual_address / PAGE_SIZE;

    //TODO: replace with call to kmemset at some point
    for(uint32_t i = 0u; i < num_of_pages; ++i) {
        bitset_set_at(start_page_index+i, virtual_pages.bitset, true);
    }

    //TODO: replace with call to kmemset at some point
    const bool memory_permission = (type == USER_USE);
    for(uint32_t i = 0u; i < num_of_pages; ++i) {
        bitset_set_at(start_page_index+i, bitmap_page_permissions, memory_permission);
    }
}

uint32_t get_physical_address(const void *const virtual_address) {
    const uint32_t page_index = (uint32_t)virtual_address / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t *const page_directory = default_page_directory;
    const uint32_t *const page_table = (uint32_t*) (page_directory[table_index] & 0xFFFFF000u);

    return page_table[page_index_in_table] & 0xFFFFF000u;
}

//TODO: maybe split into two `static` functions for the implementation (for the page table in the page directory, and page in the page table)
bool is_readable(const void* virtual_address) {
    kassert(((uint32_t)virtual_address % PAGE_SIZE == 0), false);

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t *const virt_page_directory = default_page_directory;
    if((virt_page_directory[table_index] & PD_PRESENT) != PD_PRESENT) {
        return false;
    }

    const uint32_t *const virt_page_table = (uint32_t*)((virt_page_directory[table_index]) & 0xFFFFF000u);
    if((virt_page_table[page_index_in_table] & PT_PRESENT) != PT_PRESENT) {
        return false;
    }

    return true;
}

bool is_writable(const void* virtual_address) {
    if(!is_readable(virtual_address)) {
        return false;
    }

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t *const virt_page_directory = default_page_directory;
    if((virt_page_directory[table_index] & PD_RW) != PD_RW) {
        return false;
    }

    const uint32_t *const virt_page_table = (uint32_t*)((virt_page_directory[table_index]) & 0xFFFFF000u);
    if((virt_page_table[page_index_in_table] & PT_RW) != PT_RW) {
        return false;
    }

    return true;
}

bool map_page_with_page_dir(const uint32_t page_directory, void *const virtual_address, const uint32_t phys_frame, const uint32_t pt_flags, const uint32_t pd_flags) {
    kassert(((uint32_t)virtual_address % PAGE_SIZE == 0) && (phys_frame % PAGE_SIZE == 0), false);

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    uint32_t *const virt_page_directory = (uint32_t*)page_directory;
    if((virt_page_directory[table_index] & PD_PRESENT) != PD_PRESENT) {
        const uint32_t phys_page_table = (uint32_t)global_allocate_page(CRITICAL_KERNEL_USE); //will be identity mapped

        virt_page_directory[table_index] = phys_page_table | pd_flags;
    }

    uint32_t *const virt_page_table = (uint32_t*)((virt_page_directory[table_index]) & 0xFFFFF000u);
    if((virt_page_table[page_index_in_table] & PT_PRESENT) == PT_PRESENT) {
        kprintf("Woah, mapping an already mapped page. You should fix this.\n");
    }
    //TODO: properly handle the case of someone mapping an already mapped page, maybe return `false`?

    virt_page_table[page_index_in_table] = phys_frame | pt_flags;

    flush_tlb_single_page((uint32_t)virtual_address);

    return true;
}

bool map_page(void *const virtual_address, const uint32_t phys_frame, const uint32_t pt_flags, const uint32_t pd_flags) {
    return map_page_with_page_dir((uint32_t)default_page_directory, virtual_address, phys_frame, pt_flags, pd_flags);
}

bool map_pages(void *const virtual_address, const uint32_t phys_frame, const size_t num_of_pages, const uint32_t pt_flags, const uint32_t pd_flags) {
    kassert(((uint32_t)virtual_address % PAGE_SIZE == 0) && (phys_frame % PAGE_SIZE == 0), false);

    const uint32_t virtual_address_value = (uint32_t)virtual_address;

    for(uint32_t i = 0u; i < num_of_pages; ++i) {
        if(!map_page((void*)(virtual_address_value + i*PAGE_SIZE), phys_frame + i*PAGE_SIZE, pt_flags, pd_flags)) {
            return false;
        }
    }

    return true;
}

uint32_t allocate_virtual_page(void *const virtual_address, const uint32_t pt_flags, const uint32_t pd_flags) {
    //kprintf("before global_allocate_page\n");
    const uint32_t phys_frame = (uint32_t)global_allocate_page(USER_USE);
    //kprintf("after global_allocate_page\n");

    //NULL can only be returned on failure for user allocations
    if(phys_frame) {
        map_page(virtual_address, phys_frame, pt_flags, pd_flags);
        kmemset(virtual_address, 0u, PAGE_SIZE);
    }

    return phys_frame;
}

uint32_t unmap_page(const void *const virtual_address) {
    kassert((uint32_t)virtual_address % PAGE_SIZE == 0, 0);

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    uint32_t *const virt_page_directory = default_page_directory;

    uint32_t *const virt_page_table = (uint32_t*)((virt_page_directory[table_index]) & 0xFFFFF000u);

    const uint32_t phys_frame = virt_page_table[page_index_in_table] & 0xFFFFF000u; //TODO: check if the bitmask is correct

    //TODO: consider iterating through page table to decide whether it can be freed and unmapped
    virt_page_table[page_index_in_table] = 0x0;

    flush_tlb_single_page((uint32_t)virtual_address);

    return phys_frame;
}

uint32_t unmap_pages(const void *const virtual_address, const size_t num_of_pages) {
    const uint32_t ret = unmap_page(virtual_address);
    for(size_t i = 1; i < num_of_pages; i++) {
        unmap_page((void*)((uint32_t)virtual_address + (i * PAGE_SIZE)));
    }
    return ret;
}

void free_virtual_page(const void *const virtual_address) {
    const uint32_t phys_frame = unmap_page(virtual_address);

    kassert_void(phys_frame != 0 && phys_frame % PAGE_SIZE == 0);

    global_free_page(USER_USE, (void*)phys_frame);
}


void map_kernel_inside_user(struct process_t *const process){
    struct physical_pointer cr3_ptr = create_physical_pointer(process->physical_cr3, 1);

    //As we are ensuring that the first PML4T entries are reserved to the
    //kernel, it is enough to link these ones to the kernel ones

    uint32_t *const virt_page_directory = cr3_ptr.virt;
    for(size_t i = 0; i < 1024; ++i) {
        virt_page_directory[i] = (default_page_directory[i] & 0xFFFFF000u) | PD_USER | PD_PRESENT;
    }
}

void clear_physical_page(const size_t physical) {
    struct physical_pointer ptr = create_physical_pointer(physical, 1);

    kmemset((void*)ptr.virt, 0, PAGE_SIZE);
}

bool user_map(struct process_t *const process, const size_t virt, const size_t physical) {
    struct physical_pointer cr3_ptr = create_physical_pointer(process->physical_cr3, 1);
    if(cr3_ptr.virt == 0) {
        return false;
    }

    const uint32_t page_index = ((uint32_t)cr3_ptr.virt) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    uint32_t *const virt_page_directory = (uint32_t*)cr3_ptr.virt;
    if((virt_page_directory[table_index] & PD_PRESENT) != PD_PRESENT) {
        const uint32_t physical_pd = (uint32_t)global_binary_buddy_memory_allocator_allocate_page(USER_USE);

        virt_page_directory[table_index] = physical_pd | PD_RW | PD_USER | PD_PRESENT;

        clear_physical_page(physical_pd);

        process->paging_size += PAGE_SIZE;
        const struct segment_t segment = {physical_pd, 1u};
        push_back(process->segments, &segment);
    }

    uint32_t *const virt_page_table = (uint32_t*)((virt_page_directory[table_index]) & 0xFFFFF000u);

    virt_page_table[page_index_in_table] = physical | PT_RW | PT_USER | PT_PRESENT;

    return true;
}

bool user_map_pages(struct process_t *const process, const size_t virt, const size_t physical, const size_t pages) {
    //Map each page
    for(size_t page = 0; page < pages; ++page) {
        const size_t virt_addr = virt + page * PAGE_SIZE;
        const size_t phys_addr = physical + page * PAGE_SIZE;

        if(!user_map(process, virt_addr, phys_addr)) {
            return false;
        }
    }

    return true;
}



uint32_t* get_default_page_directory(void) {
    return default_page_directory;
}
