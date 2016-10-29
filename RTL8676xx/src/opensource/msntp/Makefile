include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/msntp-1.6
all: build
#configure:
#	cd $(SRCDIR); \
#	find | xargs touch; \
#	./configure --prefix=$(TMPDISTDIR) --host=$(TARGET);
#	touch configure

build:
	make -C $(SRCDIR) all 

clean:
	cd $(SRCDIR); \
	make clean
distclean:	
	cd $(SRCDIR); \
	make distclean; 
#	rm -f configure
	
install:
	cp $(SRCDIR)/msntp $(FSROOT)/sbin/msntp
	$(STRIP) $(FSROOT)/sbin/msntp
