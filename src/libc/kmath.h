#pragma once

#include "stdint.h"

int8_t ksignbit(int8_t val);

int8_t ksign(int8_t val);

uint32_t kabs(int64_t val);

int64_t kmin(int64_t x, int64_t y);

int64_t kmax(int64_t x, int64_t y);

int64_t kmod(int64_t x, int64_t y);