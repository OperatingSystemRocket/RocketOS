#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "kstring.h"
#include "kstdio.h"


struct file_header {
    uint32_t size; //includes both the file and header, a value of `0` indicates the end of the file list
    char filename[100]; //null terminated
    char reserved[24]; //should just be zeroed out
    char file[];
};

void parse_headers(uint32_t address);
void print_file(const char* filename);
