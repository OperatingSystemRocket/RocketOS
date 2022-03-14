PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(EXEC_PREFIX)/lib

CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS := $(CPPFLAGS) -I../sysroot/usr/include/userspace -I../sysroot/usr/include/kernel/kernel -I../sysroot/usr/include/kernel/arch/i686
LIBK_CFLAGS := $(CFLAGS)
LIBK_CPPFLAGS := $(CPPFLAGS) -D__is_libk

include ../make.config

KERNEL_FLAGS ?= -std=gnu17 -ffreestanding -DRocketOS -mgeneral-regs-only

ifdef RELEASE
IMAGE_FLAGS := -ffreestanding $(RELEASE_LINK_FLAGS) -nostdlib
else
IMAGE_FLAGS := -ffreestanding $(DEBUG_LINK_FLAGS) -nostdlib
endif


BINARIES = libk.a


#libc
LIBC_C_NAMES = $(shell find libc/ -name '*.c')
LIBC_H_NAMES = $(shell find libc/ -name '*.h')

#sysroot
INCLUDE_HEADERS = $(shell find include/ -name '*.h')

LIBK_OBJS = $(LIBC_C_NAMES:.c=.libk.o)


.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c .S

all: $(BINARIES)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS) -lgcc

.c.o:
	$(CC) -MD -c $< -o $@ $(KERNEL_FLAGS) $(WARNING_FLAGS) $(FLAGS)

.c.S:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.S.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	-rm $(shell find -name '*.o')
	-rm $(shell find -name '*.d')
	-rm -r $(DESTDIR)$(INCLUDEDIR)/userspace/

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)/userspace
	cp -R --preserve=timestamps libc/include/. $(DESTDIR)$(INCLUDEDIR)/userspace/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)
