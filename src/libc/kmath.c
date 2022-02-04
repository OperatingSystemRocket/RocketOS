#include "kmath.h"

int8_t ksignbit(const int8_t val) {
    return !(val >= 0);
}

uint32_t kabs(const int32_t val) {
    return (uint32_t) ((val >= 0) ? val : -val);
}

int8_t ksign(const int8_t val) {
    if(val == 0) return 0;
    return (kabs(val) > 0) ? 1 : -1;
}

int64_t kmin(const int64_t x, const int64_t y) {
    return (x < y) ? x : y;
}

int64_t kmax(const int64_t x, const int64_t y) {
    return (x > y) ? x : y;
}

int64_t kmod(int64_t x, int64_t y) {
    return x - (y * (x/y));
}

int64_t kpow(int64_t base, int64_t exponent) {
    int64_t result = 1;
    for(int64_t i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}

uint64_t kpow_u(uint64_t base, uint64_t exponent) {
    uint64_t result = 1;
    for(uint64_t i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}
