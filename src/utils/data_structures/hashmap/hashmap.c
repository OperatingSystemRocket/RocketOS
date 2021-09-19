#include "hashmap.h"


void hashmap_init(struct hashmap *const hashmap, uint32_t (*const hash_function)(TYPE_OF_KEY), bool (*const comp)(TYPE_OF_KEY, TYPE_OF_KEY)) {
    hashmap->data_entries = NULL;
    hashmap->num_of_entries = 0u;
    hashmap->hash_function = hash_function;
    hashmap->comp = comp;
}

static struct entry* hashmap_find_entry(struct hashmap *const hashmap, const TYPE_OF_KEY key) {
    const uint32_t hash = hashmap->hash_function(key);

    if(hash >= hashmap->num_of_entries) {
        return NULL;
    }

    struct entry *const first_entry_with_hash = hashmap->data_entries[hash];
    if(hashmap->data_entries == NULL || first_entry_with_hash == NULL) {
        return NULL;
    }

    if(first_entry_with_hash->next == NULL) {
        if(hashmap->comp(first_entry_with_hash->key, key)) {
            return first_entry_with_hash;
        } else {
            return NULL;
        }
    } else {
        struct entry* current_entry = first_entry_with_hash;
        while(current_entry != NULL) {
            if(hashmap->comp(current_entry->key, key)) {
                return current_entry;
            }

            current_entry = current_entry->next;
        };

        return NULL;
    }
}

TYPE_OF_DATA* hashmap_find(struct hashmap *const hashmap, const TYPE_OF_KEY key) {
    struct entry *const entry = hashmap_find_entry(hashmap, key);

    if(entry == NULL) {
        return NULL;
    }

    return &entry->data;
}

struct return_type hashmap_remove(struct hashmap *const hashmap, const TYPE_OF_KEY key) {
    struct entry *const entry = hashmap_find_entry(hashmap, key);

    if(entry == NULL) {
        return (struct return_type){ .succeeded = false };
    }

    const struct return_type ret = { .data = entry->data, .succeeded = true };
    if(entry->prev != NULL) {
        entry->prev = entry->next;
    } else {
        //it is the first value with that hash in the bucket
        const uint32_t hash = hashmap->hash_function(key);
        hashmap->data_entries[hash] = entry->next;
    }

    if(entry->next != NULL) {
        entry->next->prev = entry->prev;
    }
    kfree(entry);
    return ret;
}

void hashmap_add(struct hashmap *const hashmap, const TYPE_OF_KEY key, const TYPE_OF_DATA data) {
    const uint32_t hash = hashmap->hash_function(key);
    if(hashmap->data_entries == NULL) {
        kassert_void(hashmap->num_of_entries == 0u);

        //hashes are indexes, so the array length must be >= hash+1 for hash to be a valid index
        hashmap->num_of_entries = round_up_to_nearest_n_power(hash+1u, 2u);
        hashmap->data_entries = zeroed_out_kmalloc(sizeof(struct entry*) * hashmap->num_of_entries);
    } else if(hash >= hashmap->num_of_entries) {
        hashmap->num_of_entries = round_up_to_nearest_n_power(hash+1u, 2u);
        hashmap->data_entries = krealloc(hashmap->data_entries, sizeof(struct entry*) * hashmap->num_of_entries);
    }

    struct entry* *const first_entry_with_hash_ptr = &hashmap->data_entries[hash];

    struct entry *const new_entry = zeroed_out_kmalloc(sizeof(struct entry));
    new_entry->key = key;
    new_entry->data = data;
    new_entry->next = NULL;

    if(*first_entry_with_hash_ptr == NULL) {
        new_entry->prev = NULL;

        *first_entry_with_hash_ptr = new_entry;
    } else {
        kprintf("first_entry_with_hash isn't null\n");
        struct entry* current_entry = *first_entry_with_hash_ptr;
        for(;;) {
            if(current_entry->next != NULL) {
                current_entry = current_entry->next;
            } else {
                break;
            }
        }

        current_entry->next = new_entry;
        new_entry->prev = current_entry;
    }
}

void hashmap_destroy(struct hashmap *const hashmap) {
    for(uint32_t i = 0u; i < hashmap->num_of_entries; ++i) {
        struct entry* current_entry = hashmap->data_entries[i];
        while(current_entry != NULL) {
            struct entry *next_entry = current_entry->next;
            kfree(current_entry);
            current_entry = next_entry;
        }
    }
}
