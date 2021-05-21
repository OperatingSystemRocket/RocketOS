#include "storage.h"


#define ATA_SR_BSY 0x80
#define ATA_SR_DRDY 0x40
#define ATA_SR_DF 0x20
#define ATA_SR_DSC 0x10
#define ATA_SR_DRQ 0x08
#define ATA_SR_CORR 0x04
#define ATA_SR_IDX 0x02
#define ATA_SR_ERR 0x01

#define ATA_ER_BBK 0x80
#define ATA_ER_UNC 0x40
#define ATA_ER_MC 0x20
#define ATA_ER_IDNF 0x10
#define ATA_ER_MCR 0x08
#define ATA_ER_ABRT 0x04
#define ATA_ER_TK0NF 0x02
#define ATA_ER_AMNF 0x01

// ATA-Commands:
#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_READ_PIO_EXT 0x24
#define ATA_CMD_READ_DMA 0xC8
#define ATA_CMD_READ_DMA_EXT 0x25
#define ATA_CMD_WRITE_PIO 0x30
#define ATA_CMD_WRITE_PIO_EXT 0x34
#define ATA_CMD_WRITE_DMA 0xCA
#define ATA_CMD_WRITE_DMA_EXT 0x35
#define ATA_CMD_CACHE_FLUSH 0xE7
#define ATA_CMD_CACHE_FLUSH_EXT 0xEA
#define ATA_CMD_PACKET 0xA0
#define ATA_CMD_IDENTIFY_PACKET 0xA1
#define ATA_CMD_IDENTIFY 0xEC

#define ATAPI_CMD_READ 0xA8
#define ATAPI_CMD_EJECT 0x1B

#define ATA_IDENT_DEVICETYPE 0
#define ATA_IDENT_CYLINDERS 2

#define ATA_IDENT_HEADS 6
#define ATA_IDENT_SECTORS 12
#define ATA_IDENT_SERIAL 20
#define ATA_IDENT_MODEL 54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID 106
#define ATA_IDENT_MAX_LBA 120
#define ATA_IDENT_COMMANDSETS 164
#define ATA_IDENT_MAX_LBA_EXT 200

#define ATA_MASTER 0x00
#define ATA_SLAVE 0x01
#define IDE_ATA 0x00
#define IDE_ATAPI 0x01
// ATA-ATAPI Task-File:
#define ATA_REG_DATA 0x00
#define ATA_REG_ERROR 0x01
#define ATA_REG_FEATURES 0x01
#define ATA_REG_SECCOUNT0 0x02
#define ATA_REG_LBA0 0x03
#define ATA_REG_LBA1 0x04
#define ATA_REG_LBA2 0x05
#define ATA_REG_HDDEVSEL 0x06
#define ATA_REG_COMMAND 0x07
#define ATA_REG_STATUS 0x07
#define ATA_REG_SECCOUNT1 0x08
#define ATA_REG_LBA3 0x09
#define ATA_REG_LBA4 0x0A
#define ATA_REG_LBA5 0x0B
#define ATA_REG_CONTROL 0x0C
#define ATA_REG_ALTSTATUS 0x0C
#define ATA_REG_DEVADDRESS 0x0D








// Channels:
#define ATA_PRIMARY 0x00
#define ATA_SECONDARY 0x01
// Directions:
#define ATA_READ 0x00
#define ATA_WRITE 0x01

#define insl(port, buf, nr) \
            __asm__ volatile("cld;rep;insl\n\t"::"d"(port), "D"(buf), "c"(nr))



unsigned char package[1];

unsigned char get_error_package(void) {
    return package[0];
}

struct channel {
    unsigned short base; // I/O Base.
    unsigned short ctrl; // Control Base
    unsigned short bmide; // Bus Master IDE
    unsigned char nIEN; // nIEN (No Interrupt);
} channels[2];

