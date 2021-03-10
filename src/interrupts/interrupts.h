#pragma once

#include <stdint.h>

#include "hardware_io.h"
#include "kstdio.h"
#include "vga_driver.h"
#include "keyboard_driver.h"
#include "keyboard_map.h"
#include "time.h"


void pic_init(void);
void isr_install(void);

void enable_timer(void);
void enable_keyboard(void);
