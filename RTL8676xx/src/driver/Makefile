ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config

subdir-$(CONFIG_DRIVERS_WLAN) += wireless 
subdir-$(CONFIG_DRIVERS_DSL) += dsl 

include $(ROOTDIR)/Rules.make