unsigned char ide_buf[2048] = {0};
unsigned static char ide_irq_invoked = 0;
unsigned static char atapi_packet[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

struct ide_device {
    unsigned char reserved; // 0 (Empty) or 1 (This Drive really exists).
    unsigned char channel; // 0 (Primary Channel) or 1 (Secondary Channel).
    unsigned char drive; // 0 (Master Drive) or 1 (Slave Drive).

    unsigned short type; // 0: ATA, 1:ATAPI.
    unsigned short sign; // Drive Signature
    unsigned short capabilities;// Features.
    unsigned int commandsets; // Command Sets Supported.
    unsigned int size; // Size in Sectors.
    unsigned char model[41]; // Model in string.
} ide_devices[4];


void ide_write(unsigned char channel, unsigned char reg, unsigned char data) {
    if (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, 0x80 |
                                                                      channels[channel].nIEN);
    if (reg < 0x08) outb(data, channels[channel].base + reg - 0x00);
    else if (reg < 0x0C) outb(data, channels[channel].base + reg - 0x06);
    else if (reg < 0x0E) outb(data, channels[channel].ctrl + reg - 0x0A);
    else if (reg < 0x16) outb(data, channels[channel].bmide + reg - 0x0E);
    if (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL,
                                            channels[channel].nIEN);
}

#define stringify(x) #x
#define read_and_print(i, reg) \
kprintf("%s", "register name: ");                                    \
kprintf("%s", stringify(reg##_)); \
kprintf("%s", ", register address offset: ");                                    \
kprintf("%s", stringify(reg));                                   \
kprintf("%s", ", actual address read from: ");                                      \
{char c = ide_read(i,reg); \
kprintf("%s", ", ide_read result: 0x");                                \
kprintf("%X\n", c);                }                             \
//if((unsigned int)c == 0) kprintf("%s"," (is zero)");} kprintf("%c", '\n')




unsigned char ide_read(unsigned char channel, unsigned char reg) {
    unsigned char result;
    uint16_t address;
    
    if (reg > 0x07 && reg < 0x0C) {
        ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
        //kprintf("%s %i", "ATA_REG_CONTROL address written to: ", 0x80 | channels[channel].nIEN);

    }
    if (reg < 0x08) {
        address = channels[channel].base + reg - 0x00;
        result =      inb(address);
        //kprintf("%X", channels[channel].base + reg - 0x00);
    }
    else if (reg < 0x0C) {
        address = channels[channel].base + reg - 0x06;
        result = inb(address);
        //kprintf("%X", channels[channel].base + reg - 0x06);

    }
    else if (reg < 0x0E) {
        address = channels[channel].ctrl + reg - 0x0A;
        result = inb(address);
        //kprintf("%X", channels[channel].ctrl + reg - 0x0A);

    }
    else if (reg < 0x16) {
        address = channels[channel].bmide + reg - 0x0E;
        result = inb(address);
        //kprintf("%X", channels[channel].bmide + reg - 0x0E);

    }
    if (reg > 0x07 && reg < 0x0C) {
        int16_t ATA_REG_CONTROL_address = channels[channel].nIEN;
        ide_write(channel, ATA_REG_CONTROL, ATA_REG_CONTROL_address);
    }
    
    kprintf("%s%X", "0x", address);




    return result;
}



void ide_read_buffer(unsigned char channel, unsigned char reg, unsigned int buffer, unsigned
int quads) {
    if (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL, 0x80 |
                                                                      channels[channel].nIEN);
    asm volatile("pushw %es; movw %ds, %ax; movw %ax, %es");
    if (reg < 0x08) insl(channels[channel].base + reg - 0x00, buffer, quads);
    else if (reg < 0x0C) insl(channels[channel].base + reg - 0x06, buffer, quads);
    else if (reg < 0x0E) insl(channels[channel].ctrl + reg - 0x0A, buffer, quads);
    else if (reg < 0x16) insl(channels[channel].bmide + reg - 0x0E, buffer, quads);
    asm volatile("popw %es;");
    if (reg > 0x07 && reg < 0x0C) ide_write(channel, ATA_REG_CONTROL,
                                            channels[channel].nIEN);
}

unsigned char ide_polling(unsigned char channel, unsigned int advanced_check) {
    // (I) Delay 400 nanosecond for BSY to be set:
    // -------------------------------------------------
    ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
    ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
    ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
    ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
    // (II) Wait for BSY to be cleared:
    // -------------------------------------------------
    while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY); // Wait for BSY to be zero.
    if (advanced_check) {
        unsigned char state = ide_read(channel, ATA_REG_STATUS); // Read Status Register.
        // (III) Check For Errors:
        // -------------------------------------------------
        if (state & ATA_SR_ERR) return 2; // Error.
        // (IV) Check If Device fault:
        // -------------------------------------------------
        if (state & ATA_SR_DF ) return 1; // Device Fault.
        // (V) Check DRQ:
        // -------------------------------------------------
        // BSY = 0; DF = 0; ERR = 0 so we should check for DRQ now.
        if (!(state & ATA_SR_DRQ)) return 3; // DRQ should be set
    }
    return 0; // No Error.
}






void ide_initialize(unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int
BAR3,unsigned int BAR4) {
    int j, k, count = 0;


    // 1- Detect I/O Ports which interface IDE Controller:
    channels[ATA_PRIMARY ].base = (BAR0 &= 0xFFFFFFFC) + 0x1F0*(!BAR0);
    channels[ATA_PRIMARY ].ctrl = (BAR1 &= 0xFFFFFFFC) + 0x3F4*(!BAR1);
    channels[ATA_SECONDARY].base = (BAR2 &= 0xFFFFFFFC) + 0x170*(!BAR2);
    channels[ATA_SECONDARY].ctrl = (BAR3 &= 0xFFFFFFFC) + 0x374*(!BAR3);
    channels[ATA_PRIMARY ].bmide = (BAR4 &= 0xFFFFFFFC) + 0; // Bus Master IDE
    channels[ATA_SECONDARY].bmide = (BAR4 &= 0xFFFFFFFC) + 8; // Bus Master IDE


    terminal_writestring("works here\n");

    // 2- Disable IRQs:
    ide_write(ATA_PRIMARY , ATA_REG_CONTROL, 2);
    ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);

// 3- Detect ATA-ATAPI Devices:
    for (uint8_t i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {

            unsigned char err = 0, type = IDE_ATA, status;
            ide_devices[count].reserved = 0; // Assuming that no drive here.

            // (I) Select Drive:
            ide_write(i, ATA_REG_HDDEVSEL, (unsigned char) (0xA0 | (j << 4))); // Select Drive.
            time_sleep_ticks(1); // Wait 1ms for drive select to work.
            // (II) Send ATA Identify Command:

            ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

            time_sleep_ticks(1); // This function should be implemented in your OS. which waits for 1 ms. it is based on System Timer Device Driver.
            // (III) Polling:
            if (!(ide_read(i, ATA_REG_STATUS))) {continue ; kprintf("%s\n", "Error: no device");}; // If Status = 0, No Device.
            while(1) {
                kprintf("%s", "actual address read from: ");
                status = ide_read(i, ATA_REG_STATUS);
                kprintf("%c", '\n');
                if ( (status & ATA_SR_ERR)) {err = 1;     kprintf("%s","Error: SR_ERR\n"); break;} // If Err, Device is not ATA.
                if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) break; // Everything is right.
                break;
                kprintf("%s %x", "ide_read status as hex: ", status);
                kprintf("%s %c\n\n", ", ide_read status as char: ", status);

            }

            // (IV) Probe for ATAPI Devices:
            if (err) {
                unsigned char cl = ide_read(i,ATA_REG_LBA1);
                unsigned char ch = ide_read(i,ATA_REG_LBA2);
                if (cl == 0x14 && ch ==0xEB) type = IDE_ATAPI;
                else if (cl == 0x69 && ch ==0x96) type = IDE_ATAPI;
                else continue; // Unknown Type (And always not be a device).
                ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
                time_sleep_ticks(1);
            }

// (V) Read Identification Space of the Device:
            ide_read_buffer(i, ATA_REG_DATA, (unsigned int) ide_buf, 128);
            asm volatile("sti");

            // (VI) Read Device Parameters:
            ide_devices[count].reserved = 1;
            ide_devices[count].type = type;
            ide_devices[count].channel = i;
            ide_devices[count].drive = j;
            ide_devices[count].sign = ((unsigned short *) (ide_buf + ATA_IDENT_DEVICETYPE
            ))[0];
            ide_devices[count].capabilities = ((unsigned short *) (ide_buf +
                                                                   ATA_IDENT_CAPABILITIES ))[0];
            ide_devices[count].commandsets = ((unsigned int *) (ide_buf +
                                                                ATA_IDENT_COMMANDSETS ))[0];
            // (VII) Get Size:
            if (ide_devices[count].commandsets & (1<<26)){
                // Device uses 48-Bit Addressing:
                ide_devices[count].size = ((unsigned int *) (ide_buf + ATA_IDENT_MAX_LBA_EXT
                ))[0];
                // Note that Quafios is 32-Bit Operating System, So last 2 Words are ignored.
            } else {
                // Device uses CHS or 28-bit Addressing:
                ide_devices[count].size = ((unsigned int *) (ide_buf + ATA_IDENT_MAX_LBA
                ))[0];
            }
            // (VIII) String indicates model of device (like Western Digital HDD and SONY DVD-RW...):
            for(k = ATA_IDENT_MODEL; k < (ATA_IDENT_MODEL+40); k+=2) {
                ide_devices[count].model[k - ATA_IDENT_MODEL] = ide_buf[k+1];
                ide_devices[count].model[(k+1) - ATA_IDENT_MODEL] = ide_buf[k];}
            ide_devices[count].model[40] = 0; // Terminate String.
            count++;
        }
    // 4- Print Summary:
    for (uint8_t i = 0; i < 4; i++)
        if (ide_devices[i].reserved == 1) {
            /*kprintf(" Found %s Drive %iGB - %s\n", /// not the issue
                   (const char *[]){"ATA", "ATAPI"}[ide_devices[i].type], //Type
                    ide_devices[i].size, // Size
                    ide_devices[i].model);*/

            kprintf("%s %s %s %i %s", "Found", (const char *[]){"ATA", "ATAPI"}[ide_devices[i].type], "drive, (index ", i, ") ");
            kprintf("%s %i", "size (bytes):", ide_devices[i].size);
            kprintf("%s %s \n", ", model: ", ide_devices[i].model);

        }
}

void ide_print_register_debug_info(void) {
    for(int i = 0; i < 2; ++i) {
        kprintf("%s %i\n", "\nChannel: ", i);
        read_and_print(i, ATA_REG_DATA);
        read_and_print(i, ATA_REG_ERROR);
        read_and_print(i, ATA_REG_FEATURES);
        read_and_print(i, ATA_REG_SECCOUNT0);
        read_and_print(i, ATA_REG_LBA0);
        read_and_print(i, ATA_REG_LBA1);
        read_and_print(i, ATA_REG_LBA2);
        read_and_print(i, ATA_REG_HDDEVSEL);
        read_and_print(i, ATA_REG_COMMAND);
        read_and_print(i, ATA_REG_STATUS);
        read_and_print(i, ATA_REG_SECCOUNT1);
        read_and_print(i, ATA_REG_LBA3);
        read_and_print(i, ATA_REG_LBA4);
        read_and_print(i, ATA_REG_LBA5);
        read_and_print(i, ATA_REG_CONTROL);
        read_and_print(i, ATA_REG_ALTSTATUS);
        read_and_print(i, ATA_REG_DEVADDRESS);


    }
}

unsigned char ide_ata_access( unsigned char direction, unsigned char drive, unsigned int
lba, unsigned char numsects, unsigned short selector, unsigned int edi) {
    unsigned char lba_mode /* 0: CHS, 1:LBA28, 2: LBA48 */, dma /* 0: No DMA, 1: DMA */, cmd;
    unsigned char lba_io[6];
    unsigned int channel = ide_devices[drive].channel; // Read the Channel.
    unsigned int slavebit = ide_devices[drive].drive; // Read the Drive [Master/Slave]
    unsigned int bus = channels[channel].base; // The Bus Base, like [0x1F0] which is also data port.
    unsigned int words = 256; // Approximatly all ATA-Drives has sector-size of 512-byte.
    unsigned short cyl, i; unsigned char head, sect, err;

    ide_write((unsigned char) channel, ATA_REG_CONTROL, channels[channel].nIEN = (ide_irq_invoked = 0x0) + 0x02);

    
    // (I) Select one from LBA28, LBA48 or CHS;
    if (lba >= 0x10000000) { // Sure Drive should support LBA in this case, or you are giving a wrong LBA.
                // LBA48:
                lba_mode = 2;
        lba_io[0] = (lba & 0x000000FF)>> 0;
        lba_io[1] = (lba & 0x0000FF00)>> 8;
        lba_io[2] = (lba & 0x00FF0000)>>16;
        lba_io[3] = (lba & 0xFF000000)>>24;
        lba_io[4] = 0; // We said that we lba is integer, so 32-bit are enough to access 2TB.
        lba_io[5] = 0; // We said that we lba is integer, so 32-bit are enough to access 2TB.
        head = 0; // Lower 4-bits of HDDEVSEL are not used here.
    } else if (ide_devices[drive].capabilities & 0x200) { // Drive supports LBA?

        // LBA28:
        lba_mode = 1;
        lba_io[0] = (lba & 0x00000FF)>> 0;
        lba_io[1] = (lba & 0x000FF00)>> 8;
        lba_io[2] = (lba & 0x0FF0000)>>16;
        lba_io[3] = 0; // These Registers are not used here.
        lba_io[4] = 0; // These Registers are not used here.
        lba_io[5] = 0; // These Registers are not used here.
        head = (lba & 0xF000000)>>24;
    } else {
        // CHS:
        lba_mode = 0;
        sect = (lba % 63) + 1;
        cyl = (lba + 1 - sect)/(16*63);
        lba_io[0] = sect;
        lba_io[1] = (cyl>>0) & 0xFF;
        lba_io[2] = (cyl>>8) & 0xFF;
        lba_io[3] = 0;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head = (lba + 1 - sect)%(16*63)/(63); // Head number is written to HDDEVSEL lower 4-bits.
    }

// (II) See if Drive Supports DMA or not;
    dma = 0; // Supports or doesn't, we don't support !!!

// (III) Wait if the drive is busy;
    while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY); // Wait if Busy.

// (IV) Select Drive from the controller;
    if (lba_mode == 0) ide_write(channel, ATA_REG_HDDEVSEL, 0xA0 | (slavebit<<4) | head); // Select Drive CHS.
    else ide_write(channel, ATA_REG_HDDEVSEL, 0xE0 | (slavebit<<4) | head); // Select Drive LBA.

// (V) Write Parameters;
    if (lba_mode == 2) {
        ide_write(channel, ATA_REG_SECCOUNT1, 0);
        ide_write(channel, ATA_REG_LBA3, lba_io[3]);
        ide_write(channel, ATA_REG_LBA4, lba_io[4]);
        ide_write(channel, ATA_REG_LBA5, lba_io[5]);
    }
    ide_write(channel, ATA_REG_SECCOUNT0, numsects);
    ide_write(channel, ATA_REG_LBA0, lba_io[0]);

    ide_write(channel, ATA_REG_LBA1, lba_io[1]);
    ide_write(channel, ATA_REG_LBA2, lba_io[2]);

// (VI) Select the command and send it;
    // Routine that is followed:
    // If ( DMA & LBA48) DO_DMA_EXT;
    // If ( DMA & LBA28) DO_DMA_LBA;
    // If ( DMA & LBA28) DO_DMA_CHS;
    // If (!DMA & LBA48) DO_PIO_EXT;
    // If (!DMA & LBA28) DO_PIO_LBA;
    // If (!DMA & !LBA#) DO_PIO_CHS;

    Code:
    if (lba_mode == 0 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO;
    if (lba_mode == 1 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO;
    if (lba_mode == 2 && dma == 0 && direction == 0) cmd = ATA_CMD_READ_PIO_EXT;
    if (lba_mode == 0 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == 1 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == 2 && dma == 1 && direction == 0) cmd = ATA_CMD_READ_DMA_EXT;
    if (lba_mode == 0 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == 1 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == 2 && dma == 0 && direction == 1) cmd = ATA_CMD_WRITE_PIO_EXT;
    if (lba_mode == 0 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == 1 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == 2 && dma == 1 && direction == 1) cmd = ATA_CMD_WRITE_DMA_EXT;
    ide_write(channel, ATA_REG_COMMAND, cmd); // Send the Command.

    if (dma)
        if (direction == 0);
            // DMA Read.
        else; // DMA Write.
    else
    if (direction == 0)
        // PIO Read.
        for (i = 0; i < numsects; i++) {
            if ((err = ide_polling((unsigned char) channel, 1))) return err; // Polling, then set error and exit if there is.
            asm volatile("pushw %es");
            asm volatile("mov %%ax, %%es"::"a"(selector));
            asm volatile("rep insw"::"c"(words), "d"(bus), "D"(edi)); // Receive Data.
            asm volatile("popw %es");
            edi += (words*2);
        } else {
        // PIO Write.
        for (i = 0; i < numsects; i++) {

            ide_polling(channel, 0); // Polling.
            asm volatile("pushw %ds");
            asm volatile("mov %%ax, %%ds"::"a"(selector));
            asm volatile("rep outsw"::"c"(words), "d"(bus), "S"(edi)); // Send Data
            asm volatile("popw %ds");
            edi += (words*2);
        }
        ide_write(channel, ATA_REG_COMMAND, (char []) { ATA_CMD_CACHE_FLUSH,
                                                        ATA_CMD_CACHE_FLUSH,
                                                        ATA_CMD_CACHE_FLUSH_EXT}[lba_mode]);
        ide_polling(channel, 0); // Polling.
    }

    return 0; // Easy, ... Isn't it?
}

void ide_wait_irq(void) {
    while (!ide_irq_invoked);
    ide_irq_invoked = 0;
}

unsigned char ide_atapi_read(unsigned char drive, unsigned int lba, unsigned char numsects,
                             unsigned short selector, unsigned int edi) {

    unsigned int channel = ide_devices[drive].channel;
    unsigned int slavebit = ide_devices[drive].drive;
    unsigned int bus = channels[channel].base;

    unsigned int words = 2048 / 2; // Sector Size in Words, Almost All ATAPI Drives has a sector size of 2048 bytes.
    unsigned char err; int i;

// Enable IRQs:
    ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN = ide_irq_invoked = 0x0);

// (I): Setup SCSI Packet:
    // ------------------------------------------------------------------
    atapi_packet[ 0] = ATAPI_CMD_READ;
    atapi_packet[ 1] = 0x0;
    atapi_packet[ 2] = (lba>>24) & 0xFF;
    atapi_packet[ 3] = (lba>>16) & 0xFF;
    atapi_packet[ 4] = (lba>> 8) & 0xFF;
    atapi_packet[ 5] = (lba>> 0) & 0xFF;
    atapi_packet[ 6] = 0x0;
    atapi_packet[ 7] = 0x0;
    atapi_packet[ 8] = 0x0;
    atapi_packet[ 9] = numsects;
    atapi_packet[10] = 0x0;
    atapi_packet[11] = 0x0;

// (II): Select the Drive:
    // ------------------------------------------------------------------
    ide_write(channel, ATA_REG_HDDEVSEL, slavebit<<4);

// (III): Delay 400 nanosecond for select to complete:
    // ------------------------------------------------------------------
    ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
    ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
    ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
    ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.

    de:
    // (IV): Inform the Controller that we use PIO mode:
    // ------------------------------------------------------------------
    ide_write(channel, ATA_REG_FEATURES, 0); // PIO mode.

// (V): Tell the Controller the size of buffer:
    // ------------------------------------------------------------------
    ide_write(channel, ATA_REG_LBA1, (words * 2) & 0xFF); // Lower Byte of Sector Size.
    ide_write(channel, ATA_REG_LBA2, (words * 2)>>8); // Upper Byte of Sector Size.

// (VI): Send the Packet Command:
    // ------------------------------------------------------------------
    ide_write(channel, ATA_REG_COMMAND, ATA_CMD_PACKET); // Send the Command.

// (VII): Waiting for the driver to finish or invoke an error:
    // ------------------------------------------------------------------
    if (err = ide_polling(channel, 1)) return err; // Polling and return if error.



    // (VIII): Sending the packet data:
    // ------------------------------------------------------------------
    asm volatile("rep outsw"::"c"(6), "d"(bus), "S"(atapi_packet)); // Send Packet Data

// (IX): Recieving Data:
    // ------------------------------------------------------------------
    for (i = 0; i < numsects; i++) {
        ide_wait_irq(); // Wait for an IRQ.
        if (err = ide_polling(channel, 1)) return err; // Polling and return if error.
        asm volatile("pushw %es");
        asm volatile("mov %%ax, %%es"::"a"(selector));
        asm volatile("rep insw"::"c"(words), "d"(bus), "D"(edi));// Receive Data.
        asm volatile("popw %es");
        edi += (words*2);
    }

// (X): Waiting for an IRQ:
    // ------------------------------------------------------------------
    ide_wait_irq();
    // (XI): Waiting for BSY & DRQ to clear:
    // ------------------------------------------------------------------
    while (ide_read(channel, ATA_REG_STATUS) & (ATA_SR_BSY | ATA_SR_DRQ));
    return 0; // Easy, ... Isn't it?
}

unsigned char ide_print_error(unsigned int drive, unsigned char err) {

    if (err == 0) return err;
    terminal_writestring(" IDE:\n");
    if (err == 1) {terminal_writestring("- Device Fault\n "); err = 19;}
    else if (err == 2) {
        unsigned char st = ide_read(ide_devices[drive].channel, ATA_REG_ERROR);
        if (st & ATA_ER_AMNF) {kprintf("%s", "- No Address Mark Found\n "); err = 7;}
        if (st & ATA_ER_TK0NF) {kprintf("%s", "- No Media or Media Error\n "); err = 3;}
        if (st & ATA_ER_ABRT) {kprintf("%s", "- Command Aborted\n "); err = 20;}
        if (st & ATA_ER_MCR) {kprintf("%s", "- No Media or Media Error\n "); err = 3;}
        if (st & ATA_ER_IDNF) {kprintf("%s", "- ID mark not Found\n "); err = 21;}
        if (st & ATA_ER_MC) {kprintf("%s", "- No Media or Media Error\n "); err = 3;}
        if (st & ATA_ER_UNC) {kprintf("%s", "- Uncorrectable Data Error\n "); err = 22;}
        if (st & ATA_ER_BBK) {kprintf("%s", "- Bad Sectors\n "); err = 13;}
    } else if (err == 3) {kprintf("%s", "- Reads Nothing\n "); err = 23;}
    else if (err == 4) {kprintf("%s", "- Write Protected\n "); err = 8;}
    kprintf("- [%s %s] %s\n",
           (const char *[]){"Primary","Secondary"}[ide_devices[drive].channel],
            (const char *[]){"Master", "Slave"}[ide_devices[drive].drive],
    ide_devices[drive].model);
    return err;
}


void ide_read_sectors(unsigned char drive, unsigned char numsects, unsigned int lba, unsigned
short es, unsigned int edi) {
    // 1: Check if the drive presents:
    // ==================================
    if (drive > 3 || ide_devices[drive].reserved == 0) { package[0] = 0x1; // Drive Not Found!
    kprintf("%s\n", "Error: Drive Not Found");
}
    // 2: Check if inputs are valid:
    // ==================================
    else if (((lba + numsects) > ide_devices[drive].size) && (ide_devices[drive].type == IDE_ATA)) {
        package[0] = 0x2; // Seeking to invalid position.
        kprintf("%s\n", "Error: seeking to invalid position");
    }

// 3: Read in PIO Mode through Polling & IRQs:
        // ============================================
    else {
        unsigned char err;
        if (ide_devices[drive].type == IDE_ATA) {
            err = ide_ata_access(ATA_READ, drive, lba, numsects, es, edi);
            kprintf("%s\n", "type is IDE_ATA");

        }
        else if (ide_devices[drive].type == IDE_ATAPI) {
            for (uint64_t i = 0; i < numsects; i++)
                err = ide_atapi_read(drive, lba + i, 1, es, edi + (i * 2048));

            kprintf("%s\n", "type is IDE_ATAPI");

        }
        package[0] = ide_print_error(drive, err);
    }
}
// package[0] is an entry of array, this entry specifies the Error Code, you can replace that.


void ide_write_sectors(unsigned char drive, unsigned char numsects, unsigned int lba, unsigned
short es, unsigned int edi) {
    // 1: Check if the drive presents:
    // ==================================
    if (drive > 3 || ide_devices[drive].reserved == 0) package[0] = 0x1; // Drive Not Found!
    // 2: Check if inputs are valid:
    // ==================================
    else if (((lba + numsects) > ide_devices[drive].size) && (ide_devices[drive].type ==
                                                              IDE_ATA))
        package[0] = 0x2; // Seeking to invalid position.
        // 3: Read in PIO Mode through Polling & IRQs:
        // ============================================
    else {
        unsigned char err; ///shouldn't this be set to 0 by default?
        if (ide_devices[drive].type == IDE_ATA)
            err = ide_ata_access(ATA_WRITE, drive, lba, numsects, es, edi);

        else if (ide_devices[drive].type == IDE_ATAPI)
            err = 4; // Write-Protected.
            kprintf("%s\n", "error code 4: write protected");
        package[0] = ide_print_error(drive, err);
    }
}

void ide_atapi_eject(unsigned char drive) {
    unsigned int channel = ide_devices[drive].channel;
    unsigned int slavebit = ide_devices[drive].drive;
    unsigned int bus = channels[channel].base;
    unsigned int words = 2048 / 2; // Sector Size in Words.
    unsigned char err = 0;
    ide_irq_invoked = 0;
    // 1: Check if the drive presents:
    // ==================================
    if (drive > 3 || ide_devices[drive].reserved == 0) package[0] = 0x1; // Drive Not Found!
    // 2: Check if drive isn't ATAPI:
    // ==================================
    else if (ide_devices[drive].type == IDE_ATA) package[0] = 20; // Command Aborted.
        // 3: Eject ATAPI Driver:
        // ============================================
    else {
        // Enable IRQs:
        ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN = ide_irq_invoked = 0x0);
        // (I): Setup SCSI Packet:

        atapi_packet[ 0] = ATAPI_CMD_EJECT;
        atapi_packet[ 1] = 0x00;
        atapi_packet[ 2] = 0x00;
        atapi_packet[ 3] = 0x00;
        atapi_packet[ 4] = 0x02;
        atapi_packet[ 5] = 0x00;
        atapi_packet[ 6] = 0x00;
        atapi_packet[ 7] = 0x00;
        atapi_packet[ 8] = 0x00;
        atapi_packet[ 9] = 0x00;
        atapi_packet[10] = 0x00;
        atapi_packet[11] = 0x00;
        // (II): Select the Drive:
        // ------------------------------------------------------------------
        ide_write(channel, ATA_REG_HDDEVSEL, slavebit<<4);
        // (III): Delay 400 nanosecond for select to complete:
        // ------------------------------------------------------------------
        ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
        ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
        ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
        ide_read(channel, ATA_REG_ALTSTATUS); // Reading Alternate Status Port wastes 100ns.
        // (IV): Send the Packet Command:
        // ------------------------------------------------------------------
        ide_write(channel, ATA_REG_COMMAND, ATA_CMD_PACKET); // Send the Command.
        // (V): Waiting for the driver to finish or invoke an error:
        // ------------------------------------------------------------------
        if (err = ide_polling(channel, 1)); // Polling and stop if error.
            // (VI): Sending the packet data:
            // ------------------------------------------------------------------
        else {
            asm volatile("rep outsw"::"c"(6), "d"(bus), "S"(atapi_packet));// Send Packet Data
            ide_wait_irq(); // Wait for an IRQ.
            err = ide_polling(channel, 1); // Polling and get error code.
            if (err == 3) err = 0; // DRQ is not needed here.
        }
        package[0] = ide_print_error(drive, err); // Return;
    }
}


