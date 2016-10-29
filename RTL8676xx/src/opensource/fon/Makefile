include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean

DDSRCDIR=$(CURDIR)/chillispot-dd
COOVASRC=$(CURDIR)/chillispot-1.0-coova.4
DROPBEARSRC=$(CURDIR)/dropbear-0.51
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

configure:chillispot-dd_configure chillispot-1.0-coova.4_configure dropbear_configure
#	echo $(rootlibdir)
chillispot-dd_configure:
	echo "TARGET:"$(TARGET)
	cd $(DDSRCDIR);\
	./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) --enable-static-exec;
	touch configure;
chillispot-1.0-coova.4_configure:
	cd $(COOVASRC);\
	./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) --enable-static-exec;
	touch configure;
dropbear_configure:
	cd $(DROPBEARSRC);\
	./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) --enable-static-exec --disable-nls --with-shared --disable-pam --enable-openpty --enable-syslog --disable-shadow --disable-lastlog --disable-utmp --disable-utmpx --disable-wtmp --disable-wtmpx --disable-loginfunc --disable-pututline --disable-pututxline --disable-zlib;
	touch configure;
	
	
build:
	make -C $(DDSRCDIR) all
	make -C $(COOVASRC) all
	make -C $(DROPBEARSRC) all
#	cp $(SRCDIR)/libntfs-3g/.libs/libntfs-3g.a			$(ROOTDIR)/build/romfs/rootfs/lib/
#	cp $(SRCDIR)/libntfs-3g/.libs/libntfs-3g.la			$(ROOTDIR)/build/romfs/rootfs/lib/
#	cp $(SRCDIR)/libntfs-3g/.libs/libntfs-3g.so 			$(ROOTDIR)/build/romfs/rootfs/lib/
#	cp $(SRCDIR)/libntfs-3g/.libs/libntfs-3g.so.54			$(ROOTDIR)/build/romfs/rootfs/lib/
#	cp $(SRCDIR)/libntfs-3g/.libs/libntfs-3g.so.54.0.0		$(ROOTDIR)/build/romfs/rootfs/lib/
	
#	cp $(SRCDIR)/src/ntfs-3g.probe				$(ROOTDIR)/build/romfs/rootfs/usr/bin/
#	cp $(SRCDIR)/include/ntfs-3g					$(ROOTDIR)/build/romfs/user
	
#	cp $(SRCDIR)/src/ntfs-3g  $(FSROOT)/bin/ntfs-3g
	
install:
	cp $(DDSRCDIR)/src/chilli  $(FSROOT)/bin/chilli
	chmod 777 $(FSROOT)/bin/chilli
	cp $(DDSRCDIR)/restartchilli  $(FSROOT)/etc/restartchilli
	chmod 777 $(FSROOT)/etc/restartchilli
	cp $(DDSRCDIR)/chilli.conf  $(FSROOT)/etc/chilli.conf
	cp $(DDSRCDIR)/fonkey  $(FSROOT)/etc/fonkey
	mkdir -p $(FSROOT)/dev/net
	rm -rf $(FSROOT)/dev/net/tun
	mknod $(FSROOT)/dev/net/tun c 10 200
	$(STRIP) $(FSROOT)/bin/chilli
	
	cp $(COOVASRC)/ipkg-install/usr/sbin/chilli_radconfig  $(FSROOT)/sbin/chilli_radconfig
	chmod 777 $(FSROOT)/sbin/chilli_radconfig
	$(STRIP) $(FSROOT)/sbin/chilli_radconfig
	
	cp $(DROPBEARSRC)/dbclient  $(FSROOT)/sbin/dbclient
	chmod 777 $(FSROOT)/sbin/dbclient
	$(STRIP) $(FSROOT)/sbin/dbclient
clean:
	make -C $(DDSRCDIR) clean
	make -C $(COOVASRC) clean
	make -C $(DROPBEARSRC) clean
	rm -rf  $(FSROOT)/bin/chilli
	rm -rf  $(FSROOT)/etc/restartchilli
	rm -rf  $(FSROOT)/etc/chilli.conf
	rm -rf  $(FSROOT)/etc/fonkey
	rm -rf  $(FSROOT)/sbin/chilli_radconfig
	rm -rf  $(FSROOT)/dev/net/tun
	rm -rf  $(FSROOT)/sbin/dbclient

distclean:
	make $(DDSRCDIR) distclean
	rm -f configure
	make -C $(COOVASRC) distclean
	rm -f configure
	make -C $(DROPBEARSRC) distclean
	rm -f configure
