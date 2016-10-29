EXTRA_CFLAGS=-fPIC -I$(BUILDDIR)/kernel/${TBS_KERNEL_VERSION}/include/ -I../msg -I../../include -I../../../include -I$(TMPDISTDIR)/include -I../syslog -Wall -I$(BUILDDIR)
EXTRA_LDFLAGS=-lutil -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz
E_TARGET=protest
obj-y= protest.o
include $(ROOTDIR)/Rules.make
installme::
	$(Q)cp protest $(FSROOT)/usr/bin/
	$(Q)$(STRIP) $(FSROOT)/usr/bin/protest
