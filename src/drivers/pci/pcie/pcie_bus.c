#include "pcie_bus.h"


void print_all_tables(ACPI_TABLE_RSDP* rsdp) {
    for(uint32_t i = 0; i < 8; ++i) {
        kprintf("%c", rsdp->Signature[i]);
    }
    kprintf("\n\n");
    kprintf("rsdp->Revision: %u\n", rsdp->Revision);
    ACPI_TABLE_HEADER* xsdt = (ACPI_TABLE_HEADER*)(rsdp->XsdtPhysicalAddress);
    for(uint32_t i = 0; i < ACPI_NAMESEG_SIZE; ++i) {
        kprintf("%c", xsdt->Signature[i]);
    }
    kprintf("\n\n");
/*
    const uint32_t entries = (xsdt->Length - sizeof(ACPI_TABLE_HEADER)) / sizeof(uint64_t);
    kprintf("xsdt->Length: %u, sizeof(ACPI_TABLE_HEADER): %u, entries: %u\n", xsdt->Length, sizeof(ACPI_TABLE_HEADER), entries);
    for(uint32_t i = 0u; i < entries; ++i) {
        ACPI_TABLE_HEADER* new_xsdt_header = (ACPI_TABLE_HEADER*)*(uint64_t*)(((uint64_t)xsdt) + sizeof(ACPI_TABLE_HEADER) + (i * sizeof(uint64_t)));
        for(uint32_t j = 0u; j < ACPI_NAMESEG_SIZE; ++j) {
            kprintf("%c", new_xsdt_header->Signature[j]);
        }
        kprintf(" ");
    }
    kprintf("\n");
*/
}

