ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/zebra-0.95a
#CC=gcc
#LD=ld
#CPP=cpp
#AR=ar

#export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include


all:configure build
configure:
	cd $(SRCDIR); \
	find | xargs touch;\
	if [ "IPV6-$(CONFIG_IPV6)" == "IPV6-" ]; then \
		./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) --disable-FEATURE --disable-ipv6 --disable-bgpd --disable-ripngd --disable-ospfd --disable-ospf6d --disable-bgp-announce; \
	else \
		./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) --disable-FEATURE --disable-bgpd --disable-ospfd --disable-ospf6d --disable-bgp-announce; \
	fi
	touch configure
build:
	make -C $(SRCDIR)
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
	-if [ -d "$(SRCDIR)" ]; then \
		cd $(SRCDIR); \
		make install; \
	fi
	
	cp $(TMPDISTDIR)/sbin/zebra $(FSROOT)/sbin/zebra
	$(STRIP) $(FSROOT)/sbin/zebra
	cp $(TMPDISTDIR)/sbin/ripd $(FSROOT)/sbin/ripd
	$(STRIP) $(FSROOT)/sbin/ripd
	if [ "IPV6-$(CONFIG_IPV6)" != "IPV6-" ]; then \
		cp $(TMPDISTDIR)/sbin/ripngd $(FSROOT)/sbin/ripngd; \
		$(STRIP) $(FSROOT)/sbin/ripngd; \
	fi

