#pragma once

#include "vga_driver.h"

//@todo disable asserts when NDEBUG is defined so that no release runtime performance penalty is kept for asserts

//@note clion keep breaking the indentation on these macros, don't bother reformatting them manually

#define assert(x, return_code)  do { \
                                    if(!(x)) { \
                                        terminal_writestring_color("Condition Failed: ", VGA_COLOR_RED); \
                                        terminal_writestring_color(#x, VGA_COLOR_RED);                   \
                                        terminal_writestring_color("\n", VGA_COLOR_RED); \
                                        return return_code; \
                                    } \
                                } while(0)

#define assert_void(x)  do { \
                            if(!(x)) { \
                                terminal_writestring_color("Condition Failed: ", VGA_COLOR_RED); \
                                terminal_writestring_color(#x, VGA_COLOR_RED);                   \
                                terminal_writestring_color("\n", VGA_COLOR_RED); \
                                return; \
                            } \
                        } while(0)

#define assert_message(x, message, return_code)  do { \
                                                    if(!(x)) { \
                                                        terminal_writestring_color(message, VGA_COLOR_RED); \
                                                        terminal_writestring_color("\n", VGA_COLOR_RED); \
                                                        return return_code; \
                                                    } \
                                                } while(0)

#define assert_message_void(x, message)  do { \
                            if(!(x)) { \
                                terminal_writestring_color(message, VGA_COLOR_RED); \
                                terminal_writestring_color("\n", VGA_COLOR_RED); \
                                return; \
                            } \
                        } while(0)
