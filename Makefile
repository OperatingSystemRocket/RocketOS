PROJECTS := userspace/ kernel/
HEADER_PROJECTS := ${PROJECTS}

.PHONY: all clean build headers
.SUFFIXES: .o .c .S

all : build

build : headers
	@for e in ${HEADER_PROJECTS}; do \
	(cd $$e; $(MAKE) install); done

headers :
	@for e in ${HEADER_PROJECTS}; do \
	(cd $$e; $(MAKE) install-headers); done
