#pragma once

#include "kstdio.h"
#include <drivers/vga_driver/default_vga_driver.h>


#ifndef NDEBUG
#define kassert(cond, return_code)  do { \
                                    if(!(cond)) { \
                                        kprintf("[file: %s :: line: %i]: ", __FILE__, __LINE__); \
                                        kprintf("Condition Failed: %s\n", #cond); \
                                        return return_code; \
                                    } \
                                } while(0)

#define kassert_void(cond)  do { \
                            if(!(cond)) { \
                                kprintf("[file: %s :: line: %i]: ", __FILE__, __LINE__); \
                                kprintf("Condition Failed: %s\n", #cond); \
                                return; \
                            } \
                        } while(0)

#define kassert_message(cond, message, return_code)  do { \
                                                    if(!(cond)) { \
                                                        kprintf("%s\n", message); \
                                                        return return_code; \
                                                    } \
                                                } while(0)

#define kassert_message_void(cond, message)  do { \
                            if(!(cond)) { \
                                kprintf("%s\n", message); \
                                return; \
                            } \
                        } while(0)
/*
#define kassert(cond, return_code)  do { \
                                    if(!(cond)) { \
                                        kprintf("%4[file: %s :: line: %i]: ", __FILE__, __LINE__); \
                                        terminal_context_writestring_color("Condition Failed: ", VGA_COLOR_RED); \
                                        terminal_context_writestring_color(#cond, VGA_COLOR_RED); \
                                        terminal_context_putchar_color('\n', VGA_COLOR_RED); \
                                        return return_code; \
                                    } \
                                } while(0)

#define kassert_void(cond)  do { \
                            if(!(cond)) { \
                                kprintf("%4[file: %s :: line: %i]: ", __FILE__, __LINE__); \
                                terminal_context_writestring_color("Condition Failed: ", VGA_COLOR_RED); \
                                terminal_context_writestring_color(#cond, VGA_COLOR_RED); \
                                terminal_context_putchar_color('\n', VGA_COLOR_RED); \
                                return; \
                            } \
                        } while(0)

#define kassert_message(cond, message, return_code)  do { \
                                                    if(!(cond)) { \
                                                        terminal_context_writestring_color(message, VGA_COLOR_RED); \
                                                        terminal_context_putchar_color('\n', VGA_COLOR_RED); \
                                                        return return_code; \
                                                    } \
                                                } while(0)

#define kassert_message_void(cond, message)  do { \
                            if(!(cond)) { \
                                terminal_context_writestring_color(message, VGA_COLOR_RED); \
                                terminal_context_putchar_color('\n', VGA_COLOR_RED); \
                                return; \
                            } \
                        } while(0)
*/
#else

#define kassert(cond, return_code)

#define kassert_void(cond)

#define kasser_message(cond, message, return_code)

#define kassert_message_void(cond, message)
#endif
