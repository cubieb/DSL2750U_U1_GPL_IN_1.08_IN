CC=../../../../buildroot/build_mips_nofpu/staging_dir/bin/mips-linux-uclibc-gcc
#########

AR=../../../../buildroot/build_mips_nofpu/staging_dir/bin/mips-linux-ar
#########

RANLIB=../../../../buildroot/build_mips_nofpu/staging_dir/bin/mips-linux-ranlib
#########

include ../../../../buildroot/.config
include ../../../../buildroot/build_mips_nofpu/busybox-1.00/.config
ifdef BR2_KERNEL_HEADERS_2_6_12
	include ../../../../linux-mips-2.6.12/.config
endif

ifdef BR2_KERNEL_HEADERS_2_6_18
	include ../../../../linux-2.6.18/.config
endif

#ifeq ($(BR2_DEFAULT_KERNEL_HEADERS),"2.6.18")
#DFLAGS += -DLINUX_2618
#include ../../../../linux-2.6.18/.config
#endif

#ifeq ($(BR2_DEFAULT_KERNEL_HEADERS),"2.6.12")
#DFLAGS += -DLINUX_MIPS2612
#include ../../../../linux-mips-2.6.12/.config
#endif


SRM = srm.a

SRMOBJS = save.o erase.o process.o register.o restore.o srmfileop.o restore_file.o ppprestore.o 
SRMSRCS = save.c erase.c process.c register.c restore.c srmfileop.c restore_file.c ppprestore.c

ROUTESRC = route.c
ROUTEOBJ = route.o

ifeq ($(CONFIG_FUSIV_USER_BR2684CTL),y)
BR2684SRC = br2684_srm.c
BR2684OBJ = br2684_srm.o
endif

ifeq ($(CONFIG_VLAN_8021Q),y)
VLANSRC = vlanlib.c
VLANOBJ = vlanlib.o
endif

ifeq ($(CONFIG_VLAN_8021Q_MODULE),y)
VLANSRC = vlanlib.c
VLANOBJ = vlanlib.o
endif

ifeq ($(CONFIG_BRIDGE),y)
BRIDGELIB = ../../../../buildroot/build_mips_nofpu/bridge-utils-1.0.6/libbridge/libbridge.a
BRIDGESRC = br_srm.c
BRIDGEOBJ = br_srm.o
endif

ifeq ($(CONFIG_BRIDGE_MODULE),y)
BRIDGELIB = ../../../../buildroot/build_mips_nofpu/bridge-utils-1.0.6/libbridge/libbridge.a
BRIDGESRC = br_srm.c
BRIDGEOBJ = br_srm.o
endif

ifeq ($(CONFIG_IP_NF_IPTABLES),y)
IPTBLSRC = iptables_srm.c
IPTBLOBJ = iptables_srm.o
endif

ifeq ($(CONFIG_FUSIV_KERNEL_PPPOE_RELAY),m)
PPPRELAYSRC = pppoerelay_srm.c
PPPRELAYOBJ = pppoerelay_srm.o
endif

ifeq ($(CONFIG_FUSIV_KERNEL_PPPOE_RELAY),y)
PPPRELAYSRC = pppoerelay_srm.c
PPPRELAYOBJ = pppoerelay_srm.o
endif

ifeq ($(CONFIG_PPPOE),y)
PPPSRC = pppsrm.c
PPPOBJ = pppsrm.o
endif

ifeq ($(CONFIG_PPPOATM),y)
PPPSRC = pppsrm.c
PPPOBJ = pppsrm.o
endif

ifeq ($(CONFIG_FUSIV_USER_LINUXATM2_4_1),y)
IPOASRC = ipoa_srm.c
IPOAOBJ	= ipoa_srm.o
endif

ifeq ($(BR2_PACKAGE_IPROUTE2),y)
IPQOSSRC = ipqos_srm.c
IPQOSOBJ = ipqos_srm.o
endif

ifeq ($(CONFIG_ADI_WLAN),y)
VINDHYASRC = wlanlib.c
VINDHYAOBJ = wlanlib.o
endif

ifeq ($(CONFIG_FUSIV_KERNEL_WLAN_ATH_11G),y)
ATHEROSSRC = atheros_srm.c
ATHEROSOBJ = atheros_srm.o
endif

ifeq ($(CONFIG_FUSIV_KERNEL_WLAN_ATH_11N),y)
ATHEROSSRC = atheros_srm.c
ATHEROSOBJ = atheros_srm.o
endif

ifeq ($(CONFIG_FUSIV_KERNEL_WLAN_RT_11G),y)
RALINKSRC = ralink_srm.c
RALINKOBJ = ralink_srm.o
endif

ifeq ($(CONFIG_FUSIV_KERNEL_WLAN_RT_11N),y)
RALINKSRC = ralink_srm.c
RALINKOBJ = ralink_srm.o
endif

ifeq ($(CONFIG_FUSIV_USER_ROCSYS),y)
ifeq ($(CONFIG_FUSIV_USER_ALG),y)
ALGSRC = alg.c
ALGOBJ = alg.o
endif

ifeq ($(CONFIG_FUSIV_USER_ATTACKSCONFIG),y)
ATTACKSRC = attack.c
ATTACKOBJ = attack.o
endif

ifeq ($(BR2_PACKAGE_ZEBRA),y)
RIPSRC = rip_srm.c
RIPOBJ = rip_srm.o
endif
ifeq ($(CONFIG_UDHCPD),y)
DHCPSRC = dhcp.c
DHCPOBJ = dhcp.o
endif
ifeq ($(BR2_PACKAGE_DHCP_RELAY),y)
DHCRSRC = dhcrelay.c
DHCROBJ = dhcrelay.o
endif
endif



SOURCES = $(SRMSRCS) $(ROUTESRC) $(BR2684SRC) $(VLANSRC) $(BRIDGESRC) $(IPTBLSRC) $(PPPRELAYSRC) $(PPPSRC) $(IPOASRC) $(IPQOSSRC) $(VINDHYASRC) $(DHCPSRC) $(DHCRSRC) $(ALGSRC) $(RIPSRC) $(ATHEROSSRC)  $(RALINKSRC) $(ATTACKSRC)
OBJECTS = $(SRMOBJS) $(ROUTEOBJ) $(BR2684OBJ) $(VLANOBJ) $(BRIDGEOBJ) $(IPTBLOBJ) $(PPPRELAYOBJ) $(PPPOBJ) $(IPOAOBJ) $(IPQOSOBJ) $(VINDHYAOBJ) $(DHCPOBJ) $(DHCROBJ) $(ALGOBJ) $(RIPOBJ) $(ATHEROSOBJ)  $(RALINKOBJ) $(ATTACKOBJ)

all: $(SRM)

$(SRM): $(BRIDGELIB) ../../acc_utillib/miscutillib.a
	$(CC) $(CFLAGS) -c $(SOURCES)
	-rm -f $(SRM)
	${AR} cru $(SRM) $(BRIDGELIB) ../../acc_utillib/miscutillib.a $(OBJECTS)
	${RANLIB} $(SRM)

clean:
	-rm -f *.elf *.gdb *.[oa] *.map

