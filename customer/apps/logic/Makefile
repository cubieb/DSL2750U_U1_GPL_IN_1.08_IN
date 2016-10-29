ifeq "$(ROOTDIR)" ""
export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif

include $(ROOTDIR)/.config
include $(BUILDDIR)/product/arch.mk

KERNEL_INCLUDE_DIR=${ROOTDIR}/build/kernel/${TBS_KERNEL_VERSION}/include
export CFLAGS = -Dlinux -D__linux__ -Dunix  -D_LINUX_2_6_ -I. -I$(TOOLCHAINS_SYSROOT)/include -I$(KERNEL_INCLUDE_DIR) -I$(TOOLCHAINS_SYSROOT)/../lib/gcc-lib/mips-linux-uclibc/3.3.5/include

obj-y := logic.o cmm/cmm.o timer/timer.o
subdir-y += cmm

ifeq ("$(CONFIG_APPS_OTHERPORT)", "y")
CFLAGS += -DCONFIG_APPS_OTHERPORT
endif
#scb+ 2011-6-28
ifeq ("$(CONFIG_APPS_LOGIC_NEW3G)","y")
subdir-y += new3g
CFLAGS += -DCONFIG_APPS_LOGIC_NEW3G -DTBS
CFLAGS += -I$(ROOTDIR)/build/apps/logic/new3g/3g-mngr
CFLAGS += -I$(ROOTDIR)/build/apps/logic/new3g/3g-mngr/lib

export CONFIG_APPS_LOGIC_NEW3G

#scb+2011-10-19 for captival-portal
ifeq ("$(CONFIG_APPS_GENERAL_CAPTIVAL_PORTAL)","y")
CFLAGS +=  -DAPPS_GENERAL_CAPTIVAL_PORTAL
CFLAGS += -DHAVE_DEFINE_TYPE
SYSTEM=TBS
export  SYSTEM
endif

#scb+ 2012-8-10
ifeq ("$(CONFIG_SUPPORT_3G_SHARP_INET_LED)","y")
CFLAGS += -DSUPPORT_3G_SHARE_INET_LED
export CONFIG_SUPPORT_3G_SHARP_INET_LED
endif

endif

export CFLAGS


#for WPS, TODO: use macro to control this definition
#export CFLAGS+=-DCONFIG_WLAN_WPS -I$(ROOTDIR)/build/blp/kernel/os_src/include

#obj-${CONFIG_APPS_LOGIC_WLAN} += wlan/wlan.o wlan/scalarvar.o wlan/wps.o

ifeq ("$(CONFIG_APPS_LOGIC_ETHLAN)","y")
CONFIG_APPS_LOGIC_PUBLIC_ETH = y
endif
ifeq ("$(CONFIG_APPS_LOGIC_ETHWAN)","y")
CONFIG_APPS_LOGIC_PUBLIC_ETH = y
endif

ifeq ("$(CONFIG_APPS_LOGIC_PPPOE)","y")
CONFIG_APPS_LOGIC_PUBLIC_WAN = y
endif
ifeq ("$(CONFIG_APPS_LOGIC_IPCONN)","y")
CONFIG_APPS_LOGIC_PUBLIC_WAN = y
endif

ifeq ("$(CONFIG_APPS_LOGIC_LAN_VLAN)","y")
CONFIG_APPS_LOGIC_PUBLIC_LAYER2BR = y
endif
ifeq ("$(CONFIG_APPS_LOGIC_IPCONN)","y")
CONFIG_APPS_LOGIC_PUBLIC_LAYER2BR = y
endif

ifeq ("$(CONFIG_APPS_LOGIC_SCHEDULE)","y")
CONFIG_APPS_LOGIC_PUBLIC_SCHEDULE = y
endif

