ifeq "$(ROOTDIR)" ""
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif

include $(ROOTDIR)/.config

TBS_DIR=$(ROOTDIR)

ifneq ($(strip ${CONFIG_DRIVERS_WLAN_ATHEROS}),)
PRODUCT_DIR=atheros/atheros_carrier
else
$(error only for atheros driver now)
endif

WLAN_CONF_MK_DIR := $(TBS_DIR)/build/driver/wireless/$(PRODUCT_DIR)

include $(WLAN_CONF_MK_DIR)/config.mk

ifeq ($(strip $(CONFIG_TBS_RELEASE)),)
HOSTAPD_DEBUG=y
endif

PATH_OPENSSL=$(TBS_DIR)/build/tmpdist/
PATH_MADWIFI := $(WLAN_CONF_MK_DIR)/carrier/core
PATH_MADWIFI_EXTRA_INC := $(WLAN_CONF_MK_DIR)/carrier

STRIPFLAGS      = --strip-all --remove-section=.note --remove-section=.comment

DIR_WPA_SUPPLICANT=.
DIR_HOSTAP=.

ifeq ($(strip $(PREFIX)),)
PREFIX:=`pwd`/_install
endif


ifndef CFLAGS
CFLAGS = -MMD -Wall -D_BYTE_ORDER=_BIG_ENDIAN 
endif


ifdef HOSTAPD_DEBUG
CFLAGS+= -g -ggdb
else
CFLAGS += -Os
endif

ifneq ($(findstring mips-linux, $(TOOLPREFIX)),)
CFLAGS += -mips2 
STRIPFLAGS += --remove-section=.pdr
else
endif

STRIPCMD        =$(STRIP) $(STRIPFLAGS)
# define HOSTAPD_DUMP_STATE to include SIGUSR1 handler for dumping state to
# a file (undefine it, if you want to save in binary size)
#CFLAGS += -DHOSTAPD_DUMP_STATE
CFLAGS += -DCONFIG_CTRL_IFACE_UDP

# Include directories for CVS version
#CFLAGS += -I. -I$(DIR_HOSTAP) -I../utils -I$(DIR_WPA_SUPPLICANT)
#CFLAGS += -I. -I$(DIR_HOSTAP) -I../utils -I$(DIR_WPA_SUPPLICANT) -I$(PATH_OPENSSL)/include

CFLAGS += -I. -I$(DIR_HOSTAP) -I$(DIR_WPA_SUPPLICANT) -I$(PATH_OPENSSL)/include -I$(PATH_MADWIFI) -I$(PATH_MADWIFI_EXTRA_INC) 
LIBPATH += -L$(PATH_OPENSSL)

# Uncomment following line and set the path to your kernel tree include
# directory if your C library does not include all header files.
# CFLAGS += -DUSE_KERNEL_HEADERS -I/usr/src/linux/include

OBJS =	hostapd.o eloop.o ieee802_1x.o eapol_sm.o radius.o md5.o rc4.o \
	common.o ieee802_11.o config.o ieee802_11_auth.o accounting.o \
	sta_info.o radius_client.o sha1.o wpa.o aes_wrap.o ctrl_iface.o \
	driver_conf.o

-include .config

ifdef CONFIG_IAPP
CFLAGS += -DCONFIG_IAPP
OBJS += iapp.o
endif

#ifdef ATH_NO_DEBUG
#CFLAGS += -DCONFIG_NO_STDOUT_DEBUG
#endif

ifdef CONFIG_RSN_PREAUTH
CFLAGS += -DCONFIG_RSN_PREAUTH
CONFIG_L2_PACKET=y
endif

ifdef CONFIG_DRIVER_HOSTAP
CFLAGS += -DCONFIG_DRIVER_HOSTAP
OBJS += driver.o
endif

ifdef CONFIG_DRIVER_WIRED
CFLAGS += -DCONFIG_DRIVER_WIRED
OBJS += driver_wired.o
endif

ifdef CONFIG_DRIVER_ATHEROS
CFLAGS += -DCONFIG_DRIVER_ATHEROS
OBJS += driver_atheros.o
CONFIG_L2_PACKET=y
endif

ifdef CONFIG_DRIVER_PRISM54
CFLAGS += -DCONFIG_DRIVER_PRISM54
OBJS += driver_prism54.o
endif

ifdef CONFIG_DRIVER_BSD
CFLAGS += -DCONFIG_DRIVER_BSD
OBJS += driver_bsd.o
CONFIG_L2_PACKET=y
CONFIG_DNET_PCAP=y
CONFIG_L2_FREEBSD=y
endif

ifdef CONFIG_DRIVER_TEST
CFLAGS += -DCONFIG_DRIVER_TEST
OBJS += driver_test.o
endif

