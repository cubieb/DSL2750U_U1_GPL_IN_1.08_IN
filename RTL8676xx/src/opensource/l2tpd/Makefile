include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean  

SRCDIR=$(CURDIR)/l2tpd-0.70
#EXTRA_CFLAGS=-g
all: build

build:
	mkdir -p  $(FSROOT)/sbin $(TMPDISTDIR)/sbin
	mkdir -p  $(FSROOT)/etc $(TMPDISTDIR)/etc
	make -C $(SRCDIR) TBS_filesystem=$(TMPDISTDIR) CC=$(CROSS_COMPILE)gcc
	install -m 0755 $(SRCDIR)/l2tpd $(TMPDISTDIR)/sbin/l2tpd

clean:
	cd $(SRCDIR); \
	make clean

install:
	cp $(TMPDISTDIR)/sbin/l2tpd $(FSROOT)/sbin/l2tpd
	$(STRIP) $(FSROOT)/sbin/l2tpd
	cd $(FSROOT)/etc; ln -sf ../var/l2tpd.conf l2tpd.conf

