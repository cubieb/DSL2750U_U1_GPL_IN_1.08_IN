include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean  

SRCDIR=$(CURDIR)/pptp
#EXTRA_CFLAGS=-g
all: build

build:
	mkdir -p  $(FSROOT)/sbin $(TMPDISTDIR)/sbin
	mkdir -p  $(FSROOT)/etc $(TMPDISTDIR)/etc
	make -C $(SRCDIR) TBS_filesystem=$(TMPDISTDIR) CC=$(CROSS_COMPILE)gcc
	install -m 0755 $(SRCDIR)/pptp $(TMPDISTDIR)/sbin/pptp

clean:
	cd $(SRCDIR); \
	make clean

install:
	cp $(TMPDISTDIR)/sbin/pptp $(FSROOT)/sbin/pptp
	$(STRIP) $(FSROOT)/sbin/pptp

