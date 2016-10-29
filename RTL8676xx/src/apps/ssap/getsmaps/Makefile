#EXTRA_CFLAGS=-I$(BUILDDIR)/kernel/${TBS_KERNEL_VERSION}/include/ -I../msg -I../../include -I../../../include -I$(TMPDISTDIR)/include -I../syslog -Wall
#EXTRA_LDFLAGS=-lutil -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz
E_TARGET=getsmaps
obj-y= getsmaps.o
include $(ROOTDIR)/Rules.make
installme::
	$(Q)cp getsmaps $(FSROOT)/usr/bin/
	$(Q)$(STRIP) $(FSROOT)/usr/bin/getsmaps
