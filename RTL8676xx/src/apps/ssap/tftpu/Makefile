#include $(ROOTDIR)/.config

include $(BUILDDIR)/arch.mk
obj-y = tftpd.o
E_TARGET = tftpd

EXTRA_CFLAGS=-fPIC -I../msg -I$(BUILDDIR) -I$(BUILDDIR)/apps/include  -I$(TMPDISTDIR)/include

EXTRA_LDFLAGS=-lutil -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz

include $(ROOTDIR)/Rules.make

installme::
	$(Q)cp $(E_TARGET) $(FSROOT)/usr/bin/
	$(Q)$(STRIP) $(FSROOT)/usr/bin/$(E_TARGET)

