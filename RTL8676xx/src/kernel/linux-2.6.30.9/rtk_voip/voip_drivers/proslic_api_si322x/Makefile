obj-y	:=
obj-y += ProSLIC.o si3226_intf.o proslic_ctrl_spi.o proslic_timer_linux.o

EXTRA_CFLAGS += -I$(DIR_VOIP)/include  -DSI32267
EXTRA_CFLAGS += -I$(DIR_VOIP)/voip_drivers

include $(DIR_VOIP)/rules.mak
