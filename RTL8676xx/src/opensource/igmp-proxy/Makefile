include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean  
SRCDIR=$(CURDIR)
EXTRA_CFLAGS=-g
CFLAGS=-Os
CC=$(CROSS_COMPILE)gcc

EXEC=igmpproxy

IGMP_PROXY_OBJ= igmpproxy.o config.o confread.o request.o udpsock.o mcgroup.o rttable.o \
		igmp.o ifvc.o callout.o kern.o syslog.o lib.o mroute-api.o

all: $(EXEC)

igmpproxy: $(IGMP_PROXY_OBJ)
	$(CC) $(CFLAGS) -o $@ $(IGMP_PROXY_OBJ)
	mkdir -p  $(FSROOT)/sbin $(TMPDISTDIR)/sbin
	install -m 0755 $(SRCDIR)/igmpproxy $(TMPDISTDIR)/sbin/igmpproxy
	install -m 0755 $(SRCDIR)/igmpproxy.sh $(TMPDISTDIR)/sbin/igmpproxy.sh

romfs:
	$(ROMFSINST) /bin/$(EXEC)
	$(ROMFSINST) /bin/igmpproxy.sh

clean:
	rm -f *.o *.asm build.h $(EXEC)  

install:
	cp $(TMPDISTDIR)/sbin/igmpproxy $(FSROOT)/sbin/igmpproxy
	$(STRIP) $(FSROOT)/sbin/igmpproxy
	cp $(TMPDISTDIR)/sbin/igmpproxy.sh $(FSROOT)/sbin/igmpproxy.sh

