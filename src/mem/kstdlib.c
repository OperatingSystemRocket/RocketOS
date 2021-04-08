#include "kstdlib.h"


#define generate_doubly_linked_free_list(T) \
struct doubly_linked_free_list##_T { \
    struct doubly_linked_free_list##_T* next; \
    struct doubly_linked_free_list##_T* prev; \
\
    T data; \
};

#define get_typename(Container, T) struct Container##_T


generate_doubly_linked_free_list(size_t);
get_typename(doubly_linked_free_list, size_t)* free_list;


void dynamic_memory_init(void) {
    
}

void* malloc(const size_t size) {
    
}

