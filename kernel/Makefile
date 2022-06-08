PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR ?= $(EXEC_PREFIX)/boot
INCLUDEDIR ?= $(PREFIX)/include

CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS := $(CPPFLAGS) -D__is_kernel -I../sysroot/usr/include/userspace -I../sysroot/usr/include/kernel/kernel -I../sysroot/usr/include/kernel/arch/i686 -I../kernel/arch/i686/ -I../kernel/kernel/

include ../make.config

KERNEL_FLAGS ?= -std=gnu17 -ffreestanding -DRocketOS -mgeneral-regs-only

ifdef RELEASE
IMAGE_FLAGS := -ffreestanding $(RELEASE_LINK_FLAGS) -nostdlib -lgcc
else
IMAGE_FLAGS := -ffreestanding $(DEBUG_LINK_FLAGS) -nostdlib -lgcc
endif


ARCHDIR = arch/$(HOST)

include $(ARCHDIR)/make.config


#arch
ARCH_AS_NAMES := $(shell find arch/ -name '*.s')
ARCH_C_NAMES := $(shell find arch/ -name '*.c' | grep -v arch/i686/acpica/)
ARCH_H_NAMES := $(shell find arch/ -name '*.h' | grep -v arch/i686/acpica/)

#kernel
KERNEL_AS_NAMES := $(shell find kernel/ -name '*.s')
KERNEL_C_NAMES := $(shell find kernel/ -name '*.c')
KERNEL_H_NAMES := $(shell find kernel/ -name '*.h')

#libk
LIBK_C_NAMES := $(shell find libk/ -name '*.c')
LIBK_H_NAMES := $(shell find libk/ -name '*.h')

#sysroot
INCLUDE_HEADERS := $(shell find include/ -name '*.h')

#list of all kernel headers with include prefix
INCLUDE_LIST := $(addprefix -I,$(dir $(INCLUDE_HEADERS) $(LIBK_H_NAMES) $(ARCH_H_NAMES) $(KERNEL_H_NAMES)))


OBJ_LIST := $(patsubst %.s,%.o,$(ARCH_AS_NAMES)) $(patsubst %.s,%.o,$(KERNEL_AS_NAMES)) $(patsubst %.c,%.o,$(ARCH_C_NAMES)) $(patsubst %.c,%.o,$(KERNEL_C_NAMES)) $(patsubst %.c,%.o,$(LIBK_C_NAMES))


.PHONY: all clean build_acpica install install-headers install-kernel
.SUFFIXES: .o .c .s

all: rocketos.kernel

rocketos.kernel: $(OBJ_LIST) $(ARCHDIR)/linker.ld build_acpica
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(IMAGE_FLAGS) $(OBJ_LIST) arch/i686/acpica/acpica.a -lgcc
	../is_multiboot.sh rocketos.kernel

build_acpica:
	@cd arch/i686/acpica; $(MAKE)

.c.o:
	$(CC) -MD -c $< -o $@ $(KERNEL_FLAGS) $(WARNING_FLAGS) $(FLAGS) $(CPPFLAGS) $(INCLUDE_LIST) -Iarch/i686/acpica/

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	-rm rocketos.kernel
	-rm $(shell find -name '*.o')
	-rm $(shell find -name '*.d')
	-rm -r $(DESTDIR)$(INCLUDEDIR)/kernel/
	@cd arch/i686/acpica; $(MAKE) clean

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)/kernel
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/kernel/.

install-kernel: rocketos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp rocketos.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
