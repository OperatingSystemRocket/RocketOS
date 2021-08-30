#include "ata_driver.h"


struct ata_driver_data {
    size_t current_byte_location;
};


struct ata_driver_data* ata_driver_init(void) {
    struct ata_driver_data *const data = kmalloc(sizeof(struct ata_driver_data));
    data->current_byte_location = 0u;
    return data;
}


