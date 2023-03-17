#pragma once

#include <higher_half_utils.h>


//in bytes
#define PAGE_SIZE 4096u


#define ADD_TWO_PTRS(ptr1, ptr2) (void*)(((uint32_t)(ptr1))+((uint32_t)(ptr2)))
#define SUB_TWO_PTRS(ptr1, ptr2) (void*)(((uint32_t)(ptr1))-((uint32_t)(ptr2)))


#define NUMBER_OF_PAGES (0x100000000u/PAGE_SIZE) // equals 1048576

// 3GiB User/ 1GiB Kernel
#define KERNEL_PAGES (NUMBER_OF_PAGES/4)

//in pages
#define MAX_ACPICA_SIZE 1024

// 1/4 GiB in pages
#define MAX_KERNEL_HEAP_SIZE 0x10000

// 0 --- (MAX_KERNEL_HEAP_SIZE-1), MAX_KERNEL_HEAP_SIZE --- (MAX_KERNEL_HEAP_SIZE*2-1), (MAX_KERNEL_HEAP_SIZE*2) --- (MAX_KERNEL_HEAP_SIZE*3-1), (MAX_KERNEL_HEAP_SIZE*3) --- (MAX_KERNEL_HEAP_SIZE*4-1)
#define HEAP_START_ADDR (P2V(MAX_KERNEL_HEAP_SIZE*3))
//#define HEAP_START_ADDR 0xC0030000
