#include "ahci.h"


//TODO: stop using static variables and use an `ahci` struct instance instead
static struct pci_device_header* pci_device_base_addr = NULL;
struct hba_memory* abar = NULL;

static struct ahci_port* ahci_ports[32] = {NULL};
static uint8_t ahci_port_count = 0u;


void start_cmd(struct ahci_port *const port) {
    while(port->port->command_and_status & HBA_PxCMD_CR);

    port->port->command_and_status |= HBA_PxCMD_FRE;
    port->port->command_and_status |= HBA_PxCMD_ST;
}

void stop_cmd(struct ahci_port *const port) {
    port->port->command_and_status &= ~HBA_PxCMD_ST;
    port->port->command_and_status &= ~HBA_PxCMD_FRE;

    while(port->port->command_and_status & HBA_PxCMD_FR || port->port->command_and_status & HBA_PxCMD_CR);
}

void configure_port(struct ahci_port *const port) {
    stop_cmd(port);

    uint32_t new_base = (uint32_t)global_phys_allocator_allocate_page();
    //TODO: maybe replace all of these kernel space identity maps with `struct physical_pointer` objects instead
    identity_map_page_in_kernel_addr(new_base, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    port->port->command_list_base = new_base;
    port->port->command_list_base_upper = 0u;
    kmemset((void*)port->port->command_list_base, 0u, 1024u);

    uint32_t fis_base = (uint32_t)global_phys_allocator_allocate_page();
    //TODO: maybe replace all of these kernel space identity maps with `struct physical_pointer` objects instead
    identity_map_page_in_kernel_addr(fis_base, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    port->port->fis_base_addr = fis_base;
    port->port->fis_base_addr_upper = 0u;
    kmemset((void*)port->port->fis_base_addr, 0u, 256u);

    struct hba_command_header* cmd_header = (struct hba_command_header*)(port->port->command_list_base);

    for(uint32_t i = 0u; i < 32u; ++i) {
        cmd_header[i].prdt_length = 8u;

        const uint32_t cmd_table_addr = (uint32_t)global_phys_allocator_allocate_page();
        //TODO: maybe replace all of these kernel space identity maps with `struct physical_pointer` objects instead
        identity_map_page_in_kernel_addr(cmd_table_addr, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
        const uint32_t address = cmd_table_addr + (i << 8u);
        cmd_header[i].command_table_base_addr = address;
        cmd_header[i].command_table_base_addr_upper = 0u;
        kmemset(cmd_table_addr, 0, 256u);
    }

    start_cmd(port);
}

bool read_sector(struct ahci_port *const port, const uint32_t sector, const uint16_t sector_count, void *const buffer) {
    port->port->interrupt_status = 0xFFFFFFFFu;

    struct hba_command_header *const cmd_header = (struct hba_command_header*)(port->port->command_list_base);
    cmd_header->command_fis_length = sizeof(struct fis_register_h2d)/sizeof(uint32_t);
    cmd_header->write = 0u;
    cmd_header->prdt_length = 1u;

    struct hba_command_table *const command_table = (struct hba_command_table*)(cmd_header->command_table_base_addr);
    kmemset(command_table, 0u, sizeof(struct hba_command_table) + (cmd_header->prdt_length-1u)*sizeof(struct hba_prdt_entry));

    command_table->prdt_entries[0].data_base_addr = (uint32_t)buffer;
    command_table->prdt_entries[0].data_base_addr_upper = 0u;
    command_table->prdt_entries[0].byte_count = (sector_count << 9u) - 1u;
    command_table->prdt_entries[0].interrupt_on_completion = 1u;

    struct fis_register_h2d *const command_fis = (struct fis_register_h2d*)(&command_table->command_fis);
    command_fis->fis_type = FIS_TYPE_REG_H2D;
    command_fis->command_control = 1u;
    command_fis->command = ATA_CMD_READ_DMA_EX;

    command_fis->lba0 = (uint8_t)(sector  & 0xFFu);
    command_fis->lba1 = (uint8_t)((sector >> 8u) & 0xFFu);
    command_fis->lba2 = (uint8_t)((sector >> 16u) & 0xFFu);
    command_fis->lba3 = 0u;
    command_fis->lba4 = 0u;
    command_fis->lba5 = 0u;

    command_fis->device_register = (1u << 6u);

    command_fis->count_low = (uint8_t)(sector_count & 0xFFu);
    command_fis->count_high = (uint8_t)((sector_count >> 8u) & 0xFFu);

    uint64_t spin = 0u;
    while(port->port->task_file_data & (ATA_DEV_BUSY | ATA_DEV_DRQ) && spin < 10000000) {
        ++spin;
    }
    if(spin == 10000000) return false;

    port->port->command_issue = 1u;

    for(;;) {
        if(port->port->command_issue == 0u) break;
        if(port->port->interrupt_status & HBA_PxIS_TFES) return false;
    }

    return true;
}


void init_ahci(struct pci_device_header *const pci_device) {
    kprintf("init_ahci\n");
    pci_device_base_addr = pci_device;

    abar = (struct hba_memory*)((struct pci_header0*)pci_device)->bar[5];
    //TODO: maybe replace all of these kernel space identity maps with `struct physical_pointer` objects instead
    identity_map_page_in_kernel_addr((uint32_t)abar, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);

    probe_ports();

    for(uint32_t i = 0u; i < ahci_port_count; ++i) {
        struct ahci_port *const port = ahci_ports[i];

        configure_port(port);

        void *const buffer = global_phys_allocator_allocate_page();
        //TODO: maybe replace all of these kernel space identity maps with `struct physical_pointer` objects instead
        identity_map_page_in_kernel_addr((uint32_t)buffer, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
        port->buffer = buffer;
        kmemset(buffer, 0u, PAGE_SIZE);

        read_sector(port, 0, 4, port->buffer);
        for(uint32_t j = 0u; j < 512u; ++j) {
            kprintf("%c", port->buffer[j]);
        }
        kprintf("\n");
    }
}

static enum hba_port_type check_port_type(struct hba_port* const port) {
    const uint32_t sata_status = port->sata_status;

    const uint8_t interface_power_management = (sata_status >> 8u) & 0x07u;
    const uint8_t device_detection = sata_status & 0x07u;

    if(device_detection != HBA_PORT_PRESENT) return HBA_PORT_TYPE_NONE;
    if(interface_power_management != HBA_PORT_IPM_ACTIVE) return HBA_PORT_TYPE_NONE;

    switch(port->signature) {
        case SATA_ATAPI_SIGNATURE:
            return HBA_PORT_TYPE_SATAPI;
        case SATA_ATA_SIGNATURE:
            return HBA_PORT_TYPE_SATA;
        case SATA_PM_SIGNATURE:
            return HBA_PORT_TYPE_PM;
        case SATA_SEMB_SIGNATURE:
            return HBA_PORT_TYPE_SEMB;
        default:
            return HBA_PORT_TYPE_NONE;
    }
}

void probe_ports(void) {
    const uint32_t ports_implemented = abar->ports_implemented;
    for(uint32_t port = 0u; port < 32u; ++port) {
        if(ports_implemented & (1u << port)) {
            kprintf("port %u\t", port);
            const enum hba_port_type port_type = check_port_type(&abar->ports[port]);
            if(port_type == HBA_PORT_TYPE_SATA || port_type == HBA_PORT_TYPE_SATAPI) {
                ahci_ports[ahci_port_count] = kmalloc(sizeof(struct ahci_port));
                ahci_ports[ahci_port_count]->type = port_type;
                ahci_ports[ahci_port_count]->port = &abar->ports[port];
                ahci_ports[ahci_port_count]->port_number = ahci_port_count;
                ++ahci_port_count;
            }
        }
    }
    kprintf("\n");
}

