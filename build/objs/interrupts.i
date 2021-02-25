# 1 "src/interrupts.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "src/interrupts.c"
# 1 "src/interrupts.h" 1
       
# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint.h" 1 3 4
# 11 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint.h" 3 4
# 1 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint-gcc.h" 1 3 4
# 34 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint-gcc.h" 3 4

# 34 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint-gcc.h" 3 4
typedef signed char int8_t;


typedef short int int16_t;


typedef long int int32_t;


typedef long long int int64_t;


typedef unsigned char uint8_t;


typedef short unsigned int uint16_t;


typedef long unsigned int uint32_t;


typedef long long unsigned int uint64_t;




typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef long int int_least32_t;
typedef long long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef short unsigned int uint_least16_t;
typedef long unsigned int uint_least32_t;
typedef long long unsigned int uint_least64_t;



typedef int int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
typedef long long int int_fast64_t;
typedef unsigned int uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef long long unsigned int uint_fast64_t;




typedef long int intptr_t;


typedef long unsigned int uintptr_t;




typedef long long int intmax_t;
typedef long long unsigned int uintmax_t;
# 12 "/home/dexter/opt/cross/lib/gcc/i686-elf/10.2.0/include/stdint.h" 2 3 4
# 3 "src/interrupts.h" 2


# 4 "src/interrupts.h"
static inline void outb(uint16_t port, uint8_t val);

struct IDT_entry{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_entry IDT[256];
# 2 "src/interrupts.c" 2
# 16 "src/interrupts.c"
void irq0_handler(void) {
    outb(0x20, 0x20);
}

void irq1_handler(void) {
    outb(0x20, 0x20);
}

void irq2_handler(void) {
    outb(0x20, 0x20);
}

void irq3_handler(void) {
    outb(0x20, 0x20);
}

void irq4_handler(void) {
    outb(0x20, 0x20);
}

void irq5_handler(void) {
    outb(0x20, 0x20);
}

void irq6_handler(void) {
    outb(0x20, 0x20);
}

void irq7_handler(void) {
    outb(0x20, 0x20);
}

void irq8_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq9_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq10_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq11_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq12_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq13_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq14_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq15_handler(void) {
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );




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
    IDT[32].selector = 0x08;
    IDT[32].zero = 0;
    IDT[32].type_attr = 0x8e;
    IDT[32].offset_higherbits = (irq0_address & 0xffff0000) >> 16;

    irq1_address = (unsigned long)irq1;
    IDT[33].offset_lowerbits = irq1_address & 0xffff;
    IDT[33].selector = 0x08;
    IDT[33].zero = 0;
    IDT[33].type_attr = 0x8e;
    IDT[33].offset_higherbits = (irq1_address & 0xffff0000) >> 16;

    irq2_address = (unsigned long)irq2;
    IDT[34].offset_lowerbits = irq2_address & 0xffff;
    IDT[34].selector = 0x08;
    IDT[34].zero = 0;
    IDT[34].type_attr = 0x8e;
    IDT[34].offset_higherbits = (irq2_address & 0xffff0000) >> 16;

    irq3_address = (unsigned long)irq3;
    IDT[35].offset_lowerbits = irq3_address & 0xffff;
    IDT[35].selector = 0x08;
    IDT[35].zero = 0;
    IDT[35].type_attr = 0x8e;
    IDT[35].offset_higherbits = (irq3_address & 0xffff0000) >> 16;

    irq4_address = (unsigned long)irq4;
    IDT[36].offset_lowerbits = irq4_address & 0xffff;
    IDT[36].selector = 0x08;
    IDT[36].zero = 0;
    IDT[36].type_attr = 0x8e;
    IDT[36].offset_higherbits = (irq4_address & 0xffff0000) >> 16;

    irq5_address = (unsigned long)irq5;
    IDT[37].offset_lowerbits = irq5_address & 0xffff;
    IDT[37].selector = 0x08;
    IDT[37].zero = 0;
    IDT[37].type_attr = 0x8e;
    IDT[37].offset_higherbits = (irq5_address & 0xffff0000) >> 16;

    irq6_address = (unsigned long)irq6;
    IDT[38].offset_lowerbits = irq6_address & 0xffff;
    IDT[38].selector = 0x08;
    IDT[38].zero = 0;
    IDT[38].type_attr = 0x8e;
    IDT[38].offset_higherbits = (irq6_address & 0xffff0000) >> 16;

    irq7_address = (unsigned long)irq7;
    IDT[39].offset_lowerbits = irq7_address & 0xffff;
    IDT[39].selector = 0x08;
    IDT[39].zero = 0;
    IDT[39].type_attr = 0x8e;
    IDT[39].offset_higherbits = (irq7_address & 0xffff0000) >> 16;

    irq8_address = (unsigned long)irq8;
    IDT[40].offset_lowerbits = irq8_address & 0xffff;
    IDT[40].selector = 0x08;
    IDT[40].zero = 0;
    IDT[40].type_attr = 0x8e;
    IDT[40].offset_higherbits = (irq8_address & 0xffff0000) >> 16;

    irq9_address = (unsigned long)irq9;
    IDT[41].offset_lowerbits = irq9_address & 0xffff;
    IDT[41].selector = 0x08;
    IDT[41].zero = 0;
    IDT[41].type_attr = 0x8e;
    IDT[41].offset_higherbits = (irq9_address & 0xffff0000) >> 16;

    irq10_address = (unsigned long)irq10;
    IDT[42].offset_lowerbits = irq10_address & 0xffff;
    IDT[42].selector = 0x08;
    IDT[42].zero = 0;
    IDT[42].type_attr = 0x8e;
    IDT[42].offset_higherbits = (irq10_address & 0xffff0000) >> 16;

    irq11_address = (unsigned long)irq11;
    IDT[43].offset_lowerbits = irq11_address & 0xffff;
    IDT[43].selector = 0x08;
    IDT[43].zero = 0;
    IDT[43].type_attr = 0x8e;
    IDT[43].offset_higherbits = (irq11_address & 0xffff0000) >> 16;

    irq12_address = (unsigned long)irq12;
    IDT[44].offset_lowerbits = irq12_address & 0xffff;
    IDT[44].selector = 0x08;
    IDT[44].zero = 0;
    IDT[44].type_attr = 0x8e;
    IDT[44].offset_higherbits = (irq12_address & 0xffff0000) >> 16;

    irq13_address = (unsigned long)irq13;
    IDT[45].offset_lowerbits = irq13_address & 0xffff;
    IDT[45].selector = 0x08;
    IDT[45].zero = 0;
    IDT[45].type_attr = 0x8e;
    IDT[45].offset_higherbits = (irq13_address & 0xffff0000) >> 16;

    irq14_address = (unsigned long)irq14;
    IDT[46].offset_lowerbits = irq14_address & 0xffff;
    IDT[46].selector = 0x08;
    IDT[46].zero = 0;
    IDT[46].type_attr = 0x8e;
    IDT[46].offset_higherbits = (irq14_address & 0xffff0000) >> 16;

    irq15_address = (unsigned long)irq15;
    IDT[47].offset_lowerbits = irq15_address & 0xffff;
    IDT[47].selector = 0x08;
    IDT[47].zero = 0;
    IDT[47].type_attr = 0x8e;
    IDT[47].offset_higherbits = (irq15_address & 0xffff0000) >> 16;


    idt_address = (unsigned long)IDT ;
    idt_ptr[0] = (sizeof (struct IDT_entry) * 256) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16 ;



    load_idt(idt_ptr);

}
