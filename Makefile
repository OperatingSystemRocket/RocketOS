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

C_OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
OBJECTS := boot.o $(C_OBJECTS)

ifdef RELEASE
FLAGS := $(RELEASE_FLAGS)
else
FLAGS := $(DEBUG_FLAGS)
endif


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


%.o: %.c
	$(CC) -c $^ -o $@ $(KERNEL_FLAGS) $(WARNING_FLAGS) $(FLAGS)

os.bin : $(OBJECTS)
	$(CC) -T linker.ld -o os.bin $(IMAGE_FLAGS) $(OBJECTS) -lgcc


CLEANUP := rm -f
MKDIR := mkdir -p
TARGET_EXTENSION := out

COMPILE = $(CC) -c
LINK = $(CC)
DEPEND = $(CC) -MM -MG -MF
CFLAGS = -I. -I$(PATHU) -I$(PATHS) -DTEST

PATHU = Unity/src/
PATHS = src/
PATHT = test/
PATHB = build/
PATHD = build/depends/
PATHO = build/objs/
PATHR = build/results/

BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)

SRCT = $(wildcard $(PATHT)*.c)

RESULTS = $(patsubst $(PATHT)Test%.c,$(PATHR)Test%.txt,$(SRCT) )

PASSED = `grep -s PASS $(PATHR)*.txt`
FAIL = `grep -s FAIL $(PATHR)*.txt`
IGNORE = `grep -s IGNORE $(PATHR)*.txt`

.PRECIOUS: $(PATHB)Test%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATHD)%.d
.PRECIOUS: $(PATHO)%.o
.PRECIOUS: $(PATHR)%.txt

.PHONY: test
test: $(BUILD_PATHS) $(RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"

$(PATHR)%.txt: $(PATHB)%.$(TARGET_EXTENSION)
	-./$< > $@ 2>&1

$(PATHB)Test%.$(TARGET_EXTENSION): $(PATHO)Test%.o $(PATHO)%.o $(PATHU)Unity.o #$(PATHD)Test%.d
	$(LINK) -o $@ $^

$(PATHO)%.o:: $(PATHT)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)%.o:: $(PATHS)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)%.o:: $(PATHU)%.c $(PATHU)%.h
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHD)%.d:: $(PATHT)%.c
	$(DEPEND) $@ $<

$(PATHB):
	$(MKDIR) $(PATHB)

$(PATHD):
	$(MKDIR) $(PATHD)

$(PATHO):
	$(MKDIR) $(PATHO)

$(PATHR):
	$(MKDIR) $(PATHR)


.PHONY: clean
clean :
	-rm os.bin kernel.i kernel.s $(OBJECTS) os.iso
	-rm -rf isodir/
	-rm warnings.txt
	$(CLEANUP) $(PATHO)*.o
	$(CLEANUP) $(PATHB)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATHR)*.txt
