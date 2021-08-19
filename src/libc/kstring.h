#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "kassert.h"


void* kmemcpy(void* restrict destination, const void* restrict source, size_t num);
void* kmemmove(void* destination, const void* source, size_t num);
void* kmemchr(void* ptr, int32_t value, size_t num);
int32_t kmemcmp(const void* ptr1, const void* ptr2, size_t num);
void* kmemset(void* ptr, int32_t value, size_t num);
char* kstrcat(char* destination, const char* source);
char* kstrncat(char* destination, const char* source, size_t num);
char* kstrchr(char* str, int32_t character); //searches from start of string
char* kstrrchr(char* str, int32_t character); //searches from end of string
int32_t kstrcmp(const char* lhs, const char* rhs);
int32_t kstrncmp(const char* const lhs, const char* rhs, size_t sz);
char* kstrcpy(char* destination, const char* source);
char* kstrncpy(char* destination, const char* source, size_t num);
int32_t kstrspn(const char* str1, const char* str2);
size_t kstrlen(const char* str);
char* kint_to_string(int64_t input, char* string_ret, size_t ret_size, uint32_t base, bool lowercase);
char kint_to_char(int8_t input);
int8_t kchar_to_int(char c);
