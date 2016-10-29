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
# $Id: //depot/sw/carriersrc/release/1.3.1/src/madwifi/madwifi/athconf/Makefile#1 $
#

#
# Makefile for the Atheros WLAN driver.
#

ifeq ($(obj),)
obj=	$(CURDIR)
endif

ifeq ($(WLAN_CR_CONF_MK_DIR),) 
WLAN_CR_CONF_MK_DIR:=$(obj)/..
include $(WLAN_CR_CONF_MK_DIR)/config.mk
endif

DEPTH=	..

include $(obj)/${DEPTH}/Makefile.inc

obj-m		+= athconf.o
athconf-objs	:= ar5416_conf.o athconfmain.o 

# If building into kernel

ifneq ($(MAKING_MODULES),1)
obj-$(CONFIG_ATHEROS)	:= $(athconf-objs)
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
O_TARGET		:= $(obj-m)
endif
endif

INCS+=	-I${HAL} -I${HAL}/${OS} -I${obj}/${ATH_HAL} \
	-I${obj}/${WLAN} -I${obj}/${DEPTH} -I${obj}/${ATH_PKTLOG} \
	-I${ATH_PHYERR} -I${obj}/../ath -I${obj} -I${HAL}/ar5416 \
	-I${obj}/${ATH_CARRIER} -I${ATH_CARRPLTFRM_BASE}/

EXTRA_CFLAGS+= ${INCS} ${COPTS}

-include $(TOPDIR)/Rules.make

all:
	$(KMAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

install: all
ifeq ($(AP71_DEBUG),1)
	test -d ${TFTPPATH}/drivers || mkdir -p ${TFTPPATH}/drivers
	cp athconf.${KMODSUF} ${TFTPPATH}/drivers/
endif
	test -d ${DESTDIR}/${MODULEPATH} || mkdir -p ${DESTDIR}/${MODULEPATH}
	${TOOLPREFIX}strip -S athconf.$(KMODSUF)
	cp athconf.${KMODSUF} ${DESTDIR}/${MODULEPATH}

clean:
	$(KMAKE) -C $(KERNELPATH) M=$(shell pwd) clean
	-rm -f Module.symvers

athconf.o: ${athconf-objs}
	$(LD) $(LDOPTS) -o athconf.$(KMODSUF) -r ${athconf-objs}

uninstall:
	rm -f ${DESTDIR}/${MODULEPATH}/athconf.${KMODSUF}