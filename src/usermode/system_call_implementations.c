#include "system_call_implementations.h"


void print(const char *const str) {
    terminal_context_writestring(str);
}
