#include "gdt.h"


void test_user_function(void) {
    //kprintf("test_user_function() called\n");
	//asm volatile("hlt");
	//volatile int32_t n = 0;
	//volatile int32_t y = 7;
	//volatile int32_t t = y/n;
	trigger_interrupt();
	for(volatile int i = 0; ;++i);
}

extern void* stack_bottom;
extern void* stack_top;


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

struct gdt_entry gdt_entries[7];
struct gdt_ptr gdt_entries_ptr;


struct tss_entry_struct {
	uint32_t prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
	uint32_t esp0;     // The stack pointer to load when changing to kernel mode.
	uint32_t ss0;      // The stack segment to load when changing to kernel mode.
	// Everything below here is unused.
	uint32_t esp1; // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
};


struct tss_entry_struct tss_entry;


void init_gdt(void) {
	kmemset(gdt_entries, 0, sizeof(gdt_entries));

	gdt_entries[1].limit_low = 0xffff;
	gdt_entries[1].settings_flags = 0b10011010;
	gdt_entries[1].size_flags = 0b11001111;

	gdt_entries[2].limit_low = 0xffff;
	gdt_entries[2].settings_flags = 0b10010010;
	gdt_entries[2].size_flags = 0b11001111;

	gdt_entries[3].limit_low = 0xffff;
	gdt_entries[3].settings_flags = 0b11111010;
	gdt_entries[3].size_flags = 0b11011111;

	gdt_entries[4].limit_low = 0xffff;
	gdt_entries[4].settings_flags = 0b11110010;
	gdt_entries[4].size_flags = 0b11011111;


	const uint32_t base = (uint32_t) &tss_entry;
    const uint32_t limit = sizeof(struct tss_entry_struct);

	gdt_entries[5].limit_low = limit & 0xFFFF;
	gdt_entries[5].base_low_1 = base & 0xFFFF;
	gdt_entries[5].base_low_2 = (base & 0xFF0000) >> 16;
	gdt_entries[5].settings_flags = 0b11101001;
	gdt_entries[5].size_flags = limit & 0xF0000;
	gdt_entries[5].base_high = (base & 0xFF000000) >> 24;


	gdt_entries_ptr.limit = (sizeof(struct gdt_entry) * 7) - 1;
	gdt_entries_ptr.base = &gdt_entries[0];
}

void write_tss(void) {
	kmemset(&tss_entry, 0, sizeof(struct tss_entry_struct));

	//may not be correct
	tss_entry.ss0 = 2*sizeof(struct gdt_entry);
	tss_entry.esp0 = ((uint32_t)&stack_bottom + 16384) - 4;

    flush_tss();
}

void set_kernel_stack(const uint32_t stack) { // Used when an interrupt occurs
	tss_entry.esp0 = stack;
}