obj-${CONFIG_APPS_LOGIC_SNMPC} += snmpc/snmpc.o
obj-${CONFIG_APPS_LOGIC_TR064BE} += tr064be/tr064c.o
obj-${CONFIG_APPS_LOGIC_TW_SAMBA} += samba/samba.o
obj-${CONFIG_APPS_LOGIC_TF_SAMBA} += tf_samba/tf_samba.o
obj-${CONFIG_APPS_LOGIC_PPPOE} += pppoe/pppou.o pppoe/pppoe.o
#obj-${CONFIG_APP_LOGIC_PPPOU} += pppoe/pppou.o
obj-${CONFIG_APPS_LOGIC_VPN} += pppoe/vpn.o
ifdef CONFIG_TBS_SUPPORT_IPV6
obj-${CONFIG_APPS_LOGIC_LAN} += lan/lanip6.o lan/lan.o
else
obj-${CONFIG_APPS_LOGIC_LAN} += lan/lan.o
endif
obj-${CONFIG_APPS_LOGIC_LAN_VLAN} += lanside_vlan/lanside_vlan.o
ifdef CONFIG_TBS_SUPPORT_IPV6
obj-${CONFIG_APPS_LOGIC_DNS} += dns/dnsipv6.o dns/dns.o
else
obj-${CONFIG_APPS_LOGIC_DNS} +=  dns/dns.o
endif
obj-${CONFIG_APPS_LOGIC_IPSEC} += ipsec/ipsec.o
obj-${CONFIG_APPS_LOGIC_ETHLAN} += ethlan/ethlan.o
obj-${CONFIG_APPS_LOGIC_ETHWAN} += ethwan/ethwan.o
obj-${CONFIG_APPS_LOGIC_PING} += ping/ping_dgn.o
obj-${CONFIG_APPS_LOGIC_WAN} += wan/wan.o
obj-${CONFIG_APPS_LOGIC_IPCONN} += ipconn/ipconn.o
obj-${CONFIG_APPS_LOGIC_TR069} += tr069be/tr069.o tr069be/events.o tr069be/download.o tr069be/upload.o
obj-${CONFIG_APPS_LOGIC_IPT} += ipt/ipt.o
obj-${CONFIG_APPS_LOGIC_PORTMAPPING} += portmapping/portmapping.o
obj-${CONFIG_APPS_LOGIC_PCP} += pcp/pcp.o
obj-${CONFIG_APPS_LOGIC_ACL} += acl/acl.o
obj-${CONFIG_APPS_LOGIC_IP_ACL} += ip_acl/ip_acl.o
obj-${CONFIG_APPS_LOGIC_DEFAULT_GATEWAY} += defaultGW/defaultGW.o
obj-${CONFIG_APPS_LOGIC_LOGGER} += logger/logger.o
obj-${CONFIG_APPS_LOGIC_DHCPS} += dhcps/dhcps.o
obj-${CONFIG_APPS_LOGIC_DHCPR} += dhcpr/dhcpr.o
obj-${CONFIG_APPS_LOGIC_ALG} += alg/alg.o
obj-${CONFIG_APPS_LOGIC_BRIDGE} += bridge/bridge.o
obj-${CONFIG_APPS_LOGIC_IGMP} += igmp/igmp.o
obj-${CONFIG_APPS_LOGIC_TF_IGMP} += tf_igmp/tf_igmp.o
obj-${CONFIG_APPS_LOGIC_MLD} += mld/mld.o
obj-${CONFIG_APPS_LOGIC_USBPIN} += usb3gpinmngr/pinmngr.o usb3gpinmngr/usb3g.o

