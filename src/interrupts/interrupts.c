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


struct IDT_entry idt[256];
static uint32_t idt_address;
static uint32_t idt_ptr[2];

void idt_register_handler(const uint8_t interrupt, const uint32_t address) {
    idt[interrupt].offset_lowerbits = address & 0xffff;
    idt[interrupt].selector = KERNEL_CODE_SEGMENT_OFFSET;
    idt[interrupt].zero = 0;
    idt[interrupt].type_attr = INTERRUPT_GATE;
    idt[interrupt].offset_higherbits = (uint16_t)((address & 0xffff0000) >> 16);
}

void pic_irq_enable(uint8_t no) {
	if (no >= 8) {
		no -= 8;
		pic2_mask &= (uint8_t) ~(1u << no);
		outb(PIC2_DATA, pic2_mask);
	} else {
		pic1_mask &= (uint8_t) ~(1u << no);
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
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Division By Zero\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr1(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Debug\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr2(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Non Maskable Interrupt\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr3(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Breakpoint\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr4(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Into Detected Overflow\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr5(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Out of Bounds\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr6(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Invalid Opcode\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr7(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("No Coprocessor\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr8(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Double Fault\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr9(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Coprocessor Segment Overrun\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr10(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Bad TSS\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr11(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Segment Not Present\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr12(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Stack Fault\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr13(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("General Protection Fault\n");

    kprintf("heres some info:\n");

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
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Page Fault\n");

    kprintf("heres some info:\n");

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

    kprintf("address in cr2: %X\n", get_faulting_address());

    asm("hlt");
}

__attribute__((interrupt)) static void isr15(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Unknown Interrupt\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr16(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Coprocessor Fault\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr17(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Alignment Check\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr18(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Machine Check\n");

    asm("hlt");
}

__attribute__((interrupt)) static void isr_reserved(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("Reserved\n");

    asm("hlt");
}


void pic_send_eoi(const uint8_t no) {
	if (no >= 8) {
		outb(PIC2_COMMAND, PIC_EOI);
	}
	outb(PIC1_COMMAND, PIC_EOI);
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

__attribute__((interrupt)) static void system_call(struct interrupt_frame *const frame) {
    (void) frame; //silence unused parameter warning as this param is needed for hardware reasons

    kprintf("system_call triggered\n");
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

    enable_time();

    idt_register_handler(128, (uint32_t)system_call); //system call, 0x80
    idt[128].type_attr = IDT_PRESENT | IDT_RING_3 | IDT_INTERRUPT;

    idt_init();
}
