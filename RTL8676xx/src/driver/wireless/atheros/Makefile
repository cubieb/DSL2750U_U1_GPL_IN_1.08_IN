ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config

subdir-${CONFIG_DRIVERS_WLAN_ATHEROS_CARRIER} += atheros_carrier
subdir-${CONFIG_DRIVERS_WLAN_ATHEROS_K2} += atheros_k2 

include $(ROOTDIR)/Rules.make
