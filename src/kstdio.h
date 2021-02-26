#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include "foo/kassert.h"
#include "foo/vga_driver.h"
#include "kstring.h"

int kprintf(const char *format, ...);
