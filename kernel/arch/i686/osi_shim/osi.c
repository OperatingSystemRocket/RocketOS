#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../acpica/acpi.h"

#include <bitmap_allocator.h>
#include <mem/initialize_kernel_memory.h>
#include <osi_virt_mem_allocator.h>
#include <paging.h>
#include <kstdlib.h>


#include <drivers/pit/pit.h>
#include <drivers/port_mapped_io/hardware_io.h>

#include <drivers/pci/legacy_pci/pci_bus.h>

//TODO: remove as this is for debugging only
#include <interrupts/interrupts.h>

//for debugging only:
#include <kstdio.h>




ACPI_STATUS AcpiOsInitialize() {
    if(!osi_virt_mem_allocator_init()) {
        return AE_ERROR;
    }
    return AE_OK;
}

ACPI_STATUS AcpiOsTerminate() {
    return AE_OK;
}

ACPI_PHYSICAL_ADDRESS AcpiOsGetRootPointer() {
    ACPI_PHYSICAL_ADDRESS ret = 0;
    AcpiFindRootPointer(&ret);
    return ret;
}

ACPI_STATUS AcpiOsPredefinedOverride(const ACPI_PREDEFINED_NAMES *PredefinedObject, ACPI_STRING *NewValue) {
    *NewValue = NULL;
    return AE_OK;
}

ACPI_STATUS AcpiOsTableOverride(ACPI_TABLE_HEADER *ExistingTable, ACPI_TABLE_HEADER **NewTable) {
    *NewTable = NULL;
    return AE_OK;
}


