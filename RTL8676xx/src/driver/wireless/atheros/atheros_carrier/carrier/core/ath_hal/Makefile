# $Id: //depot/sw/carriersrc/release/1.3.1/src/madwifi/madwifi/ath_hal/Makefile#2 $
#
# Copyright (c) 2002-2004 Sam Leffler, Errno Consulting
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#

#
# Makefile for the Atheros HAL Module.
#
# NB: we can't support MODVERSIONS because it requires building
#     from source which may not be available
#
ifeq ($(obj),)
obj=	$(CURDIR)
endif

ifeq ($(WLAN_CR_CONF_MK_DIR),) 
WLAN_CR_CONF_MK_DIR:=$(obj)/../../..
include $(WLAN_CR_CONF_MK_DIR)/config.mk
endif

DEPTH=	../..

obj-m		+= ath_hal.o
ath_hal-objs	:= ah_osdep.o hal.o
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
export-objs	:= ah_osdep.o
list-multi	:= ath_hal.o
endif

# If building into kernel

ifneq ($(MAKING_MODULES),1)
obj-$(CONFIG_ATHEROS_HAL) := ah_osdep.o hal.o
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
O_TARGET	:= ath_hal.o
endif
endif

include $(obj)/${DEPTH}/Makefile.inc

UUDECODE?=uudecode

INCS+=	-I${HAL} -I${HAL}/${OS} -I${obj}/${ATH_HAL} -I${obj}/${DEPTH}

EXTRA_CFLAGS+= ${INCS} ${COPTS}

-include $(TOPDIR)/Rules.make

all:	ah_osdep.c hal.o opt_ah.h
	$(KMAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

ah_osdep.c: opt_ah.h
	cp -f ${HAL}/${OS}/ah_osdep.c ah_osdep.c

$(obj)/hal.o hal.o:${HAL}/${OS}/obj/${TARGET}/hal.o
	cp ${HAL}/${OS}/obj/${TARGET}/hal.o .
#${UUDECODE} ${HAL}/public/${TARGET}.hal.o.uu

opt_ah.h: ${HAL}/public/${TARGET}.opt_ah.h
	cp -f ${HAL}/public/${TARGET}.opt_ah.h opt_ah.h

# Copy the unstripped version of the driver to /tftpboot/`whoami`/drivers/
# if debug is enabled.
install: all
ifeq ($(AP71_DEBUG),1)
	test -d ${TFTPPATH}/drivers || mkdir -p ${TFTPPATH}/drivers
	cp ath_hal.${KMODSUF} ${TFTPPATH}/drivers/
endif
	test -d ${DESTDIR}/${MODULEPATH} || mkdir -p ${DESTDIR}/${MODULEPATH}
	$(STRIP) -S ath_hal.$(KMODSUF)
	cp ath_hal.${KMODSUF} ${DESTDIR}/${MODULEPATH}

clean:
	$(KMAKE) -C $(KERNELPATH) M=$(shell pwd) clean
	rm -f Module.symvers
# remove the files we copied to this dir
	rm -f ah_osdep.c opt_ah.h hal.o

ath_hal.o: ${ath_hal-objs}
	$(LD) $(LDOPTS) -o ath_hal.$(KMODSUF) -r $(ath_hal-objs)
uninstall:
	rm -f ${DESTDIR}/${MODULEPATH}/ath_hal.${KMODSUF}
