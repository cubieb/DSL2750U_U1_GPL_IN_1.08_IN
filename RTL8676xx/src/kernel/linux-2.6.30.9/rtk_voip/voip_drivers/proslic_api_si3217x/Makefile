obj-y	:=

ifeq ($(CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3226),y)
obj-y += ProSLIC.o si3226_intf.o proslic_ctrl_spi.o proslic_timer_linux.o proslic_version.o
endif

ifeq "$(CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3217x)" "y"
obj-y += ProSLIC.o si3217x_intf.o proslic_ctrl_spi.o proslic_timer_linux.o proslic_version.o
endif

ifeq ($(CONFIG_RTK_VOIP_DRIVERS_FXO),y)
ifeq ($(CONFIG_RTK_VOIP_DRIVERS_SI3050),y)
obj-y += vdaa.o
endif
endif

ifeq "$(CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3217x)" "y"
EXTRA_CFLAGS += -I$(DIR_VOIP)/include  -I$(DIR_VOIP)/voip_drivers/proslic_api_si3217x/include -DSI3217X
else
EXTRA_CFLAGS += -I$(DIR_VOIP)/include  -I$(DIR_VOIP)/voip_drivers/proslic_api_si322x/include -DSI32267
endif
EXTRA_CFLAGS += -I$(DIR_VOIP)/voip_drivers

include $(DIR_VOIP)/rules.mak
