
include $(BUILDDIR)/arch.mk
#obj-y = upg_cmdline.o  upg_frw.o upg_cksum.o
obj-y = upg_frw.o upg_cksum.o
E_TARGET = upgrader

EXTRA_CFLAGS=-fPIC -I${BUILDDIR}/apps/include -I${BUILDDIR}/include -I${BUILDDIR} -DPRODUCT=\"$(PRODUCT)\"

ifeq (${PRODUCT}, UML)
EXTRA_CFLAGS += -D_CFG_UML
endif

#ifeq ($(CONFIG_DOUBLE_BACKUP), y)
#EXTRA_CFLAGS += -DCONFIG_DOUBLE_BACKUP
#endif

EXTRA_LDFLAGS=-lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lexpat -lz

include $(ROOTDIR)/Rules.make

installme::
	$(Q)cp $(E_TARGET) $(FSROOT)/usr/bin/
	$(Q)$(STRIP) $(FSROOT)/usr/bin/$(E_TARGET)

