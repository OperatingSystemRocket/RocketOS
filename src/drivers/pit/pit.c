#include "pit.h"


#define PID_HZ 3579545

#define LOWEST_HZ 18
<<<<<<< HEAD
#define HIGHEST_HZ 1193180 //1193181 //TODO: might make 1193180

#define HIGHEST_DIVISOR 65535 //65536 //TODO: might make 0 or conditionally 65535
=======
#define HIGHEST_HZ 1193180 //1193181 //TODO: might make 1193181

#define HIGHEST_DIVISOR 65535 //65536 //TODO: might make 0 or conditionally 65536
>>>>>>> acpica_integrate
#define LOWEST_DIVISOR 1

static uint8_t current_command_bitset = 0;
static uint32_t current_frequency = 0;
static uint32_t current_reload_value = 0;

void init_pit(const uint32_t requested_frequency, const enum PIT_CHANNEL channel, const enum ACCESS_MODE access_mode, const enum PIT_MODE mode) {
    const uint8_t command_bitset = (uint8_t)((channel << 6) | (access_mode << 4) | (mode << 1));
    current_command_bitset = command_bitset;
    outb(PIT_COMMAND_REG, command_bitset);

<<<<<<< HEAD
    //if(channel == PIT_CHANNEL_0) {
        uint32_t reload_value = 0;

        if(requested_frequency <= LOWEST_HZ) {
            reload_value = HIGHEST_DIVISOR;
        } else if(requested_frequency >= HIGHEST_HZ) {
            reload_value = LOWEST_DIVISOR;
        } else {
            reload_value = (HIGHEST_HZ / requested_frequency);
        }

        current_frequency = HIGHEST_HZ/reload_value;
        current_reload_value = reload_value;
        kprintf("requested frequency: %u, current_frequency: %u\n", requested_frequency, current_frequency);
        kprintf("reload_value: %u\n", reload_value);

        if(access_mode == ACCESS_MODE_LOBYTE_ONLY) {
            outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
        } else if(access_mode == ACCESS_MODE_HIBYTE_ONLY) {
            outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
        } else if(access_mode == ACCESS_MODE_LOBYTE_HIBYTE) {
            outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
            outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
        }
    //}

#if 0
    if(channel == PIT_CHANNEL_2) {
        uint32_t reload_value = 0;

        if(requested_frequency <= LOWEST_HZ) {
            reload_value = HIGHEST_DIVISOR;
        } else if(requested_frequency >= HIGHEST_HZ) {
            reload_value = LOWEST_DIVISOR;
        } else {
            reload_value = (HIGHEST_HZ / requested_frequency);
        }

        if(access_mode == ACCESS_MODE_LOBYTE_ONLY) {
            outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
        } else if(access_mode == ACCESS_MODE_HIBYTE_ONLY) {
            outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
        } else if(access_mode == ACCESS_MODE_LOBYTE_HIBYTE) {
            outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
            outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
        }

        const uint8_t tmp = inb(0x61); //TODO: figure out what `tmp` is
        if(tmp != (tmp | 3)) { //TODO: figure out what this bitwise AND with 3 is about/for
            outb(0x61, tmp | 3);
        }
    }
#endif
}

#if 0
void play_sound(const uint32_t requested_frequency) {
    outb(PIT_COMMAND_REG, 0xb6);

=======
>>>>>>> acpica_integrate
    uint32_t reload_value = 0;

    if(requested_frequency <= LOWEST_HZ) {
        reload_value = HIGHEST_DIVISOR;
    } else if(requested_frequency >= HIGHEST_HZ) {
        reload_value = LOWEST_DIVISOR;
    } else {
        reload_value = (HIGHEST_HZ / requested_frequency);
    }

<<<<<<< HEAD
    const uint8_t current_access_mode = current_command_bitset & 0x30;
    if(current_access_mode == ACCESS_MODE_LOBYTE_ONLY) {
        outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
    } else if(current_access_mode == ACCESS_MODE_HIBYTE_ONLY) {
        outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
    } else if(current_access_mode == ACCESS_MODE_LOBYTE_HIBYTE) {
        outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
        outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
    }

    const uint8_t tmp = inb(0x61); //TODO: figure out what `tmp` is
    if(tmp != (tmp | 3)) { //TODO: figure out what this bitwise AND with 3 is about/for
        outb(0x61, tmp | 3);
    }
}
#endif

