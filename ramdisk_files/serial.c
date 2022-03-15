#include "serial.h"

uint8_t is_transmit_empty(void);

void serial_putchar(char c);

void serial_write(const char* text, size_t size);

void serial_writestring(const char* text);
