#include "pit.h"


#define PID_HZ 3579545

#define LOWEST_HZ 18
#define HIGHEST_HZ 1193181 //TODO: might make 1193180

#define HIGHEST_DIVISOR 65536 //TODO: might make 0 or conditionally 65535
#define LOWEST_DIVISOR 1

static uint8_t current_command_bitset = 0;
static uint32_t current_frequency = 0;
static uint32_t current_reload_value = 0;

void init_pit(const uint32_t requested_frequency, const enum PIT_CHANNEL channel, const enum ACCESS_MODE access_mode, const enum PIT_MODE mode) {
    const uint8_t command_bitset = (uint8_t)((channel << 6) | (access_mode << 4) | (mode << 1));
    current_command_bitset = command_bitset;
    outb(PIT_COMMAND_REG, command_bitset);

    if(channel == PIT_CHANNEL_0) {
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
        kprintf("\n\n\nrequested frequency: %u, current_frequency: %u\n", requested_frequency, current_frequency);
        kprintf("reload_value: %u\n\n\n\n", reload_value);

        if(access_mode == ACCESS_MODE_LOBYTE_ONLY) {
            outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
        } else if(access_mode == ACCESS_MODE_HIBYTE_ONLY) {
            outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
        } else if(access_mode == ACCESS_MODE_LOBYTE_HIBYTE) {
            outb(PIT_CHANNEL_0_REG, reload_value & 0xFF);
            outb(PIT_CHANNEL_0_REG, (reload_value >> 8) & 0xFF);
        }
    }

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

void play_sound(const uint32_t requested_frequency) {
    outb(PIT_COMMAND_REG, 0xb6);

    uint32_t reload_value = 0;

    if(requested_frequency <= LOWEST_HZ) {
        reload_value = HIGHEST_DIVISOR;
    } else if(requested_frequency >= HIGHEST_HZ) {
        reload_value = LOWEST_DIVISOR;
    } else {
        reload_value = (HIGHEST_HZ / requested_frequency);
    }

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

void no_sound(void) {
    const uint8_t tmp = inb(0x61) & 0xFC; //TODO: figure out what `tmp` is
    outb(0x61, tmp);
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
