#include "interrupts.h"

/*   /// from a different article, but they might be useful
read_port:
    mov edx, [esp + 4]
    in al, dx
    ret

write_port:
    mov   edx, [esp + 4]
    mov   al, [esp + 4 + 4]
    out   dx, al
    ret
*/

static inline void outb(uint16_t port, uint8_t val) {
  asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
  /*There's an outb %al, $imm8  encoding, for compile-time constant port numbers
   * that fit in 8b.  (N constraint). Wider immediate constants would be
   * truncated at assemble-time (e.g. "i" constraint). The  outb  %al, %dx
   * encoding is the only option for all other cases.
   * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we
   * had the port number a wider C type */
}

static inline char inb(uint16_t port) {
  char result = -1;
  asm volatile("inb %1, %0" : : "a"(result), "Nd"(port));
  return result;
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
static unsigned long idt_address;
static unsigned long idt_ptr[2];

void idt_register_handler(uint8_t interrupt, unsigned long address) {
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

__attribute__((interrupt)) static void isr0(struct interrupt_frame* frame) {
    terminal_writestring("Division By Zero\n");
}

__attribute__((interrupt)) static void isr1(struct interrupt_frame* frame) {
    terminal_writestring("Debug\n");
}

__attribute__((interrupt)) static void isr2(struct interrupt_frame* frame) {
    terminal_writestring("Non Maskable Interrupt\n");
}

__attribute__((interrupt)) static void isr3(struct interrupt_frame* frame) {
    terminal_writestring("Breakpoint\n");
}

__attribute__((interrupt)) static void isr4(struct interrupt_frame* frame) {
    terminal_writestring("Into Detected Overflow\n");
}

__attribute__((interrupt)) static void isr5(struct interrupt_frame* frame) {
    terminal_writestring("Out of Bounds\n");
}

__attribute__((interrupt)) static void isr6(struct interrupt_frame* frame) {
    terminal_writestring("Invalid Opcode\n");
}

__attribute__((interrupt)) static void isr7(struct interrupt_frame* frame) {
    terminal_writestring("No Coprocessor\n");
}

__attribute__((interrupt)) static void isr8(struct interrupt_frame* frame) {
    terminal_writestring("Double Fault\n");
}

__attribute__((interrupt)) static void isr9(struct interrupt_frame* frame) {
    terminal_writestring("Coprocessor Segment Overrun\n");
}

__attribute__((interrupt)) static void isr10(struct interrupt_frame* frame) {
    terminal_writestring("Bad TSS\n");
}

__attribute__((interrupt)) static void isr11(struct interrupt_frame* frame) {
    terminal_writestring("Segment Not Present\n");
}

__attribute__((interrupt)) static void isr12(struct interrupt_frame* frame) {
    terminal_writestring("Stack Fault\n");
}

__attribute__((interrupt)) static void isr13(struct interrupt_frame* frame) {
    terminal_writestring("General Protection Fault\n");

    terminal_writestring("heres some info:\n");

    kprintf("ip: %i\n", frame->ip);
    kprintf("sp: %i\n", frame->sp);
    kprintf("cs: %i\n", frame->cs);

    asm("hlt");
}

__attribute__((interrupt)) static void isr14(struct interrupt_frame* frame) {
    terminal_writestring("Page Fault\n");
}

__attribute__((interrupt)) static void isr15(struct interrupt_frame* frame) {
    terminal_writestring("Unknown Interrupt\n");
}

__attribute__((interrupt)) static void isr16(struct interrupt_frame* frame) {
    terminal_writestring("Coprocessor Fault\n");
}

__attribute__((interrupt)) static void isr17(struct interrupt_frame* frame) {
    terminal_writestring("Alignment Check\n");
}

__attribute__((interrupt)) static void isr18(struct interrupt_frame* frame) {
    terminal_writestring("Machine Check\n");
}

__attribute__((interrupt)) static void isr_reserved(struct interrupt_frame* frame) {
    terminal_writestring("Reserved\n");
}


void idt_init(void) {
  idt_address = (unsigned long)idt;
  idt_ptr[0] =
      (sizeof(struct IDT_entry) * 256) + ((idt_address & 0xffff) << 16);
  idt_ptr[1] = idt_address >> 16;

  asm volatile("lidt %0\n\t"
                       "sti\n\t"
                       :
                       : "m"(idt_ptr));
}

void isr_install(void) {
  idt_register_handler(0, (unsigned long)isr0);
  idt_register_handler(1, (unsigned long)isr1);
  idt_register_handler(2, (unsigned long)isr2);
  idt_register_handler(3, (unsigned long)isr3);
  idt_register_handler(4, (unsigned long)isr4);
  idt_register_handler(5, (unsigned long)isr5);
  idt_register_handler(6, (unsigned long)isr6);
  idt_register_handler(7, (unsigned long)isr7);
  idt_register_handler(8, (unsigned long)isr8);
  idt_register_handler(9, (unsigned long)isr9);
  idt_register_handler(10, (unsigned long)isr10);
  idt_register_handler(11, (unsigned long)isr11);
  idt_register_handler(12, (unsigned long)isr12);
  idt_register_handler(13, (unsigned long)isr13);
  idt_register_handler(14, (unsigned long)isr14);
  idt_register_handler(15, (unsigned long)isr15);
  idt_register_handler(16, (unsigned long)isr16);
  idt_register_handler(17, (unsigned long)isr17);
  idt_register_handler(18, (unsigned long)isr18);
  idt_register_handler(19, (unsigned long)isr_reserved);
  idt_register_handler(20, (unsigned long)isr_reserved);
  idt_register_handler(21, (unsigned long)isr_reserved);
  idt_register_handler(22, (unsigned long)isr_reserved);
  idt_register_handler(23, (unsigned long)isr_reserved);
  idt_register_handler(24, (unsigned long)isr_reserved);
  idt_register_handler(25, (unsigned long)isr_reserved);
  idt_register_handler(26, (unsigned long)isr_reserved);
  idt_register_handler(27, (unsigned long)isr_reserved);
  idt_register_handler(28, (unsigned long)isr_reserved);
  idt_register_handler(29, (unsigned long)isr_reserved);
  idt_register_handler(30, (unsigned long)isr_reserved);
  idt_register_handler(31, (unsigned long)isr_reserved);

  idt_init();
}
