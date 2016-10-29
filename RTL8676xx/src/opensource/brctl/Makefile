include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/bridge-utils-1.2
all:configure build
CFLAGS=-Os
ifeq ("$(CONFIG_VLAN_MODE)","y")
CFLAGS +=-DCONFIG_VLAN_MODE
endif

export CFLAGS
configure:
	cd $(SRCDIR); \
	find  | xargs touch;\
	if [ ! -f configure ]; then \
	  autoconf; \
	fi; \
	./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) --with-linux-headers=$(BUILDDIR)/kernel/$(TBS_KERNEL_VERSION)/include/; 
	touch configure
build:
	make -C $(SRCDIR) all install
	#CFLAGS=${CFLAGS}; 

clean:
	cd $(SRCDIR); \
	make clean
distclean:	
	cd $(SRCDIR); \
	make distclean; 
	rm -f configure
	
install:
	cp $(TMPDISTDIR)/sbin/brctl $(FSROOT)/sbin/brctl
	$(STRIP) $(FSROOT)/sbin/brctl
