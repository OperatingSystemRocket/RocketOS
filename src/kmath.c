#include "kmath.h"

int8_t ksignbit(int8_t val) {
    int8_t bit = val & 0b10000000;
    bit = bit >> 7;
    bit = bit & 0b00000001;

    return bit;
}

int64_t max(int64_t x, int64_t y)
{
    return x ^ ((x ^ y) & -(x < y));
}
/*


int8_t algebraic_sign(int8_t val) {
    int8_t bit = val & 0b10000000;
    bit = bit >> 7;
    bit = bit & 0b00000001;
    bit = -1*(bit * 2 - 1) * kabs(val) / max(1, kabs(val));

    return bit;

}
 */

uint32_t kabs(int64_t val) {

    //return val * algebraic_sign(val);

    int8_t bit = val & 0b10000000;
    bit = bit >> 7;
    bit = bit & 0b00000001;
    bit = -1*(bit * 2 - 1) * val / max(1, kabs(val));

    return bit;
}

int8_t algebraic_sign(int8_t val) {
    return kabs(val) / max(1,kabs(val)); // I really didn't want to define algbraic sign in terms of abs, but it makes it a lot easier
}

