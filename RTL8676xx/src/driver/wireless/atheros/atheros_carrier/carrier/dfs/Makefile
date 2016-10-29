#
# Copyright (c) 2006 Atheros Communications, Inc.
# All rights reserved.
#

#
# Makefile for the Atheros DFS/AR support
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

obj-m		+= ath_dfs.o
ath_dfs-objs	:= dfs.o
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
export-objs	:= dfs.o
list-multi	:= ath_dfs.o
endif

include $(obj)/${DEPTH}/Makefile.inc

ifneq ($(MAKING_MODULES),1)
obj-$(CONFIG_DFS_ATHEROS) := $(ath_dfs-objs)
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
O_TARGET := ath_dfs.o
endif
endif

INCS+=	-I${HAL} -I${HAL}/${OS} -I${obj}/${ATH} -I${obj}/${WLAN} \
	-I${obj}/${DEPTH} -I${obj}/${ATH_PKTLOG} -I${obj}/${ATH_CARRIER} -I${ATH_CARRPLTFRM_BASE}/

EXTRA_CFLAGS+= ${INCS} ${COPTS} -DATH_SUPPORT_DFS

-include $(TOPDIR)/Rules.make

all:
	$(KMAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

install: all
	test -d ${DESTDIR}/${MODULEPATH} || mkdir -p ${DESTDIR}/${MODULEPATH}
	${TOOLPREFIX}strip -S ath_dfs.$(KMODSUF)
	cp ath_dfs.${KMODSUF} ${DESTDIR}/${MODULEPATH}

clean:
	$(KMAKE) -C $(KERNELPATH) M=$(shell pwd) clean
	-rm -f Module.symvers

ath_dfs.o: ${ath_dfs-objs}
	$(LD) $(LDOPTS) -o ath_dfs.$(KMODSUF) -r $(ath_dfs-objs)

uninstall:
	rm -f ${DESTDIR}/${MODULEPATH}/ath_dfs.${KMODSUF}
