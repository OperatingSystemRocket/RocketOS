#include <usermode/gdt.h>


void test_user_function(void) {
    const volatile uint32_t ret0 = print_int(40);
    print_int(ret0);
    const volatile uint32_t ret1 = print_int(5);
    print_int(ret1);
    const volatile uint32_t ret2 = print_int(6);
    print_int(ret2);
    const volatile uint32_t ret3 = print_int(2);
    print_int(ret3);

    const volatile uint32_t ret4 = print("test");
    print_int(ret4);
    const volatile uint32_t ret5 = print("test2");
    print_int(ret5);
    const volatile uint32_t ret6 = print("test3");
    print_int(ret6);

    for(volatile int i = 0; ; ++i);
}

//extern void* stack_bottom;
//extern void* stack_top;


struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low_1;
    uint8_t base_low_2;
    uint8_t settings_flags; //bitset
    uint8_t size_flags; //bitset
    uint8_t base_high;
};

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

//binary literals are clearer/more readable than hex in this case
//`#pragma GCC diagnostic ignored "-Wpedantic"` does not work, hence why `__extension__` is used instead
struct gdt_entry gdt_entries[7] = {
    {0, 0, 0, 0, 0, 0},
    {0xffff, 0, 0, __extension__ 0b10011010, __extension__ 0b11001111, 0},
    {0xffff, 0, 0, __extension__ 0b10010010, __extension__ 0b11001111, 0},
    {0xffff, 0, 0, __extension__ 0b11111010, __extension__ 0b11011111, 0},
    {0xffff, 0, 0, __extension__ 0b11110010, __extension__ 0b11011111, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}
};
struct gdt_ptr gdt_entries_ptr = {
    (sizeof(struct gdt_entry) * 7) - 1,
    (uint32_t) &gdt_entries[0]
};


struct tss_entry_struct tss_entry;


void init_gdt(void) {
    const uint32_t base = (uint32_t) &tss_entry;
    const uint32_t limit = sizeof(struct tss_entry_struct);

    gdt_entries[5].limit_low = limit & 0xFFFF;
    gdt_entries[5].base_low_1 = base & 0xFFFF;
    gdt_entries[5].base_low_2 = (base & 0xFF0000) >> 16;
    gdt_entries[5].settings_flags = __extension__ 0b11101001;
    gdt_entries[5].size_flags = (uint8_t) (limit & 0xF0000);
    gdt_entries[5].base_high = (uint8_t) ((base & 0xFF000000) >> 24);
}

void write_tss(void) {
    kmemset(&tss_entry, 0, sizeof(struct tss_entry_struct));

    tss_entry.ss0 = 2*sizeof(struct gdt_entry);

    void *const physical_page = global_phys_allocator_allocate_page();
    void *const virtual_page = kernel_virt_allocator_allocate_page();
    map_page_in_kernel_addr(virtual_page, (uint32_t)physical_page, PT_PRESENT | PT_RW | PT_USER, PD_PRESENT | PD_RW | PD_USER);
    tss_entry.esp0 = ((uint32_t) virtual_page) + PAGE_SIZE;

    flush_tss();
}

void set_kernel_stack(const uint32_t stack) { // Used when an interrupt occurs
    tss_entry.esp0 = stack;
}