void* AcpiOsMapMemory(ACPI_PHYSICAL_ADDRESS PhysicalAddress, ACPI_SIZE Length) {
    const uint32_t length = (uint32_t)Length;
    const uint32_t physical_address = (uint32_t)PhysicalAddress;
    kprintf("length: %X, physical_address: %X\n", length, physical_address);
    kprintf("physical_address: %X, length: %X\n", physical_address, length);
    if(length == 0u) {
        kprintf("length is 0\n");
        return NULL;
    }


    const uint32_t physical_page_address = physical_address & (~(PAGE_SIZE-1u));
    const uint32_t physical_page_offset = physical_address - physical_page_address;

    const uint32_t adjusted_length = physical_page_offset + length; //takes into account the offset, which affects page boundary overlap

    const uint32_t physical_memory_size = (adjusted_length/PAGE_SIZE) + ((adjusted_length%PAGE_SIZE)>0u); //in pages, round up

    kprintf("physical_address: %X, length: %X\n", physical_address, length);
    kprintf("length: %X\n", length);
    kprintf("length: %X, physical_address: %X\n", length, physical_address);
    kprintf("adjusted_length: %X\n", adjusted_length);
    kprintf("physical_memory_size: %X\n", physical_memory_size);
    kprintf("length: %X, adjusted_length: %X, physical_memory_size: %X\n", length, adjusted_length, physical_memory_size);

    void *const virtual_range = osi_virt_mem_allocator_allocate_pages(physical_memory_size);
    kprintf("virtual_range: %X\n", virtual_range);
    if(virtual_range == NULL) {
        return NULL;
    }
    const uint32_t virtual_page_address = (uint32_t)virtual_range;

    kprintf("physical_memory_size in AcpiOsMapMemory(): %X\n", physical_memory_size);
    kprintf("virtual_page_address in AcpiOsMapMemory: %X\n", virtual_page_address);
    map_pages_in_kernel_addr(virtual_page_address, physical_page_address, physical_memory_size, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    kputs("finished map_pages_in_kernel_addr\n");
    return (void*)(virtual_page_address+physical_page_offset);
}

void AcpiOsUnmapMemory(void *where, ACPI_SIZE length) {
    if(where == NULL || length == 0u) {
        kprintf("length is 0\n");
        return;
    }


    const uint32_t where_address = (uint32_t)where;

    const uint32_t virtual_page_address = where_address & (~(PAGE_SIZE-1u));
    const uint32_t virtual_page_offset = where_address - virtual_page_address;

    const uint32_t adjusted_length = virtual_page_offset + length; //takes into account the offset, which affects page boundary overlap

    const uint32_t virtual_memory_size = (adjusted_length/PAGE_SIZE) + ((adjusted_length%PAGE_SIZE)>0u); //in pages, round up

    //for(uint32_t i = 0u; i < virtual_memory_size; ++i) {
    //    unmap_page_in_kernel_addr((void*)(virtual_page_address+(i*PAGE_SIZE)));
    //}
    unmap_pages_in_kernel_addr(virtual_page_address, virtual_memory_size);

    osi_virt_mem_allocator_free_pages((void*)virtual_page_address, virtual_memory_size);
}

ACPI_STATUS AcpiOsGetPhysicalAddress(void *LogicalAddress, ACPI_PHYSICAL_ADDRESS *PhysicalAddress) {
    *PhysicalAddress = get_physical_address_in_kernel_addr(LogicalAddress);
    return AE_OK;
}

void *AcpiOsAllocate(ACPI_SIZE Size) {
    return kmalloc(Size);
}

void AcpiOsFree(void *Memory) {
    kfree(Memory);
}

BOOLEAN AcpiOsReadable(void *Memory, ACPI_SIZE Length) {
    if(Memory == NULL || Memory == 0u) {
        return false;
    }

    const uint32_t where_address = (uint32_t)Memory;

    const uint32_t virtual_page_address = where_address & (~(PAGE_SIZE-1u));
    const uint32_t virtual_page_offset = where_address - virtual_page_address;

    const uint32_t adjusted_length = virtual_page_offset + Length; //takes into account the offset, which affects page boundary overlap

    const uint32_t virtual_memory_size = (adjusted_length/PAGE_SIZE) + ((adjusted_length%PAGE_SIZE)>0u); //in pages, round up

    for(uint32_t i = 0u; i < virtual_memory_size; ++i) {
        if(!is_readable_in_kernel_addr((void*)(virtual_page_address+(i*PAGE_SIZE)))) {
            return false;
        }
    }

    return true;
}

BOOLEAN AcpiOsWritable(void *Memory, ACPI_SIZE Length) {
    if(Memory == NULL || Memory == 0u) {
        return false;
    }

    const uint32_t where_address = (uint32_t)Memory;

    const uint32_t virtual_page_address = where_address & (~(PAGE_SIZE-1u));
    const uint32_t virtual_page_offset = where_address - virtual_page_address;

    const uint32_t adjusted_length = virtual_page_offset + Length; //takes into account the offset, which affects page boundary overlap

    const uint32_t virtual_memory_size = (adjusted_length/PAGE_SIZE) + ((adjusted_length%PAGE_SIZE)>0u); //in pages, round up

    for(uint32_t i = 0u; i < virtual_memory_size; ++i) {
        if(!is_writable_in_kernel_addr((void*)(virtual_page_address+(i*PAGE_SIZE)))) {
            return false;
        }
    }

    return true;
}

ACPI_STATUS AcpiOsPhysicalTableOverride(ACPI_TABLE_HEADER *ExistingTable, ACPI_PHYSICAL_ADDRESS *NewAddress, UINT32 *NewTableLength) {
    return AE_SUPPORT; //copied from the windows and unix osi shims
}

ACPI_STATUS AcpiOsExecute(ACPI_EXECUTE_TYPE Type, ACPI_OSD_EXEC_CALLBACK Function, void *Context) {
    Function(Context);
    return AE_OK;
}

ACPI_CPU_FLAGS AcpiOsAcquireLock(ACPI_SPINLOCK Handle) {
    return 0u;
}

void AcpiOsWaitEventsComplete(void) {}

void AcpiOsReleaseLock(ACPI_SPINLOCK Handle, ACPI_CPU_FLAGS Flags) {}

ACPI_THREAD_ID AcpiOsGetThreadId(void) {
    return 1u;
}

ACPI_STATUS AcpiOsWaitSemaphore(ACPI_SEMAPHORE Handle, UINT32 Units, UINT16 Timeout) {
    return AE_OK;
}

ACPI_STATUS AcpiOsCreateLock(ACPI_SPINLOCK *OutHandle) {
    return AE_OK;
}

void AcpiOsDeleteLock(ACPI_SPINLOCK Handle) {}

ACPI_STATUS AcpiOsSignalSemaphore(ACPI_SEMAPHORE Handle, UINT32 Units) {
    return AE_OK;
}

ACPI_STATUS AcpiOsInstallInterruptHandler(UINT32 InterruptNumber, ACPI_OSD_HANDLER ServiceRoutine, void *Context) {
    return AE_OK;
}

ACPI_STATUS AcpiOsCreateSemaphore(UINT32 MaxUnits, UINT32 InitialUnits, ACPI_SEMAPHORE *OutHandle) {
    return AE_OK;
}

ACPI_STATUS AcpiOsRemoveInterruptHandler(UINT32 InterruptNumber, ACPI_OSD_HANDLER Handler) {
    kprintf("AcpiOsRemoveInterruptHandler\n");
    return AE_OK;
}

ACPI_PRINTF_LIKE (1) void ACPI_INTERNAL_VAR_XFACE AcpiOsPrintf(const char *Format, ...) {
#if 0
    va_list pargs;

    va_start(pargs, Format);

    kprintf_implementation(Format, &pargs);
    kprintf("\n");

    va_end(pargs);
#endif
}

ACPI_STATUS AcpiOsDeleteSemaphore(ACPI_SEMAPHORE Handle) {
    return AE_OK;
}

void AcpiOsVprintf(const char *Format, va_list Args) {
#if 0
    kprintf_implementation(Format, &Args);
    kprintf("\n");
#endif
}

UINT64 AcpiOsGetTimer(void) {
    return (get_seconds()*ACPI_100NSEC_PER_SEC) + (get_us()*ACPI_100NSEC_PER_USEC);
}

ACPI_STATUS AcpiOsEnterSleep(UINT8 SleepState, UINT32 RegaValue, UINT32 RegbValue) {
    return AE_OK;
}

void AcpiOsStall(UINT32 Microseconds) {
    enable_interrupts();
    pit_wait_us(Microseconds);
    disable_interrupts();
}

ACPI_STATUS AcpiOsReadMemory(ACPI_PHYSICAL_ADDRESS Address, UINT64 *Value, UINT32 Width) {
    void *const logical_address = AcpiOsMapMemory(Address, Width);
    if(logical_address == NULL) {
        return AE_NOT_EXIST;
    }

    switch (Width) {
        case 8:
            *Value = *((volatile uint8_t*) logical_address);
            break;

        case 16:
            *Value = *((volatile uint16_t*) logical_address);
            break;

        case 32:
            *Value = *((volatile uint32_t*) logical_address);
            break;

        case 64:
            *Value = *((volatile uint64_t*) logical_address);
            break;

        default:
            AcpiOsUnmapMemory(logical_address, Width);
            return AE_BAD_PARAMETER;
    }

    AcpiOsUnmapMemory(logical_address, Width);

    return AE_OK;
}

ACPI_STATUS AcpiOsWriteMemory(ACPI_PHYSICAL_ADDRESS Address, UINT64 Value, UINT32 Width) {
    void *const logical_address = AcpiOsMapMemory(Address, Width);
    if(logical_address == NULL) {
        return AE_NOT_FOUND;
    }

    switch (Width) {
        case 8:
            *((volatile uint8_t*) logical_address) = Value;
            break;

        case 16:
            *((volatile uint16_t*) logical_address) = Value;
            break;

        case 32:
            *((volatile uint32_t*) logical_address) = Value;
            break;

        case 64:
            *((volatile uint64_t*) logical_address) = Value;
            break;

        default:
            AcpiOsUnmapMemory(logical_address, Width);
            return AE_BAD_PARAMETER;
    }

    AcpiOsUnmapMemory(logical_address, Width);

    return AE_OK;
}

ACPI_STATUS AcpiOsSignal(UINT32 Function, void *Info) {
    return AE_OK;
}

ACPI_STATUS AcpiOsReadPciConfiguration(ACPI_PCI_ID* PciId, UINT32 Reg, UINT64* Value, UINT32 Width) {
    if (PciId->Bus >= 256 || PciId->Device >= 32 || PciId->Function >= 8){
        return AE_BAD_PARAMETER;
    }

    switch (Width) {
        case 8:
            *((uint8_t*) Value) = pci_config_read_byte(PciId->Bus, PciId->Device, PciId->Function, Reg);
            break;

        case 16:
            *((uint16_t*) Value) = pci_config_read_word(PciId->Bus, PciId->Device, PciId->Function, Reg);
            break;

        case 32:
            *((uint32_t*) Value) = pci_config_read_long(PciId->Bus, PciId->Device, PciId->Function, Reg);
            break;

        default:
            return AE_BAD_PARAMETER;
    }

    return AE_OK;
}

ACPI_STATUS AcpiOsWritePciConfiguration(ACPI_PCI_ID *PciId, UINT32 Reg, UINT64 Value, UINT32 Width) {
    if (PciId->Bus >= 256 || PciId->Device >= 32 || PciId->Function >= 8){
        return AE_BAD_PARAMETER;
    }

    switch (Width) {
        case 8:
            pci_config_write_byte(PciId->Bus, PciId->Device, PciId->Function, Reg, Value);
            break;

        case 16:
            pci_config_write_word(PciId->Bus, PciId->Device, PciId->Function, Reg, Value);
            break;

        case 32:
            pci_config_write_long(PciId->Bus, PciId->Device, PciId->Function, Reg, Value);
            break;

        default:
            return AE_BAD_PARAMETER;
    }

    return AE_OK;
}

ACPI_STATUS AcpiOsReadPort(ACPI_IO_ADDRESS Address, UINT32 *Value, UINT32 Width) {
    switch(Width) {
        case 8:
            *Value = inb(Address);
            break;
        
        case 16:
            *Value = inw(Address);
            break;

        case 32:
            *Value = inl(Address);
            break;

        default:
            kprintf("Bad width parameter: %X\n", Width);
            return AE_BAD_PARAMETER;
    }

    return AE_OK;
}

ACPI_STATUS AcpiOsWritePort(ACPI_IO_ADDRESS Address, UINT32 Value, UINT32 Width) {
    switch(Width) {
        case 8:
            outb(Address, Value);
            break;

        case 16:
            outw(Address, Value);
            break;

        case 32:
            outl(Address, Value);
            break;

        default:
            kprintf("Bad width parameter: %X\n", Width);
            return AE_BAD_PARAMETER;
    }

    return AE_OK;
}

#define US_IN_MS 1000

void AcpiOsSleep(UINT64 Milliseconds) {
    enable_interrupts();
    pit_wait_us(Milliseconds*US_IN_MS);
    disable_interrupts();
}
