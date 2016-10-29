#ifeq "$(ROOTDIR)" "" 
#export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
#endif
#
#include $(ROOTDIR)/.config
#
#EXTRA_CFLAGS=  -I$(BUILDDIR) -I$(BUILDDIR)/kernel/${TBS_KERNEL_VERSION}/include -I$(BUILDDIR)/apps/include
#EXTRA_CFLAGS += -I$(BUILDDIR)/apps/ssap/msg -I$(BUILDDIR)/apps/ssap/lib
#O_TARGET = dsl_api_vendor_wrapper.o
#obj-y = rtl8672_dsl_api_wrapper.o 
#
#include $(ROOTDIR)/Rules.make
