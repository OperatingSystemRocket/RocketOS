#include "kstring.h"

void* kmemcpy(void *restrict const destination, const void *restrict const source, const size_t num) {
    kassert(destination != NULL && source != NULL, NULL); //if this is tripped, the user has violated the contract of `restrict` and invoked UB

    for(size_t len = 0u; len < num; ++len) {
        ((char*)destination)[len] = ((const char*)source)[len];
    }
    return destination;
}

void* kmemmove(void *const destination, const void *const source, size_t num) {
    kassert(destination != NULL && source != NULL, NULL);

    if(destination == source || num == 0u) return destination;

    char* dest = (char*)destination;
    const char* src = (const char*)source;

    if(dest < src) {
        while(num--) {
            *dest++ = *src++;
        }
    } else {
        src += num;
        dest += num;
        while(num--) {
            *--dest = *--src;
        }
    }

    return destination;
}

void* kmemchr(void *const ptr, const int32_t value, const size_t num) {
    kassert(ptr != NULL, NULL);

    char *const ptr_char = (char*)ptr;

    for(size_t len = 0u; len < num; ++len) {
        if(ptr_char[len] == value) return ptr_char + len;
    }
    return NULL;
}

int32_t kmemcmp(const void *const ptr1, const void *const ptr2, const size_t num) {
    kassert(ptr1 != NULL && ptr2 != NULL, -2);

    for(size_t len = 0u; len < num; ++len) {
        if(((const unsigned char*)ptr1)[len] < ((const unsigned char*)ptr2)[len]) return -1;
        if(((const unsigned char*)ptr1)[len] > ((const unsigned char*)ptr2)[len]) return 1;
    }

    return 0;
}

void* kmemset(void *const ptr, const uint32_t value, const size_t num) {
    kassert(ptr != NULL, NULL);

    char *const ptr_char = ptr; //so we can set each byte
    const int8_t value_byte = (int8_t)(((uint32_t)value) & 0xFFu); //this is to only grab the lowest byte and discard the rest as we are writing to chars

    for(size_t i = 0u; i < num; ++i) {
        ptr_char[i] = value_byte;
    }

    return ptr;
}

char* kstrcat(char *const destination, const char* source) {
    kassert(destination != NULL && source != NULL, NULL);

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
    kassert(destination != NULL && source != NULL, NULL);

    const size_t length_of_str = kstrlen(destination);
    num -= length_of_str + 1u; //+1 for the null terminator

    char* ptr = destination + length_of_str;
    while(*source != '\0' && num > 0) {
        *ptr = *source;
        ptr++;
        source++;
        num--;
    }

    *ptr = '\0';

    return destination;
}

char* kstrchr(char *const str, const int32_t character) {
    kassert(str != NULL, NULL);

    const size_t len = kstrlen(str);

    if((char)character == '\0') return str + len;

    for(size_t index = 0u; index < len; ++index) {
        if(str[index] == (char)character) return str + index;
    }

    return NULL;
}

char* kstrrchr(char *const str, const int32_t character) {
    kassert(str != NULL, NULL);

    size_t len = kstrlen(str);

    if((char)character == '\0') return str + len;

    while(len--) {
        if(str[len] == (char)character) return str + len;
    }
    return NULL;
}

int32_t kstrcmp(const char *const lhs, const char *const rhs) {
    kassert(lhs != NULL && rhs != NULL, -2);

    size_t i = 0u;
    for(; lhs[i] && rhs[i] && lhs[i] == rhs[i]; ++i);

    if(lhs[i] < rhs[i]) {
        return -1;
    } else if(lhs[i] > rhs[i]) {
        return 1;
    }
    return 0;
}

int32_t kstrncmp(const char *const lhs, const char *const rhs, const size_t sz) {
    kassert(lhs != NULL && rhs != NULL, -2);

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

char* kstrncpy(char *const destination, const char *const source, size_t num) {
    kassert(destination != NULL && source != NULL, NULL);

    if(num == 0u) return destination;

    --num;

    const size_t size = kstrlen(source);
    size_t len = 0u;
    for(; len < num && len < size; ++len) {
        destination[len] = source[len];
    }
    destination[len] = '\0';

    return destination;
}

int32_t kstrspn(const char *const str1, const char *const str2) {
    kassert(str1 != NULL && str2 != NULL, -1);

    for(int32_t i = 0; i < (int32_t)kstrlen(str1); ++i) {
        bool cont = false;
        for(int32_t j = 0; j < (int32_t)kstrlen(str2); ++j) {
            if(str1[i] == str2[j]) {
                cont = true;
                break;
            }
        }
        if(!cont) return i;
    }
    return (int32_t)kstrlen(str1);
}

size_t kstrlen(const char *const str) {
    kassert(str != NULL, 0u);

    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

char* kint_to_string(int64_t input, char *const string_ret, const size_t ret_size, const uint32_t base, const bool lowercase) {
    kassert(string_ret != NULL, NULL);

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

char kint_to_char(const int8_t i) {
    kassert(i >= 0 && i <= 9, '\0');

    return '0' + i;
}

int8_t kchar_to_int(const char c) {
    return c-'0';
}
