#include "paging.h"


extern void load_page_directory(uint32_t* page_directory);
extern void enable_paging(void);

extern void enable_ring0_write_protect(void);


/*
Relevant Intel manual section on 32 bit paging:
https://web.archive.org/web/20151025081259if_/http://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-system-programming-manual-325384.pdf#G9.81100
*/


extern uint32_t boot_page_directory;
extern uint32_t boot_page_table;

static uint32_t* kernel_address_space;

#if 0
static uint32_t kernel_page_directory[1024] __attribute__((aligned(PAGE_SIZE)));
static uint32_t kernel_page_table[1024] __attribute__((aligned(PAGE_SIZE)));
static uint32_t mapping_page_table[1024] __attribute__((aligned(PAGE_SIZE)));
// TODO: sketch out memory address scheme
#define MAPPING_PAGE_PAGE_DIRECTORY_INDEX 1023u // uses top page table of page directory
#define MAPPING_PAGE_VIRTUAL_ADDRESS (NUMBER_OF_PAGES-1024u-1u)*PAGE_SIZE
#endif

static volatile uint32_t j;
void sus_test(void) {

}


static volatile uint32_t a;


bool paging_init(void) {
#if 0
    disable_interrupts();

    kernel_address_space = kernel_page_directory;

    const uint32_t num_of_immutable_pages_in_kernel = ((V2P(get_rodata_end())/PAGE_SIZE) + ((V2P(get_rodata_end())%PAGE_SIZE)>0u));
    const uint32_t num_of_mutable_pages_in_kernel = (((get_mutable_data_end()-get_mutable_data_start())/PAGE_SIZE) + ((V2P(get_mutable_data_end())%PAGE_SIZE)>0u));
    const uint32_t num_of_pages_in_kernel = num_of_immutable_pages_in_kernel + num_of_mutable_pages_in_kernel;

    const uint32_t endkernel_pages = V2P(get_endkernel())/PAGE_SIZE;
    const uint32_t get_mutable_data_end_pages = V2P(get_mutable_data_end())/PAGE_SIZE;
    const uint32_t mutable_data_start_pages = V2P(get_mutable_data_start())/PAGE_SIZE;

    kprintf("num_of_immutable_pages_in_kernel: %u\n", num_of_immutable_pages_in_kernel);
    kprintf("num_of_mutable_pages_in_kernel: %u\n", num_of_mutable_pages_in_kernel);
    kprintf("num_of_pages_in_kernel: %u\n", num_of_pages_in_kernel);
    kprintf("endkernel_pages: %u\n", endkernel_pages);
    kprintf("get_mutable_data_end_pages: %u\n", get_mutable_data_end_pages);
    kprintf("mutable_data_start_pages: %u\n", mutable_data_start_pages);
    kassert_message(num_of_pages_in_kernel <= 1024, "kernel is too big to be mapped in one page table\n", false);

    kmemset(kernel_page_directory, sizeof(kernel_page_directory), 0);
    kmemset(kernel_page_table, sizeof(kernel_page_table), 0);
    kmemset(mapping_page_table, sizeof(mapping_page_table), 0);

    for(uint32_t i = 1u; i < num_of_immutable_pages_in_kernel; ++i) {
        kernel_page_table[i] = (i*PAGE_SIZE) | PT_PRESENT;
    }
    // TODO: can be simplified a bit
    const uint32_t mutable_pages_start = (V2P(get_mutable_data_start())/PAGE_SIZE);
    kprintf("num_of_immutable_pages_in_kernel: %u, mutable_pages_start: %u\n", num_of_immutable_pages_in_kernel, mutable_pages_start);
    //for(uint32_t i = mutable_pages_start; i < (mutable_pages_start+num_of_mutable_pages_in_kernel); ++i) {
    //for(uint32_t i = 0u; i < 1024; ++i) {
        //kernel_page_table[i] = (i*PAGE_SIZE) | PT_PRESENT | PT_RW;
    //}

    // 1024/4 = 256
    // 256 * 3 = 768, puts it in top 1/4 of memory range, start of 'higher half' segment
    //kernel_page_directory[768] = V2P(kernel_page_table) | PD_PRESENT | PD_RW;

    for(uint32_t i = 0u; i < 1024u; ++i) {
        kernel_page_directory[i] = (&boot_page_directory)[i];
        //kernel_page_directory[i] = V2P(kernel_page_table) | PD_PRESENT | PD_RW;
    }
#endif

    kprintf("get_physical_address(&j): %X\n", get_physical_address(&boot_page_directory, &j));

    a = 700;
    kprintf("a: %u\n", a);

    kprintf("&j: %p\n", &j);
    kprintf("&a: %p\n", &a);

    for(uint32_t i = 0u; i < 1024u; ++i) {
        kprintf("boot_page_directory[%u]: %p\n", i, (&boot_page_directory)[i]);
    }

    for(uint32_t i = 0u; i < 1024u; ++i) {
        kprintf("boot_page_table[%u]: %p\n", i, (&boot_page_table)[i]);
    }

    // we now have scratch virtual address pages that can be arbitrarily mapped for accessing/writing to physical memory of page tables
    //kernel_page_directory[MAPPING_PAGE_PAGE_DIRECTORY_INDEX] = V2P(mapping_page_table) | PD_PRESENT | PD_RW;
    //const bool ret = kernel_virt_allocator_reserve_pages(MAPPING_PAGE_VIRTUAL_ADDRESS, 1024u); // reserve the above pages so they aren't allocated
    //kassert_message(ret, "Failed to reserve scratch virtual address pages for mapping page tables\n", false);

#if 0
    serial_writestring("before load_and_turn_on_paging\n");

    load_and_turn_on_paging();
    serial_writestring("after load_and_turn_on_paging\n");

    enable_interrupts();
#endif

    return true;
}

