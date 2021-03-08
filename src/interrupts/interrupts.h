#pragma once

#include <stdint.h>

#include "kstdio.h"
#include "vga_driver.h"
#include "keyboard_driver.h"
#include "keyboard_map.h"


void pic_init(void);
void isr_install(void);

void enable_keyboard(void);
