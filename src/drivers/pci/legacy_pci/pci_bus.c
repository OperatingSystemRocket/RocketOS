#include "pci_bus.h"


#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

uint16_t pci_config_read_word(const uint8_t bus, const uint8_t slot, const uint8_t func, const uint8_t offset) {
    const uint32_t address = (uint32_t)((((uint32_t)bus) << 16u) | (((uint32_t)slot) << 11u) | (((uint32_t)func) << 8u) | (((uint32_t)offset) & 0xFCu) | ((uint32_t)0x80000000u));

    outl(CONFIG_ADDRESS, address);

    return (uint16_t)((inl(CONFIG_DATA) >> ((offset & 2u) * 8u)) & 0xFFFFu);
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
