#include "kstring.h"

void* kmemcpy(void *restrict const destination, const void *restrict const source, const size_t num) {
    kassert(destination != NULL && source != NULL, NULL); //if this is tripped, the user has violated the contract of `restrict` and invoked UB

    for(size_t len = 0u; len < num; ++len) {
        ((char*)destination)[len] = ((const char*)source)[len];
    }
    return destination;
}

void* kmemmove(void *const destination, const void *const source, const size_t num) {
    kassert(destination != NULL && source != NULL, NULL);

    char temp[num]; //TODO: Get rid of this disgusting VLA

    for(size_t len = 0u; len < num; ++len) {
        temp[len] = ((const char*)source)[len];
    }

    for(size_t len = 0u; len < num; ++len) {
        ((char*)destination)[len] = temp[len];
    }

    return destination;
}

void* kmemchr(void *const ptr, const int32_t value, const size_t num) {
    kassert(ptr != NULL, 0);

    for(size_t len = 0u; len < num; ++len) {
        if(((char*)ptr)[len] == value) return ((char*) ptr) + len;
    }
    return NULL;
}

int32_t kmemcmp(const void *const ptr1, const void *const ptr2, const size_t num) {
    kassert(ptr1 != NULL && ptr2 != NULL, 0);

    for(size_t len = 0u; len < num; ++len) {
        if(((const unsigned char*)ptr1)[len] < ((const unsigned char*)ptr2)[len]) return -1;
        if(((const unsigned char*)ptr1)[len] > ((const unsigned char*)ptr2)[len]) return 1;
    }

    return 0;
}

void* kmemset(void *const ptr, const int32_t value, const size_t num) {
    char *const ptr_char = ptr; //so we can set each byte
    const int8_t value_byte = (int8_t)(((uint32_t )value) & 0xFFu); //this is to only grab the lowest byte and discard the rest as we are writing to chars

    for(size_t i = 0u; i < num; ++i) {
        ptr_char[i] = value_byte;
    }

    return ptr;
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

char* kstrncat(char *const destination, const char* source, size_t num) {
    kassert(destination != NULL && source != NULL, 0);
    
    char* ptr = destination + kstrlen(destination);
    while(*source != '\0' && num > 0) {
        *ptr = *source;
        ptr++;
        source++;
        num--;
    }

    *ptr = '\0';

    return destination;
}

char* kstrchr(char* str, int32_t character) {
    kassert(str != NULL, 0);

    const size_t len = kstrlen(str);

    if((char)character == '\0') return str + len;

    for(size_t index = 0u; index < len; ++index) {
        if(str[index] == (char)character) return str + index;
    }

    return NULL;
}

char* kstrrchr(char* str, int character) {
    kassert(str != NULL, 0);

    size_t len = kstrlen(str);

    if((char)character == '\0') return str + len;

    while(len--) {
        if(str[len] == (char)character) return str + len;
    }
    return NULL;
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

char* kstrcpy(char* destination, const char* source) {
    kassert(destination != NULL && source != NULL, NULL);

    size_t len = 0;
    for(; source[len]; ++len) {
        destination[len] = source[len];
    }
    destination[len] = '\0';
    return destination;
}

char* kstrncpy(char* destination, const char* source, size_t num) {
    kassert(destination != NULL && source != NULL, 0);

    size_t size = kstrlen(source);
    char* temp = destination;
    for(size_t len = 0u; len < num; ++len) {
        if(len < size) {
            *(temp++) = source[len];
        } else {
            *(temp++) = '\0';
        }
    }
    return destination;
}

size_t kstrspn(const char *const str1, const char *const str2) {
    bool cont = 0;
    for(size_t i = 0; i < kstrlen(str1); i++) {
        cont = false;
        for(size_t j = 0; j < kstrlen(str2); j++) {
            if(str1[i] == str2[j]) {
                cont = true;
                break;
            }
        }
        if(!cont) return i;
    }
    return kstrlen(str1);
}

static const char* kstrstr_impl(const char* haystack, const char* needle, uint8_t mode) {
    size_t needle_len = kstrlen(needle);
    for(size_t i = 0; i < kstrlen(haystack); ++i) {
        if(haystack[i] == needle[0]) {
            if(kstrncmp(haystack+i, needle, needle_len) == 0) {
                return haystack + i + (mode == 0 ? 0 : needle_len);
            }
        }
    }
    return NULL;
}

const char* kstrstr(const char* haystack, const char* needle) {
    return kstrstr_impl(haystack, needle, 0);
}

size_t kstrlen(const char *const str) {
    kassert(str != NULL, 0);

    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

char* kint_to_string(int64_t input, char *const string_ret, const size_t ret_size, const uint32_t base, const bool lowercase) {
    size_t index = 0u;
    if(0u < ret_size && input < 0u) {
        string_ret[index++] = '-';
        input *= -1;
    }
    do {
        const size_t current_input = (size_t) (((uint64_t) input) % base);
        input /= base;
        char current_char;
        if(current_input < 10u) {
            current_char = (char) (current_input + 48u);
        } else if(!lowercase) {
            current_char = (char) (current_input + 55u);
        } else {
            current_char = (char) (current_input + 87u);
        }
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

void kuint_to_string(uint64_t input, char* const string_ret, const size_t ret_size) {
    size_t index = 0u;
    do {
        size_t current_input = (size_t) (input % 10u);
        input /= 10u;
        const char current_char = (char) (current_input + 48u);
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
}

char kint_to_char(const int8_t i) {
    return '0' + i;
}

int8_t kchar_to_int(const char c) {
    return c-'0';
}

///NOTE: nonstandard, but useful. Works like strstr, but returns a pointer to the END of needle found within haystack
const char* kstrstr_end(const char* haystack, const char* needle) {
    return kstrstr_impl(haystack, needle, 1);
}

