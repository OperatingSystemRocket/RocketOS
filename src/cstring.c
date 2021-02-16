#include "cstring.h"

size_t cstrlen(const char *const str) {
    assert(str != NULL, 0);

    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

int cstrcmp(const char *const lhs, const char *const rhs) {
    assert(lhs != NULL && rhs != NULL, 0);

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
	return 0;
}
