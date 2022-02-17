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

#include "pcie_bus.h"

#include "tar.h"


//TODO: remove all 64 bit integer types as they are bigger than a word size

typedef const char* str;

GENERATE_HASHMAP_DECLARATION(str, uint32_t)
GENERATE_HASHMAP_DEFINITION(str, uint32_t, str_comp)

multiboot_info_t* mb_info = NULL;

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
        mb_info = mboot_header;
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
    disable_interrupts();

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
    kprintf("AcpiLoadTables passed\n");
    status = AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to enable subsystem: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }
    kprintf("AcpiEnableSubsystem passed\n");
    status = AcpiInitializeObjects(ACPI_FULL_INITIALIZATION);
    if(ACPI_FAILURE(status)) {
        kprintf("acpica: Impossible to initialize objects: error: %s\n", AcpiGbl_ExceptionNames_Env[status]);
    }
    kprintf("AcpiInitializeObjects passed\n");

    kprintf("\nACPICA initialized\n\n\n");

/*
    AcpiEnterSleepStatePrep(5);
    kprintf("\tAcpiEnterSleepStatePrep(5); passed\n");
    disable_interrupts();
    kprintf("\tdisable_interrupts(); passed\n");
    AcpiEnterSleepState(5);

    kprintf("\nIt did not shutdown\n");
*/

    disable_interrupts();
#if 0
    ACPI_TABLE_RSDP* rsdp = AcpiOsMapMemory(AcpiOsGetRootPointer(), sizeof(ACPI_TABLE_RSDP));
    kprintf("rsdp: %p\n", AcpiOsGetRootPointer());
    print_all_tables(rsdp);
    ACPI_TABLE_MCFG* mcfg = AcpiOsMapMemory(find_mcfg(rsdp), sizeof(ACPI_TABLE_MCFG));
    kprintf("mcfg: %p\n", mcfg);
    for(uint32_t i = 0u; i < ACPI_NAMESEG_SIZE; ++i) {
        kprintf("%c", mcfg->Header.Signature[i]);
    }
    kprintf("\n");
    enumerate_pcie(mcfg);
    ACPI_TABLE_MADT* madt = AcpiOsMapMemory(find_madt(rsdp), sizeof(ACPI_TABLE_MADT));
    /*kprintf("madt: %p\n", madt);
    for(uint32_t i = 0u; i < ACPI_NAMESEG_SIZE; ++i) {
        kprintf("%c", madt->Header.Signature[i]);
    }
    kprintf("\n");*/
    detect_cores(madt);
    //print_cores_info();
    AcpiOsUnmapMemory(madt, sizeof(ACPI_TABLE_MADT));
    AcpiOsUnmapMemory(mcfg, sizeof(ACPI_TABLE_MCFG));
    AcpiOsUnmapMemory(rsdp, sizeof(ACPI_TABLE_RSDP));
    //brute_force_check_all_buses();
#endif
    identity_map_page((uint32_t)get_default_page_directory(), mb_info->mods_addr, PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);
    multiboot_module_t *const first_module = (multiboot_module_t*)mb_info->mods_addr;
    identity_map_page((uint32_t)get_default_page_directory(), first_module, PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);
    const uint32_t addr = first_module->mod_start;
    identity_map_page((uint32_t)get_default_page_directory(), addr, PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);

    kprintf("first_module: %p\n", first_module);
    kprintf("addr: %p\n", addr);
    kprintf("mb_info->mods_addr: %X\n", mb_info->mods_addr);
    kprintf("mb_info->mods_count: %u\n", mb_info->mods_count);

    parse_headers(addr);
    //print_file("bar.txt");

    enable_interrupts();


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
