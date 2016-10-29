
all: adslstart 
adslctrl: adslstart.o

#include $(ROOTDIR)/Rules.make
CFLAGS = -Os -pipe -Wall
CFLAGS +=  -I$(BUILDDIR) -I$(BUILDDIR)/include -I$(BUILDDIR)/kernel/linux-2.6.30.9/include -I$(BUILDDIR)/apps/include
CFLAGS +=-I$(BUILDDIR)/include 
CFLAGS +=-lutil -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz

install:
	mkdir -p $(FSROOT)/usr/bin/
	install -m 755 adslstart $(FSROOT)/usr/bin/
	$(STRIP) $(FSROOT)/usr/bin/adslstart

clean:
	-rm -f adslstart *.elf *.o
