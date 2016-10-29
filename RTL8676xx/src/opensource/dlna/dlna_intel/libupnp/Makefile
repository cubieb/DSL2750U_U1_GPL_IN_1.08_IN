ifeq "$(ROOTDIR)" "" 
	export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
AUTO_BUILDDIR=$(CURDIR)/libupnp-1.6.6
EXTRA_PARAM= --disable-debug  --disable-client --enable-shared=yes --enable-static=no
LIBS=
LDFLAGS=-L$(TMPDISTDIR)/lib
CFLAGS=-I$(TMPDISTDIR)/include -Os
export LIBS LDFLAGS CFLAGS
MYMAKE=make -C $(AUTO_BUILDDIR)
.PHONY: all clean distclean 
all:configure build

configure:
	cd $(AUTO_BUILDDIR);\
	./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) $(EXTRA_PARAM)
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
	cp $(TMPDISTDIR)/lib/libupnp.so.3 $(FSROOT)/lib
	cp $(TMPDISTDIR)/lib/libthreadutil.so.2 $(FSROOT)/lib
	cp $(TMPDISTDIR)/lib/libixml.so.2 $(FSROOT)/lib