void load_and_turn_on_paging(void) {
    //kprintf("kernel_page_directory: %p\n", &kernel_page_directory[0]);
    kprintf("boot_page_directory: %p\n", &boot_page_directory);
    //for(uint32_t i = 0u; i < 1024u; ++i) {
        //kprintf("kernel_page_directory[%u]: %p\n", i, kernel_page_directory[i]);
    //}
    for(uint32_t i = 0u; i < 1024u; ++i) {
        kprintf("boot_page_directory[%u]: %p\n", i, (&boot_page_directory)[i]);
    }
    //load_page_directory(V2P(&kernel_page_directory[0]));
    load_page_directory(V2P(&boot_page_directory));

    serial_writestring("after load_page_directory\n");

#if 0
    //TODO: add flag to decide if this is done or not
    enable_ring0_write_protect();

    enable_paging();
#endif
}

uint32_t get_physical_address(uint32_t page_directory, const void* virtual_address) {
    kassert_message(is_readable(page_directory, virtual_address), "Virtual Address Not Present in Mapping Structure. Cannot Fetch Physical Address.", 0);

    const uint32_t page_index = (uint32_t)virtual_address / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t *const page_directory_ptr = (uint32_t*)page_directory;
    const uint32_t *const page_table = (uint32_t*) (page_directory_ptr[table_index] & 0xFFFFF000u);

    return page_table[page_index_in_table] & 0xFFFFF000u;
}
uint32_t get_physical_address_in_kernel_addr(const void* virtual_address) {
    return get_physical_address((uint32_t)kernel_address_space, virtual_address);
}

