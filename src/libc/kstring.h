#pragma once

#include <stddef.h>
#include <stdint.h>

#include "kassert.h"


void* kmemcpy(void *restrict destination, const void *restrict source, size_t num);
void* kmemmove(void* destination, const void* source, size_t num);
void* kmemchr(void* ptr, int32_t value, size_t num);
int32_t kmemcmp(const void* ptr1, const void* ptr2, size_t num);
void* kmemset(void *const ptr, const int32_t value, size_t num);
char* kstrcat(char* destination, const char* source);
char* kstrncat(char* const destination, const char* source, size_t num);
char* kstrchr(char* str, int32_t character);
char* kstrrchr(char* str, int character);
int32_t kstrcmp(const char* lhs, const char* rhs);
int32_t kstrncmp(const char* const lhs, const char* const rhs, const size_t sz);
char* kstrcpy(char* destination, const char* source);
char* kstrncpy(char* destination, const char* source, size_t num);
size_t kstrspn(const char* str1, const char* str2);
size_t kstrlen(const char* str);
char* kint_to_string(int64_t input, char* string_ret, size_t ret_size, uint32_t base, bool lowercase);
void kuint_to_string(uint64_t input, char* const string_ret, const size_t ret_size);
char kint_to_char(int8_t);
int8_t kchar_to_int(char c);
const char* kstrstr(const char* haystack, const char* needle);
const char* kstrstr_end(const char* haystack, const char* needle);

///Non-standard functions
const char* get_string_between_substrings(const char* haystack, const char* lhs, const char* rhs);

