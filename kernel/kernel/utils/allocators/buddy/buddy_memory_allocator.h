#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kstring.h>
#include <kassert.h>
#include <bitset.h>
#include <bitmap_allocator.h>
#include <mem_constants.h>

#include <kstdio.h>


#define GLOBAL_PHYS_MEM_SIZE 0x100000000u // 4GiB in bytes

struct global_binary_buddy_memory_allocator {
    uint32_t level10[32]; // in 4MiB blocks
    uint32_t level11[64];
    uint32_t level12[128];
    uint32_t level13[256];
    uint32_t level14[512];
    uint32_t level15[1024];
    uint32_t level16[2048];
    uint32_t level17[4096];
    uint32_t level18[8192];
    uint32_t level19[16384];
    uint32_t level20[32768]; // in individual pages
};

void global_binary_buddy_memory_allocator_init(struct global_binary_buddy_memory_allocator* allocator);
void* global_binary_buddy_memory_allocator_allocate_pages(struct global_binary_buddy_memory_allocator* allocator, size_t num_of_pages);
void* global_binary_buddy_memory_allocator_allocate_page(struct global_binary_buddy_memory_allocator* allocator);
void global_binary_buddy_memory_allocator_free_pages(struct global_binary_buddy_memory_allocator* allocator, const void* first_page, size_t num_of_pages);
void global_binary_buddy_memory_allocator_free_page(struct global_binary_buddy_memory_allocator* allocator, const void* page);


void global_allocator_init(void);
void global_reserve_physical_address(uint32_t physical_address, size_t num_of_pages, enum memory_type type);
void* global_allocate_page(enum memory_type allocation_type);
void* global_allocate_pages(enum memory_type allocation_type, size_t num_of_pages);
void global_free_page(enum memory_type allocation_type, const void* page);
void global_free_pages(enum memory_type allocation_type, const void* page, size_t num_of_pages);
size_t get_number_of_permanently_reserved_pages(void);
size_t get_number_of_pages_used(void);
void* get_first_nonreserved_address(void);
//in pages, not bytes:
size_t get_amount_of_nonreserved_memory(void);
uint32_t get_endkernel(void);
