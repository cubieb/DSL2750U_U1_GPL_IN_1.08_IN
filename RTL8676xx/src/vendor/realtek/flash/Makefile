
EXTRA_CFLAGS =-I$(BUILDDIR)/include 
EXTRA_LDFLAGS =-lutil -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz

E_TARGET=flash
obj-y = flash.o

include $(ROOTDIR)/Rules.make

installme::
	mkdir -p $(FSROOT)/usr/bin/
	install -m 755 flash $(FSROOT)/usr/bin/
	$(STRIP) $(FSROOT)/usr/bin/flash

clean:
	-rm -f flash *.elf *.o

