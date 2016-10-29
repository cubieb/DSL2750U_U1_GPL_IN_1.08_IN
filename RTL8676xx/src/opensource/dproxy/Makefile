include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean  
SRCDIR=$(CURDIR)/dproxy-nexgen
EXTRA_CFLAGS=-g
all: build

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include
ifdef CONFIG_TBS_SUPPORT_IPV6
	CFLAGS += -DUSE_IPV6
endif
ifdef CONFIG_CPU_BIG_ENDIAN
    CFLAGS += -DCONFIG_CPU_BIG_ENDIAN
endif

ifdef CONFIG_TELEFONICA_BRAZIL
CFLAGS += -DCONFIG_TELEFONICA_BRAZIL
endif

build:
	mkdir -p  $(FSROOT)/sbin $(TMPDISTDIR)/sbin
	mkdir -p  $(FSROOT)/etc $(TMPDISTDIR)/etc
	make -C $(SRCDIR) TBS_filesystem=$(TMPDISTDIR) TBS_linux_dproxy CC=$(CROSS_COMPILE)gcc
	install -m 0755 $(SRCDIR)/dproxy $(TMPDISTDIR)/sbin/dproxy

clean:
	cd $(SRCDIR); \
	make clean

install:
	cp $(TMPDISTDIR)/sbin/dproxy $(FSROOT)/usr/sbin/dproxy
	$(STRIP) $(FSROOT)/usr/sbin/dproxy
	cd $(FSROOT)/etc; ln -sf ../var/dproxy.conf dproxy.conf
