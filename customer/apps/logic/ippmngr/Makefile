
include $(ROOTDIR)/.config

O_TARGET := ippcfg.o

obj-y	:= ippmngr.o lptool.o
#obj-y	:= ippapi.o lptool.o

EXTRA_LDFLAGS =
EXTRA_CFLAGS=  -I$(BUILDDIR) -I$(BUILDDIR)/kernel/${TBS_KERNEL_VERSION}/include -I$(BUILDDIR)/apps/include
EXTRA_CFLAGS += -I$(BUILDDIR)/hal/wireless -I$(BUILDDIR)/apps/ssap/msg -I$(BUILDDIR)/apps/ssap/lib

include $(ROOTDIR)/Rules.make
