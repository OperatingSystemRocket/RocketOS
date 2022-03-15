#include "strlen.h"

size_t kstrlen(const char *const str) {
    size_t len = 0;
    while (str != NULL && str[len]) {
        len++;
    }
    return len;
}
