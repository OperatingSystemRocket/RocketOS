#pragma once
#include "stdint.h"
#include "stdbool.h"
#include "kstring.h"
#include "time.h"



void ide_print_register_debug_info(void);

void ide_soft_reset(void);

unsigned char ide_print_error(unsigned int drive, unsigned char err);

void ide_initialize(unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int
BAR3,unsigned int BAR4);

unsigned char ide_atapi_read(unsigned char drive, unsigned int lba, unsigned char numsects,
                             unsigned short selector, unsigned int edi);

void ide_read_sectors(unsigned char drive, unsigned char numsects, unsigned int lba, unsigned
short es, unsigned int edi);

void ide_write_sectors(unsigned char drive, unsigned char numsects, unsigned int lba, unsigned
short es, unsigned int edi);


void ide_atapi_eject(unsigned char drive);

