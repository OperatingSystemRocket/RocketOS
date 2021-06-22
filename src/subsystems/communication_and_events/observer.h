#pragma once


#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


enum event_types {
    KEYBOARD_EVENT = 0u,
};



#define GENERATE_SUBJECT_DECLARATION(MAX_NUM_OF_OBSERVERS, TYPE_OF_DATA) \
struct TYPE_OF_DATA##_event { \
    size_t event_type; \
    struct TYPE_OF_DATA data; \
}; \
struct TYPE_OF_DATA##_observer { /*is just a special function object*/ \
    void* context; /*stores data*/ \
    void (*on_notify)(void* context, struct TYPE_OF_DATA##_event event); /*first param should be corresponding `context` member*/ \
}; \
struct subject_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS { \
    struct TYPE_OF_DATA##_observer* observers[MAX_NUM_OF_OBSERVERS]; \
    uint32_t num_of_active_observers; /*the whole buffer is not guarenteed to be filled*/ \
}; \
bool add_observer_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS(struct subject_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS* subject, struct TYPE_OF_DATA##_observer* observer); \
bool remove_observer_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS(struct subject_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS* subject, const struct TYPE_OF_DATA##_observer* observer); \
void notify_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS(struct subject_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS* subject, struct TYPE_OF_DATA##_event event);

#define GENERATE_SUBJECT_DEFINITION(MAX_NUM_OF_OBSERVERS, TYPE_OF_DATA) \
bool add_observer_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS(struct subject_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS *const subject, struct TYPE_OF_DATA##_observer *const observer) { \
    if(subject->num_of_active_observers == MAX_NUM_OF_OBSERVERS) { \
        return false; /*can't add it since it is full*/ \
    } \
\
    subject->observers[subject->num_of_active_observers++] = observer; \
    return true; \
} \
bool remove_observer_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS(struct subject_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS *const subject, const struct TYPE_OF_DATA##_observer *const observer) { \
    for(int32_t i = 0; i < subject->num_of_active_observers; ++i) { \
        if(subject->observers[i] == observer) { \
            subject->observers[i] = subject->observers[--subject->num_of_active_observers]; \
            return true; \
        } \
    } \
\
    return false; /*`observer` wasn't in the `observers` array*/ \
} \
void notify_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS(struct subject_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS *const subject, const struct TYPE_OF_DATA##_event event) { \
    for(uint32_t i = 0; i < subject->num_of_active_observers; ++i) { \
        subject->observers[i]->on_notify(subject->observers[i]->context, event); \
    } \
}


#define GET_EVENT_TYPENAME(TYPE_OF_DATA) TYPE_OF_DATA##_event
#define GET_OBSERVER_TYPENAME(TYPE_OF_DATA) TYPE_OF_DATA##_observer
#define GET_SUBJECT_TYPENAME(MAX_NUM_OF_OBSERVERS, TYPE_OF_DATA) subject_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS

#define ADD_OBSERVER(MAX_NUM_OF_OBSERVERS, TYPE_OF_DATA, SUBJECT, OBSERVER) add_observer_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS(SUBJECT, OBSERVER)
#define REMOVE_OBSERVER(MAX_NUM_OF_OBSERVERS, TYPE_OF_DATA, SUBJECT, OBSERVER) remove_observer_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS(SUBJECT, OBSERVER)
#define NOTIFY(MAX_NUM_OF_OBSERVERS, TYPE_OF_DATA, SUBJECT, EVENT) notify_##TYPE_OF_DATA##_##MAX_NUM_OF_OBSERVERS(SUBJECT, EVENT)
