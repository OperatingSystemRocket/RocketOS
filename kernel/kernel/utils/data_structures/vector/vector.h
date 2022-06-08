#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kstdlib.h>
#include <kstring.h>
#include <kassert.h>


struct vector;

struct vector* create_vector(size_t data_size);
struct vector* copy_vector(const struct vector* v);
bool push_back(struct vector* v, const void* data);
void* at(struct vector* v, size_t index);
const void* at_const(const struct vector* v, size_t index);
size_t size(const struct vector* v);
bool erase(struct vector* v, size_t index);
bool clear(struct vector* v);
bool destructive_clear(struct vector* v);
bool destroy_vector(struct vector* v);

typedef struct vector* vector_type_t; // can be used similar to CTAD
#define vector_type(T) struct vector* // Python style type hint to increase readability of code using this type
