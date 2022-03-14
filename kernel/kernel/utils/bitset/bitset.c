#include <utils/bitset/bitset.h>


#define NUMBER_OF_BITS_IN_TYPE 8u


//use `void*` instead of `unsigned char` to allow implicit pointer conversions

bool bitset_at(const uint32_t index, const void *const bitset) {
    const unsigned char *const byte_bitset = bitset;

    return byte_bitset[index/NUMBER_OF_BITS_IN_TYPE] & (1u << (index%NUMBER_OF_BITS_IN_TYPE));
}

void bitset_set_at(const uint32_t index, void *const bitset, const bool value) {
    unsigned char *const byte_bitset = bitset;

    if(value) {
        byte_bitset[index/NUMBER_OF_BITS_IN_TYPE] |= (unsigned char)(1u << (index%NUMBER_OF_BITS_IN_TYPE));
    } else {
        byte_bitset[index/NUMBER_OF_BITS_IN_TYPE] &= (unsigned char)(0xFEu << (index%NUMBER_OF_BITS_IN_TYPE));
    }
}
