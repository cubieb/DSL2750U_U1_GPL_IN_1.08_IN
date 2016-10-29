include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all expat scew expat_clean scew_clean distclean
EXPATDIR=$(BUILDDIR)/opensource/xmlparser/expat
SCEWDIR=$(BUILDDIR)/opensource/xmlparser/scew
LDFLAGS=-L$(TMPDISTDIR)/lib
CPPFLAGS=-I$(TMPDISTDIR)/include
CPPFLAGS+=-DDONOT_CONVERT_TO_UTF8
export LDFLAGS CPPFLAGS
CFLAGS+=-DDONOT_CONVERT_TO_UTF8
export CFLAGS
all:expat scew 
configure_expat:
	cd $(EXPATDIR); \
  find | xargs touch; \
  ./configure --prefix=$(TMPDISTDIR) --host=$(TARGET);
	touch configure_expat

expat:configure_expat
	make all install -C $(EXPATDIR)

configure_scew:
	cd $(SCEWDIR); \
	find -name Makefile.in | xargs touch; \
  ./configure --prefix=$(TMPDISTDIR) --host=$(TARGET);
	touch configure_scew

scew:configure_scew
	make all install -C $(SCEWDIR)
	cp -f $(SCEWDIR)/scew/x*.h $(TMPDISTDIR)/include/scew

clean:expat_clean scew_clean
	rm -rf $(TMPDISTDIR)/lib/libscew.a

expat_clean:
	cd $(EXPATDIR); \
	make clean

scew_clean:
	cd $(SCEWDIR); \
	make clean
	
distclean:expat_distclean scew_distclean

expat_distclean:
	cd $(EXPATDIR); \
	make distclean
	rm -f configure_expat

scew_distclean:
	cd $(SCEWDIR); \
	make distclean
	rm -f configure_scew

install:
	-mkdir -p $(FSROOT)/lib
	cp -Pf $(TMPDISTDIR)/lib/libexpat.so* $(FSROOT)/lib
	${STRIP} $(TMPDISTDIR)/lib/libexpat.so*  
