#
# Makefile for tracing module
#

ifeq ($(obj),)
obj=	$(CURDIR)
endif

ifeq ($(WLAN_CR_CONF_MK_DIR),) 
WLAN_CR_CONF_MK_DIR:=$(obj)/..
include $(WLAN_CR_CONF_MK_DIR)/config.mk
endif

DEPTH=	..

# release tag versioning
-include $(KERNELPATH)/ath_version.mk
export EXTRAVERSION

obj-m		+= atrcm.o
atrcm-objs	:= atrc.o
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
export-objs	:= atrc.o
list-multi	:= atrcm.o
endif

include $(obj)/${DEPTH}/Makefile.inc

ifneq ($(MAKING_MODULES),1)
obj-$(CONFIG_ATRC_ATHEROS) := $(atrcm-objs)
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
O_TARGET := atrcm.o
endif
endif

INCS+=	-I${HAL} -I${HAL}/${OS} -I${obj}/${ATH} -I${obj}/${WLAN} \
	-I${obj}/${DEPTH} \
	-I${obj}/${ATH_CARRIER} -I${ATH_CARRPLTFRM_BASE}/

EXTRA_CFLAGS+= ${INCS} ${COPTS}

-include $(TOPDIR)/Rules.make

all:
	$(KMAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

install: all
	test -d ${DESTDIR}/${MODULEPATH} || mkdir -p ${DESTDIR}/${MODULEPATH}
	${TOOLPREFIX}strip -S atrcm.$(KMODSUF)
	cp atrcm.${KMODSUF} ${DESTDIR}/${MODULEPATH}

clean:
	$(KMAKE) -C $(KERNELPATH) M=$(shell pwd) clean
	-rm -f Module.symvers

atrcm.o: ${atrcm-objs}
	$(LD) $(LDOPTS) -o atrcm.$(KMODSUF) -r $(atrcm-objs)

uninstall:
	rm -f ${DESTDIR}/${MODULEPATH}/atrcm.${KMODSUF}