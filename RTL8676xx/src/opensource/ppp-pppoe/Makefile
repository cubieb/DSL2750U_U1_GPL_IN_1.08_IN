include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/ppp-2.4.3
KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export EXTRA_CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_  \
			    -I./ -I$(TOOLCHAINS_SYSROOT)/include  \
				-I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include  \
				-idirafter $(KERNEL_INCLUDE_DIR)
all:configure build
configure:
	cd $(SRCDIR);\
	./configure; 
	touch configure;
build:
	make -C $(SRCDIR) all 

clean:
	cd $(SRCDIR); \
	make clean
distclean:	
	cd $(SRCDIR); \
	make dist-clean; 
	rm -f configure
	
install:
	mkdir -p  $(FSROOT)/usr/sbin
	cp -f $(SRCDIR)/pppd/pppd $(FSROOT)/usr/sbin/
	cp -f $(SRCDIR)/chat/chat $(FSROOT)/usr/sbin/
	$(STRIP) $(FSROOT)/usr/sbin/pppd
	mkdir -p $(FSROOT)/usr/lib/pppd/2.4.3
	cp -f $(SRCDIR)/pppd/plugins/rp-pppoe/rp-pppoe.so $(FSROOT)/usr/lib/pppd/2.4.3
	$(STRIP) $(FSROOT)/usr/lib/pppd/2.4.3/rp-pppoe.so
	cp -f $(SRCDIR)/pppd/plugins/pppoatm/pppoatm.so $(FSROOT)/usr/lib/pppd/2.4.3
	$(STRIP) $(FSROOT)/usr/lib/pppd/2.4.3/pppoatm.so
