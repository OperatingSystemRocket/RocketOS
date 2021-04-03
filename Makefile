CC := i686-elf-gcc
AS := nasm -felf32

WARNING_FLAGS :=  -Wall -Wextra -Wundef -Wshadow -Wpointer-arith -Wcast-align \
                  -Wstrict-prototypes -Wcast-qual -Wconversion -Wunreachable-code \
                  -Wwrite-strings -Wredundant-decls -Wnested-externs \
                  -Winline -Wno-long-long -Wpedantic
KERNEL_FLAGS := -std=gnu17 -ffreestanding -mgeneral-regs-only

#Do not use these for tests
IMAGE_FLAGS := -ffreestanding -O3 -nostdlib

DEBUG_FLAGS := -fverbose-asm -Og -DDEBUG -save-temps=obj
RELEASE_FLAGS := -O3 -DNDEBUG


MKDIR := mkdir -p

PATHU := Unity/src/
PATHS := src/
PATHT := test/
PATHB := build/
PATHD := build/depends/
PATHO := build/objs/
PATHR := build/results/
PATHOT := build/tests/


#Do not replace these two occurrences of src/ with $(PATHS). It *will* break
AS_NAMES := $(shell find src/ -name '*.s')
AS_NAME_WITH_DIR := $(subst src/,build/objs/,$(AS_NAMES))
AS_OBJS_WITH_DIR := $(patsubst %.s,%.o,$(AS_NAME_WITH_DIR))
AS_OBJS_DIR := $(subst ./,,$(dir $(AS_OBJS_WITH_DIR)))
AS_OBJS_DIR_WITH_STAR := $(addsuffix *,$(AS_OBJS_DIR))

C_NAMES := $(shell find src/ -name '*.c')
H_FILES_DIR := $(dir $(shell find src/ -name '*.h'))
H_FILES_INCLUDE := $(addprefix -I,$(H_FILES_DIR))
C_OBJECTS := $(patsubst %.c,%.o,$(C_NAMES))
C_OBJECTS_NAME := $(notdir $(C_OBJECTS))
C_OBJECTS_WITH_DIR := $(subst ./,,$(subst src/,,$(C_OBJECTS)))
C_DIR := $(addprefix build/objs/,$(subst ./,,$(dir $(C_OBJECTS_WITH_DIR))))
C_DIR_WITH_STAR := $(addsuffix *,$(C_DIR))
C_OBJECTS_OUT := $(addprefix build/objs/,$(C_OBJECTS_WITH_DIR))
OBJECTS := $(AS_OBJS_WITH_DIR) $(C_OBJECTS_OUT)

OBJECTS_WITHOUT_MAIN := $(subst build/objs/boot/kernel.o,,$(OBJECTS))


TEST_C := $(shell find test/ -name '*.c')
TEST_DIR := $(addprefix $(PATHOT),$(subst test/,,$(subst ./,,$(dir $(TEST_C)))))
TEST_OBJECTS := $(patsubst %.c, %.o, $(TEST_C))
TEST_C_WITHOUT_TEST_NAMES := $(subst test/,build/objs/,$(subst Test-,,$(TEST_C)))
TEST_C_WITHOUT_TEST_OBJ := $(patsubst %.c,%.o,$(TEST_C_WITHOUT_TEST_NAMES))
TEST_C_OBJECTS_NAME := $(subst test/,,$(TEST_OBJECTS))
TEST_C_OBJECTS_OUT := $(subst test/,build/tests/,$(TEST_OBJECTS))
TEST_C_OBJECT_EXECUTABLES_WITH_PATH := $(patsubst %.c,%.bin,$(TEST_C_WITHOUT_TEST_NAMES))
TEST_C_OBJECT_EXECUTABLES := $(subst build/objs/,,$(TEST_C_OBJECT_EXECUTABLES_WITH_PATH))
TEXT_FILES := $(subst build/objs/,$(PATHOT),$(patsubst %.bin,%.txt,$(TEST_C_OBJECT_EXECUTABLES_WITH_PATH)))


