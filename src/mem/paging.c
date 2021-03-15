#include "paging.h"


void paging_init(void) {
    uint32_t* page_directory = allocate_page(); //page frame allocator returns page aligned blocks of 4KiB of memory
    for(int32_t i = 0; i < 1024; ++i) {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        page_directory[i] = 0x00000002;
    }

    uint32_t* first_page_table = allocate_page();
    for(uint32_t i = 0u; i < 1024u; ++i) {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    }

    
}
