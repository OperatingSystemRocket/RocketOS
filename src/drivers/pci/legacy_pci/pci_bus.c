#include "pci_bus.h"


#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

uint32_t pci_config_read_long(const uint8_t bus, const uint8_t device, const uint8_t function, const uint8_t offset) {
    const uint32_t address =
        (uint32_t)(1 << 31)  //enabled
        | ((uint32_t)bus << 16)  //bus number
        | ((uint32_t)device << 11)  //device number
        | ((uint32_t)function << 8) //function number
        | (((uint32_t)offset) & 0xFC); //Register number

    outl(CONFIG_ADDRESS, address);

    return inl(CONFIG_DATA);
}
uint16_t pci_config_read_word(const uint8_t bus, const uint8_t device, const uint8_t function, const uint8_t offset) {
    return (pci_config_read_long(bus, device, function, offset) >> ((offset & 3) * 8)) & 0xffff;
}
uint8_t pci_config_read_byte(const uint8_t bus, const uint8_t device, const uint8_t function, const uint8_t offset) {
    return (pci_config_read_long(bus, device, function, offset) >> ((offset & 3) * 8)) & 0xff;
}

void pci_config_write_long(const uint8_t bus, const uint8_t device, const uint8_t function, const uint8_t offset, const uint32_t value) {
    const uint32_t address =
        (uint32_t)(1 << 31)  //enabled
        | ((uint32_t)bus << 16)  //bus number
        | ((uint32_t)device << 11)  //device number
        | ((uint32_t)function << 8) //function number
        | (((uint32_t)offset) & 0xFC); //Register number

    outl(CONFIG_ADDRESS, address);

    outl(CONFIG_DATA, value);
}
void pci_config_write_word(const uint8_t bus, const uint8_t device, const uint8_t function, const uint8_t offset, const uint16_t value) {
    uint32_t tmp = pci_config_read_long(bus, device, function, offset);

    tmp &= ~(0xffff << ((offset & 3) * 8));
    tmp |= (value << ((offset & 3) * 8));

    pci_config_write_long(bus, device, function, offset, tmp);
}
void pci_config_write_byte(const uint8_t bus, const uint8_t device, const uint8_t function, const uint8_t offset, const uint8_t value) {
    uint32_t tmp = pci_config_read_long(bus, device, function, offset);

    tmp &= ~(0xff << ((offset & 3) * 8));
    tmp |= (value << ((offset & 3) * 8));

    pci_config_write_long(bus, device, function, offset, tmp);
}

uint16_t get_vendor_id(uint8_t bus, uint8_t device, uint8_t function) {
    // read "device id | vendor id"
    uint32_t large = pci_config_read_long(bus, device, function, 0);
    // extract vendor id
    return large;
}
uint8_t get_header_type(uint8_t bus, uint8_t device, uint8_t function) {
    // read "BIST | header type | latency timer | cache line size"
    uint32_t large = pci_config_read_long(bus, device, function, 12);
    // extract header type only
    return large >> 16 & 0xFF;
}
uint16_t get_device_id(uint8_t bus, uint8_t device, uint8_t function) {
    // read "device id | vendor id"
    uint32_t large = pci_config_read_long(bus, device, function, 0);
    // extract device id
    return large >> 16;
}
uint8_t get_class_code(uint8_t bus, uint8_t device, uint8_t function) {
    // read "class code | subclass | prog if | revision id"
    uint32_t large = pci_config_read_long(bus, device, function, 8);
    // extract class code only
    return large >> 24 & 0xFF;
}
uint8_t get_subclass(uint8_t bus, uint8_t device, uint8_t function) {
    // read "class code | subclass | prog if | revision id"
    uint32_t large = pci_config_read_long(bus, device, function, 8);
    // extract subclass only
    return large >> 16 & 0xFF;
}

static uint8_t num_of_devices = 0u;

void check_function(uint8_t bus, uint8_t device, uint8_t function) {
    uint16_t vendor_id = get_vendor_id(bus, device, function);
    if(vendor_id == 0xFFFF) {
        return;
    }

    uint16_t device_id  = get_device_id(bus, device, function);
    uint8_t class_code = get_class_code(bus, device, function);
    uint8_t sub_class  = get_subclass(bus, device, function);

    //kprintf("vendor_id: %X, device_id: %X, class_code: %X, subclass: %X\n", (uint32_t)vendor_id, (uint32_t)device_id, (uint32_t)class_code, (uint32_t)sub_class);
    ++num_of_devices;
}
void check_device(uint8_t bus, uint8_t device) {
    check_function(bus, device, 0);

    uint8_t header_type = get_header_type(bus, device, 0);
    if((header_type & 0x80) != 0) {
        for(uint8_t function = 1; function < 8; ++function){
            check_function(bus, device, function);
        }
    }
}
void brute_force_check_all_buses(void) {
    for(uint16_t bus = 0; bus < 256; ++bus) {
        for(uint8_t device = 0; device < 32; ++device) {
            check_device(bus, device);
        }
    }
    //kprintf("legacy PCI num_of_devices: %u\n", (uint32_t)num_of_devices);
}
