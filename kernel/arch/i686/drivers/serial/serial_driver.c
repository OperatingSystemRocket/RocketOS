#include <drivers/serial/serial_driver.h>


bool serial_init(void) {
   outb(COM1 + 1, 0x00);    // Disable all interrupts
   outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(COM1 + 1, 0x00);    //                  (hi byte)
   outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   outb(COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

   if(inb(COM1 + 0) != 0xAE) return false;

   outb(COM1 + 4, 0x0F);
   return true;
}


uint8_t serial_received(void);

uint8_t serial_read(void);

uint8_t is_transmit_empty(void);

void serial_putchar(char a);

void serial_write(const char* text, size_t size);

void serial_writestring(const char* text);


