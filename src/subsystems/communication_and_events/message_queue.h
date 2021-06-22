#pragma once


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


#define GENERATE_QUEUE_DECLARATION(NAME, ITEM_TYPE, NUM_ITEMS) \
struct NAME { \
    uint32_t read_index; \
    uint32_t write_index; \
    ITEM_TYPE items_buffer[NUM_ITEMS]; \
}; \
struct NAME##_return_type { \
    struct NAME ITEM_TYPE data; \
    bool succeeded; \
}; \
void NAME##_init(struct NAME* queue); \
bool NAME##_enqueue(struct NAME* queue, ITEM_TYPE new_item); \
struct NAME##_return_type NAME##_dequeue(struct NAME* queue); \
bool NAME##_is_empty(struct NAME* queue);

#define GENERATE_QUEUE_DEFINITION(NAME, ITEM_TYPE) \
void NAME##_init(struct NAME *const queue) { \
    queue->read_index = 0u; \
    queue->write_index = 0u; \
} \
bool NAME##_enqueue(struct NAME *const queue, const ITEM_TYPE new_item) { \
    const uint32_t num_of_elements = queue->write_index - queue->read_index; \
\
    const size_t capacity = sizeof(queue->items_buffer)/sizeof(ITEM_TYPE); \
    if(capacity == num_of_elements) { \
        return false; /*failed, do nothing more*/ \
    } \
\
    /*the modulo is to make it wrap since this is a ring buffer*/ \
    const uint32_t index = queue->write_index++ % (capacity-1u); \
    queue->write_index %= (capacity-1u); /*to stop eventual integer overflow*/\
    queue->items_buffer[i] = new_item; \
    return true; /*success*/ \
} \
struct NAME##_return_type NAME##_dequeue(struct NAME *const queue) { \
    const uint32_t num_of_elements = queue->write_index - queue->read_index; \
    if(num_of_elements == 0u) { \
        return struct NAME##_return_type{ .succeeded = false; }; \
    } \
\
    const size_t capacity = sizeof(queue->items_buffer)/sizeof(ITEM_TYPE); \
    const uint32_t index = queue->read_index++ % (capacity-1u); \
    queue->read_index %= (capacity-1u); \
\
    return NAME##_return_type{ .data = queue->items_buffer[index], .succeeded = true; }; \
} \
bool NAME##_is_empty(struct NAME *const queue) { \
    return queue->read_index == queue->write_index; \
}

