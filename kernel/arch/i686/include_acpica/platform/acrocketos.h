#pragma once

#define ACPI_MACHINE_WIDTH 32



#ifndef ACPI_DIV_64_BY_32
#define ACPI_DIV_64_BY_32(n_hi, n_lo, d32, q32, r32) \
    do { \
        UINT64 (__n) = ((UINT64) n_hi) << 32 | (n_lo); \
        (r32) = ((__n) / (d32)); \
        (q32) = (UINT32) (__n); \
    } while (0)
#endif

#ifndef ACPI_SHIFT_RIGHT_64
#define ACPI_SHIFT_RIGHT_64(n_hi, n_lo) \
    do { \
        (n_lo) >>= 1; \
        (n_lo) |= (((n_hi) & 1) << 31); \
        (n_hi) >>= 1; \
    } while (0)
#endif

#define COMPILER_DEPENDENT_UINT64   unsigned long long


//tell ACPICA to use its internal cache
#define ACPI_CACHE_T                ACPI_MEMORY_LIST
#define ACPI_USE_LOCAL_CACHE        1

//TODO: support multithreading for ACPICA in the future
#define ACPI_SINGLE_THREADED
