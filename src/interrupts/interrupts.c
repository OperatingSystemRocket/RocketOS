#include "interrupts.h"


#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)
#define PIC_EOI 0x20

#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10
#define ICW4_8086 0x01

static uint8_t pic1_mask = 0xff;
static uint8_t pic2_mask = 0xff;

#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define INTERRUPT_GATE 0x8e

enum IDTFlags {
    IDT_PRESENT = 1 << 7,
    IDT_RING_0 = 0 << 5,
    IDT_RING_1 = 1 << 5,
    IDT_RING_2 = 2 << 5,
    IDT_RING_3 = 3 << 5,
    IDT_SS = 1 << 4,
    IDT_INTERRUPT = 0xE,
    IDT_TRAP = 0xF,
};

struct interrupt_frame {
    uint16_t error_code;
    uint16_t reserved1;
    uint32_t eip;
    uint16_t cs;
    uint16_t reserved2;
    uint32_t eflags;
    uint32_t esp;
    uint16_t ss;
    uint16_t reserved3;
    uint16_t es;
    uint16_t reserved4;
    uint16_t ds;
    uint16_t reserved5;
    uint16_t fs;
    uint16_t reserved6;
    uint16_t gs;
    uint16_t reserved7;
};

struct IDT_entry{
    uint16_t offset_lowerbits;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_higherbits;
};

struct IDT_entry idt[256];
static uint32_t idt_address;
static uint32_t idt_ptr[2];

void idt_register_handler(const uint8_t interrupt, const uint32_t address) {
    idt[interrupt].offset_lowerbits = address & 0xffff;
    idt[interrupt].selector = KERNEL_CODE_SEGMENT_OFFSET;
    idt[interrupt].zero = 0;
    idt[interrupt].type_attr = INTERRUPT_GATE;
    idt[interrupt].offset_higherbits = (address & 0xffff0000) >> 16;
}

void pic_irq_enable(uint8_t no) {
	if (no >= 8) {
		no -= 8;
		pic2_mask &= ~(1 << no);
		outb(PIC2_DATA, pic2_mask);
	} else {
		pic1_mask &= ~(1 << no);
		outb(PIC1_DATA, pic1_mask);
	}
}

void pic_init(void) {
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, 0x20); // master offset 0x20
    io_wait();
    outb(PIC2_DATA, 0x28); // slave offset 0x28
    io_wait();
    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();
    outb(PIC1_DATA, pic1_mask);
    outb(PIC2_DATA, pic2_mask);
    pic_irq_enable(2);
}

