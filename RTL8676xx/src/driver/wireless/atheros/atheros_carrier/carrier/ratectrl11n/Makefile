#
# Copyright (c) 2004 Sam Leffler, Errno Consulting
# Copyright (c) 2004 Atheros Communications, Inc.
# All rights reserved.
#
# $Id: //depot/sw/carriersrc/release/1.3.1/src/madwifi/ratectrl11n/Makefile#2 $
#

#
# Makefile for the Atheros Rate Control Support.
#
ifeq ($(obj),)
obj=	$(CURDIR)
endif

#ifeq ($(WLAN_CR_CONF_MK_DIR),) 
#WLAN_CR_CONF_MK_DIR:=$(obj)/..
#include $(WLAN_CR_CONF_MK_DIR)/config.mk
#endif

DEPTH=	..

# release tag versioning
-include $(KERNELPATH)/ath_version.mk
export EXTRAVERSION

obj-m		+= ath_rate_atheros.o
ath_rate_atheros-objs	:= linux.o ratectrl.o ratectrl_11n_ln.o ar5416Phy.o ar5212Phy.o
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
export-objs	:= linux.o
list-multi	:= ath_rate_atheros.o
endif

include $(obj)/${DEPTH}/Makefile.inc

ifneq ($(MAKING_MODULES),1)
obj-$(CONFIG_RATE_ATHEROS) := $(ath_rate_atheros-objs)
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
O_TARGET := ath_rate_atheros.o
endif
endif

#COPTS=  -Wall -Werror  -g -O
INCS+=	-I${HAL} -I${HAL}/${OS} -I${obj}/${ATH} -I${obj}/${WLAN} \
	-I${obj}/${DEPTH} -I${obj}/${ATH_PKTLOG} \
	-I${obj}/${ATH_CARRIER} -I${ATH_CARRPLTFRM_BASE}/

EXTRA_CFLAGS+= ${INCS} ${COPTS}

-include $(TOPDIR)/Rules.make

all:
	$(KMAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

install: all
ifeq ($(AP71_DEBUG),1)
	test -d ${TFTPPATH}/drivers || mkdir -p ${TFTPPATH}/drivers
	cp ath_rate_atheros.${KMODSUF} ${TFTPPATH}/drivers/
endif
	test -d ${DESTDIR}/${MODULEPATH} || mkdir -p ${DESTDIR}/${MODULEPATH}
	${TOOLPREFIX}strip -S ath_rate_atheros.$(KMODSUF)
	cp ath_rate_atheros.${KMODSUF} ${DESTDIR}/${MODULEPATH}

clean:
	$(KMAKE) -C $(KERNELPATH) M=$(shell pwd) clean
	-rm -f Module.symvers

ath_rate_atheros.o: ${ath_rate_atheros-objs}
	$(LD) $(LDOPTS) -o ath_rate_atheros.$(KMODSUF) -r $(ath_rate_atheros-objs)

uninstall:
	rm -f ${DESTDIR}/${MODULEPATH}/ath_rate_atheros.${KMODSUF}
	
