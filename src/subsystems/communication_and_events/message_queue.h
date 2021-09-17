#pragma once


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


#define GENERATE_QUEUE_DECLARATION(MAX_NUM_OF_ITEMS, TYPE_OF_DATA) \
struct queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS { \
    uint32_t read_index; \
    uint32_t write_index; \
    TYPE_OF_DATA items_buffer[MAX_NUM_OF_ITEMS]; \
}; \
struct return_type_##TYPE_OF_DATA { \
    TYPE_OF_DATA data; \
    bool succeeded; \
}; \
void queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS##_init(struct queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS* queue); \
bool queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS##_enqueue(struct queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS* queue, TYPE_OF_DATA new_item); \
struct return_type_##TYPE_OF_DATA queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS##_dequeue(struct queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS* queue); \
bool queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS##_is_empty(struct queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS* queue);

#define GENERATE_QUEUE_DEFINITION(MAX_NUM_OF_ITEMS, TYPE_OF_DATA) \
void queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS##_init(struct queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS *const queue) { \
    queue->read_index = 0u; \
    queue->write_index = 0u; \
} \
bool queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS##_enqueue(struct queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS *const queue, const TYPE_OF_DATA new_item) { \
    const uint32_t num_of_elements = queue->write_index - queue->read_index; \
\
    const size_t capacity = sizeof(queue->items_buffer)/sizeof(TYPE_OF_DATA); \
    if(capacity == num_of_elements) { \
        return false; /*failed, do nothing more*/ \
    } \
\
    /*the modulo is to make it wrap since this is a ring buffer*/ \
    const uint32_t index = queue->write_index++ % (capacity-1u); \
    queue->write_index %= (capacity-1u); /*to stop eventual integer overflow*/\
    queue->items_buffer[index] = new_item; \
    return true; /*success*/ \
} \
struct return_type_##TYPE_OF_DATA queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS##_dequeue(struct queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS *const queue) { \
    const uint32_t num_of_elements = queue->write_index - queue->read_index; \
    if(num_of_elements == 0u) { \
        return (struct return_type_##TYPE_OF_DATA){ .succeeded = false }; \
    } \
\
    const size_t capacity = sizeof(queue->items_buffer)/sizeof(TYPE_OF_DATA); \
    const uint32_t index = queue->read_index++ % (capacity-1u); \
    queue->read_index %= (capacity-1u); \
\
    return (struct return_type_##TYPE_OF_DATA){ .data = queue->items_buffer[index], .succeeded = true }; \
} \
bool queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS##_is_empty(struct queue_##TYPE_OF_DATA##_##MAX_NUM_OF_ITEMS *const queue) { \
    return queue->read_index == queue->write_index; \
}
