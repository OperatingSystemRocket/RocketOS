#include <multiboot.h>


#include <stdbool.h>

#include "kstdio.h"
#include "kmath.h"
#include "interrupts.h"
#include "serial_driver.h"
#include "kassert.h"
#include "terminal_driver.h"

#include "physical_mem_allocator.h"
#include "paging.h"
#include "kstdlib.h"
#include "gdt.h"
#include "default_keyboard_logic.h"

#include "terminal_driver.h"
#include "default_terminal_functions.h"
#include "default_terminal_system.h"
#include "keyboard_callbacks.h"

#include "pci_bus.h"

#include "hashmap.h"
#include "default_hashmap_functions.h"

#include "acpi.h"


//TODO: remove all 64 bit integer types as they are bigger than a word size

typedef const char* str;

GENERATE_HASHMAP_DECLARATION(str, uint32_t)
GENERATE_HASHMAP_DEFINITION(str, uint32_t, str_comp)

void kernel_early(const uint32_t mboot_magic, const multiboot_info_t *const mboot_header) {
    (void) mboot_header; //needed for calling convention reasons, but currently unused

    if(serial_init()) { //fails if serial is faulty
        serial_writestring("Serial driver works\n");
    }

    terminal_context_initialize();

    default_keyboard_map_state_init();

    set_default_functions();

    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        terminal_context_writestring_color("Invalid Multiboot Magic!\n", VGA_COLOR_RED);
    } else {
        terminal_context_writestring("The multiboot structure was loaded properly\n");
    }
}

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

void kernel_main(void) {
    init_gdt();
    gdt_load();

    pic_init();
    isr_install();

    write_tss();

    initialize_kernel_memory();
    kdynamic_memory_init();

    scheduler_init();

    init_pit(1000, PIT_CHANNEL_0, ACCESS_MODE_LOBYTE_HIBYTE, PIT_MODE_SQUARE_WAVE_GENERATOR);


    ACPI_STATUS status = AcpiInitializeSubsystem();
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to initialize subsystem: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }
    status = AcpiInitializeTables(NULL, 16, true);
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to initialize tables: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }
    status = AcpiLoadTables();
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to load tables: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }
    status = AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to enable subsystem: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }
    status = AcpiInitializeObjects(ACPI_FULL_INITIALIZATION);
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to initialize objects: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }

    kprintf("\nACPICA initialized\n\n\n");

    AcpiEnterSleepStatePrep(5);
    kprintf("\tAcpiEnterSleepStatePrep(5); passed\n");
    disable_interrupts();
    kprintf("\tdisable_interrupts(); passed\n");
    AcpiEnterSleepState(5);

    kprintf("\nIt did not shutdown\n");

/*
    scheduler_init();

    pic_init();
    isr_install();

    write_tss();
*/

    struct default_terminal_context *const data = get_default_terminal_context();
    data->vga_context = get_default_vga_context();
    struct GET_OBSERVER_TYPENAME(key_message) *const observer = zeroed_out_kmalloc(sizeof(struct GET_OBSERVER_TYPENAME(key_message)));
    *observer = (struct GET_OBSERVER_TYPENAME(key_message)){ data, catch_keycode };
    ADD_OBSERVER(128, key_message, get_subject(), observer);
    default_context_terminal_start();
    enable_keyboard();

/*
    //jump_usermode();

    create_process(&example_function_task);
    create_process(&foo_function_task);


    //uint32_t count = 0u;
*/

    for(;;) {
        //kprintf("kernel.c with count: %u\n", count++);
        asm volatile("hlt");
    }
}
