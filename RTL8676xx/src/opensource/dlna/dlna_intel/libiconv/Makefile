ifeq "$(ROOTDIR)" "" 
	export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
AUTO_BUILDDIR=$(CURDIR)/libiconv-1.13.1
EXTRA_PARAM=
LIBS=
LDFLAGS=-L$(TMPDISTDIR)/lib
CFLAGS=-I$(TMPDISTDIR)/include -Os -DTBS_MINI_CHARSET
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
	cp -Pf $(TMPDISTDIR)/lib/libiconv*so* $(FSROOT)/lib
