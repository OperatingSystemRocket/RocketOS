#pragma once

#include <stdbool.h>

#include <global_phys_allocator.h>
#include <kernel_virt_allocator.h>
#include <osi_virt_mem_allocator.h>
#include <paging.h>


bool initialize_kernel_memory(void);
