#
# Copyright (C) 2002 - 2008 Atheros Communications Inc.
#
# Makefile for ADF & APD
#

adf_dir  = ../../adf
asf_dir  = ../../asf
app_dir	 = ../../tools/athcfg

all_path = $(PWD)/../../

CC=$(TOOLPREFIX)gcc

include $(PWD)/Kbuild.def


define cleanup_files
	@find $(1) -type f -name "*.o" -print|xargs rm -vf; 
	@find $(1) -type f -name "*.cmd" -print|xargs rm -vf;
endef

define run_config
	@cd $(1) && sh config $(arch);
endef

__run_config = $(foreach __cfg,$(1),$(call run_config,$(__cfg)))

	
ccflags-y	+= -I$(PWD)/$(asf_dir)/include/

# First pass, kernel Makefile reads module objects
ifneq ($(KERNELRELEASE),)
include $(PWD)/Kbuild.adf
include $(PWD)/Kbuild.aed
include $(PWD)/Kbuild.hst
include $(PWD)/Kbuild.tgt
include $(PWD)/Kbuild.hif
include $(PWD)/Kbuild.pktlog

#
#XXX:This needs to removed for the latest kernels
#
EXTRA_CFLAGS += $(ccflags-y)

# Second pass, the actual build.
else

KVER    ?= $(shell uname -r)
KDIR	= /lib/modules/$(KVER)/build/
PWD	= $(shell pwd)

ifeq ($(MAKECMDGOALS),clean)
include $(PWD)/Kbuild.adf
include $(PWD)/Kbuild.aed
include $(PWD)/Kbuild.hst
include $(PWD)/Kbuild.tgt
include $(PWD)/Kbuild.hif
include $(PWD)/Kbuild.pktlog
endif

#	@rm -vf $(files_to_clean)
#	@cd $(hal_path)/main && sh config $(arch)

all:
	$(call __run_config,$(cfg_path-m))
	@$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(call cleanup_files,$($(bld-mod)_path))
	$(MAKE) -C $(KDIR) M=$(PWD) clean


distclean:
	$(call cleanup_files,$(all_path))
	$(MAKE) -C $(KDIR) M=$(PWD) clean

app:
	cd $(PWD)/$(app_dir) && \
	$(MAKE) -f Makefile.linux

appclean:
	rm -rvf athcfg && \
	cd $(PWD)/$(app_dir) && \
	$(MAKE) -f Makefile.linux clean

help:
	$(MAKE) -C $(KDIR) M=$(PWD) help


# Indents the kernel source the way linux/Documentation/CodingStyle.txt
# wants it to be.
indent:
	indent -kr -i8 $($(MODULE_NAME)-objs:.o=.c)

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install

endif
