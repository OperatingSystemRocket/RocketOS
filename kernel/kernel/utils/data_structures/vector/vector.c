#include "vector.h"


struct vector {
    void* d;
    size_t data_size;
    size_t capacity;
    size_t len;
};

struct vector* create_vector(const size_t data_size) {
    struct vector *const ret = kmalloc(sizeof(struct vector));
    if(ret == NULL) return NULL;

    *ret = (struct vector){ NULL, data_size, 0u, 0u };
    return ret;
}

struct vector* copy_vector(const struct vector* v) {
    kassert(v != NULL, NULL);

    struct vector *const ret = create_vector(v->data_size);
    if(ret == NULL) return NULL;

    ret->capacity = v->capacity;
    ret->len = v->len;

    ret->d = kmalloc(v->capacity * v->data_size);
    if(ret->d == NULL) {
        destroy_vector(ret);
        return NULL;
    }

    kmemcpy(ret->d, v->d, v->len * v->data_size);

    return ret;
}

bool push_back(struct vector *const v, const void *const data) {
    //kprintf("v: %p, data: %p\n", v, data);
    kassert(v != NULL && data != NULL, false);

    const unsigned char *const d = data;

    if(v->d == NULL) {
        kassert(v->capacity == 0u && v->len == 0u, false);

        void *const new_d = kmalloc(2 * v->data_size);
        if(new_d == NULL) return false;

        v->d = new_d;
        kmemcpy(v->d, d, v->data_size);
        v->capacity = 2;
        v->len = 1;
        return true;
    }

    kassert(v->len <= v->capacity, false);
    if(v->len < v->capacity) {
        kmemcpy((void*)(((uintptr_t)v->d) + (v->len++ * v->data_size)), data, v->data_size);
        return true;
    }
    kprintf("before krealloc\n");
    void *const new_d = krealloc(v->d, 2 * v->len * v->data_size);
    if(new_d == NULL) return false;

    v->d = new_d;
    kmemcpy((void*)(((uintptr_t)v->d) + (v->len++ * v->data_size)), data, v->data_size);
    v->capacity *= 2;

    return true;
}

void* at(struct vector *const v, const size_t index) {
    kassert(v != NULL && index < v->len, NULL);

    return (void*)(((uintptr_t)v->d) + (index * v->data_size));
}

const void* at_const(const struct vector *const v, const size_t index) {
    kprintf("v: %p, index: %u\n", v, index);
    kassert(v != NULL && index < v->len, NULL);

    return (void*)(((uintptr_t)v->d) + (index * v->data_size));
}

size_t size(const struct vector* v) {
    kassert(v != NULL, 0u);

    //kprintf("size(): v->len: %u\n", v->len);

    return v->len;
}

bool erase(struct vector *const v, const size_t index) {
    kassert(v != NULL && v->d != NULL && v->capacity <= v->len && index < v->len, false);

    for(size_t i = index; i < v->len - 1; ++i) {
        kmemcpy((void*)(((uintptr_t)v->d) + (i * v->data_size)), (const void*)(((uintptr_t)v->d) + ((i+1) * v->data_size)), v->data_size);
    }
    --v->len;

    return true;
}

bool clear(struct vector *const v) {
    kassert(v != NULL, false);

    if(v->d == NULL) {
        kassert(v->capacity == 0 && v->len == 0, false);
        return true;
    }

    v->len = 0;

    return true;
}

bool destructive_clear(struct vector *const v) {
    kassert(v != NULL, false);

    kfree(v->d);
    v->d = NULL;
    v->capacity = 0;
    v->len = 0;

    return true;
}

bool destroy_vector(struct vector *const v) {
    kassert(v != NULL, false);

    kfree(v->d);

    kfree(v);

    return true;
}
