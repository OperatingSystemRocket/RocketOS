#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "acpi.h"
#include <mem/paging.h>
#include "kstring.h"

#include "kstdio.h"

struct pci_device_header {
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
};

struct pci_header0 {
    struct pci_device_header header;
    uint32_t bar[6];
    uint32_t cardbus_cis_ptr;
    uint16_t subsystem_vendor_id;
    uint16_t subsystem_id;
    uint32_t expansion_rom_base_addr;
    uint8_t capabilities_ptr;
    uint8_t reserved_0;
    uint16_t reserved_1;
    uint32_t reserved_2;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_grant;
    uint8_t max_latency;
};

void print_all_tables(ACPI_TABLE_RSDP* rsdp);
void* find_table(ACPI_TABLE_RSDP* rsdp, const char* signature);
ACPI_TABLE_MCFG* find_mcfg(ACPI_TABLE_RSDP* rsdp);
ACPI_TABLE_MADT* find_madt(ACPI_TABLE_RSDP* rsdp);
void enumerate_pcie(const ACPI_TABLE_MCFG* mcfg);
void detect_cores(ACPI_TABLE_MADT* madt);
void print_cores_info(void);


void init_ahci(struct pci_device_header* pci_device); //duplicate prototype here to avoid circular dependency/include
