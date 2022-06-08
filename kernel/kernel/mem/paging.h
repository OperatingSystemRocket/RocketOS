#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <buddy_memory_allocator.h>
#include <hardware_io.h>
#include <serial_driver.h>
#include <kassert.h>
#include "mem_constants.h"
#include <bitmap_allocator.h>
#include <process.h>
#include <physical_pointer.h>
#include <kstring.h>


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


void identity_map_page(uint32_t page_directory, uint32_t address, uint32_t pt_flags, uint32_t pd_flags);
void paging_init(void);
void load_and_turn_on_paging(void);
void reserve_virtual_address(uint32_t virtual_address, size_t num_of_pages, enum memory_type type);
uint32_t get_physical_address(const void* virtual_address);
bool is_readable(const void* virtual_address); //checks if it is in the paging structure
bool is_writable(const void* virtual_address); //checks if it is in the paging structure and if the RW flag is set
bool map_page_with_page_dir(uint32_t page_directory, void* virtual_address, uint32_t phys_frame, uint32_t pt_flags, uint32_t pd_flags);
bool map_page(void* virtual_address, uint32_t phys_frame, uint32_t pt_flags, uint32_t pd_flags);
bool map_pages(void* virtual_address, uint32_t phys_frame, size_t num_of_pages, uint32_t pt_flags, uint32_t pd_flags);
uint32_t allocate_virtual_page(void* virtual_address, uint32_t pt_flags, uint32_t pd_flags);
uint32_t unmap_page(const void* virtual_address);
uint32_t unmap_pages(const void* virtual_address, size_t num_of_pages);
void free_virtual_page(const void* virtual_address);

void map_kernel_inside_user(struct process_t* process);
void clear_physical_page(size_t physical);
bool user_map(struct process_t* process, size_t virt, size_t physical);
bool user_map_pages(struct process_t* process, size_t virt, size_t physical, size_t pages);

uint32_t* get_default_page_directory(void);
