include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/ipsec-tools-0.7.2/
KERNEL_HEADER=$(BUILDDIR)/kernel/$(TBS_KERNEL_VERSION)/include
OPENSSL_HEADER=$(BUILDDIR)/opensource/openssl/openssl-0.9.7m
#OPENSSL_HEADER=$(BUILDDIR)/opensource/openssl/openssl-0.9.8h
#FLEX_SRC=$(BUILDDIR)/opensource/flex/libflex/libfl.a
FLEX_LIB=$(BUILDDIR)/opensource/flex/lib/libfl.so

TARGET_RACOON=racoon
TARGET_SETKEY=setkey

all: configure build
configure:
	cd $(SRCDIR);\
	./configure --with-kernel-headers=$(KERNEL_HEADER) \
		--with-openssl=$(OPENSSL_HEADER) \
	       	--host=$(TARGET) \
		--enable-security-context=no --prefix= ;
	touch configure;
build:
	make -C $(SRCDIR) all 

clean:
	cd $(SRCDIR); \
	make clean
	
distclean:	
	cd $(SRCDIR); \
	make distclean; 
	rm -f configure
	
install:
	mkdir -p $(FSROOT)/usr/bin
	cp -rf $(SRCDIR)/src/racoon/$(TARGET_RACOON) $(FSROOT)/usr/bin/$(TARGET_RACOON)
	$(STRIP) $(FSROOT)/usr/bin/$(TARGET_RACOON)
	cp -rf $(SRCDIR)/src/setkey/$(TARGET_SETKEY) $(FSROOT)/usr/bin/$(TARGET_SETKEY)
	$(STRIP) $(FSROOT)/usr/bin/$(TARGET_SETKEY)
	
