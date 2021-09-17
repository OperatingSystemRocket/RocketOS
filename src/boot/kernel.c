#include <multiboot.h>


#include <stdbool.h>

#include "kstdio.h"
#include "kmath.h"
#include "interrupts.h"
#include "time.h"
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


//TODO: remove all 64 bit integer types as they are bigger than a word size

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

void kernel_main(void) {
    init_gdt();
    gdt_load();

    allocate_init();
    paging_init();
    kdynamic_memory_init();

    scheduler_init();

    pic_init();
    isr_install();

    write_tss();


    kprintf("0xFFFF: %u\n", 0xFFFFu);
    for(uint16_t bus = 0u; bus < 256u; ++bus) {
        for(uint8_t slot = 0u; slot < 32u; ++slot) {
            const uint8_t u8_bus = (uint8_t)bus;
            const uint16_t vendor = pci_config_read_word(u8_bus, slot, 0u, 0u);
            if(vendor != 0xFFFFu) {
                kprintf("bus: %u, slot: %u, vendor: %X, device: %X, header_type: %u, header_type_type: %u, header_type_mf: %u, subsystem_vendor_id: %X, subsystem_id: %X, revision_info: %u\n\n",
                    bus, slot, vendor, get_device(u8_bus, slot),
                    get_header_type(u8_bus, slot), get_header_type_header_type(u8_bus, slot), get_header_type_mf(u8_bus, slot),
                    get_subsystem_vendor_id(u8_bus, slot), get_subsystem_id(u8_bus, slot), pci_config_read_word(u8_bus, slot, 0u, 0x08u));
            }
        }
    }
    kprintf("vendor end\n");


    struct default_terminal_context *const data = get_default_terminal_context();
    data->vga_context = get_default_vga_context();
    struct GET_OBSERVER_TYPENAME(key_message) *const observer = zeroed_out_kmalloc(sizeof(struct GET_OBSERVER_TYPENAME(key_message)));
    *observer = (struct GET_OBSERVER_TYPENAME(key_message)){ data, catch_keycode };
    ADD_OBSERVER(128, key_message, get_subject(), observer);
    default_context_terminal_start();
    enable_keyboard();


    //jump_usermode();

    create_process(&example_function_task);
    create_process(&foo_function_task);


    //uint32_t count = 0u;

    for(;;) {
        //kprintf("kernel.c with count: %u\n", count++);
        asm volatile("hlt");
    }
}
