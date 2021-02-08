#must be an i686 gcc cross compiler
CC := i686-elf-gcc
AS := i686-elf-as

#may decrease number of warning flags for KERNEL_CFLAGS in the future
WARNING_FLAGS :=  -Wall -Wextra -Wundef -Wshadow -Wpointer-arith -Wcast-align \
                  -Wstrict-prototypes -Wcast-qual -Wconversion -Wunreachable-code \
                  -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
                  -Wredundant-decls -Wnested-externs -Winline -Wno-long-long
KERNEL_FLAGS := -std=gnu17 -ffreestanding
IMAGE_FLAGS := -ffreestanding -O3 -nostdlib

DEBUG_FLAGS := -fverbose-asm -Og -DDEBUG -save-temps=obj
RELEASE_FLAGS := -O3 -DNDEBUG

#Do not replace these two occurrences of src/ with $(PATHS). It *will* break
C_OBJECTS := $(patsubst %.c, %.o, $(shell find src/ -name '*.c'))
C_OBJECTS_NAME := $(subst src/, , $(C_OBJECTS))
C_OBJECTS_OUT := $(subst src/, build/objs/, $(C_OBJECTS))
OBJECTS := build/objs/boot.o $(C_OBJECTS_OUT)


MKDIR := mkdir -p

PATHU := Unity/src/
PATHS := src/
PATHT := test/
PATHB := build/
PATHD := build/depends/
PATHO := build/objs/
PATHR := build/results/


ifdef RELEASE
FLAGS := $(RELEASE_FLAGS)
else
FLAGS := $(DEBUG_FLAGS)
endif


#creates bootable image
build : create_directory_structure os.bin
	./is_multiboot.sh
	mkdir -p isodir/boot/grub
	cp build/results/os.bin isodir/boot/os.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/results/os.iso isodir

#debug is default
run : build
	qemu-system-i386 -cdrom build/results/os.iso

create_directory_structure :
	$(MKDIR) $(PATHB)
	$(MKDIR) $(PATHD)
	$(MKDIR) $(PATHO)
	$(MKDIR) $(PATHR)

build/objs/boot.o :
	$(AS) src/boot.s -o build/objs/boot.o


build/objs/%.o: src/%.c
	$(CC) -c $^ -o $@ $(KERNEL_FLAGS) $(WARNING_FLAGS) $(FLAGS)

os.bin : $(OBJECTS)
	$(CC) -T linker.ld -o build/results/os.bin $(IMAGE_FLAGS) $(OBJECTS) -lgcc



.PHONY: clean
clean :
	-rm -rf isodir/
	-rm -f $(PATHO)*
	-rm -f $(PATHR)*
	-rm warnings.txt
