#include"interrupts.h"

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

void irq0_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq1_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq2_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq3_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq4_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq5_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq6_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq7_handler(void) {
    outb(0x20, 0x20); //EOI
}

void irq8_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq9_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq10_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq11_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq12_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq13_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq14_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq15_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
     /*There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

void idt_init(void) {
    extern int load_idt();
    extern int irq0();
    extern int irq1();
    extern int irq2();
    extern int irq3();
    extern int irq4();
    extern int irq5();
    extern int irq6();
    extern int irq7();
    extern int irq8();
    extern int irq9();
    extern int irq10();
    extern int irq11();
    extern int irq12();
    extern int irq13();
    extern int irq14();
    extern int irq15();

    unsigned long irq0_address;
    unsigned long irq1_address;
    unsigned long irq2_address;
    unsigned long irq3_address;
    unsigned long irq4_address;
    unsigned long irq5_address;
    unsigned long irq6_address;
    unsigned long irq7_address;
    unsigned long irq8_address;
    unsigned long irq9_address;
    unsigned long irq10_address;
    unsigned long irq11_address;
    unsigned long irq12_address;
    unsigned long irq13_address;
    unsigned long irq14_address;
    unsigned long irq15_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    // remapping the PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 40);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    irq0_address = (unsigned long)irq0;
    IDT[32].offset_lowerbits = irq0_address & 0xffff;
    IDT[32].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[32].zero = 0;
    IDT[32].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[32].offset_higherbits = (irq0_address & 0xffff0000) >> 16;

    irq1_address = (unsigned long)irq1;
    IDT[33].offset_lowerbits = irq1_address & 0xffff;
    IDT[33].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[33].zero = 0;
    IDT[33].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[33].offset_higherbits = (irq1_address & 0xffff0000) >> 16;

    irq2_address = (unsigned long)irq2;
    IDT[34].offset_lowerbits = irq2_address & 0xffff;
    IDT[34].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[34].zero = 0;
    IDT[34].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[34].offset_higherbits = (irq2_address & 0xffff0000) >> 16;

    irq3_address = (unsigned long)irq3;
    IDT[35].offset_lowerbits = irq3_address & 0xffff;
    IDT[35].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[35].zero = 0;
    IDT[35].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[35].offset_higherbits = (irq3_address & 0xffff0000) >> 16;

    irq4_address = (unsigned long)irq4;
    IDT[36].offset_lowerbits = irq4_address & 0xffff;
    IDT[36].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[36].zero = 0;
    IDT[36].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[36].offset_higherbits = (irq4_address & 0xffff0000) >> 16;

    irq5_address = (unsigned long)irq5;
    IDT[37].offset_lowerbits = irq5_address & 0xffff;
    IDT[37].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[37].zero = 0;
    IDT[37].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[37].offset_higherbits = (irq5_address & 0xffff0000) >> 16;

    irq6_address = (unsigned long)irq6;
    IDT[38].offset_lowerbits = irq6_address & 0xffff;
    IDT[38].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[38].zero = 0;
    IDT[38].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[38].offset_higherbits = (irq6_address & 0xffff0000) >> 16;

    irq7_address = (unsigned long)irq7;
    IDT[39].offset_lowerbits = irq7_address & 0xffff;
    IDT[39].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[39].zero = 0;
    IDT[39].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[39].offset_higherbits = (irq7_address & 0xffff0000) >> 16;

    irq8_address = (unsigned long)irq8;
    IDT[40].offset_lowerbits = irq8_address & 0xffff;
    IDT[40].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[40].zero = 0;
    IDT[40].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[40].offset_higherbits = (irq8_address & 0xffff0000) >> 16;

    irq9_address = (unsigned long)irq9;
    IDT[41].offset_lowerbits = irq9_address & 0xffff;
    IDT[41].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[41].zero = 0;
    IDT[41].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[41].offset_higherbits = (irq9_address & 0xffff0000) >> 16;

    irq10_address = (unsigned long)irq10;
    IDT[42].offset_lowerbits = irq10_address & 0xffff;
    IDT[42].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[42].zero = 0;
    IDT[42].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[42].offset_higherbits = (irq10_address & 0xffff0000) >> 16;

    irq11_address = (unsigned long)irq11;
    IDT[43].offset_lowerbits = irq11_address & 0xffff;
    IDT[43].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[43].zero = 0;
    IDT[43].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[43].offset_higherbits = (irq11_address & 0xffff0000) >> 16;

    irq12_address = (unsigned long)irq12;
    IDT[44].offset_lowerbits = irq12_address & 0xffff;
    IDT[44].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[44].zero = 0;
    IDT[44].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[44].offset_higherbits = (irq12_address & 0xffff0000) >> 16;

    irq13_address = (unsigned long)irq13;
    IDT[45].offset_lowerbits = irq13_address & 0xffff;
    IDT[45].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[45].zero = 0;
    IDT[45].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[45].offset_higherbits = (irq13_address & 0xffff0000) >> 16;

    irq14_address = (unsigned long)irq14;
    IDT[46].offset_lowerbits = irq14_address & 0xffff;
    IDT[46].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[46].zero = 0;
    IDT[46].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[46].offset_higherbits = (irq14_address & 0xffff0000) >> 16;

    irq15_address = (unsigned long)irq15;
    IDT[47].offset_lowerbits = irq15_address & 0xffff;
    IDT[47].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
    IDT[47].zero = 0;
    IDT[47].type_attr = 0x8e; // INTERRUPT_GATE  
    IDT[47].offset_higherbits = (irq15_address & 0xffff0000) >> 16;

    // fill the IDT descriptor  
    idt_address = (unsigned long)IDT ;
    idt_ptr[0] = (sizeof (struct IDT_entry) * 256) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16 ;



    load_idt(idt_ptr);

}
 