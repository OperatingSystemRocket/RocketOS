#must be an i686 gcc cross compiler
CC = i686-elf-gcc
AS = i686-elf-as

#may decrease number of warning flags for KERNEL_CFLAGS in the future
#KERNEL_CFLAGS is release flags, will use -fverbose-asm -g -O0 -save-temps -std=gnu17 -ffreestanding and the warning flags in debug builds
WARNING_FLAGS :=  -Wall -Wextra -Wundef -Wshadow -Wpointer-arith -Wcast-align \
                  -Wstrict-prototypes -Wcast-qual -Wconversion -Wunreachable-code \
                  -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
                  -Wredundant-decls -Wnested-externs -Winline -Wno-long-long
KERNEL_FLAGS := -std=gnu17 -ffreestanding
IMAGE_FLAGS := -ffreestanding -O3 -nostdlib

RELEASE_FLAGS = -O3 -DNDEBUG
DEBUG_FLAGS = -fverbose-asm -Og -save-temps -DDEBUG

OBJECTS := boot.o kernel.o
DEBUG_OBJECTS := boot.o debug_kernel.o
RELEASE_OBJECTS := boot.o release_kernel.o


#creates bootable image
build_release : release_os.bin
	./is_multiboot.sh
	mkdir -p isodir/boot/grub
	cp os.bin isodir/boot/os.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o os.iso isodir

#release is default
run : build_release
	qemu-system-i386 -cdrom os.iso


#creates bootable image
build_debug : debug_os.bin
	./is_multiboot.sh
	mkdir -p isodir/boot/grub
	cp os.bin isodir/boot/os.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o os.iso isodir


run_debug : build_debug
	qemu-system-i386 -cdrom os.iso


boot.o :
	$(AS) boot.s -o boot.o

release_kernel.o :
	$(CC) -c kernel.c -o kernel.o $(KERNEL_FLAGS) $(WARNING_FLAGS) $(RELEASE_FLAGS)

release_os.bin : $(RELEASE_OBJECTS)
	$(CC) -T linker.ld -o os.bin $(IMAGE_FLAGS) $(OBJECTS) -lgcc


debug_kernel.o :
	$(CC) -c kernel.c -o kernel.o $(KERNEL_FLAGS) $(WARNING_FLAGS) $(DEBUG_FLAGS)

debug_os.bin : $(DEBUG_OBJECTS)
	$(CC) -T linker.ld -o os.bin $(IMAGE_FLAGS) $(OBJECTS) -lgcc


.PHONY : clean clean_release clean_debug
clean :
	-rm os.bin kernel.i kernel.s $(OBJECTS) os.iso
	-rm -rf isodir/

