#pragma once


#include <stdbool.h>
#include <stdint.h>

#include "hardware_io.h"


int32_t serial_received();

char read_serial();

int32_t is_transmit_empty();

void write_serial(char a);

//returns whether or not the serial is faulty
bool serial_init(void);
