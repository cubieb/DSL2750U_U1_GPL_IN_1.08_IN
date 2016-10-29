ifeq "$(ROOTDIR)" "" 
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif
include $(ROOTDIR)/.config


subdir-y += zlib
subdir-y += xmlparser

subdir-${CONFIG_OPENSOURCE_BUSYBOX} +=  busybox
subdir-${CONFIG_OPENSOURCE_BRCTL} +=  brctl
subdir-${CONFIG_OPENSOURCE_IPTABLES} +=  iptables
subdir-${CONFIG_OPENSOURCE_OPENSSL} +=  openssl
subdir-${CONFIG_OPENSOURCE_HTTPD} +=  httpd
subdir-${CONFIG_OPENSOURCE_PPPD} +=  ppp-pppoe
subdir-${CONFIG_OPENSOURCE_IPSEC} += ipsec
subdir-${CONFIG_OPENSOURCE_DPROXY} +=  dproxy
subdir-${CONFIG_OPENSOURCE_ETHTOOL} +=  ethtool
subdir-${CONFIG_OPENSOURCE_ATM} +=  atm
subdir-${CONFIG_OPENSOURCE_GDB} +=  gdb
subdir-${CONFIG_OPENSOURCE_UDHCP} +=  udhcp
subdir-${CONFIG_OPENSOURCE_DHCPV6} += dhcpv6
subdir-${CONFIG_OPENSOURCE_RADVD} += radvd
subdir-${CONFIG_OPENSOURCE_UPDATEDD} += updatedd
subdir-${CONFIG_OPENSOURCE_INADYN_MT} += inadyn
subdir-${CONFIG_OPENSOURCE_DHCPR} +=  dhcpr
subdir-${CONFIG_OPENSOURCE_EBTABLES} +=  ebtables
subdir-${CONFIG_OPENSOURCE_LIBNFNETLINK} +=  libnfnetlink
subdir-${CONFIG_OPENSOURCE_LIBNETFILTER_CONNTRACK} +=  libnetfilter_conntrack
#subdir-${CONFIG_OPENSOURCE_IGMPPROXY} +=  igmp-proxy
subdir-${CONFIG_OPENSOURCE_IGMPPROXYV3} +=  igmpproxy
subdir-$(CONFIG_OPENSOURCE_MLD) += mld
subdir-${CONFIG_OPENSOURCE_PPTP} += pptp
subdir-${CONFIG_OPENSOURCE_L2TPD} += l2tpd
subdir-${CONFIG_OPENSOURCE_SNMPA} +=  snmp
subdir-${CONFIG_OPENSOURCE_PING} +=  ping
subdir-${CONFIG_OPENSOURCE_IPROUTE2} +=  iproute2
subdir-${CONFIG_OPENSOURCE_ZEBRA} +=  zebra
subdir-${CONFIG_OPENSOURCE_DROPBEAR} +=  dropbear
subdir-${CONFIG_OPENSOURCE_MSNTP} +=  msntp
subdir-${CONFIG_OPENSOURCE_BFTPD} +=  bftpd
subdir-${CONFIG_OPENSOURCE_FTPD} += ftpd
subdir-${CONFIG_OPENSOURCE_TCPDUMP} += tcpdump
subdir-${CONFIG_OPENSOURCE_TTCP} += ttcp
subdir-${CONFIG_OPENSOURCE_UPnP} += upnp
subdir-${CONFIG_OPENSOURCE_HOSTAPD} += hostapd-0.4.8
subdir-${CONFIG_OPENSOURCE_SUPPLICANT} += wpa_supplicant-0.5.5
subdir-${CONFIG_OPENSOURCE_WIRELESS_TOOLS} += wireless_tools 
subdir-${CONFIG_OPENSOURCE_IPPD} += ippd
subdir-${CONFIG_OPENSOURCE_SAMBA} += samba
subdir-${CONFIG_OPENSOURCE_NTFS} += ntfs
subdir-${CONFIG_OPENSOURCE_SIPROXD} += libosip2
subdir-${CONFIG_OPENSOURCE_SIPROXD} += siproxd
subdir-${CONFIG_OPENSOURCE_NANOCOM} += nanocom
subdir-${CONFIG_OPENSOURCE_PEANUTHULLCLIENT} += peanuthullclient
subdir-${CONFIG_OPENSOURCE_DLNA} += dlna
subdir-${CONFIG_OPENSOURCE_LIB_PNG} += libpng
subdir-${CONFIG_OPENSOURCE_QRENCODE} += qrencode
subdir-${CONFIG_OPENSOURCE_FON} += fon
subdir-${CONFIG_OPENSOURCE_WPUT} += wput
subdir-y += libiconv
include $(ROOTDIR)/Rules.make

