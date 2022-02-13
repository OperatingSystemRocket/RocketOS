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

uint16_t get_device(const uint8_t bus, const uint8_t slot) {
    return pci_config_read_word(bus, slot, 0u, 2u);
}
uint8_t get_header_type(const uint8_t bus, const uint8_t slot) {
    return (uint8_t)(pci_config_read_word(bus, slot, 0u, 0x0Eu) & 0xFFu);
}
uint8_t get_header_type_header_type(const uint8_t bus, const uint8_t slot) {
    return (uint8_t)(get_header_type(bus, slot) & 0x7Fu);
}
bool get_header_type_mf(const uint8_t bus, const uint8_t slot) {
    return (bool)(get_header_type(bus, slot) & 0x80u);
}
uint16_t get_subsystem_vendor_id(const uint8_t bus, const uint8_t slot) {
    return pci_config_read_word(bus, slot, 0u, 0x2Cu);
}
uint16_t get_subsystem_id(const uint8_t bus, const uint8_t slot) {
    return pci_config_read_word(bus, slot, 0u, 0x2Eu);
}
