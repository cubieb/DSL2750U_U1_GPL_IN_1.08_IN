ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk

ifdef CONFIG_OPENSOURCE_DLNA_INTEL
SRCDIR=$(CURDIR)/dlna_intel
endif
ifdef CONFIG_OPENSOURCE_DLNA_USB
SRCDIR=$(CURDIR)/dlna_usb
endif
ifdef CONFIG_OPENSOURCE_DLNA_BCM
SRCDIR=$(CURDIR)/dlna_bcm
endif

#EXTRA_CFLAGS=-g

.PHONY: all build clean  

all: build

build:
	cd $(SRCDIR); \
	make all

clean:
	cd $(SRCDIR); \
	make clean

install:
	cd $(SRCDIR); \
	make install

