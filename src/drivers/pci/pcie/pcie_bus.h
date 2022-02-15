#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "acpi.h"
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

void print_all_tables(ACPI_TABLE_RSDP* rsdp);
void* find_table(ACPI_TABLE_RSDP* rsdp, const char* signature);
ACPI_TABLE_MCFG* find_mcfg(ACPI_TABLE_RSDP* rsdp);
ACPI_TABLE_MADT* find_madt(ACPI_TABLE_RSDP* rsdp);
void enumerate_pcie(const ACPI_TABLE_MCFG* mcfg);
void detect_cores(ACPI_TABLE_MADT* madt);
void print_cores_info(void);
