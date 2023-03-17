
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

#include "acpi_init.h"

#include <drivers/pci/pcie/pcie_bus.h>

#include <tar_fs/tar.h>

#include <scheduler.h>


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


    gdt_init();
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


    acpi_init();

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
    //scheduler_exec(string_new("test_program"), NULL);
    kputs("before test_program");
    enable_interrupts();
    //parse_elf_file("test_program");
    scheduler_load_process_in_new_address_space("test_program");
    kputs("after test_program");
    scheduler_start();

    for(;;) {
        kprintf("in the main function\n");
        asm volatile("hlt");
    }
}
