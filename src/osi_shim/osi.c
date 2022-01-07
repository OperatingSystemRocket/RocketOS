#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "acpi.h"

#include "bitmap_allocator.h"
#include "initialize_kernel_memory.h"
#include "osi_memory_allocator.h"
#include "paging.h"
#include "kstdlib.h"


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
        return NULL;
    }

    const uint32_t physical_page_address = PhysicalAddress & PAGE_SIZE;
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
        return;
    }

    const uint32_t where_address = (uint32_t)where;

    const uint32_t virtual_page_address = where_address & PAGE_SIZE;
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

    const uint32_t virtual_page_address = where_address & PAGE_SIZE;
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

    const uint32_t virtual_page_address = where_address & PAGE_SIZE;
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
