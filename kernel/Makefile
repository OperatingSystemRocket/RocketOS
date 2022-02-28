DEFAULT_HOST := i686-elf
HOST ?= DEFAULT_HOST
HOSTARCH := i686

CFLAGS ?= -O2 -g
CPPFLAGS ?=
LDFLAGS ?=
LIBS ?=

PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR ?= $(EXEC_PREFIX)/boot
INCLUDEDIR ?= $(PREFIX)/include

CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS := $(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS := $(LDFLAGS)
LIBS := $(LIBS) -nostdlib -lk -lgcc

ARCHDIR = arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS := $(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS := $(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS := $(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS := $(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS = \
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \

OBJS = \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

LINK_LIST = \
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \




#arch
ARCH_AS_NAMES = $(subst arch/i686/boot/boot.s,,$(shell find arch/ -name '*.s'))
ARCH_C_NAMES = $(subst arch/i686/boot/kernel.o,,$(shell find arch/ -name '*.c'))
ARCH_H_NAMES = $(shell find arch/ -name '*.h')

#entry point
ENTRY_POINT_AS_NAMES = $(shell find arch/i686/boot/ -name '*.s')
ENTRY_POINT_C_NAMES = $(shell find arch/i686/boot/ -name '*.c')

#kernel
KERNEL_AS_NAMES = $(shell find kernel/ -name '*.s')
KERNEL_C_NAMES = $(shell find kernel/ -name '*.c')
KERNEL_H_NAMES = $(shell find kernel/ -name '*.h')

#sysroot
INCLUDE_HEADERS = $(DESTDIR)





.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S

include ../make.config

all: myos.kernel

myos.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot myos.kernel

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f myos.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)/kernel$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)/kernel$(INCLUDEDIR)/.

install-kernel: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
