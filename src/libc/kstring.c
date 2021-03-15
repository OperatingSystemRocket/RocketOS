#include "kstring.h"

size_t kstrlen(const char *const str) {
    kassert(str != NULL, 0);

    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

int32_t kstrcmp(const char *const lhs, const char *const rhs) {
    kassert(lhs != NULL && rhs != NULL, 0);

	size_t index = 0u;
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

int32_t kstrncmp(const char *const lhs, const char *const rhs, const size_t sz) {
    kassert(lhs != NULL && rhs != NULL, 0);

	size_t index = 0u;
	while (index < sz) {
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

char* kstrcat(char *const destination, const char* source) {
    kassert(destination != NULL && source != NULL, 0);
    
    char* ptr = destination + kstrlen(destination);
    while(*source != '\0') {
        *ptr = *source;
        ptr++;
        source++;
    }

    *ptr = '\0';

    return destination;
}

char* kint_to_string(int64_t input, char *const string_ret, const size_t ret_size) {
    size_t index = 0u;
    if(0u < ret_size && input < 0u) {
        string_ret[index++] = '-';
        input *= -1;
    }
    do {
        size_t current_input = input % 10u;
        input /= 10u;
        const char current_char = current_input + 48u;
        if(index < ret_size) {
            string_ret[index++] = current_char;
        }
    } while(input);
    if(index < ret_size) {
        string_ret[index] = '\0';
    }
    const size_t start_index = (string_ret[0u] == '-') ? 1u : 0u;
    for(size_t i = 0u; i < index/2u; ++i) {
        const char temp = string_ret[i+start_index];
        string_ret[i+start_index] = string_ret[index-i-1u];
        string_ret[index-i-1u] = temp;
    }

    return string_ret;
}

void* kmemset(void *const ptr, const int32_t value, size_t num) {
    char *const ptr_char = ptr; //so we can set each byte
    const int8_t value_byte = value & 0xFFu; //this is to only grab the lowest byte and discard the rest as we are writing to chars

    while(num--) {
        *ptr_char = value_byte;
    }

    return ptr;
}
