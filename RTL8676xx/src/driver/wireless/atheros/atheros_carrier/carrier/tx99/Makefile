#
# Copyright (c) 2004 Sam Leffler, Errno Consulting
# Copyright (c) 2004 Atheros Communications, Inc.
# All rights reserved.
#
# $Id: //depot/sw/carriersrc/release/1.3.1/src/madwifi/tx99/Makefile#3 $
#

#
# Makefile for the Continuous Transmit (tx99) diagnostic support.
#
ifeq ($(obj),)
obj=	$(CURDIR)
endif

ifeq ($(WLAN_CR_CONF_MK_DIR),) 
WLAN_CR_CONF_MK_DIR:=$(obj)/..
include $(WLAN_CR_CONF_MK_DIR)/config.mk
endif

DEPTH=	..

obj-m		+= ath_tx99.o
list-multi	:= ath_tx99.o

include $(obj)/${DEPTH}/Makefile.inc

ifeq ($(strip ${BUS}),AHB)
BUSNAME=ahb
EXTRA_CFLAGS+= -DATH_AHB
else
BUSNAME=pci
EXTRA_CFLAGS+= -DATH_PCI
endif


INCS+=  -I${HAL} -I${HAL}/ar5416 -I${HAL}/ar5212 -I${HAL}/${OS} -I${ATH_RATE} -I${obj}/${ATH} -I${obj}/${WLAN} \
	-I${obj}/${DEPTH} -I${obj}/${DEPTH}/ath_pktlog \
	-I${obj}/${ATH_CARRIER} -I${ATH_CARRPLTFRM_BASE}/

EXTRA_CFLAGS+= ${INCS} ${COPTS} -g -O0

-include $(TOPDIR)/Rules.make

all:
	$(KMAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

install: all
ifeq ($(AP71_DEBUG),1)
	test -d ${TFTPPATH}/drivers || mkdir -p ${TFTPPATH}/drivers
	cp ath_tx99.ko ${TFTPPATH}/drivers/
endif
	test -d ${DESTDIR}/${MODULEPATH} || mkdir -p ${DESTDIR}/${MODULEPATH}
	${TOOLPREFIX}strip -S ath_tx99.$(KMODSUF)
	cp ath_tx99.${KMODSUF} ${DESTDIR}/${MODULEPATH}

clean:
	$(KMAKE) -C $(KERNELPATH) M=$(shell pwd) clean
	-rm -f Module.symvers
	
uninstall:
	rm -f ${DESTDIR}/${MODULEPATH}/ath_tx99.${KMODSUF}	
