#include "bitset.h"


#define NUMBER_OF_BITS_IN_TYPE 8u


//use `void*` instead of `uint8_t` to allow implicit pointer conversions

bool at(const uint32_t index, const void *const bitset) {
    const uint8_t *const byte_bitset = bitset;

    return byte_bitset[index/NUMBER_OF_BITS_IN_TYPE] & (1u << (index%NUMBER_OF_BITS_IN_TYPE));
}

void set_at(const uint32_t index, void *const bitset, const bool value) {
    uint8_t *const byte_bitset = bitset;

    if(value) {
        byte_bitset[index/NUMBER_OF_BITS_IN_TYPE] |= (uint8_t)(1u << (index%NUMBER_OF_BITS_IN_TYPE));
    } else {
        byte_bitset[index/NUMBER_OF_BITS_IN_TYPE] &= (uint8_t)(0xFEu << (index%NUMBER_OF_BITS_IN_TYPE));
    }
}
