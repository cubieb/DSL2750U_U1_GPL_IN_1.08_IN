include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 

SRCDIR=$(CURDIR)/libnfnetlink-0.0.33
KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/$(TBS_KERNEL_VERSION)/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

all:configure build
configure:
	if [ ! -d "$(SRCDIR)" ]; then \
		tar -zxvf libnfnetlink-0.0.33.tar.bz2; \
	fi
	cd $(SRCDIR); \
	find | xargs touch; \
	./configure --prefix=$(TMPDISTDIR) --host=${TARGET} --disable-shared;
	touch configure
build:
	make -C $(SRCDIR) all install

clean:
	cd $(SRCDIR); \
	make clean
distclean:	
	cd $(CURDIR); \
	rm -rf configure libnfnetlink-0.0.33
	
install:
	#cp -Pf $(TMPDISTDIR)/lib/libnfnetlink.so* $(FSROOT)/lib
