#
# logmonitor: Monitoring system events log file, sompress and save it to file
#     target: /usr/bin/logmonitor
#

EXTRA_CFLAGS=  -I../../include -I${BUILDDIR} -I../../ssap/msg -I${BUILDDIR}/include\
	       -I../../ssap/lib -I$(TMPDISTDIR)/include

EXTRA_LDFLAGS=-lutil -lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz


E_TARGET=logmonitor

obj-y= logger_sysevt.o

include $(ROOTDIR)/Rules.make

installme::
	$(Q)cp $(E_TARGET) $(FSROOT)/usr/bin/
	$(Q)$(STRIP) $(FSROOT)/usr/bin/$(E_TARGET)