/*

void read_disk(uint32_t sector_lba_address, uint32_t num_of_sectors_to_read, char* buf) {
    terminal_writestring("started read\n");

    register uint32_t eax asm volatile("eax") = sector_lba_address;
    register uint32_t cl asm volatile("cl") = num_of_sectors_to_read;
    register char* edi asm volatile("edi") = buf;


   __asm volatile__ volatile(
                 ".intel_syntax noprefix\n"
                 "ata_lba_read:\n"
                 "               pushfd\n"
                 "               and  eax, 0x0FFFFFFF\n"
                 "               push  eax\n"
                 "               push  ebx\n"
                 "               push  ecx\n"
                 "               push  edx\n"
                 "               push  edi\n"
                 "\n"
                 "               mov  ebx,  eax         # Save LBA in RBX\n"
                 "\n"
                 "               mov  edx, 0x01F6      # Port to send drive and bit 24 - 27 of LBA\n"
                 "               shr  eax, 24          # Get bit 24 - 27 in al\n"
                 "               or al, 0b11100000     # Set bit 6 in al for LBA mode\n"
                 "               out dx, al\n"
                 "\n"
                 "               mov  edx, 0x01F2      # Port to send number of sectors\n"
                 "               mov al, cl           # Get number of sectors from CL\n"
                 "               out dx, al\n"
                 "\n"
                 "               mov  edx, 0x1F3       # Port to send bit 0 - 7 of LBA\n"
                 "               mov  eax,  ebx         # Get LBA from  eBX\n"
                 "               out dx, al\n"
                 "\n"
                 "               mov  edx, 0x1F4       # Port to send bit 8 - 15 of LBA\n"
                 "               mov  eax,  ebx         # Get LBA from  eBX\n"
                 "               shr  eax, 8           # Get bit 8 - 15 in AL\n"
                 "               out dx, al\n"
                 "\n"
                 "\n"
                 "               mov  edx, 0x1F5       # Port to send bit 16 - 23 of LBA\n"
                 "               mov  eax,  ebx         # Get LBA from  eBX\n"
                 "               shr  eax, 16          # Get bit 16 - 23 in AL\n"
                 "               out dx, al\n"
                 "\n"
                 "               mov  edx, 0x1F7       # Command port\n"
                 "               mov al, 0x20         # Read with retry.\n"
                 "               out dx, al\n"
                 "\n"
                 ".still_going:  in al, dx\n"
                 "               test al, 8           # the sector buffer requires servicing.\n"
                 "               jz .still_going      # until the sector buffer is ready.\n"
                 "\n"
                 "               mov  eax, 256         # to read 256 words = 1 sector\n"
                 "               xor bx, bx\n"
                 "               mov bl, cl           # read CL sectors\n"
                 "               mul bx\n"
                 "               mov  ecx,  eax         # RCX is counter for INSW\n"
                 "               mov  edx, 0x1F0       # Data port, in and out\n"
                 "               rep insw             # in to [RDI]\n"
                 "\n"
                 "               pop  edi\n"
                 "               pop  edx\n"
                 "               pop  ecx\n"
                 "               pop  ebx\n"
                 "               pop  eax\n"
                 "               popfd\n"
                 "               ret\n"
                 ".att_syntax\n"
                 :
                 : 
                 : "cl", "eax", "edi", "memory");



    terminal_writestring("finished read\n");
   //__asm volatile__ volatile("extern ata_lba_read\n" "jmp ata_lba_read");
    //   __asm volatile__ volatile(".include \"/home/dexter/Desktop/repos/RocketOS/src/storage/read_disk.s\"");
}

void write_disk(uint32_t sector_lba_address, uint32_t num_of_sectors_to_write, const char* const source_buf) {
    terminal_writestring("started write\n");

    register uint32_t eax asm volatile("eax") = sector_lba_address;
    register uint32_t cl asm volatile("cl") = num_of_sectors_to_write;
    register const char* const edi asm volatile("edi") = source_buf;
    
    __asm volatile__ volatile(".intel_syntax noprefix\n"
                     "ata_lba_write:\n"
                     "    pushfd\n"
                     "    and eax, 0x0FFFFFFF\n"
                     "    push eax\n"
                     "    push ebx\n"
                     "    push ecx\n"
                     "    push edx\n"
                     "    push edi\n"
                     "\n"
                     "    mov ebx, eax         # Save LBA in RBX\n"
                     "\n"
                     "    mov edx, 0x01F6      # Port to send drive and bit 24 - 27 of LBA\n"
                     "    shr eax, 24          # Get bit 24 - 27 in al\n"
                     "    or al, 0b11100000     # Set bit 6 in al for LBA mode\n"
                     "    out dx, al\n"
                     "\n"
                     "    mov edx, 0x01F2      # Port to send number of sectors\n"
                     "    mov al, cl           # Get number of sectors from CL\n"
                     "    out dx, al\n"
                     "\n"
                     "    mov edx, 0x1F3       # Port to send bit 0 - 7 of LBA\n"
                     "    mov eax, ebx         # Get LBA from EBX\n"
                     "    out dx, al\n"
                     "\n"
                     "    mov edx, 0x1F4       # Port to send bit 8 - 15 of LBA\n"
                     "    mov eax, ebx         # Get LBA from EBX\n"
                     "    shr eax, 8           # Get bit 8 - 15 in AL\n"
                     "    out dx, al\n"
                     "\n"
                     "\n"
                     "    mov edx, 0x1F5       # Port to send bit 16 - 23 of LBA\n"
                     "    mov eax, ebx         # Get LBA from EBX\n"
                     "    shr eax, 16          # Get bit 16 - 23 in AL\n"
                     "    out dx, al\n"
                     "\n"
                     "    mov edx, 0x1F7       # Command port\n"
                     "    mov al, 0x30         # Write with retry.\n"
                     "    out dx, al\n"
                     "\n"
                     ".write_still_going:  in al, dx\n"
                     "    test al, 8           # the sector buffer requires servicing.\n"
                     "    jz .write_still_going      # until the sector buffer is ready.\n"
                     "\n"
                     "    mov eax, 256         # to read 256 words = 1 sector\n"
                     "    xor bx, bx\n"
                     "    mov bl, cl           # write CL sectors\n"
                     "    mul bx\n"
                     "    mov ecx, eax         # eCX is counter for OUTSW\n"
                     "    mov edx, 0x1F0       # Data port, in and out\n"
                     "    mov esi, edi\n"
                     "    rep outsw            # out\n"
                     "\n"
                     "    pop edi\n"
                     "    pop edx\n"
                     "    pop ecx\n"
                     "    pop ebx\n"
                     "    pop eax\n"
                     "    popfd\n"
                     "    ret\n"
                     ".att_syntax\n"
                     :
                     :
                     :"cl", "eax", "edi", "memory");
    terminal_writestring("finished write\n"); 
}
/*
#define	SATA_SIG_ATA	0x00000101	// SATA drive
#define	SATA_SIG_ATAPI	0xEB140101	// SATAPI drive
#define	SATA_SIG_SEMB	0xC33C0101	// Enclosure management bridge
#define	SATA_SIG_PM	0x96690101	// Port multiplier

#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4

#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3

void start_cmd(HBA_PORT *port);

void stop_cmd(HBA_PORT *port);


static int check_type(HBA_PORT *port)
{
    uint32_t ssts = port->ssts;

    uint8_t ipm = (ssts >> 8) & 0x0F;
    uint8_t det = ssts & 0x0F;

    if (det != HBA_PORT_DET_PRESENT)	// Check drive status
        return AHCI_DEV_NULL;
    if (ipm != HBA_PORT_IPM_ACTIVE)
        return AHCI_DEV_NULL;

    switch (port->sig)
    {
        case SATA_SIG_ATAPI:
            return AHCI_DEV_SATAPI;
        case SATA_SIG_SEMB:
            return AHCI_DEV_SEMB;
        case SATA_SIG_PM:
            return AHCI_DEV_PM;
        default:
            return AHCI_DEV_SATA;
    }
}

    void probe_port(HBA_MEM *abar)
{
  // Search disk in implemented ports
  uint32_t pi = abar->pi;
  int i = 0;
  while (i<32)
  {
    if (pi & 1)
    {
      int dt = check_type(&abar->ports[i]);
      if (dt == AHCI_DEV_SATA)
      {
        trace_ahci("SATA drive found at port %d\n", i);
      }
      else if (dt == AHCI_DEV_SATAPI)
      {
        trace_ahci("SATAPI drive found at port %d\n", i);
      }
      else if (dt == AHCI_DEV_SEMB)
      {
        trace_ahci("SEMB drive found at port %d\n", i);
      }
      else if (dt == AHCI_DEV_PM)
      {
        trace_ahci("PM drive found at port %d\n", i);
      }
      else
      {
        trace_ahci("No drive found at port %d\n", i);
      }
    }

    pi >>= 1;
    i ++;
  }
}

// Check device type

#define	AHCI_BASE	0x400000	// 4M

#define HBA_PxCMD_ST    0x0001
#define HBA_PxCMD_FRE   0x0010
#define HBA_PxCMD_FR    0x4000
#define HBA_PxCMD_CR    0x8000

void port_rebase(HBA_PORT *port, int portno)
{
  stop_cmd(port);	// Stop command engine

  // Command list offset: 1K*portno
  // Command list entry size = 32
  // Command list entry maxim count = 32
  // Command list maxim size = 32*32 = 1K per port
  port->clb = AHCI_BASE + (portno<<10);
  port->clbu = 0;
  kmemset((void*)(port->clb), 0, 1024);

  // FIS offset: 32K+256*portno
  // FIS entry size = 256 bytes per port
  port->fb = AHCI_BASE + (32<<10) + (portno<<8);
  port->fbu = 0;
  kmemset((void*)(port->fb), 0, 256);

  // Command table offset: 40K + 8K*portno
  // Command table size = 256*32 = 8K per port
  HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(port->clb);
  for (int i=0; i<32; i++)
  {
    cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
    // 256 bytes per command table, 64+16+48+16*8
    // Command table offset: 40K + 8K*portno + cmdheader_index*256
    cmdheader[i].ctba = AHCI_BASE + (40<<10) + (portno<<13) + (i<<8);
    cmdheader[i].ctbau = 0;
    kmemset((void*)cmdheader[i].ctba, 0, 256);
  }

  start_cmd(port);	// Start command engine
}

// Start command engine
void start_cmd(HBA_PORT *port)
{
  // Wait until CR (bit15) is cleared
  while (port->cmd & HBA_PxCMD_CR)
    ;

  // Set FRE (bit4) and ST (bit0)
  port->cmd |= HBA_PxCMD_FRE;
  port->cmd |= HBA_PxCMD_ST;
}

// Stop command engine
void stop_cmd(HBA_PORT *port)
{
  // Clear ST (bit0)
  port->cmd &= ~HBA_PxCMD_ST;
op_cmd(HBA_PORT *port)
{
  // Clear ST (bit0)
  port->
  // Clear FRE (bit4)
  port->cmd &= ~HBA_PxCMD_FRE;

  // Wait until FR (bit14), CR (bit15) are cleared
  while(1)
  {
    if (port->cmd & HBA_PxCMD_FR)
      continue;
    if (port->cmd & HBA_PxCMD_CR)
      continue;
    break;
  }

}

// Find a free command list slot
int find_cmdslot(HBA_PORT *port)
{
    // If not set in SACT and CI, the slot is free
    uint32_t slots = (port->sact | port->ci);
    for (int i=0; i<cmdslots; i++)
    {
        if ((slots&1) == 0)
            return i;
        slots >>= 1;
    }
    trace_ahci("Cannot find free command list entry\n");
    return -1;
}

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

bool read(HBA_PORT *port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf)
{
  port->is = (uint32_t) -1;		// Clear pending interrupt bits
  int spin = 0; // Spin lock timeout counter
  int slot = find_cmdslot(port);
  if (slot == -1)
    return false;

  HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
  cmdheader += slot;
  cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(uint32_t);	// Command FIS size
  cmdheader->w = 0;		// Read from device
  cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;	// PRDT entries count

  HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
  kmemset(cmdtbl, 0, sizeof(HBA_CMD_TBL) +
                    (cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));

  // 8K bytes (16 sectors) per PRDT
  for (int i=0; i<cmdheader->prdtl-1; i++)
  {
    cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
    cmdtbl->prdt_entry[i].dbc = 8*1024-1;	// 8K bytes (this value should always be set to 1 less than the actual value)
    cmdtbl->prdt_entry[i].i = 1;
    buf += 4*1024;	// 4K words
    count -= 16;	// 16 sectors
  }
  // Last entry
  cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
  cmdtbl->prdt_entry[i].dbc = (count<<9)-1;	// 512 bytes per sector
  cmdtbl->prdt_entry[i].i = 1;

  // Setup command
  FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);

  cmdfis->fis_type = FIS_TYPE_REG_H2D;
  cmdfis->c = 1;	// Command
  cmdfis->command = ATA_CMD_READ_DMA_EX;

  cmdfis->lba0 = (uint8_t)startl;
  cmdfis->lba1 = (uint8_t)(startl>>8);
  cmdfis->lba2 = (uint8_t)(startl>>16);
  cmdfis->device = 1<<6;	// LBA mode

  cmdfis->lba3 = (uint8_t)(startl>>24);
  cmdfis->lba4 = (uint8_t)starth;
  cmdfis->lba5 = (uint8_t)(starth>>8);

  cmdfis->countl = count & 0xFF;
  cmdfis->counth = (count >> 8) & 0xFF;

  // The below loop waits until the port is no longer busy before issuing a new command
  while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
  {
    spin++;
  }
  if (spin == 1000000)
  {
    trace_ahci("Port is hung\n");
    return false;
  }

  port->ci = 1<<slot;	// Issue command

  // Wait for completion
  while (1)
  {
    // In some longer duration reads, it may be helpful to spin on the DPS bit
    // in the PxIS port field as well (1 << 5)
    if ((port->ci & (1<<slot)) == 0)
      break;
    if (port->is & HBA_PxIS_TFES)	// Task file error
    {
      trace_ahci("Read disk error\n");
      return false;
    }
  }

  // Check again
  if (port->is & HBA_PxIS_TFES)
  {
    trace_ahci("Read disk error\n");
    return false;
  }

  return true;
}
*/
