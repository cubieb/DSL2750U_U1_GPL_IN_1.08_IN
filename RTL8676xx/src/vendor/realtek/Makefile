include $(ROOTDIR)/.config

subdir-${CONFIG_REALTEK_AUTH_APP} += auth
subdir-${CONFIG_REALTEK_FLASH_TOOLS} += flash
subdir-${CONFIG_REALTEK_ADSL_APP} += adslctrl
subdir-${CONFIG_REALTEK_SAR_APP} += sarctl
#subdir-${CONFIG_REALTEK_ATM_APP} += atm
#subdir-${CONFIG_REALTEK_ATM_APP} += mpoa
subdir-${CONFIG_REALTEK_WPS_APP} += mini_upnp
subdir-${CONFIG_REALTEK_WPS_APP} += wsc
subdir-${CONFIG_REALTEK_WPS2X_APP} += wsc_2.0
ifeq ("${CPU}", "rtl8672ve")
#use iwcontrol bin repalce wpsconfig
#subdir-${CONFIG_REALTEK_WPS_APP} += wpsconfig
endif
subdir-${CONFIG_REALTEK_ADSL_START} += adslstart
#subdir-${CONFIG_REALTEK_NET_LOGGER} += netlogger
subdir-${CONFIG_REALTEK_USB_TEST} += usbctl

include $(ROOTDIR)/Rules.make

