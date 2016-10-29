include $(ROOTDIR)/.config

O_TARGET=ftpd.o

ifeq ("$(CONFIG_APPS_LOGIC_FTPD_TBS)","y")
obj-y = ftpd_module.o
endif

ifeq ("$(CONFIG_APPS_LOGIC_FTPD_CT)","y")
obj-y = ct_ftpd_module.o
endif

obj-y += base64.o

#EXTRA_CFLAGS=  -I../../../include -I${BUILDDIR}

EXTRA_LDFLAGS =
EXTRA_CFLAGS=  -I$(BUILDDIR) -I$(BUILDDIR)/kernel/${TBS_KERNEL_VERSION}/include -I$(BUILDDIR)/apps/include
EXTRA_CFLAGS += -I$(BUILDDIR)/hal/wireless -I$(BUILDDIR)/apps/ssap/msg -I$(BUILDDIR)/apps/ssap/lib

include $(ROOTDIR)/Rules.make
