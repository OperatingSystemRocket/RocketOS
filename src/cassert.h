#pragma once

#include "vga_driver.h"

//@todo disable asserts when NDEBUG is defined so that no release runtime performance penalty is kept for asserts

//@note clion keep breaking the indentation on these macros, don't bother reformatting them manually

#define cassert(cond, return_code)  do { \
                                    if(!(cond)) { \
                                        terminal_writestring_color("Condition Failed: ", VGA_COLOR_RED); \
                                        terminal_writestring_color(#cond, VGA_COLOR_RED);                   \
                                        terminal_writestring_color("\n", VGA_COLOR_RED); \
                                        return return_code; \
                                    } \
                                } while(0)

#define cassert_void(cond)  do { \
                            if(!(cond)) { \
                                terminal_writestring_color("Condition Failed: ", VGA_COLOR_RED); \
                                terminal_writestring_color(#cond, VGA_COLOR_RED);                   \
                                terminal_writestring_color("\n", VGA_COLOR_RED); \
                                return; \
                            } \
                        } while(0)

#define cassert_message(cond, message, return_code)  do { \
                                                    if(!(cond)) { \
                                                        terminal_writestring_color(message, VGA_COLOR_RED); \
                                                        terminal_writestring_color("\n", VGA_COLOR_RED); \
                                                        return return_code; \
                                                    } \
                                                } while(0)

#define cassert_message_void(cond, message)  do { \
                            if(!(cond)) { \
                                terminal_writestring_color(message, VGA_COLOR_RED); \
                                terminal_writestring_color("\n", VGA_COLOR_RED); \
                                return; \
                            } \
                        } while(0)
