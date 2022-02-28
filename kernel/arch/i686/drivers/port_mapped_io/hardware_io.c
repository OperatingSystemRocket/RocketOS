#include "hardware_io.h"


void outw(uint16_t _port, uint16_t _data);

uint16_t inw(uint16_t _port);

void io_wait(void);

void flush_tlb_single_page(uint32_t addr);
