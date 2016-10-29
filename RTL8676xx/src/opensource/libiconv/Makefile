include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
CONV_BUILDDIR=$(CURDIR)/libiconv-1.13.1

LDFLAGS=-L$(TMPDISTDIR)/lib
CFLAGS=-I$(TMPDISTDIR)/include -Os -DTBS_MINI_CHARSET
export LIBS LDFLAGS CFLAGS
MYMAKE=make -C $(CONV_BUILDDIR)
.PHONY: all clean distclean 
all:configure build

configure:
	cd $(CONV_BUILDDIR);\
	./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) 
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
