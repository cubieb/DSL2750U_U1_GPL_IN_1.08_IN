#!/bin/sh

ATH_MOD_PATH=/lib/modules/2.6.15/net/11n/k2

#Create a symbolic link to unload script as reboot command invokes
# killVAP script

#ln -sf /etc/ath/k2-unload.sh /etc/ath/killVAP

#Export Environment Variables
#. /etc/ath/k2-apcfg


insmod ${ATH_MOD_PATH}/adf.ko
insmod ${ATH_MOD_PATH}/hif_usb.ko
insmod ${ATH_MOD_PATH}/hst_htc.ko
insmod ${ATH_MOD_PATH}/hst_hal.ko
insmod ${ATH_MOD_PATH}/hst_wlan.ko
insmod ${ATH_MOD_PATH}/hst_rate.ko
insmod ${ATH_MOD_PATH}/hst_ath.ko 
insmod ${ATH_MOD_PATH}/if_ath_usb.ko
insmod ${ATH_MOD_PATH}/hst_scansta.ko
insmod ${ATH_MOD_PATH}/hst_scanap.ko
insmod ${ATH_MOD_PATH}/hst_xauth.ko
insmod ${ATH_MOD_PATH}/hst_wep.ko
insmod ${ATH_MOD_PATH}/hst_tkip.ko
insmod ${ATH_MOD_PATH}/hst_ccmp.ko
insmod ${ATH_MOD_PATH}/hst_acl.ko
