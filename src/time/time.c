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

uint64_t ticks_since_epoch; /// Time since epoch in cpu ticks (NOT SECONDS!!)

uint64_t seconds_since_epoch;

void set_time_in_ticks(const uint64_t new_val) {
    ticks_since_epoch = new_val;
}

void set_time_in_seconds(void) {
    if(kmod(ticks_since_epoch, (uint64_t)18) == (uint64_t)0) {
        seconds_since_epoch++; /// This is grossly inaccurate
       // kprintf("%i\n", get_time_in_seconds());
    }
}

void increment_time(void) {
    ticks_since_epoch++;
}

uint64_t get_time_in_ticks(void) {
    return ticks_since_epoch;
}

uint64_t get_time_in_seconds(void) {
    return seconds_since_epoch;
}

void enable_time(void) {
    enable_timer();
    set_time_in_ticks(0);
}

time_sleep_seconds(uint64_t duration) {
    uint64_t start_time = get_time_in_seconds();
    while(get_time_in_seconds() < start_time + duration);
}

time_sleep_ticks(uint64_t duration) {
    uint64_t start_time = get_time_in_ticks();
    while(get_time_in_ticks() < start_time + duration);
}
