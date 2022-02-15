#include "pcie_bus.h"


void print_all_tables(ACPI_TABLE_RSDP* rsdp) {
    for(uint32_t i = 0; i < 8; ++i) {
        kprintf("%c", rsdp->Signature[i]);
    }
    kprintf("\n\n");
    kprintf("rsdp->Revision: %u\n", rsdp->Revision);
    ACPI_TABLE_HEADER* xsdt_header = AcpiOsMapMemory(rsdp->XsdtPhysicalAddress, sizeof(ACPI_TABLE_HEADER));
    for(uint32_t i = 0; i < ACPI_NAMESEG_SIZE; ++i) {
        kprintf("%c", xsdt_header->Signature[i]);
    }
    kprintf("\n\n");

    const uint32_t entries = (xsdt_header->Length - sizeof(ACPI_TABLE_HEADER)) / sizeof(uint64_t);
    ACPI_TABLE_XSDT* xsdt = AcpiOsMapMemory(rsdp->XsdtPhysicalAddress, xsdt_header->Length);
    kprintf("xsdt->Header.Length: %u, sizeof(ACPI_TABLE_HEADER): %u, entries: %u\n", xsdt->Header.Length, sizeof(ACPI_TABLE_HEADER), entries);
    AcpiOsUnmapMemory(xsdt_header, sizeof(ACPI_TABLE_HEADER));

    for(uint32_t i = 0u; i < entries; ++i) {
        ACPI_TABLE_HEADER* new_xsdt_header = AcpiOsMapMemory(xsdt->TableOffsetEntry[i] & 0xFFFFFFFFu, sizeof(ACPI_TABLE_HEADER));
        for(uint32_t j = 0u; j < ACPI_NAMESEG_SIZE; ++j) {
            kprintf("%c", new_xsdt_header->Signature[j]);
        }
        kprintf(" ");
        AcpiOsUnmapMemory(new_xsdt_header, sizeof(ACPI_TABLE_HEADER));
    }
    kprintf("\n");
}

void* find_table(ACPI_TABLE_RSDP* rsdp, const char* signature) {
    ACPI_TABLE_HEADER* xsdt_header = AcpiOsMapMemory(rsdp->XsdtPhysicalAddress, sizeof(ACPI_TABLE_HEADER));

    const uint32_t entries = (xsdt_header->Length - sizeof(ACPI_TABLE_HEADER)) / sizeof(uint64_t);
    ACPI_TABLE_XSDT* xsdt = AcpiOsMapMemory(rsdp->XsdtPhysicalAddress, xsdt_header->Length);
    AcpiOsUnmapMemory(xsdt_header, sizeof(ACPI_TABLE_HEADER));

    for(uint32_t i = 0u; i < entries; ++i) {
        const uint32_t new_xsdt_header_physical_addr = xsdt->TableOffsetEntry[i] & 0xFFFFFFFFu;
        ACPI_TABLE_HEADER* new_xsdt_header = AcpiOsMapMemory(new_xsdt_header_physical_addr, sizeof(ACPI_TABLE_HEADER));
        const bool is_match = kstrncmp(new_xsdt_header->Signature, signature, ACPI_NAMESEG_SIZE) == 0;
        AcpiOsUnmapMemory(new_xsdt_header, sizeof(ACPI_TABLE_HEADER));
        if(is_match) {
            return (void*)new_xsdt_header_physical_addr;
        }
    }
    return NULL;
}

ACPI_TABLE_MCFG* find_mcfg(ACPI_TABLE_RSDP *const rsdp) {
    return find_table(rsdp, ACPI_SIG_MCFG);
}
ACPI_TABLE_MADT* find_madt(ACPI_TABLE_RSDP *const rsdp) {
    return find_table(rsdp, ACPI_SIG_MADT);
}

static uint32_t last_vendor_id = 0u;
static uint32_t last_device_id = 0u;

void enumerate_function(const uint64_t device_addr, const uint64_t function) {
    const uint64_t offset = function << 12;

    const uint32_t function_addr = (device_addr + offset) & 0xFFFFFFFFu;
    identity_map_page((uint32_t)get_default_page_directory(), function_addr, PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);
    //struct pci_device_header* function_header = AcpiOsMapMemory(device_addr, sizeof(struct pci_device_header));
    struct pci_device_header *const function_header = (struct pci_device_header*)function_addr;

    if(function_header->device_id == 0u || function_header->device_id == 0xFFFFu) return;

    const uint32_t vendor_id = function_header->vendor_id;
    const uint32_t device_id = function_header->device_id;
    if(vendor_id != last_vendor_id || device_id != last_device_id) {
        kprintf("%X %X\n", vendor_id, device_id);
    }
    last_vendor_id = vendor_id;
    last_device_id = device_id;

    //AcpiOsUnmapMemory(function_header, sizeof(struct pci_device_header));
}