bool is_readable(uint32_t page_directory, const void* virtual_address) {
    kassert(((uint32_t)virtual_address % PAGE_SIZE == 0), false);

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t *const virt_page_directory = (uint32_t*)page_directory;
    if((virt_page_directory[table_index] & PD_PRESENT) != PD_PRESENT) {
        return false;
    }

    const uint32_t *const virt_page_table = (uint32_t*)((virt_page_directory[table_index]) & 0xFFFFF000u);
    if((virt_page_table[page_index_in_table] & PT_PRESENT) != PT_PRESENT) {
        return false;
    }

    return true;
}
bool is_readable_in_kernel_addr(const void* virtual_address) {
    return is_readable((uint32_t)kernel_address_space, virtual_address);
}
bool is_writable(uint32_t page_directory, const void* virtual_address) {
    if(!is_readable(page_directory, virtual_address)) {
        return false;
    }

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t *const virt_page_directory = (uint32_t*)page_directory;
    if((virt_page_directory[table_index] & PD_RW) != PD_RW) {
        return false;
    }

    const uint32_t *const virt_page_table = (uint32_t*)((virt_page_directory[table_index]) & 0xFFFFF000u);
    if((virt_page_table[page_index_in_table] & PT_RW) != PT_RW) {
        return false;
    }

    return true;
}
bool is_writable_in_kernel_addr(const void* virtual_address) {
    return is_writable((uint32_t)kernel_address_space, virtual_address);
}


bool identity_map_page(uint32_t page_directory, uint32_t address, uint32_t pt_flags, uint32_t pd_flags) {
    const uint32_t page_index = address / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t page = address | pt_flags;


    // Check if this table is present and allocate it if not
    uint32_t *const pd_virt = (uint32_t*)page_directory;
    if ((pd_virt[table_index] & PD_PRESENT) != PD_PRESENT) {
        const uint32_t pt_phys = (uint32_t)global_phys_allocator_allocate_page();

        pd_virt[table_index] = pt_phys | pd_flags;
    }

    // Get a virtual pointer to the parent page table
    uint32_t *const pt_virt = (uint32_t*) (pd_virt[table_index] & 0xFFFFF000u);

    // Update page in page table
    pt_virt[page_index_in_table] = page;

    return true;
}
bool identity_map_pages(uint32_t page_directory, uint32_t address, uint32_t num_of_pages, uint32_t pt_flags, uint32_t pd_flags) {
    kassert(address % PAGE_SIZE == 0, false);

    for(uint32_t i = 0u; i < num_of_pages; ++i) {
        if(!identity_map_page(page_directory, address + i*PAGE_SIZE, pt_flags, pd_flags)) {
            return false;
        }
    }

    return true;
}
bool identity_map_page_in_kernel_addr(uint32_t address, uint32_t pt_flags, uint32_t pd_flags) {
    return identity_map_pages_in_kernel_addr(address, 1, pt_flags, pd_flags);
}
bool identity_map_pages_in_kernel_addr(uint32_t address, uint32_t num_of_pages, uint32_t pt_flags, uint32_t pd_flags) {
    return identity_map_pages((uint32_t)kernel_address_space, address, num_of_pages, pt_flags, pd_flags);
}

