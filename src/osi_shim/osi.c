#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "acpi.h"

#include "bitmap_allocator.h"
#include "initialize_kernel_memory.h"
#include "osi_memory_allocator.h"
#include "paging.h"
#include "kstdlib.h"


#include "pit.h"
#include "hardware_io.h"

//TODO: remove as this is for debugging only
#include "interrupts.h"

//for debugging only:
#include "kstdio.h"


static struct osi_memory_allocator acpica_memory_allocator;


ACPI_STATUS AcpiOsInitialize() {
    osi_memory_allocator_init(&acpica_memory_allocator, get_acpica_start(), get_acpica_size());
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

void *AcpiOsMapMemory(ACPI_PHYSICAL_ADDRESS PhysicalAddress, ACPI_SIZE Length) {
    if(Length == 0u) {
        kprintf("Length is 0\n");
        return NULL;
    }


    const uint32_t physical_page_address = PhysicalAddress & (~(PAGE_SIZE-1u));
    const uint32_t physical_page_offset = PhysicalAddress - physical_page_address;

    const uint32_t adjusted_length = physical_page_offset + Length; //takes into account the offset, which affects page boundary overlap

    const uint32_t physical_memory_size = (adjusted_length/PAGE_SIZE) + ((adjusted_length%PAGE_SIZE)>0u); //in pages, round up

    void *const virtual_range = osi_memory_allocator_allocate(&acpica_memory_allocator, physical_memory_size);
    if(virtual_range == NULL) {
        return NULL;
    }
    const uint32_t virtual_page_address = (uint32_t)virtual_range;
    for(uint32_t i = 0u; i < physical_memory_size; ++i) {
        map_page((void*)(virtual_page_address+(i*PAGE_SIZE)), (physical_page_address+(i*PAGE_SIZE)), PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    }
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

    for(uint32_t i = 0u; i < virtual_memory_size; ++i) {
        unmap_page((void*)(virtual_page_address+(i*PAGE_SIZE)));
    }

    osi_memory_allocator_free(&acpica_memory_allocator, (void*)virtual_page_address, virtual_memory_size);
}

ACPI_STATUS AcpiOsGetPhysicalAddress(void *LogicalAddress, ACPI_PHYSICAL_ADDRESS *PhysicalAddress) {
    *PhysicalAddress = get_physical_address(LogicalAddress);
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
        if(!is_readable((void*)(virtual_page_address+(i*PAGE_SIZE)))) {
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
        if(!is_writable((void*)(virtual_page_address+(i*PAGE_SIZE)))) {
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
    return AE_OK;
}

ACPI_PRINTF_LIKE (1) void ACPI_INTERNAL_VAR_XFACE AcpiOsPrintf(const char *Format, ...) {
    va_list pargs;

    va_start(pargs, Format);

    kprintf_implementation(Format, &pargs);
    kprintf("\n");

    va_end(pargs);
}

ACPI_STATUS AcpiOsDeleteSemaphore(ACPI_SEMAPHORE Handle) {
    return AE_OK;
}

void AcpiOsVprintf(const char *Format, va_list Args) {
    kprintf_implementation(Format, &Args);
    kprintf("\n");
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
    switch(Width) {
    case 8:
    case 16:
    case 32:
    case 64:
        *Value = 0u;
        break;

    default:
        return AE_BAD_PARAMETER;
    }

    return AE_OK;
}

ACPI_STATUS AcpiOsWriteMemory(ACPI_PHYSICAL_ADDRESS Address, UINT64 Value, UINT32 Width) {
    return AE_OK;
}

ACPI_STATUS AcpiOsSignal(UINT32 Function, void *Info) {
    return AE_OK;
}

ACPI_STATUS AcpiOsReadPciConfiguration(ACPI_PCI_ID *PciId, UINT32 Reg, UINT64 *Value, UINT32 Width) {
    *Value = 0u;
    return AE_OK;
}

ACPI_STATUS AcpiOsWritePciConfiguration(ACPI_PCI_ID *PciId, UINT32 Reg, UINT64 Value, UINT32 Width) {
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
