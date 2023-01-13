#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <paging.h>
#include "kstdlib.h"
#include <global_phys_allocator.h>
#include "kstring.h"

#include <drivers/pci/pcie/pcie_bus.h>


struct hba_port {
    uint32_t command_list_base;
    uint32_t command_list_base_upper;
    uint32_t fis_base_addr;
    uint32_t fis_base_addr_upper;
    uint32_t interrupt_status;
    uint32_t interrupt_enable;
    uint32_t command_and_status;
    uint32_t reserved_0;
    uint32_t task_file_data;
    uint32_t signature;
    uint32_t sata_status;
    uint32_t sata_control;
    uint32_t sata_error;
    uint32_t sata_active;
    uint32_t command_issue;
    uint32_t sata_notification;
    uint32_t fis_switch_control;
    uint32_t reserved_1[11];
    uint32_t vendor_data[4];
};

struct hba_memory {
    uint32_t host_capabilities;
    uint32_t global_host_control;
    uint32_t interrupt_status;
    uint32_t ports_implemented;
    uint32_t version;
    uint32_t ccc_control;
    uint32_t ccc_ports;
    uint32_t enclosure_management_location;
    uint32_t enclosure_management_control;
    uint32_t host_capabilities_extended;
    uint32_t bios_handoff_control_and_status;
    uint8_t reserved[0x74];
    uint8_t vendor_specific[0x60];
    struct hba_port ports[];
};

//TODO: stop using bitset members and instead manually set the bits as C bitsets are pretty bad
struct hba_command_header {
    uint8_t command_fis_length : 5;
    uint8_t atapi : 1;
    uint8_t write : 1;
    uint8_t prefetchable : 1;

    uint8_t reset : 1;
    uint8_t bist : 1;
    uint8_t clear_busy : 1;
    uint8_t reserved_0 : 1;
    uint8_t port_multiplier : 4;

    uint16_t prdt_length;
    uint32_t prdb_count;
    uint32_t command_table_base_addr;
    uint32_t command_table_base_addr_upper;
    uint32_t reserved_1[4];
};

struct fis_register_h2d {
    uint8_t fis_type;

    uint8_t port_multiplier : 4;
    uint8_t reserved_0 : 3;
    uint8_t command_control : 1;

    uint8_t command;
    uint8_t feature_low;

    uint8_t lba0;
    uint8_t lba1;
    uint8_t lba2;
    uint8_t device_register;

    uint8_t lba3;
    uint8_t lba4;
    uint8_t lba5;
    uint8_t feature_high;

    uint8_t count_low;
    uint8_t count_high;
    uint8_t iso_command_completion;
    uint8_t control;

    uint8_t reserved_1[4];
};

struct hba_prdt_entry {
    uint32_t data_base_addr;
    uint32_t data_base_addr_upper;
    uint32_t reserved_0;

    uint32_t byte_count : 22;
    uint32_t reserved_1 : 9;
    uint32_t interrupt_on_completion : 1;
};

struct hba_command_table {
    uint8_t command_fis[64];

    uint8_t atapi_command[16];

    uint8_t reserved[48];

    struct hba_prdt_entry prdt_entries[];
};


enum hba_port_type {
    HBA_PORT_TYPE_NONE = 0,
    HBA_PORT_TYPE_SATA = 1,
    HBA_PORT_TYPE_SEMB = 2,
    HBA_PORT_TYPE_PM = 3,
    HBA_PORT_TYPE_SATAPI = 4,
};

enum fis_type {
    FIS_TYPE_REG_H2D = 0x27,
    FIS_TYPE_REG_D2H = 0x34,
    FIS_TYPE_DMA_ACTIVATE = 0x39,
    FIS_TYPE_DMA_SETUP = 0x41,
    FIS_TYPE_DATA = 0x46,
    FIS_TYPE_BIST = 0x58,
    FIS_TYPE_PIO_SETUP = 0x5F,
    FIS_TYPE_DEV_BITS = 0xA1,
};

#define HBA_PORT_PRESENT 0x03
#define HBA_PORT_IPM_ACTIVE 0x01
#define SATA_ATAPI_SIGNATURE 0xEB140101
#define SATA_ATA_SIGNATURE 0x00000101
#define SATA_SEMB_SIGNATURE 0xC33C0101
#define SATA_PM_SIGNATURE 0x96690101

#define HBA_PxCMD_CR 0x8000
#define HBA_PxCMD_FRE 0x0010
#define HBA_PxCMD_ST 0x0001
#define HBA_PxCMD_FR 0x4000

#define ATA_CMD_READ_DMA_EX 0x25

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

#define HBA_PxIS_TFES (1u << 30u)

struct ahci_port {
    struct hba_port* port;
    enum hba_port_type type;
    uint8_t* buffer;
    uint8_t port_number;
};


void init_ahci(struct pci_device_header* pci_device);
void probe_ports(void);
void start_cmd(struct ahci_port* port);
void stop_cmd(struct ahci_port* port);
void configure_port(struct ahci_port* port);
bool read_sector(struct ahci_port* port, uint32_t sector, uint16_t sector_count, void* buffer);
