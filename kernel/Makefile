PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR ?= $(EXEC_PREFIX)/boot
INCLUDEDIR ?= $(PREFIX)/include

CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS := $(CPPFLAGS) -D__is_kernel -I../sysroot/usr/include/userspace -I../sysroot/usr/include/kernel/kernel -I../sysroot/usr/include/kernel/arch/i686 -I../kernel/arch/i686/ -I../kernel/kernel/
LDFLAGS := $(LDFLAGS)
LIBS := $(LIBS) -nostdlib -lk -lgcc

include ../make.config

KERNEL_FLAGS ?= -std=gnu17 -ffreestanding -DRocketOS -mgeneral-regs-only

ifdef RELEASE
IMAGE_FLAGS := -ffreestanding $(RELEASE_LINK_FLAGS) -nostdlib
else
IMAGE_FLAGS := -ffreestanding $(DEBUG_LINK_FLAGS) -nostdlib
endif


ARCHDIR = arch/$(HOST)

include $(ARCHDIR)/make.config


#arch
ARCH_AS_NAMES = $(shell find arch/ -name '*.s')
ARCH_C_NAMES = $(shell find arch/ -name '*.c' | grep -v arch/i686/acpica/)
ARCH_H_NAMES = $(shell find arch/ -name '*.h' | grep -v arch/i686/acpica/)

#kernel
KERNEL_AS_NAMES = $(shell find kernel/ -name '*.s')
KERNEL_C_NAMES = $(shell find kernel/ -name '*.c')
KERNEL_H_NAMES = $(shell find kernel/ -name '*.h')

#sysroot
INCLUDE_HEADERS = $(shell find include/ -name '*.h')

OBJ_LIST = $(patsubst %.c,%.o,$(ARCH_C_NAMES)) $(patsubst %.c,%.o,$(KERNEL_C_NAMES)) $(patsubst %.s,%.o,$(ARCH_AS_NAMES)) $(patsubst %.s,%.o,$(KERNEL_AS_NAMES))


.PHONY: all clean build_acpica install install-headers install-kernel
.SUFFIXES: .o .c .s

all: rocketos.kernel

rocketos.kernel: $(OBJ_LIST) $(ARCHDIR)/linker.ld build_acpica
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(IMAGE_FLAGS) $(OBJ_LIST) $(shell find arch/i686/acpica -name '*o') ../sysroot/usr/lib/libk.a -lgcc
	grub-file --is-x86-multiboot rocketos.kernel

build_acpica:
	@cd arch/i686/acpica; $(MAKE)

.c.o:
	$(CC) -MD -c $< -o $@ $(KERNEL_FLAGS) $(WARNING_FLAGS) $(FLAGS) $(CPPFLAGS) -Iarch/i686/acpica/acpi.h

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	-rm rocketos.kernel
	-rm $(shell find -name '*.o')
	-rm $(shell find -name '*.d')
	-rm -r $(DESTDIR)$(INCLUDEDIR)/kernel/

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)/kernel
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/kernel/.

install-kernel: rocketos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp rocketos.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
