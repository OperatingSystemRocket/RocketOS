#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "acpi.h"

#include "kstdio.h"

void print_all_tables(ACPI_TABLE_RSDP* rsdp);

