ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config

subdir-${CONFIG_DRIVERS_WLAN_RALINK_RT3070_SOFTAP_V1_3_1_0} += RT3070_Linux_SoftAP_v1.3.1.0
subdir-${CONFIG_DRIVERS_WLAN_RALINK_RT3070_SOFTAP_V2_1_3_0} += RT3070_Linux_SoftAP_v2.1.3.0
subdir-${CONFIG_DRIVERS_WLAN_RALINK_RT288x} += RT288x_SDK_3.2
subdir-${CONFIG_DRIVERS_WLAN_RALINK_RT2860_11N} += RT2860_11N

include $(ROOTDIR)/Rules.make
