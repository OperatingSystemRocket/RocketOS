#include "bitset.h"

bool at(const uint32_t index, const uint32_t *const bitset) {
    return bitset[index/32u] & (1u << (index%32u));
}

void set_at(const uint32_t index, uint32_t *const bitset, const bool value) {
    if(value) {
        bitset[index/32u] |= 1u << (index%32u);
    } else {
        bitset[index/32u] &= 0xFFFFFFFE << (index%32u);
    }
}