void enumerate_device(const uint64_t bus_addr, const uint64_t device) {
    last_vendor_id = 0u;
    last_device_id = 0u;

    const uint64_t offset = device << 15;

    const uint32_t device_addr = (bus_addr + offset) & 0xFFFFFFFFu;
    identity_map_page((uint32_t)get_default_page_directory(), device_addr, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    //struct pci_device_header* device_header = AcpiOsMapMemory(device_addr, sizeof(struct pci_device_header));
    struct pci_device_header *const device_header = (struct pci_device_header*)device_addr;

    if(device_header->device_id == 0u || device_header->device_id == 0xFFFFu) return;

    for(uint32_t function = 0u; function < 8u; ++function) {
        enumerate_function(device_addr, device);
    }

    //AcpiOsUnmapMemory(device_header, sizeof(struct pci_device_header));
}

void enumerate_bus(const uint64_t base_addr, const uint64_t bus) {
    const uint64_t offset = bus << 20;

    const uint32_t bus_addr = (base_addr + offset) & 0xFFFFFFFFu;
    identity_map_page((uint32_t)get_default_page_directory(), bus_addr, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    //struct pci_device_header* bus_header = AcpiOsMapMemory(bus_addr, sizeof(struct pci_device_header));
    struct pci_device_header *const bus_header = (struct pci_device_header*)bus_addr;

    if(bus_header->device_id == 0u || bus_header->device_id == 0xFFFFu) {
        //kprintf("non-existent_bus ");
        return;
    }
    //kprintf("enumerate_bus ");

    for(uint32_t device = 0u; device < 32u; ++device) {
        enumerate_device(bus_addr, device);
    }

    //AcpiOsUnmapMemory(bus_header, sizeof(struct pci_device_header));
}

void enumerate_pcie(const ACPI_TABLE_MCFG *const mcfg) {
    //kprintf("mcfg->Header.Length: %u, sizeof(ACPI_TABLE_MCFG): %u, sizeof(ACPI_MCFG_ALLOCATION): %u\n", mcfg->Header.Length, sizeof(ACPI_TABLE_MCFG), sizeof(ACPI_MCFG_ALLOCATION));
    const uint32_t entries = (mcfg->Header.Length - sizeof(ACPI_TABLE_MCFG)) / sizeof(ACPI_MCFG_ALLOCATION);
    //kprintf("enumerate_pcie entries: %u\n", entries);

    for (uint32_t i = 0; i < entries; ++i) {
        const uint32_t device_config_addr = (((uint64_t)mcfg) + sizeof(ACPI_TABLE_MCFG) + (sizeof(ACPI_MCFG_ALLOCATION) * i)) & 0xFFFFFFFFu;
        identity_map_page((uint32_t)get_default_page_directory(), device_config_addr, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
        ACPI_MCFG_ALLOCATION* new_device_config = (ACPI_MCFG_ALLOCATION*)device_config_addr;
        //ACPI_MCFG_ALLOCATION* new_device_config = AcpiOsMapMemory(device_config_addr, sizeof(ACPI_MCFG_ALLOCATION));
        const uint32_t number_of_buses = (new_device_config->EndBusNumber-new_device_config->StartBusNumber)+1u;
        //kprintf("enumerate_pcie new_device_config->EndBusNumber-new_device_config->StartBusNumber: %u\n", number_of_buses);
        for(uint32_t bus = new_device_config->StartBusNumber; bus <= new_device_config->EndBusNumber; ++bus) {
            enumerate_bus(new_device_config->Address, bus);
        }
        //AcpiOsUnmapMemory(new_device_config, sizeof(ACPI_MCFG_ALLOCATION));
    }
}

static uint8_t lapic_ids[256] = {0u}; // CPU core Local APIC IDs
static uint8_t numcore = 0u;          // number of cores detected
static uint32_t lapic_ptr = 0u;       // pointer to the Local APIC MMIO registers
static uint32_t ioapic_ptr = 0u;      // pointer to the IO APIC MMIO registers

void detect_cores(ACPI_TABLE_MADT *const madt) {
    lapic_ptr = madt->Address;
    identity_map_page((uint32_t)get_default_page_directory(), lapic_ptr, PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);
    const length = madt->Header.Length;
    uint8_t* const end_of_madt_struct = (uint8_t*)(((uint32_t)madt) + sizeof(ACPI_TABLE_MADT));
    uint8_t* const end_of_madt = (uint8_t*)(((uint32_t)madt) + length);
    for(uint8_t* current_addr = end_of_madt_struct; current_addr < end_of_madt; current_addr += current_addr[1]) {
        switch(current_addr[0]) {
            case 0u: // Processor Local APIC
                if(current_addr[4] & 1u) {
                    lapic_ids[numcore++] = current_addr[3];
                }
                break;
            case 1u: // I/O APIC
                ioapic_ptr = *((uint32_t*)(current_addr+4));
                identity_map_page((uint32_t)get_default_page_directory(), ioapic_ptr, PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);
                break;
            case 5u: // Local APIC Address Override
                lapic_ptr = (uint32_t)((*((uint64_t*)(current_addr+4))) & 0xFFFFFFFFu);
                identity_map_page((uint32_t)get_default_page_directory(), lapic_ptr, PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);
                break;
        }
    }
}
void print_cores_info(void) {
    kprintf("Found %u cores, IOAPIC %X, LAPIC %X, Processor IDs:", (uint32_t)numcore, ioapic_ptr, lapic_ptr);
    for(uint32_t i = 0u; i < numcore; ++i) {
        kprintf(" %u", (uint32_t)lapic_ids[i]);
    }
    kprintf("\n");
}
