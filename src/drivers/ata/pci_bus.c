#include "pci_bus.h"


#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

uint16_t pci_config_read_word(const uint8_t bus, const uint8_t slot, const uint8_t func, const uint8_t offset) {
    const uint32_t address = (uint32_t)((((uint32_t)bus) << 16u) | (((uint32_t)slot) << 11u) | (((uint32_t)func) << 8u) | (((uint32_t)offset) & 0xFCu) | ((uint32_t)0x80000000u));

    outl(CONFIG_ADDRESS, address);

    return (uint16_t)((inl(CONFIG_DATA) >> ((offset & 2u) * 8u)) & 0xFFFFu);
}

