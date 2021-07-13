#pragma once

#include <stdint.h>

#include "kmath.h"
#include "scheduler.h"


void enable_time(void);

void set_time_in_ticks(uint32_t new_val);
void set_time_in_seconds(void);
void increment_time(void);
uint32_t get_time_in_ticks(void);
uint32_t get_time_in_seconds(void);
void time_sleep_seconds(uint32_t duration);
void time_sleep_ticks(uint32_t duration);

/*
void set_time(double new_val);
void increment_time(double amount);
double get_time(void);
*/
