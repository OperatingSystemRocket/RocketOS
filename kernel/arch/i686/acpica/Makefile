# acpica
ACPICA_C_NAMES = $(shell find -name '*.c')
ACPICA_H_NAMES = $(shell find -name '*.h')

# add -I
ACPICA_H_FILES_INCLUDE := $(addprefix -I,$(dir $(ACPICA_H_NAMES)))


CC := i686-elf-gcc

all: acpica.a

acpica.a: $(ACPICA_C_NAMES) $(ACPICA_H_NAMES)
	$(CC) $(ACPICA_C_NAMES) -c $(ACPICA_H_FILES_INCLUDE) -std=gnu17 -ffreestanding -DRocketOS -nostdlib
