#for test only
export BOARD_TYPE?=ar7130


LIB_WL_API = libwl_api.a

PATH_WIRELESS_TOOLS:=./wireless_tools.29
WLAN_DRIVER_MODE = wlan_carrier
ifeq ("$(BOARD_TYPE)","solosw")
		WLAN_CONF_MK_DIR=$(WLAN_API_DIR)/../solosw
    include $(WLAN_CONF_MK_DIR)/config.mk
    
    INCLUDES =  -I$(PATH_UMAC)/include \
                     -I$(PATH_GCC_INCLUDES) \
                     -I$(PATH_KERNEL_INCLUDES) \
                     -I$(PATH_WLAN_DRIVE)/include \
                     -I$(PATH_PRISMOIDS) \
                     -I$(PATH_VENDER_SPECIFIC)
    CFLAGS = -DWL_TYPE=WL_SOLOS -DLINUX -DNETLINK_SUPPORT -MD -Os -Wall -Wstrict-prototypes -pipe \
                 -msoft-float
else
ifeq ("$(BOARD_TYPE)","ar7130")
    # for ar7130
    CFLAGS = -DWL_TYPE=WL_AR7130 -MD -Os -Wall -Wstrict-prototypes -pipe \
                 -msoft-float
    CFLAGS += -DUSE_COMMON_HOSTAPD

    ifeq ($(PRODUCT),AR7130_GAW2.2Z56-B)
    	CFLAGS += -DPRODUCT_GAW2
    endif

    PATH_MADWIFI := $(TBS_DIR)/src/driver/wireless/ar7130_carrier/core
    PATH_MADWIFI_EXTRA_INC := $(TBS_DIR)/src/driver/wireless/ar7130_carrier

    ifeq ($(PRODUCT),AR7130_AP699E7.2U10-4)
    endif

   # WLAN_CONF_MK_DIR=$(TBS_TOP_DIR)/build/driver/wireless
    WLAN_CONF_MK_DIR=$(ROOTDIR)/build/driver/wireless
    include $(WLAN_CONF_MK_DIR)/config.mk    
    INCLUDES =  -I$(PATH_GCC_INCLUDES) \
                     -I$(PATH_KERNEL_INCLUDES) \
                     -I$(PATH_MADWIFI) \
                     -I$(PATH_WIRELESS_TOOLS) \
                     -I$(PATH_VENDER_SPECIFIC) \
		     -I${WLAN_API_DIR} 
    ifeq ("$(WLAN_DRIVER_MODE)","wlan_carrier")
        INCLUDES += -include $(PATH_MADWIFI_EXTRA_INC)/core/include/compat.h
        CFLAGS +=-DUSE_CARRIER_VER
    else
        INCLUDES += -include $(PATH_MADWIFI_EXTRA_INC)/include/compat.h
        CFLAGS +=-DUSE_FUSION_VER
    endif
else
    $(error  "Unknown Board type: $(BOARD_TYPE)")
endif
endif

CFLAGS +=$(INCLUDES)

CFLAGS +=-DDEBUG_WL_API
