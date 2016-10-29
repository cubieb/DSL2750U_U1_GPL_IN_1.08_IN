#!/bin/sh

temp1 = $(PWD)
space :=
remove := fusiv_src/kernel/drivers/voice
VOICE_LINUX_KERNEL_PATH := $(subst $(remove),$(comma),$(temp1))linux-2.6.18
RCS_FIND_IGNORE := \( -name SCCS -o -name BitKeeper -o -name .svn -o -name CVS -o -name .pc \) -prune -o

dsp_driver: 
	echo $(VOICE_LINUX_KERNEL_PATH)
	cd ipvoice; \
	make -C $(VOICE_LINUX_KERNEL_PATH)  M=$(PWD)/ipvoice; \
	
dsp_firmware_install:
ifdef CONFIG_FUSIV_KERNEL_DSP_MODULE
	echo  "Yes BUilding"
else
	echo "not selected"
endif

clean: 
	@find ./ipvoice \
        \( -name '*.[oas]' -o -name '*.ko' -o -name '.*.cmd' \
	-o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' \) \
                -type f -print | xargs rm -f



