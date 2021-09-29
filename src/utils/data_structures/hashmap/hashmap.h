#pragma once


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "kassert.h"
#include "default_keyboard_logic.h"


//this is a type safe dynamic (heap allocating) hashmap implementation

#define HASHMAP_RESIZE_FACTOR 2
#define HASHMAP_LOAD_FACTOR 2 //entries:buckets ratio of when to reallocate the entries array for more buckets
#define HASHMAP_STARTING_NUM_OF_BUCKETS 2 //256


#define GENERATE_HASHMAP_DECLARATION(TYPE_OF_KEY, TYPE_OF_DATA) \
struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA { \
    struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA { \
        TYPE_OF_KEY key; \
        TYPE_OF_DATA data; \
        struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA* prev; \
        struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA* next; \
    }** list_of_buckets; \
    size_t num_of_buckets; \
    size_t num_of_entries; \
    uint32_t (*hash_function)(TYPE_OF_KEY); \
}; \
struct return_type_##TYPE_OF_DATA { \
    TYPE_OF_DATA data; \
    bool succeeded; \
}; \
void hashmap_init_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA* hashmap, uint32_t (*hash_function)(TYPE_OF_KEY)); \
TYPE_OF_DATA* hashmap_find_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA* hashmap, TYPE_OF_KEY key); \
struct return_type_##TYPE_OF_DATA hashmap_remove_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA* hashmap, TYPE_OF_KEY key); \
void hashmap_add_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA* hashmap, TYPE_OF_KEY key, TYPE_OF_DATA data); \
void hashmap_destroy_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA* hashmap);



#define GENERATE_HASHMAP_DEFINITION(TYPE_OF_KEY, TYPE_OF_DATA, COMP) \
void hashmap_init_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA *const hashmap, uint32_t (*const hash_function)(TYPE_OF_KEY)) { \
    hashmap->num_of_entries = 0u; \
    hashmap->num_of_buckets = HASHMAP_STARTING_NUM_OF_BUCKETS; \
    hashmap->list_of_buckets = zeroed_out_kmalloc(sizeof(struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA*) * hashmap->num_of_buckets); \
    hashmap->hash_function = hash_function; \
} \
static uint32_t get_hash_##TYPE_OF_KEY##_##TYPE_OF_DATA(const struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA *const hashmap, const TYPE_OF_KEY key) { \
    const uint32_t hash = hashmap->hash_function(key); \
    /*equivalent to `return hash % hashmap->num_of_buckets;`*/ \
    return hash & (hashmap->num_of_buckets - 1u); \
} \
static void resize_hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA *const hashmap) { \
    struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA new_hashmap = *hashmap; \
\
    new_hashmap.num_of_buckets *= HASHMAP_RESIZE_FACTOR; \
    new_hashmap.list_of_buckets = zeroed_out_kmalloc(sizeof(struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA*) * new_hashmap.num_of_buckets); \
\
    size_t num_of_entries_transferred = 0u; \
    for(size_t i = 0u; i < hashmap->num_of_buckets; ++i) { \
        struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA** first_entry_with_hash_ptr = &hashmap->list_of_buckets[i]; \
        while((*first_entry_with_hash_ptr) != NULL) { \
            struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA* first_entry_with_hash = *first_entry_with_hash_ptr; \
\
            if(first_entry_with_hash->next != NULL) { \
                first_entry_with_hash->next->prev = NULL; \
            } \
            *first_entry_with_hash_ptr = first_entry_with_hash->next; \
\
            const uint32_t new_hash = get_hash_##TYPE_OF_KEY##_##TYPE_OF_DATA(&new_hashmap, first_entry_with_hash->key); \
            if(new_hashmap.list_of_buckets[new_hash] == NULL) { \
                new_hashmap.list_of_buckets[new_hash] = first_entry_with_hash; \
                first_entry_with_hash->next = NULL; \
                first_entry_with_hash->prev = NULL; \
            } else { \
                struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA* current_entry = new_hashmap.list_of_buckets[new_hash]; \
                while(current_entry->next != NULL) { \
                    current_entry = current_entry->next; \
                } \
\
                current_entry->next = first_entry_with_hash; \
                first_entry_with_hash->prev = current_entry; \
                first_entry_with_hash->next = NULL; \
            } \
            ++num_of_entries_transferred; \
        } \
    } \
\
    kfree(hashmap->list_of_buckets); \
\
    kassert_void(num_of_entries_transferred == hashmap->num_of_entries); \
    kassert_void(num_of_entries_transferred == new_hashmap.num_of_entries); \
\
    *hashmap = new_hashmap; \
} \
static struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA* hashmap_find_entry_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA *const hashmap, const TYPE_OF_KEY key) { \
    const uint32_t hash = get_hash_##TYPE_OF_KEY##_##TYPE_OF_DATA(hashmap, key); \
\
    struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA *const first_entry_with_hash = hashmap->list_of_buckets[hash]; \
    if(first_entry_with_hash == NULL) { \
        return NULL; \
    } \
\
    if(first_entry_with_hash->next == NULL) { \
        if(COMP(first_entry_with_hash->key, key)) { \
            return first_entry_with_hash; \
        } else { \
            return NULL; \
        } \
    } else { \
        struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA* current_entry = first_entry_with_hash; \
        while(current_entry != NULL) { \
            if(COMP(current_entry->key, key)) { \
                return current_entry; \
            } \
\
            current_entry = current_entry->next; \
        }; \
\
        return NULL; \
    } \
} \
TYPE_OF_DATA* hashmap_find_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA *const hashmap, const TYPE_OF_KEY key) { \
    struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA *const entry = hashmap_find_entry_##TYPE_OF_KEY##_##TYPE_OF_DATA(hashmap, key); \
\
    if(entry == NULL) { \
        return NULL; \
    } \
\
    return &entry->data; \
} \
struct return_type_##TYPE_OF_DATA hashmap_remove_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA *const hashmap, const TYPE_OF_KEY key) { \
    struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA *const entry = hashmap_find_entry_##TYPE_OF_KEY##_##TYPE_OF_DATA(hashmap, key); \
