include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean disclean install

SRCDIR=$(CURDIR)/siproxd-0.7.2

all: configure build


configure:
	cd $(SRCDIR); \
	./configure --prefix=$(TMPDISTDIR) --host=arm-linux --with-libosip-prefix=$(TMPDISTDIR) \
	--disable-shared --enable-static-libosip2 CFLAGS=-Os;
	touch configure

build:
	make -C $(SRCDIR) all 

clean:
	cd $(SRCDIR); \
	make clean

distclean:
	make -C $(SRCDIR) distclean
	rm -f configure

install:
#	make -C $(SRCDIR) install
	cp $(SRCDIR)/src/siproxd $(FSROOT)/sbin/siproxd
	$(STRIP) $(FSROOT)/sbin/siproxd	