bool map_page(uint32_t page_directory, void* virtual_address, uint32_t phys_frame, uint32_t pt_flags, uint32_t pd_flags) {
    kassert(((uint32_t)virtual_address % PAGE_SIZE == 0) && (phys_frame % PAGE_SIZE == 0), false);

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    uint32_t *const virt_page_directory = (uint32_t*)page_directory;
    if((virt_page_directory[table_index] & PD_PRESENT) != PD_PRESENT) {
        const uint32_t phys_page_table = (uint32_t)global_phys_allocator_allocate_page(); //will be identity mapped

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
bool map_pages(uint32_t page_directory, void* virtual_address, uint32_t phys_frame, size_t num_of_pages, uint32_t pt_flags, uint32_t pd_flags) {
    kassert(((uint32_t)virtual_address % PAGE_SIZE == 0) && (phys_frame % PAGE_SIZE == 0), false);

    const uint32_t virtual_address_value = (uint32_t)virtual_address;

    for(uint32_t i = 0u; i < num_of_pages; ++i) {
        if(!map_page(page_directory, (void*)(virtual_address_value + i*PAGE_SIZE), phys_frame + i*PAGE_SIZE, pt_flags, pd_flags)) {
            return false;
        }
    }

    return true;
}
bool map_page_in_kernel_addr(void* virtual_address, uint32_t phys_frame, uint32_t pt_flags, uint32_t pd_flags) {
    return map_pages_in_kernel_addr(virtual_address, phys_frame, 1, pt_flags, pd_flags);
}
bool map_pages_in_kernel_addr(void* virtual_address, uint32_t phys_frame, size_t num_of_pages, uint32_t pt_flags, uint32_t pd_flags) {
    return map_pages((uint32_t)kernel_address_space, virtual_address, phys_frame, num_of_pages, pt_flags, pd_flags);
}

uint32_t unmap_page(uint32_t page_directory, const void* virtual_address) {
    kassert((uint32_t)virtual_address % PAGE_SIZE == 0, 0);

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    uint32_t *const virt_page_directory = (uint32_t*)page_directory;

    uint32_t *const virt_page_table = (uint32_t*)((virt_page_directory[table_index]) & 0xFFFFF000u);

    const uint32_t phys_frame = virt_page_table[page_index_in_table] & 0xFFFFF000u; //TODO: check if the bitmask is correct

    //TODO: consider iterating through page table to decide whether it can be freed and unmapped
    virt_page_table[page_index_in_table] = 0x0;

    flush_tlb_single_page((uint32_t)virtual_address);

    return phys_frame;
}
uint32_t unmap_pages(uint32_t page_directory, const void* virtual_address, size_t num_of_pages) {
    const uint32_t ret = unmap_page(page_directory, virtual_address);
    for(size_t i = 1; i < num_of_pages; i++) {
        unmap_page(page_directory, (void*)((uint32_t)virtual_address + (i * PAGE_SIZE)));
    }
    return ret;
}
uint32_t unmap_page_in_kernel_addr(const void* virtual_address) {
    return unmap_pages_in_kernel_addr(virtual_address, 1);
}
uint32_t unmap_pages_in_kernel_addr(const void* virtual_address, size_t num_of_pages) {
    return unmap_pages((uint32_t)kernel_address_space, virtual_address, num_of_pages);
}


void map_kernel_inside_user(struct process_t* process) {
    struct physical_pointer cr3_ptr = create_physical_pointer(process->physical_cr3, 1);

    //As we are ensuring that the first page table entries are reserved to the
    //kernel, it is enough to link these ones to the kernel ones

    uint32_t *const virt_page_directory = cr3_ptr.virt;
    for(size_t i = 0; i < 1024; ++i) {
        virt_page_directory[i] = (kernel_address_space[i] & 0xFFFFF000u) | PD_PRESENT;
    }

    free_physical_pointer(&cr3_ptr);
}
void clear_physical_page(size_t physical) {
    struct physical_pointer ptr = create_physical_pointer(physical, 1);

    kmemset((void*)ptr.virt, 0, PAGE_SIZE);

    free_physical_pointer(&ptr);
}
bool user_map(struct process_t* process, size_t virt, size_t physical) {
    struct physical_pointer cr3_ptr = create_physical_pointer(process->physical_cr3, 1);
    if(cr3_ptr.virt == 0) {
        return false;
    }

    const uint32_t page_index = ((uint32_t)cr3_ptr.virt) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    uint32_t *const virt_page_directory = (uint32_t*)cr3_ptr.virt;
    if((virt_page_directory[table_index] & PD_PRESENT) != PD_PRESENT) {
        const uint32_t physical_pd = (uint32_t)global_phys_allocator_allocate_page();

        virt_page_directory[table_index] = physical_pd | PD_RW | PD_USER | PD_PRESENT;

        clear_physical_page(physical_pd);

        process->paging_size += PAGE_SIZE;
        const struct segment_t segment = {physical_pd, 1u};
        push_back(process->segments, &segment);
    }

    uint32_t *const virt_page_table = (uint32_t*)((virt_page_directory[table_index]) & 0xFFFFF000u);

    virt_page_table[page_index_in_table] = physical | PT_RW | PT_USER | PT_PRESENT;

    free_physical_pointer(&cr3_ptr);

    return true;
}
bool user_map_pages(struct process_t* process, size_t virt, size_t physical, size_t pages) {
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

uint32_t* get_kernel_page_directory(void) {
    return kernel_address_space;
}