__attribute__((interrupt)) static void isr0(struct interrupt_frame *const frame) {
    terminal_writestring("Division By Zero\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr1(struct interrupt_frame *const frame) {
    terminal_writestring("Debug\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr2(struct interrupt_frame *const frame) {
    terminal_writestring("Non Maskable Interrupt\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr3(struct interrupt_frame *const frame) {
    terminal_writestring("Breakpoint\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr4(struct interrupt_frame *const frame) {
    terminal_writestring("Into Detected Overflow\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr5(struct interrupt_frame *const frame) {
    terminal_writestring("Out of Bounds\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr6(struct interrupt_frame *const frame) {
    terminal_writestring("Invalid Opcode\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr7(struct interrupt_frame *const frame) {
    terminal_writestring("No Coprocessor\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr8(struct interrupt_frame *const frame) {
    terminal_writestring("Double Fault\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr9(struct interrupt_frame *const frame) {
    terminal_writestring("Coprocessor Segment Overrun\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr10(struct interrupt_frame *const frame) {
    terminal_writestring("Bad TSS\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr11(struct interrupt_frame *const frame) {
    terminal_writestring("Segment Not Present\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr12(struct interrupt_frame *const frame) {
    terminal_writestring("Stack Fault\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr13(struct interrupt_frame *const frame) {
    //terminal_writestring("General Protection Fault\n");
    kprintf("General Protection Fault\n");

    //terminal_writestring("heres some info:\n");

    kprintf("error_code: %X\n", frame->error_code);
    kprintf("eip: %X\n", frame->eip);
    kprintf("cs: %X\n", frame->cs);
    kprintf("eflags: %X\n", frame->eflags);
    kprintf("esp: %X\n", frame->esp);
    kprintf("ss: %X\n", frame->ss);
    kprintf("es: %X\n", frame->es);
    kprintf("ds: %X\n", frame->ds);
    kprintf("fs: %X\n", frame->fs);
    kprintf("gs: %X\n", frame->gs);

    asm("hlt");
}

__attribute__((interrupt)) static void isr14(struct interrupt_frame *const frame) {
    terminal_writestring("Page Fault\n");

    terminal_writestring("heres some info:\n");

    kprintf("address in cr2: %x\n", get_faulting_address());

    asm("hlt");
}

__attribute__((interrupt)) static void isr15(struct interrupt_frame *const frame) {
    terminal_writestring("Unknown Interrupt\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr16(struct interrupt_frame *const frame) {
    terminal_writestring("Coprocessor Fault\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr17(struct interrupt_frame *const frame) {
    terminal_writestring("Alignment Check\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr18(struct interrupt_frame *const frame) {
    terminal_writestring("Machine Check\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr_reserved(struct interrupt_frame *const frame) {
    terminal_writestring("Reserved\n");

    asm("hlt");
}


void pic_send_eoi(const uint8_t no) {
	if (no >= 8) {
		outb(PIC2_COMMAND, PIC_EOI);
	}
	outb(PIC1_COMMAND, PIC_EOI);
}

__attribute__((interrupt)) static void timer_irq(struct interrupt_frame *const frame) {
    increment_time();
    set_time_in_seconds();
    pic_send_eoi(1);
}

__attribute__((interrupt)) static void keyboard_irq(struct interrupt_frame *const frame) {
    const unsigned char scancode = inb(0x60);

    if (scancode & 128u) {
        // This is a release scancode, just ignore it
        if(scancode == 170) lshift = false;
        if(scancode == 182) rshift = false;
        pic_send_eoi(1);
        return;
    }

    process_keystroke(scancode);

	pic_send_eoi(1);
}


void idt_init(void) {
    idt_address = (uint32_t)idt;
    idt_ptr[0] =
        (sizeof(struct IDT_entry) * 256) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;

    asm volatile("lidt %0\n\t"
                        "sti\n\t"
                        :
                        : "m"(idt_ptr));
}

void enable_timer(void) {
    pic_irq_enable(0);
    idt_register_handler(32, (uint32_t)timer_irq);
}

void enable_keyboard(void) {
    pic_irq_enable(1);
    idt_register_handler(33, (uint32_t)keyboard_irq);
}

__attribute__((interrupt)) static void system_call(struct interrupt_frame *const frame) {
    terminal_writestring("system_call triggered\n");
}

void isr_install(void) {
    idt_register_handler(0, (uint32_t)isr0);
    idt_register_handler(1, (uint32_t)isr1);
    idt_register_handler(2, (uint32_t)isr2);
    idt_register_handler(3, (uint32_t)isr3);
    idt_register_handler(4, (uint32_t)isr4);
    idt_register_handler(5, (uint32_t)isr5);
    idt_register_handler(6, (uint32_t)isr6);
    idt_register_handler(7, (uint32_t)isr7);
    idt_register_handler(8, (uint32_t)isr8);
    idt_register_handler(9, (uint32_t)isr9);
    idt_register_handler(10, (uint32_t)isr10);
    idt_register_handler(11, (uint32_t)isr11);
    idt_register_handler(12, (uint32_t)isr12);
    idt_register_handler(13, (uint32_t)isr13);
    
    idt_register_handler(14, (uint32_t)isr14);
    idt[14].type_attr = IDT_PRESENT | IDT_RING_0 | IDT_INTERRUPT;

    idt_register_handler(15, (uint32_t)isr15);
    idt_register_handler(16, (uint32_t)isr16);
    idt_register_handler(17, (uint32_t)isr17);
    idt_register_handler(18, (uint32_t)isr18);
    idt_register_handler(19, (uint32_t)isr_reserved);
    idt_register_handler(20, (uint32_t)isr_reserved);
    idt_register_handler(21, (uint32_t)isr_reserved);
    idt_register_handler(22, (uint32_t)isr_reserved);
    idt_register_handler(23, (uint32_t)isr_reserved);
    idt_register_handler(24, (uint32_t)isr_reserved);
    idt_register_handler(25, (uint32_t)isr_reserved);
    idt_register_handler(26, (uint32_t)isr_reserved);
    idt_register_handler(27, (uint32_t)isr_reserved);
    idt_register_handler(28, (uint32_t)isr_reserved);
    idt_register_handler(29, (uint32_t)isr_reserved);
    idt_register_handler(30, (uint32_t)isr_reserved);
    idt_register_handler(31, (uint32_t)isr_reserved);

    idt_register_handler(128, (uint32_t)system_call); //system call, 0x80
    idt[128].type_attr = IDT_PRESENT | IDT_RING_3 | IDT_INTERRUPT;

    idt_init();
}
