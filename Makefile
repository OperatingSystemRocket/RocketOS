ifdef TEST
CC := gcc
AS := as
else
CC := i686-elf-gcc
AS := i686-elf-as
endif

#may decrease number of warning flags for KERNEL_CFLAGS in the future
WARNING_FLAGS :=  -Wall -Wextra -Wundef -Wshadow -Wpointer-arith -Wcast-align \
                  -Wstrict-prototypes -Wcast-qual -Wconversion -Wunreachable-code \
                  -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
                  -Wredundant-decls -Wnested-externs -Winline -Wno-long-long
KERNEL_FLAGS := -std=gnu17 -ffreestanding

#Do not use these for tests
IMAGE_FLAGS := -ffreestanding -O3 -nostdlib

DEBUG_FLAGS := -fverbose-asm -Og -DDEBUG -save-temps=obj
RELEASE_FLAGS := -O3 -DNDEBUG

#Do not replace these two occurrences of src/ with $(PATHS). It *will* break
C_NAMES := $(shell find src/ -name '*.c')
C_OBJECTS := $(patsubst %.c,%.o,$(shell find src/ -name '*.c'))
C_OBJECTS_NAME := $(subst src/,,$(C_OBJECTS))
C_OBJECTS_OUT := $(subst src/,build/objs/,$(C_OBJECTS))
OBJECTS := build/objs/boot.o $(C_OBJECTS_OUT)

OBJECTS_WITHOUT_MAIN := $(subst build/objs/kernel.o,,$(C_OBJECTS_OUT))

MKDIR := mkdir -p

PATHU := Unity/src/
PATHS := src/
PATHT := test/
PATHB := build/
PATHD := build/depends/
PATHO := build/objs/
PATHR := build/results/
PATHOT := build/tests/


DEPEND := $(TEST_CC) -MM -MG -MF
CFLAGS := -I. -I$(PATHU) -I$(PATHS) -DTEST


ifdef RELEASE
FLAGS := $(RELEASE_FLAGS)
else
FLAGS := $(DEBUG_FLAGS)
endif


all : build


#creates bootable image
build : create_directory_structure run_static_analyzers os.bin
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
	$(MKDIR) $(PATHOT)


build/objs/boot.o :
	$(AS) src/boot.s -o build/objs/boot.o


build/objs/%.o: src/%.c
	$(CC) -c $^ -o $@ $(KERNEL_FLAGS) $(WARNING_FLAGS) $(FLAGS)

os.bin : $(OBJECTS)
	$(CC) -T linker.ld -o build/results/os.bin $(IMAGE_FLAGS) $(OBJECTS) -lgcc


TEST_C := $(wildcard test/*.c)
TEST_OBJECTS := $(patsubst %.c, %.o, $(shell find test/ -name '*.c'))
TEST_C_WITHOUT_TEST_NAMES := $(subst test/,build/objs/,$(subst Test-,,$(TEST_C)))
TEST_C_WITHOUT_TEST_OBJ := $(patsubst %.c,%.o,$(TEST_C_WITHOUT_TEST_NAMES))
TEST_C_OBJECTS_NAME := $(subst test/,,$(TEST_OBJECTS))
TEST_C_OBJECTS_OUT := $(subst test/,build/tests/,$(TEST_OBJECTS))
TEST_C_OBJECT_EXECUTABLES_WITH_PATH := $(patsubst %.c,%.out,$(TEST_C_WITHOUT_TEST_NAMES))
TEST_C_OBJECT_EXECUTABLES := $(subst build/objs/,,$(TEST_C_OBJECT_EXECUTABLES_WITH_PATH))
TEXT_FILES := $(subst build/objs/,$(PATHOT),$(patsubst %.out,%.txt,$(TEST_C_OBJECT_EXECUTABLES_WITH_PATH)))



test: create_directory_structure run_static_analyzers $(TEST_C_OBJECTS_OUT) $(PATHD)unity.o $(OBJECTS_WITHOUT_MAIN) $(TEST_C_OBJECT_EXECUTABLES)
	@echo "\n"
	cat $(TEXT_FILES)
	@echo "\n"
	@echo "\n-----------------------\nIGNORES:\n-----------------------"
	@echo `grep -s IGNORE $(PATHOT)*.txt`
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo `grep -s FAIL $(PATHOT)*.txt`
	@echo "\nDONE"
	! grep -s FAIL $(PATHOT)*.txt


$(PATHD)%.o : Unity/src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(KERNEL_FLAGS) $(WARNING_FLAGS) $(DEBUG_FLAGS)


$(PATHOT)%.o : test/%.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(KERNEL_FLAGS) $(WARNING_FLAGS) $(DEBUG_FLAGS)


%.out : $(TEST_C_OBJECTS_OUT) $(PATHD)unity.o $(OBJECTS_WITHOUT_MAIN)
	$(CC) $(OBJECTS_WITHOUT_MAIN) $(PATHD)unity.o $(PATHOT)Test-$(patsubst %.out,%.o,$@) -o $(PATHOT)$@ -ffreestanding -O3 -lgcc
	-./$(PATHOT)$@ > $(PATHOT)$(patsubst %.out,%.txt,$@)


run_static_analyzers :
	clang-tidy $(C_NAMES) $(TEST_C) | tee warnings.txt
	clang-format $(C_NAMES) $(TEST_C) --dry-run | tee warnings.txt -a


.PHONY: all
.PHONY: build
.PHONY: run

.PHONY: test

.PHONY: run_static_analyzers

.PHONY: clean
clean :
	-rm -rf isodir/
	-rm -f $(PATHD)*
	-rm -f $(PATHO)*
	-rm -f $(PATHR)*
	-rm -f $(PATHOT)*
	-rm warnings.txt
