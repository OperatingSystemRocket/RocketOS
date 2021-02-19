#include "cstring.h"

size_t cstrlen(const char *const str) {
    cassert(str != NULL, 0);

    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

int cstrcmp(const char *const lhs, const char *const rhs) {
    cassert(lhs != NULL && rhs != NULL, 0);

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

void cint_to_string(ssize_t input, char *const string_ret, const size_t ret_size) {
    size_t index = 0u;
    if(0u < ret_size && input < 0u) {
        string_ret[index++] = '-';
        input *= -1;
    }
    while(input) {
        size_t current_input = input % 10u;
        input /= 10u;
        const char current_char = current_input + 48u;
        if(index < ret_size) {
            string_ret[index++] = current_char;
        }
    }
    if(index < ret_size) {
        string_ret[index] = '\0';
    }
    const size_t start_index = (string_ret[0u] == '-') ? 1u : 0u;
    for(size_t i = 0u; i < index/2u; ++i) {
        const char temp = string_ret[i+start_index];
        string_ret[i+start_index] = string_ret[index-i-1u];
        string_ret[index-i-1u] = temp;
    }
}
