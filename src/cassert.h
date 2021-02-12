#pragma once

#include "vga_driver.h"

#define assert(x, failure)   do { \
                                if(!x) {  \
                                        \
                                    return failure; \
                                } \
                            } while(0)
