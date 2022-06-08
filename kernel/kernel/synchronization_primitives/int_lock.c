#include "int_lock.h"


void disable_hwint(size_t* rflags);
void enable_hwint(size_t* rflags);


void int_lock(struct int_lock* lock);
void int_unlock(struct int_lock* lock);

