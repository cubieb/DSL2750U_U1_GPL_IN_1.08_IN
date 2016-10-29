ifeq "$(ROOTDIR)" "" 
	export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
AUTO_BUILDDIR=$(CURDIR)/libdlna-0.2.3
EXTRA_PARAM=--with-ffmpeg-dir=$(TMPDISTDIR) --disable-static
LIBS=
LDFLAGS= -L$(TMPDISTDIR)/lib -L$(TOOLCHAINS_SYSROOT)/lib
CFLAGS=-I$(TMPDISTDIR)/include -Os
export LIBS LDFLAGS CFLAGS
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

