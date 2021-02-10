#include "cstring.h"

size_t cstrlen(const char *const str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

int cstrcmp(const char *lhs, const char *rhs) {
    //@todo implement
}
