
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
#include <paging.h>
#include <kstdlib.h>
#include <initialize_kernel_memory.h>
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

extern uint32_t boot_page_directory;
extern uint32_t boot_page_table;

extern uint32_t upper_memory;


void kernel_main(const uint32_t mboot_magic, const uint32_t mboot_header) {
    if(serial_init()) { //fails if serial is faulty
        serial_writestring("Serial driver works\n");
    }

    for(uint32_t i = 0u; i < 1024u; ++i) {
        kprintf("boot_page_directory[%u]: %p\n", i, (&boot_page_directory)[i]);
    }

    for(uint32_t i = 0u; i < 1024u; ++i) {
        kprintf("boot_page_table[%u]: %p\n", i, (&boot_page_table)[i]);
    }

    kprintf("upper_memory: %p\n", &upper_memory);


    init_gdt();
    gdt_load();

    pic_init();
    isr_install();

    initialize_kernel_memory();
    kdynamic_memory_init();

    terminal_context_initialize();
    default_keyboard_map_state_init();
    set_default_functions();

    serial_writestring("after interrupts installed\n");

    serial_writestring("Kernel memory initialized\n");

    kprintf("mboot_magic: %X\n", mboot_magic);
    if (mboot_magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        kprintf("Invalid Multiboot Magic!\n");
    } else {
        kprintf("The multiboot structure was loaded properly\n");
    }

    struct multiboot_tag_module* module = NULL;

    struct multiboot_tag* tag = (struct multiboot_tag*)(mboot_header + 8);
    identity_map_page_in_kernel_addr((uint32_t)mboot_header & PAGE_BITMASK, PT_PRESENT, PD_PRESENT);
    identity_map_page_in_kernel_addr((uint32_t)tag & PAGE_BITMASK, PT_PRESENT, PD_PRESENT);
    kprintf("tag address: %p\n", tag);
    uint32_t size = *(uint32_t*)mboot_header;
    for(; tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag*)((multiboot_uint8_t*)tag  + ((tag->size+7) & ~7))) {
        identity_map_page_in_kernel_addr((uint32_t)tag & PAGE_BITMASK, PT_PRESENT, PD_PRESENT);
        if(tag->type == MULTIBOOT_TAG_TYPE_MODULE) {
            module = (struct multiboot_tag_module*)tag;
            kprintf("Module at 0x%X-0x%X. Command line [%s]\n",
                    module->mod_start,
                    module->mod_end,
                    module->cmdline);
        }
    }

    const uint32_t addr = module->mod_start;
    identity_map_page_in_kernel_addr(addr & PAGE_BITMASK, PT_PRESENT, PD_PRESENT);

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

    write_tss();


    struct default_terminal_context *const data = get_default_terminal_context();
    data->vga_context = get_default_vga_context();
    struct GET_OBSERVER_TYPENAME(key_message) *const observer = zeroed_out_kmalloc(sizeof(struct GET_OBSERVER_TYPENAME(key_message)));
    *observer = (struct GET_OBSERVER_TYPENAME(key_message)){ data, catch_keycode };
    ADD_OBSERVER(128, key_message, get_subject(), observer);
    default_context_terminal_start();
    enable_keyboard();


    kprintf("before scheduler_init()\n");
    scheduler_init();
    scheduler_exec(string_new("test_program"), NULL);
    enable_interrupts();
    scheduler_start();

    for(;;) {
        kprintf("in the main function\n");
        asm volatile("hlt");
    }
}
