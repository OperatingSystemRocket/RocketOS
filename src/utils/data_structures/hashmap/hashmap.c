#include "hashmap.h"


#define RESIZE_FACTOR 2
#define LOAD_FACTOR 2 //entries:buckets ratio of when to reallocate the entries array for more buckets
#define STARTING_NUM_OF_BUCKETS 2 //256


void hashmap_init(struct hashmap *const hashmap, uint32_t (*const hash_function)(TYPE_OF_KEY), bool (*const comp)(TYPE_OF_KEY, TYPE_OF_KEY)) {
    hashmap->num_of_entries = 0u;
    hashmap->num_of_buckets = STARTING_NUM_OF_BUCKETS;
    hashmap->list_of_buckets = zeroed_out_kmalloc(sizeof(struct entry*) * hashmap->num_of_buckets);
    hashmap->hash_function = hash_function;
    hashmap->comp = comp;
}

static uint32_t get_hash(const struct hashmap *const hashmap, const TYPE_OF_KEY key) {
    const uint32_t hash = hashmap->hash_function(key);
    //equivalent to `return hash % hashmap->num_of_buckets;`
    kprintf("hashmap->num_of_buckets: %u, clamped hash: %u\n", hashmap->num_of_buckets, (hash & (hashmap->num_of_buckets - 1u)));
    return hash & (hashmap->num_of_buckets - 1u);
}

static void resize_hashmap(struct hashmap *const hashmap) {
    kprintf("\n\nresize_hashmap start\n");

    struct hashmap new_hashmap = *hashmap;

    new_hashmap.num_of_buckets *= RESIZE_FACTOR;
    new_hashmap.list_of_buckets = zeroed_out_kmalloc(sizeof(struct entry*) * new_hashmap.num_of_buckets);

    size_t num_of_entries_transferred = 0u;
    for(size_t i = 0u; i < hashmap->num_of_buckets; ++i) {
        struct entry** first_entry_with_hash_ptr = &hashmap->list_of_buckets[i];
        while((*first_entry_with_hash_ptr) != NULL) {
            struct entry* first_entry_with_hash = *first_entry_with_hash_ptr;

            if(first_entry_with_hash->next != NULL) {
                first_entry_with_hash->next->prev = NULL;
            }
            *first_entry_with_hash_ptr = first_entry_with_hash->next;

            const uint32_t new_hash = get_hash(&new_hashmap, first_entry_with_hash->key);
            if(new_hashmap.list_of_buckets[new_hash] == NULL) {
                kprintf("new_hashmap.list_of_buckets[new_hash] == NULL\n");

                new_hashmap.list_of_buckets[new_hash] = first_entry_with_hash;
                first_entry_with_hash->next = NULL;
                first_entry_with_hash->prev = NULL;
            } else {
                kprintf("new_hashmap.list_of_buckets[new_hash] != NULL\n");

                struct entry* current_entry = new_hashmap.list_of_buckets[new_hash];
                while(current_entry->next != NULL) {
                    current_entry = current_entry->next;
                }

                current_entry->next = first_entry_with_hash;
                first_entry_with_hash->prev = current_entry;
                first_entry_with_hash->next = NULL;
            }
            ++num_of_entries_transferred;
        }
    }

    kfree(hashmap->list_of_buckets);

    //kassert_void(hashmap->num_of_entries == new_hashmap.num_of_entries);
    kprintf("num_of_entries_transferred: %u, hashmap->num_of_entries: %u\n", num_of_entries_transferred, hashmap->num_of_entries);
    kassert_void(num_of_entries_transferred == hashmap->num_of_entries);
    kassert_void(num_of_entries_transferred == new_hashmap.num_of_entries);

    *hashmap = new_hashmap;

    kprintf("resize_hashmap end\n\n");
}

static struct entry* hashmap_find_entry(struct hashmap *const hashmap, const TYPE_OF_KEY key) {
    kprintf("hashmap_find_entry start\n");

    const uint32_t hash = get_hash(hashmap, key);

    struct entry *const first_entry_with_hash = hashmap->list_of_buckets[hash];
    if(first_entry_with_hash == NULL) {
        kprintf("\n\nfirst_entry_with_hash == NULL\n");
        kprintf("hashmap_find_entry end\n");

        return NULL;
    }

    if(first_entry_with_hash->next == NULL) {
        if(hashmap->comp(first_entry_with_hash->key, key)) {
            kprintf("\n\nkeys match\n");
            kprintf("hashmap_find_entry end\n");
    
            return first_entry_with_hash;
        } else {
            kprintf("\n\nkeys don't match\n");
            kprintf("hashmap_find_entry end\n");
    
            return NULL;
        }
    } else {
        struct entry* current_entry = first_entry_with_hash;
        while(current_entry != NULL) {
            if(hashmap->comp(current_entry->key, key)) {
                kprintf("\n\nkeys match in while loop\n");
                kprintf("hashmap_find_entry end\n");
                kprintf("current_entry->key: %s, current_entry->data: %u\n", current_entry->key, current_entry->data);

                return current_entry;
            }

            current_entry = current_entry->next;
        };

        kprintf("\n\nkeys don't match in while loop\n");
        kprintf("hashmap_find_entry end\n");

        return NULL;
    }
}

TYPE_OF_DATA* hashmap_find(struct hashmap *const hashmap, const TYPE_OF_KEY key) {
    struct entry *const entry = hashmap_find_entry(hashmap, key);

    if(entry == NULL) {
        kprintf("\n\nentry == NULL\n\n\n");
        return NULL;
    }

    return &entry->data;
}

struct return_type hashmap_remove(struct hashmap *const hashmap, const TYPE_OF_KEY key) {
    kprintf("hashmap_remove start\n");

    struct entry *const entry = hashmap_find_entry(hashmap, key);

    if(entry == NULL) {
        return (struct return_type){ .succeeded = false };
    }

    const struct return_type ret = { .data = entry->data, .succeeded = true };
    if(entry->prev != NULL) {
        entry->prev = entry->next;
    } else {
        //it is the first value with that hash in the bucket
        const uint32_t hash = get_hash(hashmap, key);
        hashmap->list_of_buckets[hash] = entry->next;
    }

    if(entry->next != NULL) {
        entry->next->prev = entry->prev;
    }
    kfree(entry);

    --hashmap->num_of_entries;

    kprintf("hashmap_remove end\n");

    return ret;
}

void hashmap_add(struct hashmap *const hashmap, const TYPE_OF_KEY key, const TYPE_OF_DATA data) {
    kprintf("\n\nhashmap_add start\n");

    if((hashmap->num_of_entries/hashmap->num_of_buckets) >= LOAD_FACTOR) {
        resize_hashmap(hashmap);
    }

    const uint32_t hash = get_hash(hashmap, key);

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

    kprintf("HASHMAP ADD\n");
    kprintf("hashmap_add end\n\n");
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
