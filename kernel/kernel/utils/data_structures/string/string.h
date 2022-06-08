#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kstring.h>
#include <kstdlib.h>


struct string {
    char* data;
    size_t length;
};


struct string string_new(const char* data);
struct string string_new_with_len(const char* data, size_t len);
bool assign_string(struct string* str, const char* data);
bool assign_string_with_len(struct string* str, const char* data, size_t len);
bool assign_string_from_string(struct string* str, const struct string* other);
bool destroy_string(struct string* str);
