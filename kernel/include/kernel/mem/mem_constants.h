#pragma once


//in bytes
#define PAGE_SIZE 4096

#define NUMBER_OF_PAGES_IN_ACPICA_RANGE 1024

#define NUMBER_OF_PAGES_IN_GLOBAL_VIRT_ALLOCATOR 1024

#define NUMBER_OF_PAGES (0x100000000u/PAGE_SIZE)

#define NUMBER_OF_PAGES_IN_KERNEL_HEAP (4096u)

#define MAX_SIZE_OF_HEAP 0x40000u //number of pages in 1GiB

#define ACPICA_SIZE 1024u //in pages

#define CACHE_N 20u

enum memory_type {
    CRITICAL_KERNEL_USE = 0,
    KERNEL_USE = 1,
    ACPICA_USE = 2,
    USER_USE = 3
};
