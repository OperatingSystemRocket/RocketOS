#pragma once

#include <stdint.h>
#include <stddef.h>

#include "hardware_io.h"
#include "kstdio.h"


#define PIT_CHANNEL_0_REG 0x40
#define PIT_CHANNEL_1_REG 0x41
#define PIT_CHANNEL_2_REG 0x42
#define PIT_COMMAND_REG 0x43

enum PIT_CHANNEL {
    PIT_CHANNEL_0 = 0,
    PIT_CHANNEL_2 = 2,
    PIT_READ_BACK_COMMAND = 3
};

enum ACCESS_MODE {
    ACCESS_MODE_LATCH_COUNT = 0,
    ACCESS_MODE_LOBYTE_ONLY = 1,
    ACCESS_MODE_HIBYTE_ONLY = 2,
    ACCESS_MODE_LOBYTE_HIBYTE = 3
};

enum PIT_MODE {
    PIT_MODE_INTERRUPT_ON_TERMINAL_COUNT = 0,
    PIT_MODE_HARDWARE_RETRIGGERABLE_ONE_SHOT = 1,
    PIT_MODE_RATE_GENERATOR = 2,
    PIT_MODE_SQUARE_WAVE_GENERATOR = 3,
    PIT_MODE_SOFTWARE_TRIGGERED_STROBE = 4,
    PIT_MODE_HARDWARE_TRIGGERED_STROBE = 5
};


void init_pit(uint32_t requested_frequency, enum PIT_CHANNEL channel, enum ACCESS_MODE access_mode, enum PIT_MODE mode);

void play_sound(uint32_t frequency);
void no_sound(void);
void restore_timer(void);
