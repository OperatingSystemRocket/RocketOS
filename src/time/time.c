#include "time.h"

/*
double time_since_epoch; // Time since epoch in second

void set_time(double new_val) {
    time_since_epoch = new_val;
}

void increment_time(double amount) {
    time_since_epoch += amount;
}

double get_time(void) {
    return time_since_epoch;
}
*/

uint32_t ticks_since_epoch; /// Time since epoch in cpu ticks (NOT SECONDS!!)

uint32_t seconds_since_epoch;

void set_time_in_ticks(const uint32_t new_val) {
    ticks_since_epoch = new_val;
}

void set_time_in_seconds(void) {
    if(kmod(ticks_since_epoch, 18u) == 0) {
        seconds_since_epoch++; /// This is grossly inaccurate
       // kprintf("%i\n", get_time_in_seconds());
    }
}

void increment_time(void) {
    ticks_since_epoch++;
}

uint32_t get_time_in_ticks(void) {
    return ticks_since_epoch;
}

uint32_t get_time_in_seconds(void) {
    return seconds_since_epoch;
}

void enable_time(void) {
    enable_timer();
    set_time_in_ticks(0);
}

void time_sleep_seconds(uint32_t duration) {
    const uint32_t start_time = get_time_in_seconds();
    while(get_time_in_seconds() < start_time + duration);
}

void time_sleep_ticks(uint32_t duration) {
    const uint32_t start_time = get_time_in_ticks();
    while(get_time_in_ticks() < start_time + duration);
}