obj-${CONFIG_APPS_LOGIC_TW_SAMBA} += samba/samba.o
obj-${CONFIG_APPS_LOGIC_TF_SAMBA} += tf_samba/tf_samba.o
obj-${CONFIG_APPS_LOGIC_QOS_IFB} += qos/qos_ifb.o
obj-${CONFIG_APPS_LOGIC_QOS_IMQ} += qos/qos_imq.o
obj-${CONFIG_APPS_LOGIC_STATIC_ROUTING} += staticrouting/staticrouting.o
obj-${CONFIG_APPS_LOGIC_DROUTE} += droute/droute.o
obj-${CONFIG_APPS_LOGIC_AUTH} += auth/auth.o
obj-${CONFIG_APPS_LOGIC_FIREWALL} += firewall/firewall.o
obj-${CONFIG_APPS_LOGIC_TF_FIREWALL} += tf_firewall/tf_firewall.o
obj-${CONFIG_APPS_LOGIC_ANTIATTACK} += antiattack/antiattack.o
obj-${CONFIG_APPS_LOGIC_SNTP} += sntp/sntp.o
obj-${CONFIG_APPS_LOGIC_USB_MASS} += usb/msg_module/usbmsg_mass.o
subdir-${CONFIG_APPS_LOGIC_IPPMNGR} += ippmngr
obj-${CONFIG_APPS_LOGIC_IPPMNGR} += ippmngr/ippcfg.o
subdir-${CONFIG_APPS_LOGIC_FTPD} += ftpd
obj-${CONFIG_APPS_LOGIC_FTPD} += ftpd/ftpd.o
obj-${CONFIG_APPS_LOGIC_NATPRIO} += natprio/natprio.o
obj-${CONFIG_APPS_LOGIC_DEVINFO} += devinfo/devinfo.o
obj-${CONFIG_APPS_LOGIC_URL_FILTER} += urlfilter/urlfilter.o
obj-${CONFIG_APPS_LOGIC_MACFILTER} += macfilter/macfilter.o
obj-${CONFIG_APPS_LOGIC_UPNP} += upnp/upnp.o  upnp/uuid.o
obj-${CONFIG_APPS_LOGIC_ATM} += atm/atm.o
obj-${CONFIG_APPS_LOGIC_SUPP} += supp/supp.o
obj-${CONFIG_APPS_LOGIC_PROUTE} += proute/proute.o
obj-${CONFIG_APPS_LOGIC_WEBPROXY} += webproxy/webproxy.o
obj-${CONFIG_APPS_LOGIC_PORTTRIGGER} += porttrigger/porttrigger.o
obj-${CONFIG_APPS_LOGIC_DDNS} += ddns/ddns.o
obj-${CONFIG_APPS_LOGIC_WANSELECT} += wanselect/wanselect.o
obj-${CONFIG_APPS_LOGIC_SCHEDULE} += schedules/schedules.o
obj-${CONFIG_APPS_LOGIC_SPT} += shareprotection/shareprotection.o
obj-${CONFIG_APPS_LOGIC_DEVCONFIG} += devconfig/devconfig.o
obj-${CONFIG_APPS_LOGIC_TRADITIONALNAT} += traditionalnat/traditionalnat.o
obj-${CONFIG_APPS_LOGIC_FIREWALLLOG} += firewalllog/firewalllog.o
obj-${CONFIG_APPS_LOGIC_IPMACFILTER} += ipmacfilter/ipmacfilter.o
obj-${CONFIG_APPS_LOGIC_TR143} += tr143/udpecho/udpecho.o
obj-${CONFIG_APPS_LOGIC_TR143} += tr143/downloaddiagnostics/downloaddiagnostics.o
obj-${CONFIG_APPS_LOGIC_TR143} += tr143/uploaddiagnostics/uploaddiagnostics.o
obj-${CONFIG_APPS_LOGIC_TF_PORTMAPPING} += tf_portmapping/tf_portmapping.o
obj-${CONFIG_APPS_LOGIC_TF_GUI} += tf_gui/customer_device.o

