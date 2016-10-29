ifeq "$(ROOTDIR)" ""  
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk
include $(ROOTDIR)/Rules.make
O_TARGET=dsl_module.o
obj-y=dsl_api.o dsl.o
obj-y+=rtl8672/rtl8672_dsl_api_wrapper.o

EXTRA_LDFLAGS =  
CFLAGS +=  -I$(BUILDDIR) -I$(BUILDDIR)/include -I$(BUILDDIR)/kernel/${TBS_KERNEL_VERSION}/include -I$(BUILDDIR)/apps/include  
CFLAGS += -I$(BUILDDIR)/hal/wireless -I$(BUILDDIR)/apps/ssap/msg -I$(BUILDDIR)/apps/ssap/lib
CFLAGS += -I$(BUILDDIR)/driver/dsl/infineon/drv_dsl_cpe_api/src/include

ifeq (${PRODUCT}, UML)
	EXTRA_CFLAGS +=
endif
ifdef CONFIG_TBS_APPS_DEBUG_DSL
EXTRA_CFLAGS += -DDSL_DEBUG
endif

#include $(ROOTDIR)/Rules.make

# 编译前先clean，以解决ssap/pc和logic/dsl循环依赖的问题
first_rule: clean
