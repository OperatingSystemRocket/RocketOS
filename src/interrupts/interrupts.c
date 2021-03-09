#include "interrupts.h"

static inline void outb(const uint16_t port, const uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
    /*There's an outb %al, $imm8  encoding, for compile-time constant port numbers
    * that fit in 8b.  (N constraint). Wider immediate constants would be
    * truncated at assemble-time (e.g. "i" constraint). The  outb  %al, %dx
    * encoding is the only option for all other cases.
    * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we
    * had the port number a wider C type */
}

static inline uint8_t inb(const uint16_t port) {
	uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline void io_wait(void) {
    asm volatile("outb %%al, $0x80" : : "a"(0));
}

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

struct interrupt_frame {
    uintptr_t ip;
    uintptr_t cs;
    uintptr_t flags;
    uintptr_t sp;
    uintptr_t ss;
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
    terminal_writestring("General Protection Fault\n");

    terminal_writestring("heres some info:\n");

    kprintf("ip: %i\n", frame->ip);
    kprintf("sp: %i\n", frame->sp);
    kprintf("cs: %i\n", frame->cs);

    asm("hlt");
}

__attribute__((interrupt)) static void isr14(struct interrupt_frame *const frame) {
    terminal_writestring("Page Fault\n");

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

__attribute__((interrupt)) static void timer_irq(struct interrupt_frame* frame) {
    terminal_cursor_blink(false);
    pic_send_eoi(1);
}

__attribute__((interrupt)) static void keyboard_irq(struct interrupt_frame* frame) {
    unsigned char scancode = inb(0x60);

    if (scancode & 128u) {
        // This is a release scancode, just ignore it
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

    idt_init();
}
