#	$FreeBSD: src/tools/tools/ath/Makefile,v 1.3 2003/12/07 21:38:44 sam Exp $
#
# Copyright (c) 2002-2004	Sam Leffler, Errno Consulting
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

#ifeq ($(WLAN_CR_CONF_MK_DIR),) 
#WLAN_CR_CONF_MK_DIR:=$(CURDIR)/..
#include $(WLAN_CR_CONF_MK_DIR)/config.mk
#endif

DEPTH=	..

ifeq ($(BINDIR),)
BINDIR=	/usr/local/bin
endif

include ${DEPTH}/Makefile.inc

ALL=	athstats athstatsclr 80211stats athkey athchans athctrl \
	athdebug 80211debug wlanconfig athcwm athampdutrc atrc \
	pktlogconf pktlogdump radartool mibtool

TAR_FILE=debug.tgz
TAR_BALL = athstats athstatsclr athampdutrc athcwm athdebug 80211stats atrc pktlogconf \
	 pktlogdump 80211debug radartool	mibtool
INSTALL_FILE= wlanconfig mibtool
	   
all:	${ALL}

INCS+=	-I. -I${HAL} -I${HAL}/linux -I${ATH_HAL} -I${WLAN} -I${DEPTH} -I../core/ath
CFLAGS=	${INCS} -Os  -static
ifeq (${BUILD_STATIC}, y)
LDFLAGS= -static
endif

ifeq ($(SHOW_AP_STA_INFO),y)
CFLAGS+= -DSHOW_AP_STA_INFO
endif

all:	${ALL}

athstats: athstats.c
	${CC} -o athstats ${CFLAGS} -I${ATH} ${LDFLAGS} athstats.c
athstatsclr: athstatsclr.c
	${CC} -o athstatsclr ${CFLAGS} -I${ATH} ${LDFLAGS} athstatsclr.c
athampdutrc: athampdutrc.c
	${CC} -o athampdutrc ${CFLAGS} -I${ATH} ${LDFLAGS} athampdutrc.c
80211stats: 80211stats.c
	${CC} -o 80211stats ${CFLAGS} ${LDFLAGS} 80211stats.c
athkey: athkey.c
	${CC} -o athkey ${CFLAGS} ${LDFLAGS} athkey.c
athctrl: athctrl.c
	${CC} -o athctrl ${CFLAGS} ${LDFLAGS} athctrl.c
athdebug: athdebug.c
	${CC} -o athdebug ${CFLAGS} ${LDFLAGS} athdebug.c
wlanconfig: wlanconfig.c
	${CC} -o wlanconfig ${CFLAGS} ${LDFLAGS} wlanconfig.c
80211debug: 80211debug.c
	${CC} -o 80211debug ${CFLAGS} ${LDFLAGS} 80211debug.c
athcwm: athcwm.c
	${CC} -o athcwm ${CFLAGS} -I${ATH} ${LDFLAGS} athcwm.c
atrc: atrc.c
	${CC} -o atrc ${CFLAGS} -I../atrc ${LDFLAGS} atrc.c
pktlogdump: pktlogdump.c
	${CC} -o pktlogdump ${CFLAGS} -I${ATH_PKTLOG} ${LDFLAGS} pktlogdump.c -lm
pktlogconf: pktlogconf.c
	${CC} -o pktlogconf ${CFLAGS} -I${ATH_PKTLOG} ${LDFLAGS} pktlogconf.c
radartool: radartool.c
	${CC} -o radartool ${CFLAGS} -I../dfs -I${ATH} ${LDFLAGS} radartool.c

install: ${ALL}
#cp athstats ${DESTDIR}/${BINDIR}
#cp athstatsclr ${DESTDIR}/${BINDIR}
#cp athampdutrc ${DESTDIR}/${BINDIR}
#cp athcwm ${DESTDIR}/${BINDIR}
#cp athdebug ${DESTDIR}/${BINDIR}
#cp 80211stats ${DESTDIR}/${BINDIR}
#cp atrc ${DESTDIR}/${BINDIR}
#cp pktlogconf ${DESTDIR}/${BINDIR}
#cp pktlogdump ${DESTDIR}/${BINDIR}
#-rm ${DESTDIR}/${BINDIR}/wlanstats
#ln ${DESTDIR}/${BINDIR}/80211stats ${DESTDIR}/${BINDIR}/wlanstats
#cp 80211debug ${DESTDIR}/${BINDIR}
#-rm ${DESTDIR}/${BINDIR}/wlandebug
#ln ${DESTDIR}/${BINDIR}/80211debug ${DESTDIR}/${BINDIR}/wlandebug
#cp wlanconfig ${DESTDIR}/${BINDIR}
#cp radartool ${DESTDIR}/${BINDIR}/radartool
	for i in $(INSTALL_FILE); do \
		cp -f $$i ${DESTDIR}/${BINDIR}/ ; \
		$(STRIP) ${DESTDIR}/${BINDIR}/$$i; \
	done
	for i in $(TAR_BALL); do \
		$(STRIP) $$i; \
	done
ifeq ($(strip $(CONFIG_TBS_RELEASE)),)
	tar czvf ${DESTDIR}/${BINDIR}/$(TAR_FILE) ${TAR_BALL}
endif

clean:
	rm -f ${ALL} core a.out
uninstall:
	for i in $(INSTALL_FILE); do \
		rm -f ${DESTDIR}/${BINDIR}/$$i; \
	done
ifeq ($(strip $(CONFIG_TBS_RELEASE)),)
	rm -f ${DESTDIR}/${BINDIR}/$(TAR_FILE)
endif

