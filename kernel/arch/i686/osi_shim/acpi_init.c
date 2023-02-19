#include "acpi_init.h"


static const char* AcpiGbl_ExceptionNames_Env[] = {
    "AE_OK",
    "AE_ERROR",
    "AE_NO_ACPI_TABLES",
    "AE_NO_NAMESPACE",
    "AE_NO_MEMORY",
    "AE_NOT_FOUND",
    "AE_NOT_EXIST",
    "AE_ALREADY_EXISTS",
    "AE_TYPE",
    "AE_NULL_OBJECT",
    "AE_NULL_ENTRY",
    "AE_BUFFER_OVERFLOW",
    "AE_STACK_OVERFLOW",
    "AE_STACK_UNDERFLOW",
    "AE_NOT_IMPLEMENTED",
    "AE_SUPPORT",
    "AE_LIMIT",
    "AE_TIME",
    "AE_ACQUIRE_DEADLOCK",
    "AE_RELEASE_DEADLOCK",
    "AE_NOT_ACQUIRED",
    "AE_ALREADY_ACQUIRED",
    "AE_NO_HARDWARE_RESPONSE",
    "AE_NO_GLOBAL_LOCK",
    "AE_ABORT_METHOD",
    "AE_SAME_HANDLER",
    "AE_NO_HANDLER",
    "AE_OWNER_ID_LIMIT",
    "AE_NOT_CONFIGURED",
    "AE_ACCESS",
    "AE_IO_ERROR"
};

void acpi_init(void) {
    disable_interrupts();

    ACPI_STATUS status = AcpiInitializeSubsystem();
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to initialize subsystem: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }
    kprintf("before AcpiInitializeTables\n");
    status = AcpiInitializeTables(NULL, 16, true);
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to initialize tables: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }
    kprintf("before AcpiLoadTables\n");
    status = AcpiLoadTables();
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to load tables: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }
    kprintf("before AcpiEnableSubsystem\n");
    status = AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to enable subsystem: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }
    kprintf("before AcpiInitializeObjects\n");
    status = AcpiInitializeObjects(ACPI_FULL_INITIALIZATION);
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to initialize objects: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }
    kprintf("AcpiInitializeObjects passed\n");

    kprintf("\nACPICA initialized\n\n\n");

    disable_interrupts();
}
