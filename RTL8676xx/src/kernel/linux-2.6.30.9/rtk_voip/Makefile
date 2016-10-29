ifeq "$(VERSION).$(PATCHLEVEL)" "2.4"
DIR_VOIP := $(shell /bin/pwd)
O_TARGET := rtk_voip.o
else
DIR_VOIP := $(src)
endif

export DIR_VOIP

obj-y := main.o

ifneq "$(CONFIG_RTK_VOIP_DRIVERS_11N_MP)" "y"
obj-$(CONFIG_RTK_VOIP_RX) += voip_rx/
obj-$(CONFIG_RTK_VOIP_TX) += voip_tx/
obj-$(CONFIG_RTK_VOIP_SRTP) += srtp/
ifneq ($(CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST),y)
ifeq "$(CONFIG_AUDIOCODES_VOIP)" "y"
ifeq "$(CONFIG_VOIP_SDK)" "y"
obj-y += voip_dsp/dsp_r1/t38.obj
obj-y += voip_dsp/cpu_setup.obj  
else
obj-y += voip_dsp/dsp_r1/t38/
obj-y += voip_dsp/cpu_setup.o 
endif
obj-y += voip_dsp/ivr/
obj-y += voip_dsp/cp3/
else
obj-$(CONFIG_RTK_VOIP_DSP) += voip_dsp/
endif
endif
obj-$(CONFIG_RTK_VOIP_MANAGER) += voip_manager/
ifneq ($(CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST),y)
obj-$(CONFIG_RTK_VOIP_DRIVERS) += voip_drivers/
endif
endif

obj-y += voip_fs/

EXTRA_CFLAGS += -I$(DIR_VOIP)/include

ifeq ($(CONFIG_RTK_VOIP),m)
EXTRA_CFLAGS += -mlong-calls
EXTRA_CFLAGS += -DMODULE
endif

ifeq "$(CONFIG_RTK_VOIP_PACKAGE_867X)" "y"
ifeq "$(CONFIG_RLE0412)" "y"
CFLAGS_KERNEL += -march=5281
AFLAGS_KERNEL += -march=5281
else
CFLAGS_KERNEL += -march=5181
AFLAGS_KERNEL += -march=5181
endif
endif

include $(DIR_VOIP)/rules.mak
