# 1 "src/kmath.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/kmath.c"
# 1 "src/kmath.h" 1
       

# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint.h" 1 3 4
# 11 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint.h" 3 4
# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint-gcc.h" 1 3 4
# 34 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint-gcc.h" 3 4

# 34 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint-gcc.h" 3 4
typedef signed char int8_t;


typedef short int int16_t;


typedef long int int32_t;


typedef long long int int64_t;


typedef unsigned char uint8_t;


typedef short unsigned int uint16_t;


typedef long unsigned int uint32_t;


typedef long long unsigned int uint64_t;




typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef long int int_least32_t;
typedef long long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef short unsigned int uint_least16_t;
typedef long unsigned int uint_least32_t;
typedef long long unsigned int uint_least64_t;



typedef int int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long long int int_fast64_t;
typedef unsigned int uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef long long unsigned int uint_fast64_t;




typedef long int intptr_t;


typedef long unsigned int uintptr_t;




typedef long long int intmax_t;
typedef long long unsigned int uintmax_t;
# 12 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint.h" 2 3 4
# 4 "src/kmath.h" 2


# 5 "src/kmath.h"
int8_t ksignbit(int8_t val);



int64_t kmax(int64_t x, int64_t y);

int8_t ksign(int8_t val);

uint32_t kabs(int64_t val);

int64_t kmin(int64_t x, int64_t y);

int64_t kmax(int64_t x, int64_t y);
# 2 "src/kmath.c" 2

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

int64_t min(int64_t x, int64_t y) {
    return (x < y) ? x : y;
}

int64_t max(int64_t x, int64_t y) {
    return (x > y) ? x : y;
}
