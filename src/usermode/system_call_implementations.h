#pragma once

#include <stdint.h>
#include <stddef.h>


extern uint32_t __attribute__((naked)) __attribute__((regparm(1))) print_int(uint32_t eax);

extern uint32_t __attribute__((naked)) __attribute__((regparm(1))) print(const char* str);
