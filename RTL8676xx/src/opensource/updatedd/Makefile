include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/updatedd-2.3
all:configure build
configure:
	cd $(SRCDIR);\
	./configure --prefix=/ --target=$(TARGET) --host=$(TARGET); 
	touch configure;
build:
	make -C $(SRCDIR) all 

clean:
	cd $(SRCDIR); \
	make clean
	#cd $(SRCDIR)/src/plugins; \
	#make uninstall-pkglibLTLIBRARIES
	#cd $(SRCDIR)/src/plugins; \
	#make clean-pkglibLTLIBRARIES

distclean:	
	cd $(SRCDIR); \
	make distclean; 
	rm -f configure
	
install:
	cp $(SRCDIR)/src/updatedd $(FSROOT)/usr/bin 
	$(STRIP) $(FSROOT)/usr/bin/updatedd
	mkdir -p $(FSROOT)/lib/updatedd
	cp $(SRCDIR)/src/plugins/*.so* $(FSROOT)/lib/updatedd/ -rf
	$(STRIP) $(FSROOT)/lib/updatedd/*.so*
	
