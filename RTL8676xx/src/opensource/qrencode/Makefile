include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean

vers=3.1.1
src_dir=$(shell pwd)/qrencode-$(vers)

all:prepare build install

prepare:
	if [ -d $(src_dir) ]; then rm -rf $(src_dir); fi
	tar -jxf qrencode-$(vers).tar.bz2
	cd $(src_dir);CC=$(CROSS_COMPILE)gcc CFLAGS=-I$(TMPDISTDIR)/include LDFLAGS=-L$(TMPDISTDIR)/lib ./configure --prefix=$(TMPDISTDIR) --host=${TARGET}

build:
	make -C $(src_dir) 

clean:
	rm -rf $(FSROOT)/lib/libqrencode.so* $(FSROOT)/sbin/qrencode
	rm -rf $(src_dir)

install:
	make -C $(src_dir) install
	cp $(TMPDISTDIR)/bin/qrencode $(FSROOT)/sbin/qrencode
	if [ ! -d $(FSROOT)/lib ]; then mkdir -p $(FSROOT)/lib; fi
	cp -Pf $(TMPDISTDIR)/lib/libqrencode.so* $(FSROOT)/lib
