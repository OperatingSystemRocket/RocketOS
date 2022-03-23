#pragma once

#include <stdint.h>
#include <stddef.h>


#define SYS_LOG 0
#define SYS_MMAP 1
#define SYS_MUNMAP 2
#define SYS_OPEN 3
#define SYS_CLOSE 4
#define SYS_READ 5
#define SYS_WRITE 6


extern uint32_t __attribute__((naked)) __attribute__((regparm(1))) print_int(uint32_t eax);

extern uint32_t __attribute__((naked)) __attribute__((regparm(1))) print(const char* str);