#zzh add fon.2012-02-25
obj-${CONFIG_APPS_LOGIC_FON} +=  fon/fon.o
#for public
obj-${CONFIG_APPS_LOGIC_PUBLIC_ETH} += public/eth.o
obj-${CONFIG_APPS_LOGIC_PUBLIC_WAN} += public/wanpub.o
obj-${CONFIG_APPS_LOGIC_PUBLIC_LAYER2BR} += public/layer2br.o
obj-${CONFIG_APPS_LOGIC_PUBLIC_SCHEDULE} += public/sch.o


#lyl for bridge filter
obj-y += bridge_list/bridge_list.o

subdir-${CONFIG_APPS_LOGIC_LOGGER} += logger
subdir-${CONFIG_APPS_LOGIC_WLAN} += wlan
#subdir-$(CONFIG_APPS_LOGIC_URL_FILTER) +=urlfilter
subdir-$(CONFIG_APPS_LOGIC_DSL) +=dsl
subdir-$(CONFIG_APPS_LOGIC_USB_MASS) += usb/hotplug

obj-${CONFIG_APPS_LOGIC_WLAN} += wlan/wlan_module.o
obj-${CONFIG_APPS_LOGIC_DSL} += dsl/dsl_module.o
obj-${CONFIG_APPS_LOGIC_DDNS} += ddns/ddns.o
obj-${CONFIG_APPS_LOGIC_TRACERT} += tracert/tracert.o

#ipv6
obj-${CONFIG_APPS_LOGIC_VDSL} += vdsl/vdsl.o
obj-${CONFIG_APPS_LOGIC_RADVD} += radvd/radvd.o
obj-${CONFIG_APPS_LOGIC_V6SROUTE} += v6sroute/v6sroute.o
obj-${CONFIG_APPS_LOGIC_SUPPORT_PPPV6} += pppoe/pppv6.o
ifdef CONFIG_TBS_SUPPORT_IPV6
obj-${CONFIG_APPS_LOGIC_DHCP6S} += dhcp6s/dhcp6s.o
endif
obj-${CONFIG_APPS_LOGIC_IPV6_ENABLE} += ipv6_enable/ipv6_enable.o

obj-${CONFIG_APPS_LOGIC_V6CONN} += v6conn/v6conn.o
subdir-${CONFIG_APPS_LOGIC_DIAG} += diagnostics
obj-${CONFIG_APPS_LOGIC_DIAG} += diagnostics/diag.o

#yuanchaoqun      
obj-${CONFIG_APPS_LOGIC_SUPPORT_CPUUSAGE} += os_info/os_info.o

#chenkai
obj-${CONFIG_APPS_LOGIC_DLNA} += dlna/dlna.o
#lbw
obj-${CONFIG_APPS_LOGIC_IPTUNNEL} += iptunnel/iptunnel.o

#wangjiaming
obj-${CONFIG_APPS_LOGIC_GRE} += gre/gre.o

#lan port vlan
obj-${CONFIG_APPS_LOGIC_LAN_PORT_VLAN} += lanport_vlan/lanport_vlan.o

#tr069hold
obj-${CONFIG_APPS_LOGIC_TR069HOLD} += tr069hold/tr069hold.o

#ber test
obj-${CONFIG_APPS_LOGIC_BER_TEST} += ber_test/ber_test.o

#liangbingtong dsl diag
obj-${CONFIG_APPS_LOGIC_TR069DIAG} += dslloopdiag/diagnostics.o

obj-${CONFIG_APPS_LOGIC_MACCLONE} += macclone/macclone.o
E_TARGET=logic
EXTRA_CFLAGS= -I${BUILDDIR} -I./ -I./include -I./public -I../include \
			  -I../ssap/msg -I../omlib -I../ssap/lib  -I../../include\
              -I$(TOOLCHAINS_SYSINCLUDE)/netinet/ -I$(TMPDISTDIR)/include \
              -I${BUILDDIR}/include \
	      	  -DTBS_VERSION=\"$(TBS_VERSION)\"

ifeq (${PRODUCT}, UML)
EXTRA_CFLAGS += -DWLAN_UT_GCC -D_CFG_UML
endif

