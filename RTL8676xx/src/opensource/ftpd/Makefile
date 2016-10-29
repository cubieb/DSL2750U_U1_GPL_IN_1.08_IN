
.PHONY: all build clean
SRCDIR=$(CURDIR)/bftpd
all: build

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

build:
	make -C $(SRCDIR) all

clean:
	cd $(SRCDIR); \
        make clean

install:
	make -C $(SRCDIR) install
