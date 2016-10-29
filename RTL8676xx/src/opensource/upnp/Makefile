include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean  

ifdef CONFIG_OPENSOURCE_UPnP_12
SRCDIR=$(CURDIR)/miniupnpd-1.2
endif
ifdef CONFIG_OPENSOURCE_UPnP_15
SRCDIR=$(CURDIR)/miniupnpd-1.5
endif
all: build

build:
	make -C $(SRCDIR) -f Makefile.tbs CC=$(CROSS_COMPILE)gcc
	install -m 0755 $(SRCDIR)/miniupnpd $(TMPDISTDIR)/sbin/miniupnpd 
	install -m 0755 $(CURDIR)/upnp.sh $(TMPDISTDIR)/sbin/upnp.sh

clean :
	make -C $(SRCDIR) -f Makefile.tbs clean

install :
	cp $(TMPDISTDIR)/sbin/miniupnpd $(FSROOT)/sbin/miniupnpd
	cp $(TMPDISTDIR)/sbin/upnp.sh $(FSROOT)/sbin/upnp.sh
	$(STRIP) $(FSROOT)/sbin/miniupnpd
