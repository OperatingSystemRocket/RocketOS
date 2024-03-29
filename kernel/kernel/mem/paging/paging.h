#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <global_phys_allocator.h>
#include <kernel_virt_allocator.h>
#include <hardware_io.h>
#include <serial_driver.h>
#include <kassert.h>
#include "mem_constants.h"
#include <process.h>
#include <physical_pointer.h>
#include <kstring.h>
#include <higher_half_utils.h>


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

#define PAGE_BITMASK 0xFFFFF000


bool paging_init(void);

void load_and_turn_on_paging(void);

uint32_t get_physical_address_in_boot(const void* virtual_address);
uint32_t get_physical_address(uint32_t page_directory, const void* virtual_address);
uint32_t get_physical_address_in_kernel_addr(const void* virtual_address);
uint32_t get_physical_address_in_current_addr(const void* virtual_address);

bool is_readable(uint32_t page_directory, const void* virtual_address); //checks if it is in the paging structure
bool is_readable_in_kernel_addr(const void* virtual_address); //checks if it is in the kernel address space
bool is_readable_in_current_addr(const void* virtual_address);
bool is_writable(uint32_t page_directory, const void* virtual_address); //checks if it is in the paging structure and if the RW flag is set
bool is_writable_in_kernel_addr(const void* virtual_address); //checks if it is in the kernel address space and if the RW flag is set
bool is_writable_in_current_addr(const void* virtual_address);


bool identity_map_page(uint32_t page_directory, uint32_t address, uint32_t pt_flags, uint32_t pd_flags);
bool identity_map_pages(uint32_t page_directory, uint32_t address, uint32_t num_of_pages, uint32_t pt_flags, uint32_t pd_flags);
bool identity_map_page_in_kernel_addr(uint32_t address, uint32_t pt_flags, uint32_t pd_flags);
bool identity_map_pages_in_kernel_addr(uint32_t address, uint32_t num_of_pages, uint32_t pt_flags, uint32_t pd_flags);
bool identity_map_page_in_current_addr(uint32_t address, uint32_t pt_flags, uint32_t pd_flags);
bool identity_map_pages_in_current_addr(uint32_t address, uint32_t num_of_pages, uint32_t pt_flags, uint32_t pd_flags);

void* map_to_arbitrary_kernel_virt_page_in_current_addr(uint32_t phys_addr, uint32_t page_index_in_table);
void* map_to_arbitrary_kernel_virt_pages_in_current_addr(uint32_t phys_addr, uint32_t page_index_in_table, uint32_t size); // `size` is number of pages

bool unmap_arbitrary_kernel_virt_page_in_current_addr(uint32_t page_index_in_table);
bool unmap_arbitrary_kernel_virt_pages_in_current_addr(uint32_t page_index_in_table, uint32_t size); // `size` is number of pages

bool map_page(uint32_t page_directory, void* virtual_address, uint32_t phys_frame, uint32_t pt_flags, uint32_t pd_flags);
bool map_pages(uint32_t page_directory, void* virtual_address, uint32_t phys_frame, size_t num_of_pages, uint32_t pt_flags, uint32_t pd_flags);
bool map_page_in_kernel_addr(void* virtual_address, uint32_t phys_frame, uint32_t pt_flags, uint32_t pd_flags);
bool map_pages_in_kernel_addr(void* virtual_address, uint32_t phys_frame, size_t num_of_pages, uint32_t pt_flags, uint32_t pd_flags);
bool map_page_in_current_addr(void* virtual_address, uint32_t phys_frame, uint32_t pt_flags, uint32_t pd_flags);
bool map_pages_in_current_addr(void* virtual_address, uint32_t phys_frame, size_t num_of_pages, uint32_t pt_flags, uint32_t pd_flags);

uint32_t unmap_page(uint32_t page_directory, const void* virtual_address);
uint32_t unmap_pages(uint32_t page_directory, const void* virtual_address, size_t num_of_pages);
uint32_t unmap_page_in_kernel_addr(const void* virtual_address);
uint32_t unmap_pages_in_kernel_addr(const void* virtual_address, size_t num_of_pages);
uint32_t unmap_page_in_current_addr(const void* virtual_address);
uint32_t unmap_pages_in_current_addr(const void* virtual_address, size_t num_of_pages);


inline uint32_t return_page_address(const uint32_t address) {
    return address & 0xFFFFF000u;
}
inline uint32_t return_page_offset(const uint32_t address) {
    return address & 0x00000FFFu;
}
inline uint32_t get_rounded_up_num_of_pages(const uint32_t address) {
    return ((address / PAGE_SIZE) + ((address % PAGE_SIZE) > 0u));
}

void map_kernel_inside_user(struct process_t* process);
void clear_physical_page(size_t physical);
bool user_map(struct process_t* process, size_t virt, size_t physical);
bool user_map_pages(struct process_t* process, size_t virt, size_t physical, size_t pages);


extern uintptr_t get_current_cr3(void); // returns currently loaded page directory phys addr
extern void set_cr3(uint32_t* new_page_directory_phys_addr);
uint32_t* get_kernel_page_directory(void);
void set_current_page_directory(uint32_t* new_current_page_directory);
uint32_t get_current_page_directory(void); // returns virtual address of current paging subsytem page directory (in kernel space)
uint32_t get_current_page_directory_phys_addr(void); // returns physical address of current paging subsytem page directory
