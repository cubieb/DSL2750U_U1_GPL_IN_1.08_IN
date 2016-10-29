include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean

DDSRCDIR=$(CURDIR)/wput-0.6.2

all:configure build

configure:
	echo "TARGET:"$(TARGET)
	cd $(DDSRCDIR);\
	./configure --prefix=$(TMPDISTDIR) --host=$(TARGET) --enable-static-exec;
	touch configure;

build:
	make -C $(DDSRCDIR) all
	
install:
	cp $(DDSRCDIR)/wput  $(FSROOT)/usr/bin/wput
	chmod 777 $(FSROOT)/usr/bin/wput

clean:
	make -C $(DDSRCDIR) clean
	rm -rf  $(FSROOT)/bin/wput

distclean:
	make $(DDSRCDIR) distclean
	rm -f configure
