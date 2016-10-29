include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/zlib-1.2.3
all:configure build
configure:
	cd $(SRCDIR); \
	find | xargs touch; \
	./configure --shared --prefix=$(TMPDISTDIR);
	touch configure
build:
	make -C $(SRCDIR) all install

clean:
	make -C $(SRCDIR) clean
	rm -rf $(TMPDISTDIR)/lib/libz.so* $(FSROOT)/lib/libz.so*

distclean: clean	
	make -C $(SRCDIR) distclean
	rm -f configure
	
install:
	cp -Pf $(TMPDISTDIR)/lib/libz.so* $(FSROOT)/lib
