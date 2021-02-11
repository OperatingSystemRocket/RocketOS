#include "cstring.h"

size_t cstrlen(const char *const str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

int cstrcmp(const char *lhs, const char *rhs) {
	size_t index = 0;
	while (lhs[index]) {
		if (lhs[index] == rhs[index]) {
			index++;
		} else if (lhs[index] < rhs[index]) {
			return -1;
		} else {
			return 1;
		}
	}
}
