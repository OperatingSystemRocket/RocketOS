# acpica
ACPICA_C_NAMES := $(shell find -name '*.c')
ACPICA_H_NAMES := $(shell find -name '*.h')
ACPICA_O_NAMES := $(patsubst %.c,%.o,$(ACPICA_C_NAMES))


# add -I
ACPICA_H_FILES_INCLUDE := $(addprefix -I,$(dir $(ACPICA_H_NAMES)))


CC := i686-elf-gcc

.PHONY: all build
.SUFFIXES: .o .c

all: acpica.a

acpica.a: $(ACPICA_O_NAMES)
	i686-elf-ar rcs $@ $(ACPICA_O_NAMES) -lgcc

.c.o:
	$(CC) -MD -c $< -o $@ $(ACPICA_H_FILES_INCLUDE) -std=gnu17 -ffreestanding -DRocketOS -nostdlib

clean:
	-rm $(shell find -name '*.o')
	-rm $(shell find -name '*.d')
	-rm acpica.a
