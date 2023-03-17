#include "ata_driver.h"



#define PRIMARY_IO_BASE_PORT 0x1F0
#define SECONDARY_IO_BASE_PORT 0x170
#define PRIMARY_CONTROL_BASE_PORT 0x3F6
#define SECONDARY_CONTROL_BASE_PORT 0x376

bool ata_driver_init(struct ata_driver *const driver) {
    kassert(driver != NULL, false);

    ata_device_init(&driver->primary_master, true, false);
    ata_device_init(&driver->primary_slave, true, true);
    ata_device_init(&driver->secondary_master, false, false);
    ata_device_init(&driver->secondary_slave, false, true);

    return true;
}

bool ata_device_init(struct ata_device *const device, bool primary, bool slave) {
    kassert(device != NULL, false);

    const uint16_t io_base = primary ? PRIMARY_IO_BASE_PORT : SECONDARY_IO_BASE_PORT;
    const uint16_t control_base = primary ? PRIMARY_CONTROL_BASE_PORT : SECONDARY_CONTROL_BASE_PORT;

    device->data = io_base;
    device->error = io_base + 1;
    device->sector_count = io_base + 2;
    device->sector_num = io_base + 3;
    device->cylinder_low = io_base + 4;
    device->cylinder_high = io_base + 5;
    device->drive = io_base + 6;
    device->status = io_base + 7;

    device->alternate_status = control_base;
    device->device_address = control_base + 1;

    device->slave = slave;
    device->primary = primary;

    device->enabled = false;

    return true;
}


#define STATUS_REG_BSY 0x80
#define STATUS_REG_DRQ 0x08
#define STATUS_REG_ERR 0x01

#define IDENTIFY_COMMAND 0xEC

bool ata_identity(struct ata_device *const device) {
    kassert(device != NULL, false);

    outb(device->drive, (0xA0 | (device->slave << 4)));

    outb(device->sector_count, 0);
    outb(device->lba_low, 0);
    outb(device->lba_mid, 0);
    outb(device->lba_high, 0);

    outb(device->command, IDENTIFY_COMMAND);

    const uint8_t status = inb(device->status);
    if (!status) {
        return false;
    }

    while (true) {
        const uint8_t status = inb(device->status);
        if (status & STATUS_REG_DRQ) {
            break;
        }
        if(status & STATUS_REG_ERR) {
            return false;
        }
    }

    for (uint32_t i = 0; i < 256; i++) {
        inw(device->data);
    }

    device->enabled = true;

    return true;
}

bool read_single_sector(struct ata_device *const device, uint32_t lba, uint8_t *const buffer) {
    kassert(device != NULL, false);
    kassert(buffer != NULL, false);

    const uint8_t status = inb(device->status);
    if ((status & STATUS_REG_BSY) || (status & STATUS_REG_DRQ)) {
        outb(device->device_control, 4);
        outb(device->device_control, 0);
    }

    outb(device->drive, (0xE0 | (device->slave << 4)) | ((lba >> 24) & 0x0F));

    outb(device->sector_count, 1);

    outb(device->lba_low, lba & 0xFF);
    outb(device->lba_mid, (lba >> 8) & 0xFF);
    outb(device->lba_high, (lba >> 16) & 0xFF);

    outb(device->command, 0x20);

    while (true) {
        const uint8_t status = inb(device->status);
        if (!(status & STATUS_REG_BSY) && (status & STATUS_REG_DRQ)) {
            break;
        }
    }

    for (size_t i = 0; i < 256; i++) {
        ((uint16_t*)buffer)[i] = inw(device->data);
    }

    for (int i = 0; i < 4; i++) {
        inb(device->alternate_status);
    }

    return true;
}
