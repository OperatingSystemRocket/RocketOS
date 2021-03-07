ifdef TEST
CC := gcc
AS := as
else
CC := i686-elf-gcc
AS := i686-elf-as
endif

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

OBJECTS_WITHOUT_MAIN := $(subst build/objs/boot/boot.o,,$(subst build/objs/boot/kernel.o,,$(C_OBJECTS_OUT)))


TEST_C := $(shell find test/ -name '*.c')
TEST_DIR := $(addprefix $(PATHOT),$(subst test/,,$(subst ./,,$(dir $(TEST_C)))))
TEST_OBJECTS := $(patsubst %.c, %.o, $(TEST_C))
TEST_C_WITHOUT_TEST_NAMES := $(subst test/,build/objs/,$(subst Test-,,$(TEST_C)))
TEST_C_WITHOUT_TEST_OBJ := $(patsubst %.c,%.o,$(TEST_C_WITHOUT_TEST_NAMES))
TEST_C_OBJECTS_NAME := $(subst test/,,$(TEST_OBJECTS))
TEST_C_OBJECTS_OUT := $(subst test/,build/tests/,$(TEST_OBJECTS))
TEST_C_OBJECT_EXECUTABLES_WITH_PATH := $(patsubst %.c,%.out,$(TEST_C_WITHOUT_TEST_NAMES))
TEST_C_OBJECT_EXECUTABLES := $(subst build/objs/,,$(TEST_C_OBJECT_EXECUTABLES_WITH_PATH))
TEXT_FILES := $(subst build/objs/,$(PATHOT),$(patsubst %.out,%.txt,$(TEST_C_OBJECT_EXECUTABLES_WITH_PATH)))


DEPEND := $(TEST_CC) -MM -MG -MF
CFLAGS := -I. -Itest/ -I$(PATHU) $(H_FILES_INCLUDE) -DTEST


#debug is default
ifdef RELEASE
FLAGS := $(RELEASE_FLAGS)
else
FLAGS := $(DEBUG_FLAGS)
endif


all : build


#creates bootable image
build : create_directory_structure os.bin
	./is_multiboot.sh
	mkdir -p isodir/boot/grub
	cp build/results/os.bin isodir/boot/os.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o build/results/os.iso isodir


run : build
	qemu-system-i386 -cdrom build/results/os.iso


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
	$(CC) -c $^ -o $@ $(KERNEL_FLAGS) $(WARNING_FLAGS) $(FLAGS) -I. $(H_FILES_INCLUDE)

os.bin : $(OBJECTS)
	$(CC) -T linker.ld -o build/results/os.bin $(IMAGE_FLAGS) $(OBJECTS) -lgcc


test: create_directory_structure $(TEST_C_OBJECTS_OUT) $(PATHD)unity.o $(OBJECTS_WITHOUT_MAIN) $(TEST_C_OBJECT_EXECUTABLES) run_static_analyzers
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
	$(CC) $(OBJECTS_WITHOUT_MAIN) $(PATHD)unity.o $(PATHOT)$(dir $(patsubst %.out,%.o,$@))Test-$(notdir $(patsubst %.out,%.o,$@)) -o $(PATHOT)$@ -ffreestanding -O3 -lgcc
	./$(PATHOT)$@ > $(PATHOT)$(patsubst %.out,%.txt,$@)


run_static_analyzers :
	-clang-tidy $(C_NAMES) $(TEST_C)
	-clang-format $(C_NAMES) $(TEST_C) --dry-run


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
	-rm -f $(C_DIR_WITH_STAR)
	-rm -f $(AS_OBJS_DIR_WITH_STAR)
	-rm -rf build/
