#include "string.h"


struct string string_new(const char* data) {
    return string_new_with_len(data, kstrlen(data));
}

struct string string_new_with_len(const char* data, size_t len) {
    kassert(data != NULL, ((struct string){ NULL, 0u }));

    char *const str = kmalloc(len + 1);

    if(str == NULL) return (struct string){ NULL, 0u };

    kmemcpy(str, data, len);
    str[len] = '\0';

    return (struct string){ str, len };
}

bool assign_string(struct string* str, const char* data) {
    return assign_string_with_len(str, data, kstrlen(data));
}

bool assign_string_with_len(struct string* str, const char* data, size_t len) {
    kassert(str != NULL, false);

    if(!destroy_string(str)) return false;

    struct string new_str = string_new_with_len(data, len);

    if(new_str.data == NULL) return false;

    *str = new_str;

    return true;
}

bool assign_string_from_string(struct string* str, const struct string* other) {
    return assign_string_with_len(str, other->data, other->length);
}

bool destroy_string(struct string* str) {
    kassert(str != NULL, false);

    kfree(str->data);
    str->data = NULL;
    str->length = 0u;

    return true;
}
