#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include "cassert.h"
#include "vga_driver.h"
#include "cstring.h"

int cprintf(const char *format, ...);
