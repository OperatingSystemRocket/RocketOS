#pragma once

#include "stdint.h"
#include "src/float/float.h"

int8_t ksignbit(int8_t val);

int8_t ksign(int8_t val);

uint32_t kabs(int64_t val);

int64_t kmin(int64_t x, int64_t y);

int64_t kmax(int64_t x, int64_t y);

int64_t kmod(int64_t x, int64_t y);

int64_t kpow(int64_t base, int64_t exponent);

uint64_t kpow_u(uint64_t base, uint64_t exponent);

double kpow_d(double base, double exponent);

