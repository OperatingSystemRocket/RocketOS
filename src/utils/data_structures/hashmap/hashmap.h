#pragma once


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "kstdio.h"

#include "kassert.h"
#include "default_keyboard_logic.h"


//this is a type safe dynamic (heap allocating) hashmap implementation

typedef uint32_t TYPE_OF_DATA;
typedef const char* TYPE_OF_KEY;

struct hashmap {
    struct entry {
        TYPE_OF_DATA data;
        TYPE_OF_KEY key;
        struct entry* prev;
        struct entry* next;
    }** list_of_buckets;
    size_t num_of_buckets;
    size_t num_of_entries;
    uint32_t (*hash_function)(TYPE_OF_KEY);
    bool (*comp)(TYPE_OF_KEY, TYPE_OF_KEY);
};

struct return_type {
    TYPE_OF_DATA data;
    bool succeeded;
};

void hashmap_init(struct hashmap* hashmap, uint32_t (*hash_function)(TYPE_OF_KEY), bool (*comp)(TYPE_OF_KEY, TYPE_OF_KEY));
TYPE_OF_DATA* hashmap_find(struct hashmap* hashmap, TYPE_OF_KEY key);
struct return_type hashmap_remove(struct hashmap* hashmap, TYPE_OF_KEY key);
void hashmap_add(struct hashmap* hashmap, TYPE_OF_KEY key, TYPE_OF_DATA data);
void hashmap_destroy(struct hashmap* hashmap);
