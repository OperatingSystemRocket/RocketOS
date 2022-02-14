#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "hardware_io.h"


uint32_t pci_config_read_long(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
uint8_t pci_config_read_byte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

void pci_config_write_long(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint32_t value);
void pci_config_write_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint16_t value);
void pci_config_write_byte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint8_t value);

uint16_t get_device(uint8_t bus, uint8_t slot);
uint8_t get_header_type(uint8_t bus, uint8_t slot);
uint8_t get_header_type_header_type(uint8_t bus, uint8_t slot);
//mf = multi-function
bool get_header_type_mf(uint8_t bus, uint8_t slot);
uint16_t get_subsystem_vendor_id(uint8_t bus, uint8_t slot);
uint16_t get_subsystem_id(uint8_t bus, uint8_t slot);
