#pragma once

#include "vga_driver.h"

#define UNITY_EXCLUDE_SETJMP_H
#define UNITY_EXCLUDE_MATH_H
#define UNITY_EXCLUDE_FLOAT
#define UNITY_EXCLUDE_DOUBLE
#define UNITY_OUTPUT_CHAR(a) terminal_putchar(a)
