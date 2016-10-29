include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean
SRCDIR=$(CURDIR)/dhcp-forwarder-0.4
all: build

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

build:
	cd $(SRCDIR);\
	chmod +x configure;\
	CFLAGS="-Os -DNDEBUG=0" ./configure --disable-dietlibc --build=i386-linux --host=mipsel-linux
	make -C $(SRCDIR) CC=$(CROSS_COMPILE)gcc

clean:
	make -C $(SRCDIR) clean

distclean:
	make -C $(SRCDIR) distclean

install:
	mkdir -p $(FSROOT)/sbin;
	cp -fr $(SRCDIR)/dhcp-fwd $(FSROOT)/sbin;
	chmod +x $(FSROOT)/sbin;
	$(STRIP) $(FSROOT)/sbin/dhcp-fwd
