#pragma once

#include <stdbool.h>

#define GENERATE_OPTIONAL(T) \
struct optional_##T { \
    T data; \
    bool has_value; \
};

#define OPTIONAL_NAME(T) optional_##T

