#WLAN_CR_TOPDIR := $(shell pwd)
export PATH_KERNEL=$(TBS_DIR)/build/kernel/linux-2.6.18.8
export PATH_KERNEL_INCLUDES=${PATH_KERNEL}/include
export HAL=$(WLAN_CR_TOPDIR)/core/hal/main
export ATH_RATE=$(WLAN_CR_TOPDIR)/ratectrl11n
export ATH_PHYERR=$(WLAN_CR_TOPDIR)/dfs

#disable output message or not 
#export ATH_NO_DEBUG=1
#export CONFIG_NO_STDOUT_DEBUG=1

export SHOW_AP_STA_INFO=y
export ATH_DEBUG=0
#pktlog source code has many problem
export ATH_PKT_LOG=0
#export ATH_PKTLOG=$(WLAN_CR_TOPDIR)/madwifi/pktlog
#export BINDIR:=usr/sbin
#export MODULEPATH:=usr/lib

