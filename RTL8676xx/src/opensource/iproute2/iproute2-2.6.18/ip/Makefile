#IPOBJ=ip.o ipaddress.o iproute.o iprule.o \
#    rtm_map.o iptunnel.o ip6tunnel.o tunnel.o ipneigh.o ipntable.o iplink.o \
#    ipmaddr.o ipmonitor.o ipmroute.o ipprefix.o \
#    ipxfrm.o xfrm_state.o xfrm_policy.o xfrm_monitor.o

include ${ROOTDIR}/.config
RTMONOBJ=rtmon.o

CFLAGS += -I$(BUILDDIR)
IPOBJ-y = ip.o rtm_map.o
IPOBJ-${CONFIG_OPENSOURCE_IP_LINK} += iplink.o
IPOBJ-${CONFIG_OPENSOURCE_IP_ADDR} += ipaddress.o	
IPOBJ-${CONFIG_OPENSOURCE_IP_RT_TAB} += iproute.o
IPOBJ-${CONFIG_OPENSOURCE_IP_RT_RULE} += iprule.o
IPOBJ-${CONFIG_OPENSOURCE_IP_TUNNEL} += iptunnel.o ip6tunnel.o tunnel.o
IPOBJ-${CONFIG_OPENSOURCE_IP_NEIGH} += ipneigh.o
IPOBJ-${CONFIG_OPENSOURCE_IP_NTAB} += ipntable.o
IPOBJ-${CONFIG_OPENSOURCE_IP_MADDR} += ipmaddr.o
IPOBJ-${CONFIG_OPENSOURCE_IP_MROUTE}  += ipmroute.o
IPOBJ-${CONFIG_OPENSOURCE_IP_MONITOR} += ipmonitor.o ipprefix.o
IPOBJ-${CONFIG_OPENSOURCE_IP_XFRM} += ipxfrm.o xfrm_state.o xfrm_policy.o xfrm_monitor.o

ALLOBJ=$(IPOBJ-y) $(RTMONOBJ)
SCRIPTS=ifcfg rtpr routel routef
#TARGETS=ip rtmon
TARGETS=ip

#all: $(TARGETS) $(SCRIPTS)
all: $(TARGETS)
ip: $(IPOBJ-y)

rtmon: $(RTMONOBJ) $(LIBNETLINK)

install: all
	install -m 0755 $(TARGETS) $(DESTDIR)$(SBINDIR)
#	install -m 0755 $(SCRIPTS) $(DESTDIR)$(SBINDIR)

clean:
	rm -f *.o $(TARGETS)
