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

#include "hashmap.h"
#include "default_hashmap_functions.h"


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


    kprintf("\n0xFFFF: %u\n", 0xFFFFu);
    bool first_pci_entry = true;
    for(uint16_t bus = 0u; bus < 256u; ++bus) {
        for(uint8_t slot = 0u; slot < 32u; ++slot) {
            const uint8_t u8_bus = (uint8_t)bus;
            const uint16_t vendor = pci_config_read_word(u8_bus, slot, 0u, 0u);
            if(vendor != 0xFFFFu) {
                if(!first_pci_entry) {
                    kprintf("\n");
                }
                kprintf("bus: %u, slot: %u, vendor: %X, device: %X, header_type: %u, header_type_type: %u, header_type_mf: %u, subsystem_vendor_id: %X, subsystem_id: %X, revision_info: %u\n",
                    bus, slot, vendor, get_device(u8_bus, slot),
                    get_header_type(u8_bus, slot), get_header_type_header_type(u8_bus, slot), get_header_type_mf(u8_bus, slot),
                    get_subsystem_vendor_id(u8_bus, slot), get_subsystem_id(u8_bus, slot), pci_config_read_word(u8_bus, slot, 0u, 0x08u));
                first_pci_entry = false;
            }
        }
    }
    kprintf("vendor end\n\n");


    struct HASHMAP_STRUCT(str, uint32_t) hashmap;
    HASHMAP_INIT(str, uint32_t, &hashmap, &str_hash_function);

    HASHMAP_ADD(str, uint32_t, &hashmap, "Hello", 5);
    uint32_t *const find_hello_1 = HASHMAP_FIND(str, uint32_t, &hashmap, "Hello");

    HASHMAP_ADD(str, uint32_t, &hashmap, "foo", 4);
    uint32_t *const find_hello_2 = HASHMAP_FIND(str, uint32_t, &hashmap, "Hello");
    uint32_t *const find_foo_1 = HASHMAP_FIND(str, uint32_t, &hashmap, "foo");

    HASHMAP_ADD(str, uint32_t, &hashmap, "foo2", 3);
    uint32_t *const find_hello_3 = HASHMAP_FIND(str, uint32_t, &hashmap, "Hello");
    uint32_t *const find_foo_2 = HASHMAP_FIND(str, uint32_t, &hashmap, "foo");
    uint32_t *const find_foo2_1 = HASHMAP_FIND(str, uint32_t, &hashmap, "foo2");

    HASHMAP_ADD(str, uint32_t, &hashmap, "foo3", 7);
    uint32_t *const find_hello_4 = HASHMAP_FIND(str, uint32_t, &hashmap, "Hello");
    uint32_t *const find_foo_3 = HASHMAP_FIND(str, uint32_t, &hashmap, "foo");
    uint32_t *const find_foo2_2 = HASHMAP_FIND(str, uint32_t, &hashmap, "foo2");
    uint32_t *const find_foo3_1 = HASHMAP_FIND(str, uint32_t, &hashmap, "foo3");

    HASHMAP_ADD(str, uint32_t, &hashmap, "lsakdj", 1029);
    uint32_t *const find_hello_5 = HASHMAP_FIND(str, uint32_t, &hashmap, "Hello");
    uint32_t *const find_foo_4 = HASHMAP_FIND(str, uint32_t, &hashmap, "foo");
    uint32_t *const find_foo2_3 = HASHMAP_FIND(str, uint32_t, &hashmap, "foo2");
    uint32_t *const find_foo3_2 = HASHMAP_FIND(str, uint32_t, &hashmap, "foo3");
    uint32_t *const find_lsakdj_1 = HASHMAP_FIND(str, uint32_t, &hashmap, "lsakdj");

    kassert_void((*find_hello_1) == 5);
    kassert_void((*find_foo_1) == 4);
    kassert_void((*find_foo2_1) == 3);
    kassert_void((*find_foo3_1) == 7);
    kassert_void((*find_lsakdj_1) == 1029);

    kassert_void(find_hello_1 == find_hello_2);
    kassert_void(find_hello_2 == find_hello_3);
    kassert_void(find_hello_3 == find_hello_4);
    kassert_void(find_hello_4 == find_hello_5);

    kassert_void(find_foo_1 == find_foo_2);
    kassert_void(find_foo_2 == find_foo_3);
    kassert_void(find_foo_3 == find_foo_4);

    kassert_void(find_foo2_1 == find_foo2_2);
    kassert_void(find_foo2_2 == find_foo2_3);

    kassert_void(find_foo3_1 == find_foo3_2);

    HASHMAP_DESTROY(str, uint32_t, &hashmap);


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
