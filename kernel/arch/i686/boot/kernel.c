
#include "multiboot.h"


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <kstdio.h>
#include <kmath.h>
#include <interrupts/interrupts.h>
#include <drivers/serial/serial_driver.h>
#include <kassert.h>
#include <subsystems/terminal/terminal_driver.h>

#include <buddy_memory_allocator.h>
#include <mem/paging.h>
#include <kstdlib.h>
#include <mem/global_virt_allocator.h>
#include <usermode/gdt.h>
#include <drivers/keyboard/default_keyboard_logic.h>

#include <subsystems/terminal/default_terminal_functions.h>
#include <subsystems/terminal/default_terminal_system.h>
#include <subsystems/terminal/keyboard_callbacks.h>

#include <drivers/pci/legacy_pci/pci_bus.h>

#include <utils/data_structures/hashmap/hashmap.h>
#include <utils/data_structures/hashmap/default_hashmap_functions.h>
#include <vector.h>
#include <string.h>

#include "../acpica/acpi.h"

#include <drivers/pci/pcie/pcie_bus.h>

#include <tar_fs/tar.h>

#include <scheduler.h>


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

void kernel_main(const uint32_t mboot_magic, const uint32_t mboot_header) {
    if(serial_init()) { //fails if serial is faulty
        serial_writestring("Serial driver works\n");
    }

    terminal_context_initialize();

    default_keyboard_map_state_init();

    set_default_functions();

    init_gdt();
    gdt_load();

    pic_init();
    isr_install();

    initialize_kernel_memory();
    serial_writestring("after initialize_kernel_memory\n");
    kdynamic_memory_init();
    serial_writestring("Kernel memory initialized\n");
    global_virt_allocator_init();

    if (mboot_magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        kprintf("Invalid Multiboot Magic!\n");
    } else {
        kprintf("The multiboot structure was loaded properly\n");
    }

    struct multiboot_tag_module* module = NULL;

    struct multiboot_tag* tag = (struct multiboot_tag*)(mboot_header + 8);
    kprintf("tag address: %p\n", tag);
    uint32_t size = *(uint32_t*)mboot_header;
    for(; tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag*)((multiboot_uint8_t*)tag  + ((tag->size+7) & ~7))) {
        if(tag->type == MULTIBOOT_TAG_TYPE_MODULE) {
            module = (struct multiboot_tag_module*)tag;
            kprintf("Module at 0x%X-0x%X. Command line [%s]\n",
                    module->mod_start,
                    module->mod_end,
                    module->cmdline);
        }
    }

    const uint32_t addr = module->mod_start;

    kprintf("first_module: %p\n", module);
    kprintf("addr: %p\n", addr);
    kprintf("module->mod_start: %X\n", module->mod_start);

    kprintf("Module at 0x%X-0x%X. Command line [%s]\n",
            module->mod_start,
            module->mod_end,
            module->cmdline);

    parse_headers(addr);
    print_file("bar.txt");
    kprintf("\n");
    enable_interrupts();

    //write_tss();

    //scheduler_init();

    //init_pit(1024, PIT_CHANNEL_0, ACCESS_MODE_LOBYTE_HIBYTE, PIT_MODE_SQUARE_WAVE_GENERATOR);
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

    write_tss();

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

    identity_map_page((uint32_t)get_default_page_directory(), mb_info->mods_addr, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    multiboot_module_t *const first_module = (multiboot_module_t*)mb_info->mods_addr;
    identity_map_page((uint32_t)get_default_page_directory(), first_module, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    const uint32_t addr = first_module->mod_start;
    identity_map_page((uint32_t)get_default_page_directory(), addr, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);

    kprintf("first_module: %p\n", first_module);
    kprintf("addr: %p\n", addr);
    kprintf("mb_info->mods_addr: %X\n", mb_info->mods_addr);
    kprintf("mb_info->mods_count: %u\n", mb_info->mods_count);

    parse_headers(addr);
    //print_file("bar.txt");
#endif



    struct default_terminal_context *const data = get_default_terminal_context();
    data->vga_context = get_default_vga_context();
    struct GET_OBSERVER_TYPENAME(key_message) *const observer = zeroed_out_kmalloc(sizeof(struct GET_OBSERVER_TYPENAME(key_message)));
    *observer = (struct GET_OBSERVER_TYPENAME(key_message)){ data, catch_keycode };
    ADD_OBSERVER(128, key_message, get_subject(), observer);
    default_context_terminal_start();
    enable_keyboard();

    enable_interrupts();


//    jump_usermode();


    //create_thread(&example_function_task);
    //create_thread(&foo_function_task);
    enable_interrupts();
    parse_elf_file("test_program");


    kprintf("before scheduler_init()\n");
    scheduler_init();
    vector_type(struct string) params = create_vector(sizeof(struct string));
    struct string first_str = string_new("foo");
    struct string second_str = string_new("bar");
    push_back(params, &first_str);
    push_back(params, &second_str);
    //scheduler_exec(string_new("test_program"), params);
    scheduler_start();
    //init_task();


    for(;;) {
        kprintf("in the main function\n");
        asm volatile("hlt");
    }
}