ifeq (${PRODUCT}, UML)
EXTRA_CFLAGS += -DLOGGER_DIRECT_READ_FLASH
endif

ifeq ("$(CONFIG_APPS_LOGIC_WAN_VLAN)","y")
EXTRA_CFLAGS += -DWAN_VLAN_ENABLE
endif

ifeq ("$(CONFIG_APPS_LOGIC_DHCPC_OPTION15)","y")
EXTRA_CFLAGS += -DENABLE_OPT15
endif

ifeq ("$(CONFIG_APPS_LOGIC_DHCPC_OPTION42)","y")
EXTRA_CFLAGS += -DENABLE_OPT42
endif

ifeq ("$(CONFIG_APPS_LOGIC_DHCPC_OPTION250)","y")
EXTRA_CFLAGS += -DENABLE_OPT250
endif

ifeq ("$(CONFIG_APPS_LOGIC_DHCPS_OPTION125)","y")
EXTRA_CFLAGS += -DENABLE_OPT125
endif

ifeq ("$(CONFIG_APPS_LOGIC_TR111)", "y")
EXTRA_CFLAGS += -DTR111
endif

##########################for debug#############################
debug-y :=
debug-${CONFIG_TBS_APPS_DEBUG_CMM} += -D_CMM_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_TIMER} += -DTIMER_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_LAN} += -DLAN_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_LVLAN} += -DLSVLAN_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_WAN} += -DWAN_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_ETHLAN} += -DETHLAN_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_ETHWAN} += -DETHWAN_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_ACL} += -DACL_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_ALG} += -DALG_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_ANTIATTACK} += -DANTI_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_ATM} += -DATM_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_AUTH} += -DAUTH_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_DEVINFO} += -DDEVINFO_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_DHCPR} += -DDHCPR_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_DHCPS} += -DDHCPS_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_DNS} += -DDNS_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_DHCP6S} += -DDHCP6S_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_DSL} += -DDSL_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_DROUTE} += -DDYNAMIC_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_FIREWALL} += -DFW_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_IGMP} += -DIGMP_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_MLD} += -DMLD_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_IPCONN} += -DIPCONN_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_IPT} += -DIPT_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_GRE} += -DGRE_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_LOGGER} += -DLOGGER_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_PING} += -D_DGN_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_PORTMAP} += -DPTMP_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_PCP} += -DPCP_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_PPPOE} += -DPPPOE_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_VPN} += -DVPN_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_QOS} += -DQOS_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_SNMP} += -DSNMPC_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_SAMBA} += -DSAMBA_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_SNTP} += -DSNTP_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_SUPP} += -DSUPP_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_SROUTE} += -DSTATIC_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_V6SROUTE} += -DV6STATIC_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_TR069BE} += -DTR069_BE_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_TRACERT} += -DTRACERT_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_UPNP} += -DUPNP_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_URLFILTER} += -DURLFILTER_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_WLAN} += -DWLAN_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_VDSL} += -DVDSL_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_WEBP} += -DWEBP_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_DDNS} += -DDDNS_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_MACFILTER} += -DMACFILTER_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_PORTTRIGGER} += -DPT_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_DIAG} += -DDG_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_WANSELECT} += -DWANSELECT_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_SCHEDULE} += -DSCHEDULE_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_TF_PORTMAPPING} += -DTF_PORTMAPPING_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_LANPORT_VLAN} += -DLANPORT_VLAN_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_TF_GUI} += -DTF_GUI_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_FON} += -DFON_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_FIREWALLLOG} += -DFIREWALLLOG_DEBUG

debug-${CONFIG_TBS_APPS_DEBUG_TR069HOLD} += -DTR069HOLD_DEBUG

debug-${CONFIG_TBS_APPS_DEBUG_V6CONN} += -DV6CONN_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_IPTUNNEL} += -DIPTUNNEL_DEBUG