ifdef CONFIG_L2_PACKET
ifdef CONFIG_DNET_PCAP
CFLAGS += -DUSE_DNET_PCAP
ifdef CONFIG_L2_FREEBSD
LIBS += -lpcap
OBJS += $(DIR_WPA_SUPPLICANT)/l2_packet_freebsd.o
else
LIBS += -ldnet -lpcap
OBJS += $(DIR_WPA_SUPPLICANT)/l2_packet_pcap.o
endif
else
OBJS += $(DIR_WPA_SUPPLICANT)/l2_packet_linux.o
endif
endif


ifdef CONFIG_EAP_MD5
CFLAGS += -DEAP_MD5
OBJS += eap_md5.o
endif

ifdef CONFIG_EAP_TLS
CFLAGS += -DEAP_TLS
OBJS += eap_tls.o
TLS_FUNCS=y
endif

ifdef CONFIG_EAP_PEAP
CFLAGS += -DEAP_PEAP
OBJS += eap_peap.o
TLS_FUNCS=y
CONFIG_EAP_TLV=y
CONFIG_EAP_MSCHAPV2=y
endif

ifdef CONFIG_EAP_TTLS
CFLAGS += -DEAP_TTLS
OBJS += eap_ttls.o
TLS_FUNCS=y
endif

ifdef CONFIG_EAP_MSCHAPV2
CFLAGS += -DEAP_MSCHAPv2
OBJS += eap_mschapv2.o
MS_FUNCS=y
endif

ifdef CONFIG_EAP_GTC
CFLAGS += -DEAP_GTC
OBJS += eap_gtc.o
endif

ifdef CONFIG_EAP_SIM
CFLAGS += -DEAP_SIM
OBJS += eap_sim.o $(DIR_WPA_SUPPLICANT)/eap_sim_common.o
# Example EAP-SIM interface for GSM authentication. This can be replaced with
# another file implementating the interface specified in eap_sim_db.h.
OBJS += eap_sim_db.o
endif

ifdef CONFIG_EAP_PAX
CFLAGS += -DEAP_PAXbak_WscRefImpl_Intel_b3_0
OBJS += eap_pax.o $(DIR_WPA_SUPPLICANT)/eap_pax_common.o
endif

ifdef CONFIG_EAP_PSK
CFLAGS += -DEAP_PSK
OBJS += eap_psk.o $(DIR_WPA_SUPPLICANT)/eap_psk_common.o
endif

ifdef CONFIG_EAP_TLV
CFLAGS += -DEAP_TLV
OBJS += eap_tlv.o
endif

ifdef CONFIG_EAP
CFLAGS += -DEAP_SERVER
OBJS += eap.o eap_identity.o
endif

ifdef TLS_FUNCS
# Shared TLS functions (needed for EAP_TLS, EAP_PEAP, and EAP_TTLS)
CFLAGS += -DEAP_TLS_FUNCS
OBJS += eap_tls_common.o $(DIR_WPA_SUPPLICANT)/tls_openssl.o
LIBS += -lssl -lcrypto
LIBS_p += -lcrypto
else
OBJS += $(DIR_WPA_SUPPLICANT)/tls_none.o
endif

ifdef CONFIG_PKCS12
CFLAGS += -DPKCS12_FUNCS
endif

ifdef MS_FUNCS
ifndef TLS_FUNCS
LIBS += -lcrypto
endif
OBJS += $(DIR_WPA_SUPPLICANT)/ms_funcs.o $(DIR_WPA_SUPPLICANT)/crypto.o
endif

ifdef CONFIG_RADIUS_SERVER
CFLAGS += -DRADIUS_SERVER
OBJS += radius_server.o
endif

ifdef CONFIG_IPV6
CFLAGS += -DCONFIG_IPV6
endif

ifdef CONFIG_WFA_SIMPLE_CONFIG
CFLAGS += -DEAP_WSC -DSIMPLE_CONFIG -DWSC_AKM_NO_1X
OBJS += eap_wsc.o UdpLib.o wsc_ie.o
endif

ifeq ($(strip $(CONFIG_TBS_RELEASE)),)
ALL=hostapd 
CFG_PATH=conf
#CFG=hostapd.conf hostapd.eap_user template.conf template.conf_1 template.conf_2 template.conf_3 hostapd.open hostapd.open_1 hostapd.open_2 hostapd.open_3 hostapd.wep hostapd.wep_1 hostapd.wep_2 hostapd.wep_3 8021x.conf wpapsk.conf wpaEAP.conf wpa2-psk.conf wpa2EAP.conf activateVAP
CFG=$(CFG_PATH)/hostapd.conf $(CFG_PATH)/hostapd.open $(CFG_PATH)/hostapd.wpa1_eap $(CFG_PATH)/hostapd.wpa1_psk \
 	$(CFG_PATH)/hostapd.wpa2_eap $(CFG_PATH)/hostapd.wpa2_psk $(CFG_PATH)/hostapd.wpa_mix_psk hostapd.eap_user
