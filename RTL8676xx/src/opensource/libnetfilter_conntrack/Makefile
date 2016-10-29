include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/libnetfilter_conntrack-0.0.89

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

all:configure build
configure:
	if [ ! -d "$(SRCDIR)" ]; then \
		tar -zxvf libnetfilter_conntrack-0.0.89.tar.bz2; \
	fi
	cd $(SRCDIR); \
	find | xargs touch; \
	export PKG_CONFIG_PATH=$(TMPDISTDIR)/lib/pkgconfig; \
	LIBNFNETLINK_CFLAGS=-I$(TMPDISTDIR)/include \
	LIBNFNETLINK_LIBS="-L$(TMPDISTDIR)/lib -lnfnetlink" \
	LIBNETFILTER_CONNTRACK_LIBS="-L$(TMPDISTDIR)/lib -lnetfilter_conntrack" \
	./configure --prefix=$(TMPDISTDIR) --host=${TARGET} --disable-shared;
	touch configure

build:
	make -C $(SRCDIR) all install
	cd $(SRCDIR)/utils; \
	$(CROSS_COMPILE)gcc -o flush_conntrack conntrack_flush.o -L$(TMPDISTDIR)/lib -lnetfilter_conntrack -lnfnetlink; \
	install -m 0755 $(SRCDIR)/utils/flush_conntrack $(TMPDISTDIR)/sbin/flush_conntrack

clean:
	cd $(SRCDIR); \
	make clean
distclean:	
	cd $(CURDIR); \
	rm -rf configure libnetfilter_conntrack-0.0.89
	
install:
#	cp -Pf $(TMPDISTDIR)/lib/libnetfilter_conntrack.so* $(FSROOT)/lib
	cp $(TMPDISTDIR)/sbin/flush_conntrack $(FSROOT)/sbin/flush_conntrack
	$(STRIP) $(FSROOT)/sbin/flush_conntrack
	
