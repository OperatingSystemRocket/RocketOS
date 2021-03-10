#pragma once
#include <stdint.h>
#include <kmath.h>

void enable_timer(void);

void set_time_in_ticks(uint64_t new_val);

void set_time_in_seconds(void);

void increment_time(void);

uint64_t get_time_in_ticks(void);

uint64_t get_time_in_seconds(void);

void enable_time(void);

/*
void set_time(double new_val);

void increment_time(double amount);

double get_time(void);*/