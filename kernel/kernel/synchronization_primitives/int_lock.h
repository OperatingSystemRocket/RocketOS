#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kstdio.h>


inline void disable_hwint(size_t *const rflags) {
    asm volatile("pushf; pop %0; cli;" : "=g" (*rflags));
}

inline void enable_hwint(size_t *const rflags) {
    asm volatile("push %0; popf; " :: "g" (*rflags));
}

// interrupt lock, disables and enables interrupts, does *not* do memory synchronization
struct int_lock {
    size_t rflags;
};

inline void int_lock(struct int_lock *const lock) {
    disable_hwint(&lock->rflags);
}
inline void int_unlock(struct int_lock *const lock) {
    enable_hwint(&lock->rflags);
}

