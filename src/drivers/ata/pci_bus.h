#pragma once

#include <stdint.h>
#include <stddef.h>

#include "hardware_io.h"


uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

