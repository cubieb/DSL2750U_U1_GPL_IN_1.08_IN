include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean distclean 
SRCDIR=$(CURDIR)/inadyn-mt
all:configure build
configure:
	cd $(SRCDIR);\
	./configure --disable-sound  --prefix=/ --target=$(TARGET) --host=$(TARGET); 
	touch configure;
build:
	make -C $(SRCDIR) 

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
	rm -rf configure
	
install:
	install -dv ${FSROOT}/etc/inadyn-mt
	cp ${SRCDIR}/lang/en.lng  ${FSROOT}/etc/inadyn-mt
	cp $(SRCDIR)/src/inadyn-mt $(FSROOT)/usr/bin 
	$(STRIP) $(FSROOT)/usr/bin/inadyn-mt
	
