include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean

SRCDIR=$(CURDIR)/ntfs-3g
#export PATH:=$(TOOLCHAINS_PATH)/../${TARGET}/bin:$(PATH)
#LDFLAGS +=-L$(TOOLCHAINS_SYSROOT)./lib -L$(TOOLCHAINS_SYSROOT)./usr/lib -L$(TMPDISTDIR)/lib -I${BUILDDIR} -I$(SRCDIR)/include/fuse-lite -I$(SRCDIR)/include/ntfs-3g
#CPPFLAGS=-I$(TOOLCHAINS_SYSINCLUDE) -I${BUILDDIR} 
#export LDFLAGS CPPFLAGS
#export DESTDIR:=#$(FSROOT)
#export libdir:=$(FSROOT)/lib
#export rootbindir:=$(FSROOT)/bin
#export rootlibdir:=$(FSROOT)/lib
#export INSTALL:=$(FSROOT)/usr/bin/install
#BIN_PATH=$(FSROOT)/usr/sbin
#LIB_PATH=$(FSROOT)/lib


all:configure build

configure:
#	echo $(rootlibdir)
	echo "TARGET:"$(TARGET)
	cd $(SRCDIR);\
	./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) --enable-really-static --with-fuse=internal;
	touch configure;
	
build:
	make -C $(SRCDIR) all
#	cp $(SRCDIR)/libntfs-3g/.libs/libntfs-3g.a			$(ROOTDIR)/build/romfs/rootfs/lib/
#	cp $(SRCDIR)/libntfs-3g/.libs/libntfs-3g.la			$(ROOTDIR)/build/romfs/rootfs/lib/
#	cp $(SRCDIR)/libntfs-3g/.libs/libntfs-3g.so 			$(ROOTDIR)/build/romfs/rootfs/lib/
#	cp $(SRCDIR)/libntfs-3g/.libs/libntfs-3g.so.54			$(ROOTDIR)/build/romfs/rootfs/lib/
#	cp $(SRCDIR)/libntfs-3g/.libs/libntfs-3g.so.54.0.0		$(ROOTDIR)/build/romfs/rootfs/lib/
	
#	cp $(SRCDIR)/src/ntfs-3g.probe				$(ROOTDIR)/build/romfs/rootfs/usr/bin/
#	cp $(SRCDIR)/include/ntfs-3g					$(ROOTDIR)/build/romfs/user
	
#	cp $(SRCDIR)/src/ntfs-3g  $(FSROOT)/bin/ntfs-3g
	
install:
	cp $(SRCDIR)/src/ntfs-3g  $(FSROOT)/bin/ntfs-3g
	chmod 777 $(FSROOT)/bin/ntfs-3g
#	$(MAKE) -C $(SRCDIR) install
#	cp -f $(TMPDISTDIR)/ $(FSROOT)/
	$(STRIP) $(FSROOT)/bin/ntfs-3g
clean:
	make -C $(SRCDIR) clean

distclean:
	make $(SRCDIR) distclean
	rm -f configure