# for public
debug-${CONFIG_TBS_APPS_DEBUG_PUBLIC_ETH} += -DPUBLIC_ETH_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_PUBLIC_WAN} += -DWANPUB_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_PUBLIC_LAYER2BR} += -DLAYER2BR_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_PUBLIC_SCHEDULE} += -DSCH_DEBUG

debug-${CONFIG_TBS_APPS_DEBUG_DHCP6S} += -DDHCP6S_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_V6SROUTE} += -DV6SROUTE_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_V6CONN} += -DV6CONN_DEBUG
debug-${CONFIG_APPS_LOGIC_TR069DIAG} += -DTR069DIAG_DEBUG
debug-${CONFIG_APPS_LOGIC_MACCLONE} += -DMACCLONE_DEBUG
debug-${CONFIG_TBS_APPS_DEBUG_BER_TEST} += -DBER_DEBUG

#scb+ for new 3g 2011-6-24
obj-${CONFIG_APPS_LOGIC_NEW3G} += new3g/new3g.o
#obj-${CONFIG_APPS_LOGIC_NEW3G} += new3g/3g-mngr/lib/3g-lib-eject.o
#obj-${CONFIG_APPS_LOGIC_NEW3G} += new3g/3g-mngr/lib/3g-lib-msg.o
#obj-${CONFIG_APPS_LOGIC_NEW3G} += new3g/3g-mngr/lib/3g-lib-util.o

EXTRA_CFLAGS += ${debug-y}

##################################################################

EXTRA_LDFLAGS=-lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz -lcrypt -lm

#scb+ for new 3g 2011-9-21
ifeq ("$(CONFIG_APPS_LOGIC_NEW3G)","y")
EXTRA_LDFLAGS += -l3g-util -L./new3g/3g-mngr/lib
endif

ifeq ("${CONFIG_APPS_LOGIC_USB_MASS}","y")
	#subdir-y += usb/hotplug

	## usb lib include
	EXTRA_CFLAGS+= -I./usb/lib -I./usb/hotplug

	## usb module
	EXTRA_LDFLAGS+= usb/lib/libusb.a

	lib-y += usb/lib/libusb.a
endif



include $(ROOTDIR)/Rules.make

