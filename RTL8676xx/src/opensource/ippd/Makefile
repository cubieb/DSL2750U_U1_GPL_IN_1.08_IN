include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean

SRCDIR=$(CURDIR)/ippd
FIRMWARE=$(CURDIR)/firmware

all: build

build:
	make -C $(SRCDIR) all

clean:
	cd $(SRCDIR); \
	make clean

install:
	make -C $(SRCDIR) install
	make -C $(FIRMWARE) install
