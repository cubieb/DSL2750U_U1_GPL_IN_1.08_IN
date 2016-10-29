ifeq "$(ROOTDIR)" "" 
	export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
AUTO_BUILDDIR=$(CURDIR)/ushare-1.1a
EXTRA_PARAM=--enable-dlna --disable-nls
LIBS=
LDFLAGS=-L$(TMPDISTDIR)/lib -lm -lz 
CFLAGS=-I$(TMPDISTDIR)/include -Os -I$(AUTO_BUILDDIR)/
PKG_CONFIG_PATH=$(TMPDISTDIR)/lib/pkgconfig
export LIBS LDFLAGS CFLAGS PKG_CONFIG_PATH
MYMAKE=make -C $(AUTO_BUILDDIR)
.PHONY: all clean distclean 
all:configure build

configure:
	cd $(AUTO_BUILDDIR);\
	./configure --prefix=$(TMPDISTDIR) --cross-compile --cross-prefix=$(CROSS_COMPILE) $(EXTRA_PARAM)
	touch configure
# build file depends
build:configure
	$(MYMAKE)
	$(MYMAKE) install
	touch build
clean:
	$(MYMAKE) clean
	rm -f build
distclean:    
	$(MYMAKE) distclean
	rm -f	configure build
        
install:
	cp $(TMPDISTDIR)/bin/ushare $(FSROOT)/sbin/ushare