XML_LIST-${CONFIG_APPS_LOGIC_SNMPC} += snmpc/snmpc.xml
#XML_LIST-${CONFIG_APPS_LOGIC_TW_SAMBA} += samba/samba.xml
XML_LIST-${CONFIG_APPS_LOGIC_PPPOE} += pppoe/pppoe.xml
XML_LIST-${CONFIG_APPS_LOGIC_VPN} += pppoe/vpn.xml
XML_LIST-${CONFIG_APPS_LOGIC_IPSEC} += ipsec/ipsec.xml
XML_LIST-${CONFIG_APPS_LOGIC_IPCONN} += ipconn/ipconn.xml
XML_LIST-${CONFIG_APPS_LOGIC_LAN} += lan/lan.xml
XML_LIST-${CONFIG_APPS_LOGIC_LAN_VLAN} += lanside_vlan/lanside_vlan.xml
XML_LIST-${CONFIG_APPS_LOGIC_DNS} += dns/dns.xml
XML_LIST-${CONFIG_APPS_LOGIC_ETHLAN} += ethlan/ethlan.xml
#XML_LIST-${CONFIG_APPS_LOGIC_PUBLIC_ETH} += public/public.xml
XML_LIST-${CONFIG_APPS_LOGIC_ETHWAN} += ethwan/ethwan.xml
XML_LIST-${CONFIG_APPS_LOGIC_PING} += ping/ping.xml
XML_LIST-${CONFIG_APPS_LOGIC_WAN} += wan/wan.xml
XML_LIST-${CONFIG_APPS_LOGIC_TR069} += tr069be/tr069be.xml
XML_LIST-${CONFIG_APPS_LOGIC_TR064} += tr064be/tr064be.xml
XML_LIST-${CONFIG_APPS_LOGIC_IPT} += ipt/ipt.xml
XML_LIST-${CONFIG_APPS_LOGIC_PORTMAPPING} += portmapping/portmapping.xml
XML_LIST-${CONFIG_APPS_LOGIC_PCP} += pcp/pcp.xml
XML_LIST-${CONFIG_APPS_LOGIC_ACL} += acl/acl.xml
XML_LIST-${CONFIG_APPS_LOGIC_IP_ACL} += ip_acl/ip_acl.xml
XML_LIST-${CONFIG_APPS_LOGIC_DEFAULT_GATEWAT} += defaultGW/defaultGW.xml
XML_LIST-${CONFIG_APPS_LOGIC_LOGGER} += logger/logger.xml
XML_LIST-${CONFIG_APPS_LOGIC_DHCPS} += dhcps/dhcps.xml
XML_LIST-${CONFIG_APPS_LOGIC_ALG} += alg/alg.xml
XML_LIST-${CONFIG_APPS_LOGIC_IGMP} += igmp/igmp.xml
XML_LIST-${CONFIG_APPS_LOGIC_TF_IGMP} += tf_igmp/tf_igmp.xml
XML_LIST-${CONFIG_APPS_LOGIC_MLD} += mld/mld.xml
XML_LIST-${CONFIG_APPS_LOGIC_SNMPC} += snmpc/snmpc.xml
XML_LIST-${CONFIG_APPS_LOGIC_TF_SAMBA} += tf_samba/tf_samba.xml
XML_LIST-${CONFIG_APPS_LOGIC_QOS_IFB} += qos/qos_ifb.xml
XML_LIST-${CONFIG_APPS_LOGIC_QOS_IMQ} += qos/qos_imq.xml
XML_LIST-${CONFIG_APPS_LOGIC_STATIC_ROUTING} += staticrouting/staticrouting.xml
XML_LIST-${CONFIG_APPS_LOGIC_DROUTE} += droute/droute.xml
XML_LIST-${CONFIG_APPS_LOGIC_AUTH} += auth/auth.xml
XML_LIST-${CONFIG_APPS_LOGIC_FIREWALL} += firewall/firewall.xml
XML_LIST-${CONFIG_APPS_LOGIC_ANTIATTACK} += antiattack/antiattack.xml
XML_LIST-${CONFIG_APPS_LOGIC_SNTP} += sntp/sntp.xml
XML_LIST-${CONFIG_APPS_LOGIC_NATPRIO} += natprio/natprio.xml
XML_LIST-${CONFIG_APPS_LOGIC_DEVINFO} += devinfo/devinfo.xml
XML_LIST-${CONFIG_APPS_LOGIC_URL_FILTER} += urlfilter/urlfilter.xml
XML_LIST-${CONFIG_APPS_LOGIC_MACFILTER} += macfilter/macfilter.xml
XML_LIST-${CONFIG_APPS_LOGIC_UPNP} += upnp/upnp.xml
XML_LIST-${CONFIG_APPS_LOGIC_ATM} += atm/atm.xml
XML_LIST-${CONFIG_APPS_LOGIC_WLAN} += wlan/wlan.xml
XML_LIST-${CONFIG_APPS_LOGIC_SUPP} += supp/supp.xml
XML_LIST-${CONFIG_APPS_LOGIC_DSL} += dsl/dsl.xml
XML_LIST-${CONFIG_APPS_LOGIC_DDNS} += ddns/ddns.xml
XML_LIST-${CONFIG_APPS_LOGIC_USB_MASS} += usb/msg_module/usbmsg_mass.xml
XML_LIST-${CONFIG_APPS_LOGIC_FTPD} += ftpd/ftpd.xml
XML_LIST-${CONFIG_APPS_LOGIC_TRACERT} += tracert/tracert.xml
XML_LIST-${CONFIG_APPS_LOGIC_IPPMNGR} += ippmngr/ippmngr.xml
XML_LIST-${CONFIG_APPS_LOGIC_PORTTRIGGER} += porttrigger/porttrigger.xml
XML_LIST-${CONFIG_APPS_LOGIC_DDNS} += ddns/ddns.xml
XML_LIST-${CONFIG_APPS_LOGIC_VDSL} += vdsl/vdsl.xml
XML_LIST-${CONFIG_APPS_LOGIC_DIAG} += diagnostics/diag.xml
XML_LIST-${CONFIG_APPS_LOGIC_WANSELECT} += wanselect/wanselect.xml
XML_LIST-${CONFIG_APPS_LOGIC_SCHEDULE} += schedules/schedules.xml
XML_LIST-${CONFIG_APPS_LOGIC_SPT} += shareprotection/shareprotection.xml
XML_LIST-${CONFIG_APPS_LOGIC_DEVCONFIG} += devconfig/devconfig.xml
XML_LIST-${CONFIG_APPS_LOGIC_TRADITIONALNAT} += traditionalnat/traditionalnat.xml
XML_LIST-${CONFIG_APPS_LOGIC_PROUTE} += proute/proute.xml
XML_LIST-${CONFIG_APPS_LOGIC_TF_GUI} += tf_gui/customer_device.xml

