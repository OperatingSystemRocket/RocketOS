#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <kstring.h>
#include <mem_constants.h>
#include <higher_half_utils.h>
#include <bitset.h>
#include <buddy_memory_allocator.h>


#define GLOBAL_PHYS_MEM_SIZE 0x100000000u // 4GiB in bytes


bool global_phys_allocator_init(void);
void* global_phys_allocator_allocate_page(void);
void* global_phys_allocator_allocate_pages(size_t num_of_pages);
bool global_phys_allocator_free_page(void* page_phys_addr);
bool global_phys_allocator_free_pages(void* page_phys_addr, size_t num_of_pages);


uint32_t get_immutable_data_start(void);
uint32_t get_text_start(void);
uint32_t get_text_end(void);
uint32_t get_rodata_start(void);
uint32_t get_rodata_end(void);
uint32_t get_immutable_data_end(void);
uint32_t get_mutable_data_start(void);
uint32_t get_data_start(void);
uint32_t get_data_end(void);
uint32_t get_bss_start(void);
uint32_t get_bss_end(void);
uint32_t get_mutable_data_end(void);
uint32_t get_endkernel(void);
