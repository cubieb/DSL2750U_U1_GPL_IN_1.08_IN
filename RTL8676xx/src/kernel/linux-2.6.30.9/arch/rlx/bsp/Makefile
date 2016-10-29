# Realtek Semiconductor Corp.
#
# Tony Wu (tonywu@realtek.com)
# 

core-y   += arch/rlx/bsp/
cflags-y += -Iinclude/asm-rlx -Iarch/rlx/bsp/
# TBS_TAG Modified by xiachaoren 2011-10-26
load-y   += 0x80010000
# TBS_END_TAG

extra-y  += vmlinux.lds

obj-y := prom.o setup.o irq.o timer.o serial.o
obj-y += rtl8676_gpio.o rtl_usb_phy.o
#obj-$(CONFIG_DEV_xDSL) += adsl71.lib
obj-$(CONFIG_DEV_xDSL) += adsl.o
obj-$(CONFIG_EOC) += netadsl.o
obj-y += watchdog.o
obj-$(CONFIG_PCI) += pci.o
obj-$(CONFIG_RTL867X_NETLOG) += netlog.o
# 2010-11-19 paula
ifeq ($(CONFIG_RTL8676_SW_USB_LED), y)
obj-y    += led-usb.o
endif

EXTRA_AFLAGS := $(CFLAGS)
#EXTRA_CFLAGS += -I$(DIR_LINUX)/drivers/net/rtl819x/AsicDriver
#EXTRA_CFLAGS += -I$(DIR_LINUX)/drivers/net/rtl819x/common
