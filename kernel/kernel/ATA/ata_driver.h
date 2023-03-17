#pragma once


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <kassert.h>
#include <hardware_io.h>


struct ata_device {
    uint16_t data;
    union {
        uint16_t error;
        uint16_t feature;
    };
    uint16_t sector_count;

    union {
        uint16_t sector_num;
        uint16_t lba_low;
    };
    union {
        uint16_t cylinder_low;
        uint16_t lba_mid;
    };
    union {
        uint16_t cylinder_high;
        uint16_t lba_high;
    };
    union {
        uint16_t drive;
        uint16_t head;
    };
    union {
        uint16_t status;
        uint16_t command;
    };

    // Below is based on offset from Control base.
    union {
        uint16_t alternate_status;
        uint16_t device_control;
    };
    uint16_t device_address;

    bool slave;
    bool primary;
    bool enabled;
};

struct ata_driver {
    struct ata_device primary_master;
    struct ata_device primary_slave;
    struct ata_device secondary_master;
    struct ata_device secondary_slave;
};


bool ata_driver_init(struct ata_driver* driver);
bool ata_device_init(struct ata_device* device, bool primary, bool slave);