XML_LIST-${CONFIG_APPS_LOGIC_DNS} += dhcp6s/dhcp6s.xml
XML_LIST-${CONFIG_APPS_LOGIC_V6CONN} += v6conn/v6conn.xml
XML_LIST-${CONFIG_APPS_LOGIC_V6SROUTE} += v6sroute/v6sroute.xml
XML_LIST-${CONFIG_APPS_LOGIC_RADVD} += radvd/radvd.xml
XML_LIST-${CONFIG_APPS_LOGIC_TF_FIREWALL} += tf_firewall/tf_firewall.xml
XML_LIST-${CONFIG_APPS_LOGIC_TF_PORTMAPPING} += tf_portmapping/tf_portmapping.xml
XML_LIST-${CONFIG_APPS_LOGIC_LAN_PORT_VLAN} += lanport_vlan/lanport_vlan.xml

#scb+ 2011-6-28
XML_LIST-${CONFIG_APPS_LOGIC_NEW3G} += new3g/new3g.xml

#lyl
XML_LIST-y += bridge_list/bridge_list.xml

#yuanchaoqun      
XML_LIST-${CONFIG_APPS_LOGIC_SUPPORT_CPUUSAGE} += os_info/os_info.xml

#chenkai
XML_LIST-${CONFIG_APPS_LOGIC_DLNA} += dlna/dlna.xml
#lbw
XML_LIST-${CONFIG_APPS_LOGIC_IPTUNNEL} += iptunnel/iptunnel.xml

#wangjiaming
XML_LIST-${CONFIG_APPS_LOGIC_GRE} += gre/gre.xml

#liangbingtong
XML_LIST-${CONFIG_APPS_LOGIC_TR069DIAG} += dslloopdiag/diagnostics.xml

XML_LIST-${CONFIG_APPS_LOGIC_MACCLONE} += macclone/macclone.xml

XML_LIST-${CONFIG_APPS_LOGIC_BER_TEST} += ber_test/ber_test.xml

installme::
	$(Q)cp logic $(FSROOT)/usr/bin/logic
ifndef CONFIG_TBS_GDB_DEBUG
	$(Q)$(STRIP) $(FSROOT)/usr/bin/logic
endif
	cp $(ROOTDIR)/tools/bin/xmltool ./
	./xmltool $(XML_LIST-y)
	if [ -f config_full.xml ];then \
	cp -f config_full.xml $(FSROOT)/etc/; \
	rm -f ./config_full.xml; \
	fi
	rm -f xmltool

usb/lib/libusb.a:
	$(MAKE) -C usb/lib/