//static uint32_t sound_reload_value = 0;
//static uint32_t sound_frequency = 0;

void play_sound(const uint32_t requested_frequency) {
    /*uint32_t reload_value = 0;

    if(requested_frequency <= LOWEST_HZ) {
        reload_value = HIGHEST_DIVISOR;
    } else if(requested_frequency >= HIGHEST_HZ) {
        reload_value = LOWEST_DIVISOR;
    } else {
        reload_value = (HIGHEST_HZ / requested_frequency);
    }

    sound_reload_value = reload_value;
    sound_frequency = requested_frequency;

    outb(PIT_COMMAND_REG, 0xb6);

    const uint8_t current_access_mode = current_command_bitset & 0x30;
    if(current_access_mode == ACCESS_MODE_LOBYTE_ONLY) {
        outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
    } else if(current_access_mode == ACCESS_MODE_HIBYTE_ONLY) {
        outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
    } else if(current_access_mode == ACCESS_MODE_LOBYTE_HIBYTE) {
        outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
        outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
    }*/

=======
    current_frequency = HIGHEST_HZ/reload_value;
    current_reload_value = reload_value;
    kprintf("requested frequency: %u, current_frequency: %u\n", requested_frequency, current_frequency);
    kprintf("reload_value: %u\n", reload_value);

    if(access_mode == ACCESS_MODE_LOBYTE_ONLY) {
        outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
    } else if(access_mode == ACCESS_MODE_HIBYTE_ONLY) {
        outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
    } else if(access_mode == ACCESS_MODE_LOBYTE_HIBYTE) {
        outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
        outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
    }
}

void play_sound(const uint32_t requested_frequency) {
>>>>>>> acpica_integrate
    init_pit(requested_frequency, PIT_CHANNEL_2, ACCESS_MODE_LOBYTE_HIBYTE, PIT_MODE_SQUARE_WAVE_GENERATOR);

    const uint8_t tmp = inb(0x61); //TODO: figure out what `tmp` is
    if(tmp != (tmp | 3)) { //TODO: figure out what this bitwise AND with 3 is about/for
        outb(0x61, tmp | 3);    
    }
}

void no_sound(void) {
    const uint8_t tmp = inb(0x61) & 0xFC; //TODO: figure out what `tmp` is
    outb(0x61, tmp);
}

void on_sound(void) {
    play_sound(current_frequency);
}

void restore_timer(void) {
    outb(PIT_COMMAND_REG, current_command_bitset);

    const uint8_t current_access_mode = current_command_bitset & 0x30;
    if(current_access_mode == ACCESS_MODE_LOBYTE_ONLY) {
        outb(PIT_CHANNEL_0_REG, current_reload_value & 0xFF);
    } else if(current_access_mode == ACCESS_MODE_HIBYTE_ONLY) {
        outb(PIT_CHANNEL_0_REG, (current_reload_value >> 8) & 0xFF);
    } else if(current_access_mode == ACCESS_MODE_LOBYTE_HIBYTE) {
        outb(PIT_CHANNEL_0_REG, current_reload_value & 0xFF);
        outb(PIT_CHANNEL_0_REG, (current_reload_value >> 8) & 0xFF);
    }
}
<<<<<<< HEAD
=======

static uint64_t seconds = 0u;
static uint64_t microseconds = 0u;

#define US_IN_SEC 1000000

void reset_timer(void) {
    reset_seconds();
    reset_us();
}
void reset_seconds(void) {
    seconds = 0u;
}
void reset_us(void) {
    microseconds = 0u;
}
void increment_timer(void) {
    if(++microseconds == US_IN_SEC) {
        microseconds = 0u;
        ++seconds;
    }
}
uint64_t get_seconds(void) {
    return seconds;
}
uint64_t get_us(void) {
    return microseconds;
}


static uint32_t current_ticks = 0u;

void reset_ticks(void) {
    current_ticks = 0u;
}
void increment_ticks(void) {
    ++current_ticks;
}

void pit_wait_ticks(const uint32_t ticks) {
    reset_ticks();
    while(current_ticks < ticks);
}

#define US_IN_SECS 1000000

void pit_wait_us(const uint32_t desired_us) {
    kassert_void(current_frequency != 0 && current_reload_value != 0);
    pit_wait_ticks(((current_frequency*desired_us)/US_IN_SECS) + (((current_frequency*desired_us)%US_IN_SECS)>0));
}
>>>>>>> acpica_integrate