ifdef TEST
DEPEND := $(TEST_CC) -MM -MG -MF
CFLAGS := -I. -Itest/ -I$(PATHU) $(H_FILES_INCLUDE) -DTEST
endif


#debug is default
ifdef RELEASE
FLAGS := $(RELEASE_FLAGS)
else
FLAGS := $(DEBUG_FLAGS)
endif


all : build


#creates bootable image
build : create_directory_structure os.bin
	./is_multiboot.sh build/results/os.bin
	$(MKDIR) isodir/boot/grub
	cp build/results/os.bin isodir/boot/os.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/results/os.iso isodir


run : build
	qemu-system-i386 -serial stdio -cdrom build/results/os.iso


create_directory_structure :
	$(MKDIR) $(PATHB)
	$(MKDIR) $(PATHD)
	$(MKDIR) $(PATHO)
	$(MKDIR) $(PATHR)
	$(MKDIR) $(PATHOT)
	$(MKDIR) $(C_DIR)
	$(MKDIR) $(AS_OBJS_DIR)
	$(MKDIR) $(TEST_DIR)


build/objs/%.o : src/%.s
	@echo $(AS_OBJS_WITH_DIR)
	@echo $(C_OBJECTS_OUT)
	$(AS) $(subst build/objs,src/,$^) -o $@




build/objs/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(KERNEL_FLAGS) $(WARNING_FLAGS) $(FLAGS) -I. $(H_FILES_INCLUDE)

os.bin : $(OBJECTS)
	@echo $(OBJECTS)
	$(CC) -T linker.ld -o build/results/os.bin $(IMAGE_FLAGS) $(OBJECTS) -lgcc


test: create_directory_structure $(TEST_C_OBJECTS_OUT) $(PATHD)unity.o $(OBJECTS_WITHOUT_MAIN) $(TEST_C_OBJECT_EXECUTABLES)
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


%.bin : $(TEST_C_OBJECTS_OUT) $(PATHD)unity.o $(OBJECTS_WITHOUT_MAIN)
	$(CC) -T linker.ld $(OBJECTS_WITHOUT_MAIN) $(PATHD)unity.o $(PATHOT)$(dir $(patsubst %.bin,%.o,$@))Test-$(notdir $(patsubst %.bin,%.o,$@)) -o $(PATHOT)$@ $(IMAGE_FLAGS) -lgcc
	./is_multiboot.sh $(PATHOT)$@
	$(MKDIR) build/results/$(subst .bin,,$@)/isodir/boot/grub
	cp $(PATHOT)$@ build/results/$(subst .bin,,$@)/isodir/boot/$(notdir $@)
	./generate_grubcfg.sh build/results/$(subst .bin,,$@)/isodir/boot/grub/grub.cfg $(subst .bin,,$(notdir $@)) /boot/$(notdir $@)
	#cp grub.cfg build/results/$(subst .bin,,$@)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(patsubst %.bin,%.iso,build/results/$@) build/results/$(subst .bin,,$@)/isodir/
	sh -c 'qemu-system-i386 -cdrom $(patsubst %.bin,%.iso,build/results/$@) -display none -serial file:$(PATHOT)$(patsubst %.bin,%.txt,$@) -device isa-debug-exit,iobase=0xf4,iosize=0x04'; \
	exit `expr $$? - 33`


.PHONY: all
.PHONY: build
.PHONY: run

.PHONY: test

.PHONY: clean


clean :
	-rm -rf isodir/
	-rm -f $(PATHD)*
	-rm -f $(PATHO)*
	-rm -f $(PATHR)*
	-rm -f $(PATHOT)*
	-rm -f $(C_DIR_WITH_STAR)
	-rm -f $(AS_OBJS_DIR_WITH_STAR)
	-rm -rf build/