\
    if(entry == NULL) { \
        return (struct return_type_##TYPE_OF_DATA){ .succeeded = false }; \
    } \
\
    const struct return_type_##TYPE_OF_DATA ret = { .data = entry->data, .succeeded = true }; \
    if(entry->prev != NULL) { \
        entry->prev = entry->next; \
    } else { \
        /*it is the first value with that hash in the bucket*/ \
        const uint32_t hash = get_hash_##TYPE_OF_KEY##_##TYPE_OF_DATA(hashmap, key); \
        hashmap->list_of_buckets[hash] = entry->next; \
    } \
\
    if(entry->next != NULL) { \
        entry->next->prev = entry->prev; \
    } \
    kfree(entry); \
\
    --hashmap->num_of_entries; \
\
    return ret; \
} \
void hashmap_add_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA *const hashmap, const TYPE_OF_KEY key, const TYPE_OF_DATA data) { \
    if((hashmap->num_of_entries/hashmap->num_of_buckets) >= HASHMAP_LOAD_FACTOR) { \
        resize_hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA(hashmap); \
    } \
\
    const uint32_t hash = get_hash_##TYPE_OF_KEY##_##TYPE_OF_DATA(hashmap, key); \
\
    struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA* *const first_entry_with_hash_ptr = &hashmap->list_of_buckets[hash]; \
\
    struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA *const new_entry = zeroed_out_kmalloc(sizeof(struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA)); \
    new_entry->key = key; \
    new_entry->data = data; \
    new_entry->next = NULL; \
\
    if(*first_entry_with_hash_ptr == NULL) { \
        new_entry->prev = NULL; \
\
        *first_entry_with_hash_ptr = new_entry; \
    } else { \
        struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA* current_entry = *first_entry_with_hash_ptr; \
        for(;;) { \
            if(current_entry->next != NULL) { \
                current_entry = current_entry->next; \
            } else { \
                break; \
            } \
        } \
\
        current_entry->next = new_entry; \
        new_entry->prev = current_entry; \
    } \
\
    ++hashmap->num_of_entries; \
} \
void hashmap_destroy_##TYPE_OF_KEY##_##TYPE_OF_DATA(struct hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA *const hashmap) { \
    for(uint32_t i = 0u; i < hashmap->num_of_buckets; ++i) { \
        struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA* current_entry = hashmap->list_of_buckets[i]; \
        while(current_entry != NULL) { \
            struct entry_##TYPE_OF_KEY##_##TYPE_OF_DATA *const next_entry = current_entry->next; \
            kfree(current_entry); \
            current_entry = next_entry; \
        } \
    } \
}



#define HASHMAP_INIT(TYPE_OF_KEY, TYPE_OF_DATA, HASHMAP, HASH_FUNCTION) hashmap_init_##TYPE_OF_KEY##_##TYPE_OF_DATA(HASHMAP, HASH_FUNCTION)
#define HASHMAP_FIND(TYPE_OF_KEY, TYPE_OF_DATA, HASHMAP, KEY) hashmap_find_##TYPE_OF_KEY##_##TYPE_OF_DATA(HASHMAP, KEY)
#define HASHMAP_REMOVE(TYPE_OF_KEY, TYPE_OF_DATA, HASHMAP, KEY) hashmap_remove_##TYPE_OF_KEY##_##TYPE_OF_DATA(HASHMAP, KEY)
#define HASHMAP_ADD(TYPE_OF_KEY, TYPE_OF_DATA, HASHMAP, KEY, DATA) hashmap_add_##TYPE_OF_KEY##_##TYPE_OF_DATA(HASHMAP, KEY, DATA)
#define HASHMAP_DESTROY(TYPE_OF_KEY, TYPE_OF_DATA, HASHMAP) hashmap_destroy_##TYPE_OF_KEY##_##TYPE_OF_DATA(HASHMAP)
#define HASHMAP_STRUCT(TYPE_OF_KEY, TYPE_OF_DATA) hashmap_##TYPE_OF_KEY##_##TYPE_OF_DATA
#define HASHMAP_RETURN_TYPE(TYPE_OF_DATA) return_type_##TYPE_OF_DATA
