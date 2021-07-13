#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "physical_mem_allocator.h"
#include "hardware_io.h"
#include "serial_driver.h"
#include "kassert.h"


// Page Table Entry flags
enum page_table_entry_flags {
    PT_PRESENT = (1<<0),
    PT_RW = (1<<1),
    PT_USER = (1<<2),
    PT_WRITETHROUGH = (1<<3)
};

// Page Directory Entry flags
enum page_directory_entry_flags {
    PD_PRESENT = (1<<0),
    PD_RW = (1<<1),
    PD_USER = (1<<2),
    PD_WRITETHROUGH = (1<<3),
    PD_DISABLECACHE = (1<<4)    
};

// Page Fault error flags
enum page_fault_error_flags {
    PF_PRESENT = (1<<0),      // Was the page present?
    PF_RW = (1<<1),           // Was the page wrongfully written to?
    PF_USER = (1<<2),         // Was the CPU in user-mode?
    PF_RESERVED = (1<<3),     // Were the CPU-reserved bytes overwritten?
    PF_ID = (1<<4)           // Was the fault caused by an instruction fetch?
};


extern uintptr_t get_cr3(void);


void paging_init(void);
void* get_physical_memory(void* virtual_address);
void map_page(void* virtual_address, uint32_t phys_frame, uint32_t pt_flags, uint32_t pd_flags);
uint32_t allocate_virtual_page(void* virtual_address, uint32_t pt_flags, uint32_t pd_flags);
uint32_t unmap_page(const void* virtual_address);
void free_virtual_page(const void* virtual_address);

uint32_t* get_default_page_directory(void);
