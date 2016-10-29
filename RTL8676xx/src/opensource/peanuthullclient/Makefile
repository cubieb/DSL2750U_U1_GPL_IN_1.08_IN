include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean
SRCDIR=$(CURDIR)/peanuthullclient-0.1

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

ifdef CONFIG_CPU_BIG_ENDIAN
CFLAGS += -DCONFIG_CPU_BIG_ENDIAN
endif

all: build

build:
	make -C $(SRCDIR) 

clean:
	cd $(SRCDIR); \
	make clean

install:
	mkdir -p $(FSROOT)/bin
	cp -fr $(SRCDIR)/peanuthullc $(FSROOT)/sbin/peanuthullc
	$(STRIP) $(FSROOT)/sbin/peanuthullc
