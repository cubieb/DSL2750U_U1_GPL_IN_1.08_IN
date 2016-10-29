#
# Copyright (c) 2002-2004 Sam Leffler, Errno Consulting
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer,
#    without modification.
# 2. Redistributions in binary form must reproduce at minimum a disclaimer
#    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
#    redistribution must be conditioned upon including a substantially
#    similar Disclaimer requirement for further binary redistribution.
# 3. Neither the names of the above-listed copyright holders nor the names
#    of any contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# Alternatively, this software may be distributed under the terms of the
# GNU General Public License ("GPL") version 2 as published by the Free
# Software Foundation.
#
# NO WARRANTY
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
# AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
# THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
# OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
# IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGES.
#
# $Id: //depot/sw/carriersrc/release/1.3.1/src/madwifi/madwifi/ath/Makefile#2 $
#

#
# Makefile for the Atheros WLAN driver.
#

ifeq ($(obj),)
obj=	$(CURDIR)
endif

ifeq ($(WLAN_CR_CONF_MK_DIR),) 
WLAN_CR_CONF_MK_DIR:=$(obj)/../../..
include $(WLAN_CR_CONF_MK_DIR)/config.mk
endif

DEPTH=	../..

ifeq ($(strip ${BUS}),AHB)
BUSNAME=ahb
COPTS+= -DATH_AHB
else
BUSNAME=pci
COPTS+= -DATH_PCI
endif

include $(obj)/${DEPTH}/Makefile.inc

obj-m		+= ath_${BUSNAME}.o
ath_${BUSNAME}-objs	:= if_ath.o if_owl.o if_ath_cwm.o if_ath_${BUSNAME}.o
ath_${BUSNAME}-objs	+= sort.o

ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
export-objs	:= if_ath_${BUSNAME}.o if_ath.o
list-multi	:= ath_${BUSNAME}.o
export-objs	+= sort.o
endif


# If building into kernel

ifneq ($(MAKING_MODULES),1)
obj-$(CONFIG_ATHEROS)	:= $(ath_${BUSNAME}-objs)
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
O_TARGET		:= $(obj-m)
endif
endif

INCS+=	-I${HAL} -I${HAL}/${OS} -I${obj}/${ATH_HAL} \
	-I${obj}/${WLAN} -I${obj}/${DEPTH} -I${obj}/${ATH_PKTLOG} \
	-I${ATH_PHYERR} -I${obj}/${ATH_CARRIER} -I${ATH_CARRPLTFRM_BASE}/

ifneq ($(strip ${ATH_CAP_TX99}),0)
INCS+= -I${obj}/${DEPTH}/tx99
endif

ifdef ATH_CARRPLTFRM_BASE
EXTRA_CFLAGS+= ${INCS} ${COPTS}
else
EXTRA_CFLAGS+= ${INCS} ${COPTS} -Werror
endif

-include $(TOPDIR)/Rules.make

all:
	$(KMAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

install: 
ifeq ($(AP71_DEBUG),1)
	test -d ${TFTPPATH}/drivers || mkdir -p ${TFTPPATH}/drivers
	cp ath_${BUSNAME}.${KMODSUF} ${TFTPPATH}/drivers/
endif
	test -d ${DESTDIR}/${MODULEPATH} || mkdir -p ${DESTDIR}/${MODULEPATH}
	${TOOLPREFIX}strip -S ath_${BUSNAME}.$(KMODSUF)
	cp ath_${BUSNAME}.${KMODSUF} ${DESTDIR}/${MODULEPATH}

clean:
	$(KMAKE) -C $(KERNELPATH) M=$(shell pwd) clean
	-rm -f Module.symvers

ath_${BUSNAME}.o: ${ath_${BUSNAME}-objs}
	$(LD) $(LDOPTS) -o ath_${BUSNAME}.$(KMODSUF) -r $(ath_${BUSNAME}-objs)
uninstall:
	rm -f ${DESTDIR}/${MODULEPATH}/ath_${BUSNAME}.${KMODSUF}
