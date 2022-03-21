#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef volatile bool mutex_t;

void acquire_spinlock(mutex_t* mutex);
void release_spinlock(mutex_t* mutex);
