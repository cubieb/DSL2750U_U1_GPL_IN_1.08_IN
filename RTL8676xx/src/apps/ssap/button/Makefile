EXTRA_CFLAGS= -fPIC -I$(KERNEL_DIR)/include -I../msg -I../../include  -I$(TMPDISTDIR)/include -I../syslog -DMON_DEBUG -Wall -I$(BUILDDIR)/include -I$(BUILDDIR)
EXTRA_LDFLAGS=-lutil -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz
E_TARGET=btn
obj-y= btn_main.o app_btn_cfg.o
include $(ROOTDIR)/Rules.make
installme::
	$(Q)cp btn $(FSROOT)/usr/bin/
	$(Q)$(STRIP) $(FSROOT)/usr/bin/btn
