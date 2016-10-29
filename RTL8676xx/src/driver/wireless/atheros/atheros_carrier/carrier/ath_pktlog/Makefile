
#
# Makefile for the Packet logging module.
#
ifeq ($(obj),)
obj=	$(CURDIR)
endif

ifeq ($(WLAN_CR_CONF_MK_DIR),) 
WLAN_CR_CONF_MK_DIR:=$(obj)/..
include $(WLAN_CR_CONF_MK_DIR)/config.mk
endif

DEPTH=	..

obj-m		+= ath_pktlog.o
ath_pktlog-objs := pktlog.o
ifeq ($(findstring 2.6,$(KERNELRELEASE)),)
export-obs      := pktlog.o
list-multi	:= ath_pktlog.o
endif

include $(obj)/${DEPTH}/Makefile.inc

INCS+=	-I${HAL} -I${HAL}/${OS} -I${obj}/${ATH_HAL} -I${obj}/${DEPTH} \
	-I${obj}/${ATH} -I${obj}/${ATH_PKTLOG} \
	-I${ATH_RATE} \
	-I${obj}/${ATH_CARRIER} -I${ATH_CARRPLTFRM_BASE}/

EXTRA_CFLAGS+= ${INCS} ${COPTS}

-include $(TOPDIR)/Rules.make

all:    	
	$(KMAKE) -C $(KERNELPATH) SUBDIRS=$(shell pwd) modules

install: all
	test -d ${DESTDIR}/${MODULEPATH} || mkdir -p ${DESTDIR}/${MODULEPATH}
	$(STRIP) -S ath_pktlog.$(KMODSUF)
	cp ath_pktlog.${KMODSUF} ${DESTDIR}/${MODULEPATH}

clean:
	$(KMAKE) -C $(KERNELPATH) M=$(shell pwd) clean
	-rm -f Module.symvers


ath_pktlog.o: ${ath_pktlog-objs}
	$(LD) $(LDOPTS) -o ath_pktlog.$(KMODSUF) -r ${ath_pktlog-objs}

uninstall:
	rm -f ${DESTDIR}/${MODULEPATH}/ath_pktlog.${KMODSUF}