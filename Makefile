PROJECTS := userspace/ kernel/
HEADER_PROJECTS := ${PROJECTS}

.PHONY: all clean build headers
.SUFFIXES: .o .c .S

all : build_iso

build_iso : build
	mkdir -p isodir/
	mkdir -p isodir/boot/
	mkdir -p isodir/boot/grub/
	cp kernel/rocketos.kernel isodir/boot/rocketos.kernel
	cp ramdisk.img isodir/boot/ramdisk.img
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o os.iso isodir

run : build_iso
	qemu-system-i386 -machine q35 -soundhw pcspk -serial stdio -drive file=disk.img,format=raw,index=0 -cdrom os.iso

build : headers
	@for e in ${HEADER_PROJECTS}; do \
	(cd $$e; $(MAKE) install); done

release : headers
	@for e in ${HEADER_PROJECTS}; do \
	(cd $$e; $(MAKE) install RELEASE=1); done

headers :
	@for e in ${HEADER_PROJECTS}; do \
	(cd $$e; $(MAKE) install-headers); done

clean :
	@for e in ${PROJECTS}; do \
	(cd $$e; $(MAKE) clean); done
	-rm -r sysroot/
