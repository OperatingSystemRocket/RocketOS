PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(EXEC_PREFIX)/lib

CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS := $(CPPFLAGS) -I../sysroot/usr/include/userspace -I../sysroot/usr/include/kernel/kernel -I../sysroot/usr/include/kernel/arch/i686
LIBC_CFLAGS := $(CFLAGS)
LIBC_CPPFLAGS := $(CPPFLAGS) -D__is_libc

include ../make.config

KERNEL_FLAGS ?= -std=gnu17 -ffreestanding -DRocketOS -mgeneral-regs-only

ifdef RELEASE
IMAGE_FLAGS := -ffreestanding $(RELEASE_LINK_FLAGS) -nostdlib
else
IMAGE_FLAGS := -ffreestanding $(DEBUG_LINK_FLAGS) -nostdlib
endif


BINARIES = libc.a


#libc
LIBC_C_NAMES = $(shell find libc/ -name '*.c')
LIBC_H_NAMES = $(shell find libc/ -name '*.h')

#sysroot
INCLUDE_HEADERS = $(shell find include/ -name '*.h')

LIBC_OBJS = $(LIBC_C_NAMES:.c=.libc.o)


.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libc.o .c .S

all: $(BINARIES)

libc.a: $(LIBC_OBJS)
	$(AR) rcs $@ $(LIBC_OBJS) -lgcc

.c.o:
	$(CC) -MD -c $< -o $@ $(KERNEL_FLAGS) $(WARNING_FLAGS) $(FLAGS)

.c.S:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.libc.o:
	$(CC) -MD -c $< -o $@ $(LIBC_CFLAGS) $(LIBC_CPPFLAGS)

.S.libc.o:
	$(CC) -MD -c $< -o $@ $(LIBC_CFLAGS) $(LIBC_CPPFLAGS)

clean:
	-rm libc.a
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
