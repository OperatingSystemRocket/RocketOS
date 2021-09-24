#include "hashmap.h"


#define RESIZE_FACTOR 2
#define LOAD_FACTOR 2 //entries:buckets ratio of when to reallocate the entries array for more buckets
#define STARTING_NUM_OF_BUCKETS 256


void hashmap_init(struct hashmap *const hashmap, uint32_t (*const hash_function)(TYPE_OF_KEY), bool (*const comp)(TYPE_OF_KEY, TYPE_OF_KEY)) {
    hashmap->list_of_buckets = NULL;
    hashmap->num_of_buckets = 0u;
    hashmap->num_of_entries = 0u;
    hashmap->hash_function = hash_function;
    hashmap->comp = comp;
}

static uint32_t get_hash(const struct hashmap *const hashmap, const TYPE_OF_KEY key) {
    const uint32_t hash = hashmap->hash_function(key);
    //equivalent to `return hash % hashmap->num_of_buckets;`
    return hash & (hashmap->num_of_buckets - 1);
}

static void resize_hashmap(struct hashmap *const hashmap) {
    struct hashmap new_hashmap = *hashmap;

    new_hashmap.num_of_buckets *= RESIZE_FACTOR;
    new_hashmap.list_of_buckets = zeroed_out_kmalloc(sizeof(struct entry*) * new_hashmap.num_of_buckets);

    for(size_t i = 0u; i < hashmap->num_of_buckets; ++i) {
        struct entry** first_entry_with_hash_ptr = &hashmap->list_of_buckets[i];
        struct entry* first_entry_with_hash = *first_entry_with_hash_ptr;
        if(first_entry_with_hash != NULL) {
            if(first_entry_with_hash->next != NULL) {
                first_entry_with_hash->next->prev = NULL;
            }
            *first_entry_with_hash_ptr = first_entry_with_hash->next;
            if(first_entry_with_hash->next != NULL) {
                first_entry_with_hash->next->prev = NULL;
            }

            const uint32_t new_hash = get_hash(&new_hashmap, first_entry_with_hash->key);
            if(new_hashmap.list_of_buckets[new_hash] == NULL) {
                new_hashmap.list_of_buckets[new_hash] = first_entry_with_hash;
                first_entry_with_hash->next = NULL;
                first_entry_with_hash->prev = NULL;
            } else {
                struct entry* current_entry = new_hashmap.list_of_buckets[new_hash];
                while(current_entry->next != NULL) {
                    current_entry = current_entry->next;
                }

                current_entry->next = first_entry_with_hash;
                first_entry_with_hash->prev = current_entry;
                first_entry_with_hash->next = NULL;
            }
        }
    }

    kfree(hashmap->list_of_buckets);

    kassert_void(hashmap->num_of_entries == new_hashmap.num_of_entries);

    *hashmap = new_hashmap;
}

static struct entry* hashmap_find_entry(struct hashmap *const hashmap, const TYPE_OF_KEY key) {
    const uint32_t hash = hashmap->hash_function(key);

    if(hash >= hashmap->num_of_buckets) {
        return NULL;
    }

    struct entry *const first_entry_with_hash = hashmap->list_of_buckets[hash];
    if(hashmap->list_of_buckets == NULL || first_entry_with_hash == NULL) {
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
        hashmap->list_of_buckets[hash] = entry->next;
    }

    if(entry->next != NULL) {
        entry->next->prev = entry->prev;
    }
    kfree(entry);

    --hashmap->num_of_entries;

    return ret;
}

void hashmap_add(struct hashmap *const hashmap, const TYPE_OF_KEY key, const TYPE_OF_DATA data) {
    const uint32_t hash = hashmap->hash_function(key);
    if(hashmap->list_of_buckets == NULL) {
        kassert_void(hashmap->num_of_buckets == 0u);

        //hashes are indexes, so the array length must be >= hash+1 for hash to be a valid index
        hashmap->num_of_buckets = round_up_to_nearest_n_power(hash+1u, 2u);
        hashmap->list_of_buckets = zeroed_out_kmalloc(sizeof(struct entry*) * hashmap->num_of_buckets);
    } else if(hash >= hashmap->num_of_buckets) {
        hashmap->num_of_buckets = round_up_to_nearest_n_power(hash+1u, 2u);
        hashmap->list_of_buckets = krealloc(hashmap->list_of_buckets, sizeof(struct entry*) * hashmap->num_of_buckets);
    }

    struct entry* *const first_entry_with_hash_ptr = &hashmap->list_of_buckets[hash];

    struct entry *const new_entry = zeroed_out_kmalloc(sizeof(struct entry));
    new_entry->key = key;
    new_entry->data = data;
    new_entry->next = NULL;

    if(*first_entry_with_hash_ptr == NULL) {
        new_entry->prev = NULL;

        *first_entry_with_hash_ptr = new_entry;
    } else {
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

    ++hashmap->num_of_entries;
}

void hashmap_destroy(struct hashmap *const hashmap) {
    for(uint32_t i = 0u; i < hashmap->num_of_buckets; ++i) {
        struct entry* current_entry = hashmap->list_of_buckets[i];
        while(current_entry != NULL) {
            struct entry *next_entry = current_entry->next;
            kfree(current_entry);
            current_entry = next_entry;
        }
    }
}
