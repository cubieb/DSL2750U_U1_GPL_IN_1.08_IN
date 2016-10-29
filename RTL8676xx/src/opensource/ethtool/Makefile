include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
#SRCDIR=$(CURDIR)/ethtool-5
all:configure build
configure:
#	cd $(SRCDIR); \
	find | xargs touch; \
	./configure --prefix=$(TMPDISTDIR) --host=$(TARGET);
#	touch configure

build:
	$(CC) -o ethtool -Os $(CFLAGS) ethtool.c -DHAVE_CONFIG_H 
#	make -C $(SRCDIR) all install  SOURCES="ethtool.c ethtool-copy.h ethtool-util.h"

clean:
#	cd $(SRCDIR); \
#	make clean
	rm -f *.o ethtool
distclean:clean
#	cd $(SRCDIR); \
#	make distclean; 
#	rm -f configure
	
install:
	cp ethtool $(FSROOT)/sbin/ethtool
	$(STRIP) $(FSROOT)/sbin/ethtool
