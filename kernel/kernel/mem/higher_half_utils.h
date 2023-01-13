#pragma once

#define KERNEL_OFFSET 0xC0000000
#define KERNEL_START 0x10000
#define V2P(a) (((uint32_t)(a)) - KERNEL_OFFSET)
#define P2V(a) (((uint32_t)(a)) + KERNEL_OFFSET)

