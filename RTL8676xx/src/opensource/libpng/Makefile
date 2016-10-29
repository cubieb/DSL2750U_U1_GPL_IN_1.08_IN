include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk

.PHONY: all preparedir build clean distclean

VERS=1.2.44
SRC_DIR=$(shell pwd)/libpng-$(VERS)

all:preparedir build install

preparedir:
	if [ -d $(SRC_DIR) ]; then rm -rf $(SRC_DIR); fi
	tar -jxf libpng-$(VERS).tar.bz2
	cd $(SRC_DIR); LDFLAGS=-L$(TMPDISTDIR)/lib CFLAGS=-I$(TMPDISTDIR)/include CPPFLAGS=-I$(TMPDISTDIR)/include ./configure --prefix=$(TMPDISTDIR) --host=${TARGET}

build:
	make -C $(SRC_DIR)

clean:
	rm -rf $(FSROOT)/lib/libpng12.so*
	rm -rf $(SRC_DIR)
	
install:
	make -C $(SRC_DIR) install
	if [ ! -d $(FSROOT)/lib ]; then mkdir -p $(FSROOT)/lib; fi
	cp -Pf $(TMPDISTDIR)/lib/libpng12.so* $(FSROOT)/lib
