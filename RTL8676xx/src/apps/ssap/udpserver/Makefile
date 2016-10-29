
include $(BUILDDIR)/arch.mk
obj-y = udpserver.o
E_TARGET = udpserver

EXTRA_CFLAGS=-fPIC -I${BUILDDIR}/apps/include -I${BUILDDIR}/include -I${BUILDDIR} -DPRODUCT=\"$(PRODUCT)\"

ifeq (${PRODUCT}, UML)
EXTRA_CFLAGS += -D_CFG_UML
endif

EXTRA_LDFLAGS=-lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lexpat -lz

include $(ROOTDIR)/Rules.make

installme::
	$(Q)cp $(E_TARGET) $(FSROOT)/usr/bin/
	$(Q)$(STRIP) $(FSROOT)/usr/bin/$(E_TARGET)

