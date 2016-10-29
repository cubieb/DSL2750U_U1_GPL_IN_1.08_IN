EXTRA_CFLAGS=-fPIC -I../msg -I$(BUILDDIR) -I$(BUILDDIR)/apps/include  -I$(TMPDISTDIR)/include -I../syslog -Wall -D_PC_DEBUG -I$(SRCDIR)/include
EXTRA_LDFLAGS=-lutil -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz
E_TARGET=flash_test

obj-y = flash_test.o

include $(ROOTDIR)/Rules.make
installme::
	$(Q)cp flash_test $(FSROOT)/usr/bin/
	$(Q)$(STRIP) $(FSROOT)/usr/bin/flash_test

