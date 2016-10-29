include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean
SRCDIR=$(CURDIR)/ping
all: build

build:
	make -C $(SRCDIR) CC=$(CROSS_COMPILE)gcc all 

clean:
	cd $(SRCDIR); \
	make clean

install:
	mkdir -p $(FSROOT)/bin
	cp -fr $(SRCDIR)/tbsping  $(FSROOT)/bin/
	$(STRIP) $(FSROOT)/bin/tbsping
