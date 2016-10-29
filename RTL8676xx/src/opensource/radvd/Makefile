include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean install 
SRCDIR=$(CURDIR)/radvd-1.5
all:configure build
configure:
	cd $(SRCDIR); \
	./configure --prefix=$(TMPDISTDIR) --host=${TARGET} --sysconfdir=/etc CFLAGS=-Os;
	touch configure

build:
	make -C $(SRCDIR) all

clean:
	make -C $(SRCDIR) clean
	
distclean:
	make -C $(SRCDIR) distclean
	rm -f configure
	
install:
	mkdir -p $(FSROOT)/usr/sbin
	install -c $(SRCDIR)/radvd $(FSROOT)/usr/sbin/radvd
	$(STRIP) $(FSROOT)/usr/sbin/radvd
ifdef CONFIG_OPENSOURCE_RADVDUMP
	install -c $(SRCDIR)/radvdump $(FSROOT)/usr/sbin/radvdump
	$(STRIP) $(FSROOT)/usr/sbin/radvdump
endif
	touch -f $(FSROOT)/var/radvd.conf
	-cd $(FSROOT)/etc; ln -fs ../var/radvd.conf ./radvd.conf