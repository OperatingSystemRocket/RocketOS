#pragma once
#include "stdint.h"

int8_t ksignbit(int8_t val);

int8_t alg_sign_abs(int8_t val);

int64_t max(int64_t x, int64_t y);


int8_t algebraic_sign(int8_t val); // NOTE: not in ISO C, but this sign function probably behaves closer to your expectations. Also it makes implementing abs a bit easier

uint32_t kabs(int64_t val);