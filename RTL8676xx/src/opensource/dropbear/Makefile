ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

LDFLAGS=-L$(TMPDISTDIR)/lib
CPPFLAGS=-I$(TMPDISTDIR)/include
export LDFLAGS CPPFLAGS CFLAGS
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/dropbear-0.51
all:configure build
configure:
	if [ ! -d "$(SRCDIR)" ]; then \
		tar jxf dropbear-0.51.tar.bz2; \
	fi
	cd $(SRCDIR); \
	find | xargs touch;\
	$(GDBPREFIX) ./configure --prefix=$(TMPDISTDIR) --host=$(TARGET);
	touch configure
build:
	make -C $(SRCDIR) all install


clean:
	if [ -d "$(SRCDIR)" ]; then \
		cd $(SRCDIR); \
		make clean;  \
	fi

distclean:
	
	-if [ -d "$(SRCDIR)" ]; then \
		cd $(SRCDIR); \
		make distclean; \
	fi
	rm -f configure

install:
	mkdir -p  $(FSROOT)/usr/sbin/
	cp $(SRCDIR)/dropbear $(FSROOT)/usr/sbin/dropbear
	$(STRIP) $(FSROOT)/usr/sbin/dropbear
	mkdir -p  $(FSROOT)/etc/dropbear
	cp dropbear_dss_host_key  $(FSROOT)/etc/dropbear
	cp dropbear_rsa_host_key  $(FSROOT)/etc/dropbear