CFG_FILE=$(notdir $(CFG)) 
DFT_CFG=default_hostapd.conf
else
ALL=hostapd 
CFG=hostapd.eap_user
CFG_FILE=$(notdir $(CFG))
DFT_CFG=
endif

all: verify_config $(ALL)

verify_config:
	cp -f defconfig .config
#	@if [ ! -r .config ]; then \
#		echo 'Building hostapd requires a configuration file'; \
#		echo '(.config). See README for more instructions. You can'; \
#		echo 'run "cp defconfig .config" to create an example'; \
#		echo 'configuration.'; \
#		exit 1; \
#	fi

install: 
	
	@mkdir -p $(CONFIG_PATH)
	@mkdir -p $(DEFAULT_CFG)
#	$(STRIP) $(ALL)
	for i in $(ALL); do cp -f $$i $(PATH_ROOTFS_BIN)/$$i; $(STRIP) $(PATH_ROOTFS_BIN)/$$i; done
	for i in $(CFG); do cp -f $$i $(CONFIG_PATH)/ ; done
	for i in $(DFT_CFG); do cp -f $$i $(DEFAULT_CFG)/$$i; done

uninstall:
	for i in $(ALL); do rm -f $(PATH_ROOTFS_BIN)/$$i; done
	for i in $(CFG_FILE); do rm -f $(CONFIG_PATH)/$$i; done
	for i in $(DFT_CFG); do rm -f $(DEFAULT_CFG)/$$i; done
hostapd: $(OBJS)
	$(CC) -o hostapd $(OBJS) $(LIBS) $(LIBPATH)
ifndef HOSTAPD_DEBUG
	$(STRIPCMD) hostapd
endif

driver_conf.c: Makefile .config
	rm -f driver_conf.c
	echo '/* THIS FILE AUTOMATICALLY GENERATED, DO NOT EDIT! */' \
							> driver_conf.c
	echo '#include <stdlib.h>'			>> driver_conf.c
	echo '#include <stdio.h>'			>> driver_conf.c
	echo '#include <sys/types.h>'			>> driver_conf.c
	echo '#include <netinet/in.h>'			>> driver_conf.c
	echo '#include "hostapd.h"'			>> driver_conf.c
	echo '#include "driver.h"'			>> driver_conf.c
ifdef CONFIG_DRIVER_HOSTAP
	echo "void hostap_driver_register(void);"	>> driver_conf.c
endif
ifdef CONFIG_DRIVER_WIRED
	echo "void wired_driver_register(void);"	>> driver_conf.c
endif
ifdef CONFIG_DRIVER_ATHEROS
	echo "void atheros_driver_register(void);"	>> driver_conf.c
endif
ifdef CONFIG_DRIVER_PRISM54
	echo "void prism54_driver_register(void);"	>> driver_conf.c
endif
ifdef CONFIG_DRIVER_BSD
	echo "void bsd_driver_register(void);"		>> driver_conf.c
endif
ifdef CONFIG_DRIVER_TEST
	echo "void test_driver_register(void);"		>> driver_conf.c
endif
	echo 'void register_drivers(void) {'		>> driver_conf.c
ifdef CONFIG_DRIVER_HOSTAP
	echo "hostap_driver_register();"		>> driver_conf.c
endif
ifdef CONFIG_DRIVER_WIRED
	echo "wired_driver_register();"			>> driver_conf.c
endif
ifdef CONFIG_DRIVER_ATHEROS
	echo "atheros_driver_register();"		>> driver_conf.c
endif
ifdef CONFIG_DRIVER_PRISM54
	echo "prism54_driver_register();"		>> driver_conf.c
endif
ifdef CONFIG_DRIVER_BSD
	echo "bsd_driver_register();"			>> driver_conf.c
endif
ifdef CONFIG_DRIVER_TEST
	echo "test_driver_register();"			>> driver_conf.c
endif
	echo '}'					>> driver_conf.c

hostapd_cli: hostapd_cli.o $(DIR_WPA_SUPPLICANT)/wpa_ctrl.o
	$(CC) -o hostapd_cli hostapd_cli.o $(DIR_WPA_SUPPLICANT)/wpa_ctrl.o

clean:
	rm -f core *~ *.o hostapd hostapd_cli *.d driver_conf.c

-include $(OBJS:%.o=%.d)
