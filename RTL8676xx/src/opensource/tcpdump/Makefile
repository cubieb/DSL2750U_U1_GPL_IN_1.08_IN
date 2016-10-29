ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all tcpdump libpcap tcpdump_clean libpcap_clean distclean
TCPDUMPDIR=$(BUILDDIR)/opensource/tcpdump/tcpdump-3.9.8
LIBPCAPDIR=$(BUILDDIR)/opensource/tcpdump/libpcap-0.9.8
LDFLAGS=-L$(TMPDISTDIR)/lib
CPPFLAGS=-I$(TMPDISTDIR)/include
export LDFLAGS CPPFLAGS
all:libpcap tcpdump
configure_tcpdump:
	cd $(TCPDUMPDIR); \
  find | xargs touch; \
  ./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) --without-crypto;
	touch configure_tcpdump

tcpdump:configure_tcpdump
	make all install -C $(TCPDUMPDIR)

configure_libpcap:
	cd $(LIBPCAPDIR); \
	find -name Makefile.in | xargs touch; \
  ./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) --with-pcap=linux;
	touch configure_libpcap

libpcap:configure_libpcap
	make all install -C $(LIBPCAPDIR)
#	cp -f $(LIBPCAPDIR)/libpcap/x*.h $(TMPDISTDIR)/include/libpcap

clean:tcpdump_clean libpcap_clean


tcpdump_clean:
	cd $(TCPDUMPDIR); \
	make clean

libpcap_clean:
	cd $(LIBPCAPDIR); \
	make clean
	
distclean:tcpdump_distclean libpcap_distclean

tcpdump_distclean:
	cd $(TCPDUMPDIR); \
	make distclean
	rm -f configure_tcpdump

libpcap_distclean:
	cd $(LIBPCAPDIR); \
	make distclean
	rm -f configure_libpcap

install:
#	cp -Pf $(TMPDISTDIR)/lib/libtcpdump.so* $(FSROOT)/lib
#	${STRIP} $(TMPDISTDIR)/lib/libtcpdump.so*  
	${STRIP} $(TMPDISTDIR)/sbin/tcpdump
	cp -Pf $(TMPDISTDIR)/sbin/tcpdump $(FSROOT)/sbin
	
