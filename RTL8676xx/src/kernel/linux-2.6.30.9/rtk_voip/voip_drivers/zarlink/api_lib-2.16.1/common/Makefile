
obj-y	:= 	vp_api.o		\
			vp_api_common.o \
			vp_debug.o 

# vp_api_cslac_seq.c requires Zarlink NDA
ifeq ($(CONFIG_VOIP_SDK),y)
	ifeq ($(CONFIG_RTK_VOIP_SLIC_ZARLINK_880_SERIES)$(CONFIG_RTK_VOIP_SLIC_ZARLINK_890_SERIES),yy)
ifeq ($(CONFIG_RTL8676),y)
	obj-y += vp_api_cslac_seq_rtl8672.obj
endif
ifeq ($(CONFIG_RTL_89xxC),y)
	obj-y += vp_api_cslac_seq_rtl89xxC.obj
endif
	else 
		ifeq ($(CONFIG_RTK_VOIP_SLIC_ZARLINK_880_SERIES),y)
ifeq ($(CONFIG_RTL8676),y)
	obj-y += vp_api_cslac_seq_vp880_rtl8672.obj
endif
ifeq ($(CONFIG_RTL_89xxC),y)
	obj-y += vp_api_cslac_seq_vp880_rtl89xxC.obj
endif
		endif
		ifeq ($(CONFIG_RTK_VOIP_SLIC_ZARLINK_890_SERIES),y)
ifeq ($(CONFIG_RTK_VOIP_PACKAGE_867X),y)
	obj-y += vp_api_cslac_seq_vp890_rtl8672.obj
endif
ifeq ($(CONFIG_RTL_89xxC),y)
	obj-y += vp_api_cslac_seq_vp890_rtl89xxC.obj
endif
		endif
	endif
else
	obj-y += vp_api_cslac_seq.o
endif

EXTRA_CFLAGS += -I$(DIR_VOIP)/include
EXTRA_CFLAGS += -I$(DIR_VOIP)/voip_drivers/
EXTRA_CFLAGS += -I$(DIR_VOIP)/voip_drivers/zarlink/$(ZARLINK_LIB)/includes/
EXTRA_CFLAGS += -I$(DIR_VOIP)/voip_drivers/zarlink/$(ZARLINK_LIB)/arch/rtl89xxb/

ifeq ($(CONFIG_RTK_VOIP_SLIC_ZARLINK_890_SERIES),y) 
EXTRA_CFLAGS += -I$(DIR_VOIP)/voip_drivers/zarlink/$(ZARLINK_LIB)/vp890_api/
endif

ifeq ($(CONFIG_RTK_VOIP_SLIC_ZARLINK_880_SERIES),y) 
EXTRA_CFLAGS += -I$(DIR_VOIP)/voip_drivers/zarlink/$(ZARLINK_LIB)/vp880_api/
endif

include $(DIR_VOIP)/rules.mak
