ifeq "$(ROOTDIR)" ""
	export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif

include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: configure all build clean distclean 

ifdef CONFIG_IPTABLES_1_3_3
IPTABLES_VER=iptables-1.3.3
endif

ifdef CONFIG_IPTABLES_1_4_4
IPTABLES_VER=iptables-1.4.4
endif

ifdef CONFIG_IPTABLES_1_4_6
IPTABLES_VER=iptables-1.4.6
endif


SRCDIR=$(CURDIR)/$(IPTABLES_VER)

ifndef CONFIG_OPENSOURCE_IPT_V4
ENABLE_IPV4_TRUE="#"
else 
ENABLE_IPV4_TRUE=
endif 

ifndef CONFIG_OPENSOURCE_IPT_V6
ENABLE_IPV6_TRUE="#"
else 
ENABLE_IPV6_TRUE=
endif 

all: configure build

build:
	make -C $(SRCDIR) all install 
	
configure:
	cd $(SRCDIR); ./configure --prefix=$(TMPDISTDIR) \
	--host=$(TARGET) --with-ksource=$(KERNEL_DIR) \
	--enable-static --disable-shared
	touch configure
clean:
	if [ -s $(SRCDIR)/Makefile ]; then make -C $(SRCDIR) clean; fi
	rm -f $(CURDIR)/configure
distclean:      
	if [ -s $(SRCDIR)/Makefile ]; then make -C $(SRCDIR) distclean fi
	rm -f $(CURDIR)/configure
	
install:
#	cp -Prf $(TMPDISTDIR)/lib/libiptc.so* $(FSROOT)/lib/
#	cp -Prf $(TMPDISTDIR)/lib/libxtables.so* $(FSROOT)/lib/
#	cp -Pf $(SRCDIR)/extensions/*.so $(FSROOT)/lib/
#	cp -Prf $(TMPDISTDIR)/libexec/xtables $(FSROOT)/lib/
	cp $(TMPDISTDIR)/sbin/iptables $(FSROOT)/sbin/iptables
	$(STRIP) $(TMPDISTDIR)/sbin/iptables $(FSROOT)/sbin/iptables
	cp $(TMPDISTDIR)/sbin/ip6tables $(FSROOT)/sbin/ip6tables
	$(STRIP) $(TMPDISTDIR)/sbin/ip6tables $(FSROOT)/sbin/ip6tables
