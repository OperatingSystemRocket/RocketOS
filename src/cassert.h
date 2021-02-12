#pragma once

#include "vga_driver.h"

#define assert(x, return_code)  do { \
                                    if(!(x)) { \
                                        terminal_write_color("Condition Failed: ", VGA_COLOR_RED); \
                                        terminal_write_color(#x, VGA_COLOR_RED);                   \
                                        terminal_write_color("\n", VGA_COLOR_RED); \
                                        return return_code; \
                                    } \
                                } while(0)

#define assert_void(x)  do { \
                            if(!(x)) { \
                                terminal_write_color("Condition Failed: ", VGA_COLOR_RED); \
                                terminal_write_color(#x, VGA_COLOR_RED);                   \
                                terminal_write_color("\n", VGA_COLOR_RED); \
                                return; \
                            } \
                        } while(0)
