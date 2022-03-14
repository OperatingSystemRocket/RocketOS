#include <synchronization_primitives/spinlock.h>

void acquire_spinlock(mutex_t *const mutex) {
    bool expected = false;
    while(!__atomic_compare_exchange_n(mutex, &expected, true, true, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED)) {
        __atomic_clear(&expected, __ATOMIC_RELAXED);
        asm volatile("pause");
    }
}

void release_spinlock(mutex_t *const mutex) {
    __atomic_clear(mutex, __ATOMIC_RELEASE);
}
