ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif

include $(ROOTDIR)/.config

EXTRA_CFLAGS=  -I$(BUILDDIR) -I$(BUILDDIR)/kernel/linux-2.6.18.8/include -I$(BUILDDIR)/apps/include
EXTRA_CFLAGS += -I$(BUILDDIR)/hal/wireless -I$(BUILDDIR)/apps/ssap/msg -I$(BUILDDIR)/apps/ssap/lib
O_TARGET = wl_api_vendor_wrapper.o
obj-y += ar7130_wl_api_wrapper.o 

include $(ROOTDIR)/Rules.make
