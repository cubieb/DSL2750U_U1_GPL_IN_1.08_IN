include $(ROOTDIR)/.config
include $(BUILDDIR)/product/arch.mk

O_TARGET=cmm.o
obj-y := cmm_adddel.o cmm_comm.o cmm_main.o cmm_nameget.o cmm_valget.o cmm_attr.o cmm_err.o cmm_other.o cmm_valset.o 
EXTRA_CFLAGS := -I../../include -I$(BUILDDIR) -I ../../ssap/msg
ifeq (${PRODUCT}, UML)
	EXTRA_CFLAGS += -I../../include -I$(BUILDDIR)
endif
ifdef CONFIG_TBS_APPS_DEBUG_CMM
EXTRA_CFLAGS += -D_CMM_DEBUG
endif
include $(ROOTDIR)/Rules.make

