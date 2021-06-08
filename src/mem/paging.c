#include "paging.h"


extern void load_page_directory(uint32_t* page_directory);
extern void enable_paging(void);

extern void enable_ring0_write_protect(void);


/*
Relevant Intel manual on 32 bit paging:
https://web.archive.org/web/20151025081259/http://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-system-programming-manual-325384.pdf
*/


uint32_t* page_directory;


void identity_map_page(const uint32_t page_directory, const uint32_t address, const uint32_t pt_flags, const uint32_t pd_flags) {
    const uint32_t page_index = address / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    const uint32_t page = address | pt_flags;


    // Check if this table is present and allocate it if not
    uint32_t *const pd_virt = (uint32_t*)page_directory;
    if ((pd_virt[table_index] & PD_PRESENT) == 0) {
        const uint32_t pt_phys = (uint32_t)allocate_page(CRITICAL_KERNEL_USE);

        pd_virt[table_index] = pt_phys | pd_flags;
    }

    // Get a virtual pointer to the parent page table
    uint32_t *const pt_virt = (uint32_t)((pd_virt[table_index]) & 0xFFFFF000u);

    // Update page in page table
    pt_virt[page_index_in_table] = page;
}

void paging_init(void) {
    page_directory = allocate_page(CRITICAL_KERNEL_USE); //page frame allocator returns page aligned blocks of 4KiB of memory
    for(int32_t i = 0; i < 1024; ++i) {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        page_directory[i] = 0x00000002u;
    }
    serial_writestring("page_directory created successfully\n");

    for(uint32_t i = 0u; i < get_first_nonreserved_address(); i += PAGE_SIZE) {
        identity_map_page((uint32_t)page_directory, i, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    }
    serial_writestring("indentity mapping done successfully\n");


    uint32_t *const first_page_table = allocate_page(CRITICAL_KERNEL_USE);
    for(uint32_t i = 0u; i < 1024u; ++i) {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        first_page_table[i] = (i * 4096u) | 3u; // attributes: supervisor level, read/write, present.
    }
    serial_writestring("page table created successfully\n");


    // attributes: supervisor level, read/write, present
    page_directory[0] = ((uint32_t)first_page_table) | 3u;
    serial_writestring("page table set successfully\n");


    load_page_directory(page_directory);
    serial_writestring("page directory loaded successfully\n");


    //TODO: add flag to decide if this is done or not
    enable_ring0_write_protect();


    enable_paging();
    serial_writestring("paging enabled successfully\n");
}

void map_page(void *const virtual_address, const uint32_t phys_frame, const uint32_t pt_flags, const uint32_t pd_flags) {
    kassert_void(((uint32_t)virtual_address % PAGE_SIZE == 0) && (phys_frame % PAGE_SIZE == 0));

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    uint32_t *const virt_page_directory = page_directory;
    if((virt_page_directory[table_index] & PD_PRESENT) == 0) {
        const uint32_t phys_page_table = (uint32_t)allocate_page(CRITICAL_KERNEL_USE); //will be identity mapped

        virt_page_directory[table_index] = phys_page_table | pd_flags;
    }

    uint32_t *const virt_page_table = (uint32_t*)((virt_page_directory[table_index]) & 0xFFFFF000u);
    if((virt_page_table[page_index] & PD_PRESENT) == 0) {
        kprintf("Woah, mapping an already mapped page. You should fix this.\n"); //find out why this is being triggered
    }
    //TODO: properly handle the case of someone mapping an already mapped page

    virt_page_table[page_index_in_table] = phys_frame | pt_flags;

    flush_tlb_single_page((uint32_t)virtual_address);
}

uint32_t allocate_virtual_page(void *const virtual_address, const uint32_t pt_flags, const uint32_t pd_flags) {
    const uint32_t phys_frame = (uint32_t)allocate_page(USER_USE);

    if(phys_frame) {
        map_page(virtual_address, phys_frame, pt_flags, pd_flags);
    }

    return phys_frame;
}

uint32_t unmap_page(const void *const virtual_address) {
    kassert((uint32_t)virtual_address % PAGE_SIZE == 0, 0);

    const uint32_t page_index = ((uint32_t)virtual_address) / PAGE_SIZE;
    const uint32_t table_index = page_index / 1024;
    const uint32_t page_index_in_table = page_index % 1024;

    uint32_t *const virt_page_directory = page_directory;

    uint32_t *const virt_page_table = (uint32_t*)((virt_page_directory[table_index]) & 0xFFFFF000u);

    const uint32_t phys_frame = virt_page_table[page_index_in_table] & 0xFFFFF000u; //TODO: check if the bitmask is correct

    //TODO: consider iterating through page table to decide whether it can be freed and unmapped
    virt_page_table[page_index_in_table] = 0x0;

    flush_tlb_single_page((uint32_t)virtual_address);

    return phys_frame;
}

void free_virtual_page(const void *const virtual_address) {
    const uint32_t phys_frame = unmap_page(virtual_address);

    kassert_void(phys_frame != 0 && phys_frame % PAGE_SIZE == 0);

    free_page(phys_frame, USER_USE);
}
