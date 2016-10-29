include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean  
SRCDIR=$(CURDIR)/freesupplicant
EXTRA_CFLAGS=-g
all: build

build:
	mkdir -p  $(FSROOT)/sbin $(TMPDISTDIR)/sbin
	make -C $(SRCDIR) TBS_filesystem=$(TMPDISTDIR) TBS_supp CC=$(CROSS_COMPILE)gcc
	install -m 0755 $(SRCDIR)/supp $(TMPDISTDIR)/sbin/supp

clean:
	cd $(SRCDIR); \
	make clean

install:
	cp $(TMPDISTDIR)/sbin/supp $(FSROOT)/sbin/supp
	$(STRIP) $(FSROOT)/sbin/supp
