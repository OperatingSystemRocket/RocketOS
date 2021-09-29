#include "default_hashmap_functions.h"


uint32_t str_hash_function(const char *const str_to_hash) {
    uint32_t hash = 0u;

    const size_t str_len = kstrlen(str_to_hash);
    for(uint32_t i = 0u; i < str_len; ++i) {
        hash ^= (uint32_t)str_to_hash[i];
    }

    return hash;
}

bool str_comp(const char *const str1, const char *const str2) {
    return kstrcmp(str1, str2) == 0;
}
