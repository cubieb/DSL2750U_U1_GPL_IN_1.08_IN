include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean
SRCDIR=$(CURDIR)/udhcp-0.9.7

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include


ifeq ("$(CONFIG_APPS_LOGIC_DHCPC_OPTION15)","y")
CFLAGS += -DENABLE_OPT15
endif

ifeq ("$(CONFIG_APPS_LOGIC_DHCPC_OPTION42)","y")
CFLAGS += -DENABLE_OPT42
endif

ifeq ("$(CONFIG_APPS_LOGIC_DHCPC_OPTION250)","y")
CFLAGS += -DENABLE_OPT250
endif

ifeq ("$(CONFIG_APPS_LOGIC_DHCPS_OPTION125)","y")
CFLAGS += -DENABLE_OPT125
endif

CFLAGS += -I$(BUILDDIR)

all: build

build:
	CFLAGS="$(CFLAGS)";make -C $(SRCDIR) CC=$(CROSS_COMPILE)gcc all 

clean:
	cd $(SRCDIR); \
	make clean

install:
	mkdir -p $(FSROOT)/bin
	cp -fr $(SRCDIR)/udhcpd $(FSROOT)/sbin/udhcpd
	-ln -s udhcpd $(FSROOT)/sbin/udhcpc
	$(STRIP) $(FSROOT)/sbin/udhcpd
