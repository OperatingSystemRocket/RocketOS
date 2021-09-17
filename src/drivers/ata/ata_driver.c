#include "ata_driver.h"


#define PRIMARY_IO_PORT_BASE_BEGIN 0x1F0 //also called the primary bus "IO Base"
#define PRIMARY_IO_PORT_BASE_END 0x1F7
#define PRIMARY_CONTROL_PORT_BASE_BEGIN 0x3F6
#define PRIMARY_CONTROL_PORT_BASE_END 0x3F7

#define SECONDARY_IO_PORT_BASE_BEGIN 0x170 //also called the secondary bus "IO Base"
#define SECONDARY_IO_PORT_BASE_END 0x177
#define SECONDARY_CONTROL_PORT_BASE_BEGIN 0x376
#define SECONDARY_CONTROL_PORT_BASE_END 0x377


struct ata_driver_data {
    size_t current_byte_location;
};


struct ata_driver_data* ata_driver_init(void) {
    struct ata_driver_data *const data = kmalloc(sizeof(struct ata_driver_data));
    data->current_byte_location = 0u;
    return data;
}


