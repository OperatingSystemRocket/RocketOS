#must be an i686 gcc cross compiler
CC = i686-elf-gcc
AS = i686-elf-as

#may decrease number of warning flags for KERNEL_CFLAGS in the future
WARNING_FLAGS :=  -Wall -Wextra -Wundef -Wshadow -Wpointer-arith -Wcast-align \
                  -Wstrict-prototypes -Wcast-qual -Wconversion -Wunreachable-code \
                  -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
                  -Wredundant-decls -Wnested-externs -Winline -Wno-long-long
KERNEL_FLAGS := -std=gnu17 -ffreestanding
IMAGE_FLAGS := -ffreestanding -O3 -nostdlib

DEBUG_FLAGS = -fverbose-asm -Og -save-temps -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG

OBJECTS := boot.o kernel.o
OBJECTS := boot.o kernel.o


ifdef RELEASE
FLAGS := $(RELEASE_FLAGS)
else
FLAGS := $(DEBUG_FLAGS)
endif


PATHU = unity/src/
PATHS = src/
PATHT = test/
PATHB = build/
PATHD = build/depends/
PATHO = build/objs/
PATHR = build/results/

BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)


#creates bootable image
build : os.bin
	./is_multiboot.sh
	mkdir -p isodir/boot/grub
	cp os.bin isodir/boot/os.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o os.iso isodir

#debug is default
run : build
	qemu-system-i386 -cdrom os.iso


boot.o :
	$(AS) boot.s -o boot.o

kernel.o :
	$(CC) -c kernel.c -o kernel.o $(KERNEL_FLAGS) $(WARNING_FLAGS) $(FLAGS)

os.bin : $(OBJECTS)
	$(CC) -T linker.ld -o os.bin $(IMAGE_FLAGS) $(OBJECTS) -lgcc



.PHONY : clean
clean :
	-rm os.bin kernel.i kernel.s $(OBJECTS) os.iso
	-rm -rf isodir/
	-rm warnings.txt
