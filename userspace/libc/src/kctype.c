#include "kctype.h"


int32_t kisalnum(const int32_t ch) {
    return kisalpha(ch) || kisdigit(ch);
}

int32_t kisalpha(const int32_t ch) {
    return kisupper(ch) || kislower(ch);
}

int32_t kislower(const int32_t ch) {
    return 'a' <= ch && ch <= 'z';
}

int32_t kisupper(const int32_t ch) {
    return 'A' <= ch && ch <= 'Z';
}

int32_t kisdigit(const int32_t ch) {
    return '0' <= ch && ch <= '9';
}

int32_t kisxdigit(const int32_t ch) {
    return kisdigit(ch) || ('a' <= ch && ch <= 'f') || ('A' <= ch && ch <= 'F');
}

int32_t kiscntrl(const int32_t ch) {
    return (0 <= ch && ch <= 31) || (ch == 127);
}

int32_t kisgraph(const int32_t ch) {
    return 33 <= ch && ch <= 126;
}

int32_t kisspace(const int32_t ch) {
    return (9 <= ch && ch <= 13) || (ch == 32);
}

int32_t kisblank(const int32_t ch) {
    return ch == 9 || ch == 32;
}

int32_t kisprint(const int32_t ch) {
    return ch == ' ' || kisgraph(ch);
}

int32_t kispunct(const int32_t ch) {
    return (33 <= ch && ch <= 47) || (58 <= ch && ch <= 64) || (91 <= ch && ch <= 96) || (123 <= ch && ch <= 126);
}

int32_t ktolower(const int32_t ch) {
    if(kisupper(ch)) {
        return ch - 32; 
    }
    return ch;
}

int32_t ktoupper(const int32_t ch) {
    if(kislower(ch)) {
        return ch + 32;
    }
    return ch;
}
