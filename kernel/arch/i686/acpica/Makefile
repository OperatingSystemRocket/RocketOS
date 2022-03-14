# acpica
ACPICA_C_NAMES = $(shell find -name '*.c')
ACPICA_H_NAMES = $(shell find -name '*.h')

# add -I
ACPICA_H_FILES_INCLUDE := $(addprefix -I,$(dir $(ACPICA_H_NAMES)))


CC := i686-elf-gcc

.PHONY: all build
.SUFFIXES: .o .c

all: build

build: $(patsubst %.c,%.o,$(ACPICA_C_NAMES))

.c.o:
	$(CC) -MD -c $< -o $@ $(ACPICA_H_FILES_INCLUDE) -std=gnu17 -ffreestanding -DRocketOS -nostdlib

clean:
	-rm $(shell find -name '*.o')
	-rm $(shell find -name '*.d')
