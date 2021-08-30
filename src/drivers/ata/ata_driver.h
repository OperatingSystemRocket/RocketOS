#pragma once

#include <stdint.h>
#include <stddef.h>

#include "kstdlib.h"


struct ata_driver_data;


struct ata_driver_data* ata_driver_init(void);
void write_to_disk(struct ata_driver_data* ata_data, const char* datastream);
void write_to_disk_n(struct ata_driver_data* ata_data, const char* datastream, size_t n);
const char* read_from_disk(struct ata_driver_data* ata_data);
const char* read_from_disk_n(struct ata_driver_data* ata_data, size_t n);
void ata_driver_seek(struct ata_driver_data* ata_data, size_t byte_location_on_hdd);
void ata_driver_destroy(struct ata_driver_data* ata_data);

