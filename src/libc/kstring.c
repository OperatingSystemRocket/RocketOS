#include "kstring.h"

void* kmemcpy(void* destination, const void* source, size_t num) {
    kassert(destination != NULL && source != NULL, 0);

    size_t len = 0;
    while(len < num) {
        *((char*)destination + len) = *((char*)source + len);
        len++;
    }
    return destination;
}

void* kmemmove(void* destination, const void* source, size_t num) {
    kassert(destination != NULL && source != NULL, 0);

    char temp[num];
    size_t len = 0;
    while(len < num) {
        *(temp + len) = *((char*)source + len);
        len++;
    }
    len = 0;
    while(len < num) {
        *((char*)destination + len) = *(temp + len);
        len++;
    }
    return destination;
}

void* kmemchr(void* ptr, int value, size_t num) {
    kassert(ptr != NULL, 0);

    size_t len = 0;
    while(len < num) {
        if(*((char*)ptr + len) = value) return ptr + len;
        len++;
    }
    return NULL;
}

int kmemcmp(const void* ptr1, const void* ptr2, size_t num) {
    kassert(ptr1 != NULL && ptr2 != NULL, 0);

    size_t len = 0;
    while(len < num) {
        if(*((unsigned char*)ptr1 + len) < *((unsigned char*)ptr2 + len)) return -1;
        if(*((unsigned char*)ptr1 + len) > *((unsigned char*)ptr2 + len)) return 1;
        len++;
    }
    return 0;
}

void* kmemset(void *const ptr, const int32_t value, size_t num) {
    char *const ptr_char = ptr; //so we can set each byte
    const int8_t value_byte = value & 0xFFu; //this is to only grab the lowest byte and discard the rest as we are writing to chars

    while(num--) {
        *ptr_char = value_byte;
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

char* kstrchr(char* str, int character) {
    kassert(str != NULL, 0);
    
    if((char)character == '\0') return str + kstrlen(str);

    size_t len = kstrlen(str);
    size_t index = 0;
    while(index < len) {
        if(str[index] == (char)character) return str + index;
        index++;
    }
    return NULL;
}

char* kstrrchr(char* str, int character) {
    kassert(str != NULL, 0);
    
    if((char)character == '\0') return str + kstrlen(str);

    size_t len = kstrlen(str);
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

int kstrcoll(const char* str1, const char* str2) {
    //TODO
}

char* kstrcpy(char* destination, const char* source) {
    kassert(destination != NULL && source != NULL, 0);

    size_t len = 0;
    while(source[len]) {
       destination[len] = source[len];
       len++;
    }
    destination[len] = '\0';
    return destination;
}

char* kstrncpy(char* destination, const char* source, size_t num) {
    kassert(destination != NULL && source != NULL, 0);

    size_t len = 0;
    size_t size = kstrlen(source);
    char* temp = destination;
    while(len < num) {
        if(len < size) {
            *(temp++) = source[len];
        } else {
            *(temp++) = '\0';
        }
        len++;
    }
    return destination;
}

char* kstrerror(int errnum) {
    //TODO
}

size_t kstrspn(const char* str1, const char* str2) {
    size_t cont = 0;
    for(size_t i = 0; i < kstrlen(str1); i++) {
        cont = 0;
        for(size_t j = 0; j < kstrlen(str2); j++) {
            if(str1[i] == str2[j]) {
                cont = 1;
                break;
            }
        }
        if(!cont) return i;
    }
    return kstrlen(str1);
}

size_t kstrcspn(const char* str1, const char* str2) {
    //TODO
}

const char* kstrpbrk(const char* str1, const char* str2) {
    //TODO
}

const char* kstrstr(const char* str1, const char* str2) {
    //TODO
}

char* kstrtok(char* str, const char* delimiters) {
    //TODO
}

size_t kstrxfrm(char* destination, const char* source, size_t num) {
    //TODO
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
        const size_t current_input = input % base;
        input /= base;
        char current_char;
        if(current_input >= 0u && current_input < 10u) {
            current_char = current_input + 48u;
        } else if(!lowercase) {
            current_char = current_input + 55u;
        } else {
            current_char = current_input + 87u;
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
