ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config

subdir-${CONFIG_VENDOR_ATHERNOS_HOSTAPD_CFG} += atheros/hostapd_cfg
subdir-${CONFIG_VENDOR_REALTEK} += realtek
#
#TBS-TAG: by ouyangdi 2008-12-1
#DESC:compile infineon dsl apps
#
subdir-${CONFIG_INFINEON_DSL_APP} += infineon

#ralink tools
subdir-${CONFIG_RALINK_RT2860APD_APP} += ralink/802.1x

subdir-${CONFIG_IKANOS_DSL_APP} += ikanos



include $(ROOTDIR)/Rules.make
