#include "kmath.h"

int8_t ksignbit(const int8_t val) {
    return (val >= 0) ? 0 : 1;
}

int64_t kmax(const int64_t x, const int64_t y) {
    return (x >= y) ? x : y;
}

uint32_t kabs(const int64_t val) {
    return (val >= 0) ? val : -val;
}

int8_t ksign(const int8_t val) {
    if(val == 0) return 0;
    return (kabs(val) > 0) ? 1 : -1;
}
