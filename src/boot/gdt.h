#pragma once

#include <stdint.h>

#include "kstring.h"
#include "kstdio.h"

extern install_tss(void);
extern jump_usermode(void);
extern flush_tss(void);

