include $(ROOTDIR)/BaseVar.mk
include $(ROOTDIR)/.config
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/iproute2-2.6.18
all: build
#configure:
#	cd $(SRCDIR); \
#	./configure --prefix=$(TMPDISTDIR) --host=$(TARGET); 
#	touch configure

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export EXTRA_CFLAGS = -fPIC -Dlinux -D__linux__ -Dunix -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) \
			          -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include
ifdef CONFIG_CPU_BIG_ENDIAN
EXTRA_CFLAGS += -DCONFIG_CPU_BIG_ENDIAN
endif

build:
	make -C $(SRCDIR)

clean:
	make -C $(SRCDIR) clean

distclean:	
	make -C $(SRCDIR) distclean
	
install:
	make -C $(SRCDIR) install
ifdef CONFIG_OPENSOURCE_TC_UTIL
	$(STRIP) $(FSROOT)/usr/sbin/tc
endif
ifdef CONFIG_OPENSOURCE_IP_UTIL
	$(STRIP) $(FSROOT)/usr/sbin/ip
endif
