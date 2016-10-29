EXTRA_CFLAGS=-fPIC -I../msg -I$(BUILDDIR) -I$(BUILDDIR)/apps/include  -I$(TMPDISTDIR)/include -I../syslog -Wall
EXTRA_CFLAGS+=-I$(BUILDDIR)/apps/logic/dsl -I$(BUILDDIR)/driver/dsl/infineon/drv_dsl_cpe_api/src/include
EXTRA_CFLAGS+=-I../msg -I../../include  -I$(TMPDISTDIR)/include -I../syslog -Wall -I$(BUILDDIR)/include -I$(ROOTDIR)/build/kernel/${TBS_KERNEL_VERSION}/include
#EXTRA_LDFLAGS=-lutil -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz
EXTRA_LDFLAGS= -L$(BUILDDIR)/apps/ssap/lib -lssap -L$(TMPDISTDIR)/lib  -lexpat -lz

E_TARGET=mon
obj-y = mon_main.o mon_func.o $(BUILDDIR)/apps/logic/dsl/dsl_api_sub.o

include $(ROOTDIR)/Rules.make
installme::
	$(Q)cp mon $(FSROOT)/usr/bin/
	$(Q)$(STRIP) $(FSROOT)/usr/bin/mon
	$(Q)ln -sf mon $(FSROOT)/usr/bin/btn
