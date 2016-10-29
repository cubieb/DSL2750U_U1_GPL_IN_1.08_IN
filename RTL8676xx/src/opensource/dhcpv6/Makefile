include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean install 
SRCDIR=$(CURDIR)/wide-dhcpv6
all: build

build:
	make -C $(SRCDIR)

clean:
	make -C $(SRCDIR) clean

distclean:	
	make -C $(SRCDIR) distclean

CONFDIR=$(FSROOT)/var/dhcpv6
	
install:
	make -C $(SRCDIR) install
	$(STRIP) $(FSROOT)/usr/sbin/dhcp6s
	$(STRIP) $(FSROOT)/usr/sbin/dhcp6c
	mkdir -p $(CONFDIR)
	touch -f $(CONFDIR)/dhcp6s.conf
	touch -f $(CONFDIR)/dhcp6c.conf
	-cd $(FSROOT)/etc; ln -fs ../var/dhcpv6/dhcp6s.conf ./dhcp6s.conf; \
    ln -fs ../var/dhcpv6/dhcp6c.conf ./dhcp6c.conf;
