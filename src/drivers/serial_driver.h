#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "kstring.h"
#include "hardware_io.h"


//returns whether or not the serial is faulty. true = serial works properly. false = serial is faulty.
bool serial_init(void);

int8_t serial_received(void);

char serial_read(void);

int8_t is_transmit_empty(void);

void serial_write(const char* text, size_t size);

void serial_writestring(const char* text);

void serial_putchar(char a);
