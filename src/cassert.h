#pragma once

#include "vga_driver.h"

#define assert(x, failure)   do { \
                                if(!(x)) { \
                                    terminal_write_color("Condition Failed: ", VGA_COLOR_RED); \
                                    terminal_write_color(#x, VGA_COLOR_RED);                   \
                                    terminal_write_color("\n", VGA_COLOR_RED); \
                                    return failure; \
                                } \
                            } while(0)
