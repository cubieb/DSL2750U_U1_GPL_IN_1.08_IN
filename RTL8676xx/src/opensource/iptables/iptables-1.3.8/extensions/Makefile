#! /usr/bin/make
# WARNING:
# only add extensions here that are either present in the kernel, or whose
# header files are present in the include/linux directory of this iptables
# package (HW)
#
#PF_EXT_SLIB:=connlimit IPID ah addrtype time string comment connmark conntrack dscp ecn esp hashlimit helper icmp iprange length limit mac mark multiport owner physdev pkttype policy realm sctp standard state tcp tcpmss tos ttl udp unclean CLASSIFY CONENAT CONNMARK DNAT DSCP ECN LOG MARK MASQUERADE MIRROR NETMAP NFQUEUE NOTRACK REDIRECT REJECT SAME SNAT TCPMSS TOS TTL ULOG 
#PF6_EXT_SLIB:=connmark eui64 hl icmp6 length limit mac mark multiport owner physdev policy standard state tcp udp CONNMARK HL LOG NFQUEUE MARK TCPMSS

PF_EXT_SLIB-y := standard
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_ADDRTYPE} += addrtype
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_AH} += ah
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_COMMENT} += comment
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_CONDITION} += condition
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_CONNBYTES} += connbytes
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_CONNMARK} += connmark
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_CONNRATE} += connrate
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_CONNTRACK} += conntrack
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_DCCP} += dccp
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_DSCP} += dscp
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_ECN} += ecn
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_ESP} += esp
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_HASHLIMIT} += hashlimit
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_HELPER} += helper
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_ICMP} += icmp
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_IPRANGE} += iprange
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_LENGTH} += length
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_LIMIT} += limit
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_MAC} += mac
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_MARK} += mark
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_MULTIPORT} += multiport
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_OWNER} += owner
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_PHYSDEV} += physdev
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_PKTTYPE} += pkttype
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_POLICY} += policy
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_QUOTA} += quota
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_REALM} += realm
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_RECENT} += recent
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_SCTP} += sctp
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_SET} += set
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_STATE} += state
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_STATISTIC} += statistic
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_STRING} += string
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_TCP} += tcp
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_TCPMSS} += tcpmss
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_TIME} += time
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_TOS} += tos
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_TTL} += ttl 
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_UDP} += udp
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_UNCLEAN} += unclean
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_CONNLIMIT} += connlimit

PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_CLASSIFY} += CLASSIFY
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_CLUSTERIP} += CLUSTERIP
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_CONENAT} += CONENAT
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_CONNMARK_T} += CONNMARK
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_DNAT} += DNAT
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_DSCP_T} += DSCP
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_ECN_T} += ECN
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_LOG} += LOG
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_LED} += LED
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_MARK_T} += MARK
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_MASQUERADE} += MASQUERADE
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_MIRROR} += MIRROR
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_NETMAP} += NETMAP
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_NFLOG} += NFLOG
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_NFQUEUE} += NFQUEUE
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_NOTRACK} += NOTRACK
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_REDIRECT} += REDIRECT
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_REJECT} += REJECT
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_SAME} += SAME
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_SET_T} += SET
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_SNAT} += SNAT
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_TOS_T} += TOS
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_TTL_T} += TTL
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_ULOG} += ULOG
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_TRIGGER} += TRIGGER
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_IPID} += IPID
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_IMQ} += IMQ
PF_EXT_SLIB-${CONFIG_OPENSOURCE_IPT_TCPMSS_T} += TCPMSS

PF6_EXT_SLIB-y := standard
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_AH} += ah
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_CONDITION} += condition
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_CONNMARK} += connmark
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_DST} += dst
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_ESP} += esp
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_EUI64} += eui64
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_FRAG} += frag
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_HASHLIMIT} += hashlimit
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_HBH} += hbh
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_HL} += hl
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_ICMP6} += icmp6
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_IPV6HEADER} += ipv6header
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_LENGTH} += length
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_LIMIT} += limit
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_MAC} += mac
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_MARK} += mark
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_MH} += mh
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_MULTIPORT} += multiport
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_OWNER} += owner
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_PHYSDEV} += physdev
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_POLICY} += policy
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_RT} += rt
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_SCTP} += sctp
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_STATE} += state
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_TCP} += tcp
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_UDP} += udp

PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_CONNMARK_T} += CONNMARK
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_CONNSECMARK} += CONNSECMARK
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_HL_T} += HL
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_LOG} += LOG
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_IMQ} += IMQ
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_MARK_T} += MARK
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_NFLOG} += NFLOG
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_NFQUEUE} += NFQUEUE
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_REJECT} += REJECT
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_SECMARK} += SECMARK
PF6_EXT_SLIB-${CONFIG_OPENSOURCE_IP6T_TCPMSS_T} += TCPMSS

ifeq ($(DO_SELINUX), 1)
#PF_EXT_SE_SLIB:=SECMARK CONNSECMARK
#PF6_EXT_SE_SLIB:=SECMARK CONNSECMARK
PF_EXT_SE_SLIB-y :=
PF_EXT_SE_SLIB-{CONFIG_OPENSOURCE_IPT_CONNSECMARK} += CONNSECMARK
PF_EXT_SE_SLIB-{CONFIG_OPENSOURCE_IPT_SECMARK} += SECMARK

PF6_EXT_SE_SLIB-y := 
PF6_EXT_SE_SLIB-{CONFIG_OPENSOURCE_IP6T_CONNSECMARK} += CONNSECMARK
PF6_EXT_SE_SLIB-{CONFIG_OPENSOURCE_IP6T_SECMARK} += SECMARK
endif

# Optionals
PF_EXT_SLIB_OPTS:=$(foreach T,$(wildcard extensions/.*-test),$(shell KERNEL_DIR=$(KERNEL_DIR) $(T)))
PF6_EXT_SLIB_OPTS:=$(foreach T,$(wildcard extensions/.*-test6),$(shell KERNEL_DIR=$(KERNEL_DIR) $(T)))

PF_EXT_ALL_SLIB:=$(patsubst extensions/libipt_%.c, %, $(wildcard extensions/libipt_*.c))
PF6_EXT_ALL_SLIB:=$(patsubst extensions/libip6t_%.c, %, $(wildcard extensions/libip6t_*.c))

PF_EXT_MAN_ALL_MATCHES:=$(foreach T,$(PF_EXT_ALL_SLIB),$(shell test -f extensions/libipt_$(T).man && grep -q register_match extensions/libipt_$(T).c  && echo $(T)))
PF_EXT_MAN_ALL_TARGETS:=$(foreach T,$(PF_EXT_ALL_SLIB),$(shell test -f extensions/libipt_$(T).man && grep -q register_target extensions/libipt_$(T).c && echo $(T)))
PF6_EXT_MAN_ALL_MATCHES:=$(foreach T,$(PF6_EXT_ALL_SLIB),$(shell test -f extensions/libip6t_$(T).man && grep -q register_match6 extensions/libip6t_$(T).c  && echo $(T)))
PF6_EXT_MAN_ALL_TARGETS:=$(foreach T,$(PF6_EXT_ALL_SLIB),$(shell test -f extensions/libip6t_$(T).man && grep -q register_target6 extensions/libip6t_$(T).c && echo $(T)))

PF_EXT_MAN_MATCHES:=$(filter $(PF_EXT_ALL_SLIB), $(PF_EXT_MAN_ALL_MATCHES))
PF_EXT_MAN_TARGETS:=$(filter $(PF_EXT_ALL_SLIB), $(PF_EXT_MAN_ALL_TARGETS))
PF_EXT_MAN_EXTRA_MATCHES:=$(filter-out $(PF_EXT_MAN_MATCHES), $(PF_EXT_MAN_ALL_MATCHES))
PF_EXT_MAN_EXTRA_TARGETS:=$(filter-out $(PF_EXT_MAN_TARGETS), $(PF_EXT_MAN_ALL_TARGETS))
PF6_EXT_MAN_MATCHES:=$(filter $(PF6_EXT_ALL_SLIB), $(PF6_EXT_MAN_ALL_MATCHES))
PF6_EXT_MAN_TARGETS:=$(filter $(PF6_EXT_ALL_SLIB), $(PF6_EXT_MAN_ALL_TARGETS))
PF6_EXT_MAN_EXTRA_MATCHES:=$(filter-out $(PF6_EXT_MAN_MATCHES), $(PF6_EXT_MAN_ALL_MATCHES))
PF6_EXT_MAN_EXTRA_TARGETS:=$(filter-out $(PF6_EXT_MAN_TARGETS), $(PF6_EXT_MAN_ALL_TARGETS))


allman:
	@echo ALL_SLIB: $(PF_EXT_ALL_SLIB)
	@echo ALL_MATCH: $(PF_EXT_MAN_ALL_MATCHES)
	@echo ALL_TARGET: $(PF_EXT_MAN_ALL_TARGETS)

PF_EXT_SLIB+=$(PF_EXT_SLIB_OPTS)
PF6_EXT_SLIB+=$(PF6_EXT_SLIB_OPTS)

OPTIONALS+=$(patsubst %,IPv4:%,$(PF_EXT_SLIB_OPTS))
OPTIONALS+=$(patsubst %,IPv6:%,$(PF6_EXT_SLIB_OPTS))

ifndef NO_SHARED_LIBS
SHARED_LIBS+=$(foreach T,$(PF_EXT_SLIB-y),extensions/libipt_$(T).so)
SHARED_SE_LIBS+=$(foreach T,$(PF_EXT_SE_SLIB-y),extensions/libipt_$(T).so)
EXTRA_INSTALLS+=$(foreach T, $(PF_EXT_SLIB-y), $(DESTDIR)$(LIBDIR)/iptables/libipt_$(T).so)
EXTRA_INSTALLS+=$(foreach T, $(PF_EXT_SE_SLIB-y), $(DESTDIR)$(LIBDIR)/iptables/libipt_$(T).so)

ifeq ($(DO_IPV6), 1)
SHARED_LIBS+=$(foreach T,$(PF6_EXT_SLIB-y),extensions/libip6t_$(T).so)
SHARED_SE_LIBS+=$(foreach T,$(PF6_EXT_SE_SLIB-y),extensions/libip6t_$(T).so)
EXTRA_INSTALLS+=$(foreach T, $(PF6_EXT_SLIB-y), $(DESTDIR)$(LIBDIR)/iptables/libip6t_$(T).so)
EXTRA_INSTALLS+=$(foreach T, $(PF6_EXT_SE_SLIB-y), $(DESTDIR)$(LIBDIR)/iptables/libip6t_$(T).so)
endif
else 	# NO_SHARED_LIBS
EXT_OBJS+=$(foreach T,$(PF_EXT_SLIB-y),extensions/libipt_$(T).o)
EXT_OBJS+=$(foreach T,$(PF_EXT_SE_SLIB-y),extensions/libipt_$(T).o)
EXT_FUNC+=$(foreach T,$(PF_EXT_SLIB-y),ipt_$(T))
EXT_FUNC+=$(foreach T,$(PF_EXT_SE_SLIB-y),ipt_$(T))
EXT_OBJS+= extensions/initext.o
ifeq ($(DO_IPV6), 1)
EXT6_OBJS+=$(foreach T,$(PF6_EXT_SLIB-y),extensions/libip6t_$(T).o)
EXT6_OBJS+=$(foreach T,$(PF6_EXT_SE_SLIB-y),extensions/libip6t_$(T).o)
EXT6_FUNC+=$(foreach T,$(PF6_EXT_SLIB-y),ip6t_$(T))
EXT6_FUNC+=$(foreach T,$(PF6_EXT_SE_SLIB-y),ip6t_$(T))
EXT6_OBJS+= extensions/initext6.o
endif	# DO_IPV6
endif	# NO_SHARED_LIBS

ifndef TOPLEVEL_INCLUDED
local:
	cd .. && $(MAKE) $(SHARED_LIBS) $(SHARED_SE_LIBS)
endif

ifdef NO_SHARED_LIBS
extensions/libext.a: $(EXT_OBJS)
	rm -f $@; ar crv $@ $(EXT_OBJS)

extensions/libext6.a: $(EXT6_OBJS)
	rm -f $@; ar crv $@ $(EXT6_OBJS)

extensions/initext.o: extensions/initext.c
extensions/initext6.o: extensions/initext6.c

extensions/initext.c: extensions/Makefile
	echo "" > $@
	for i in $(EXT_FUNC); do \
		echo "extern void $${i}_init(void);" >> $@; \
	done
	echo "void init_extensions(void) {" >> $@
	for i in $(EXT_FUNC); do \
		echo "	$${i}_init();" >> $@; \
	done
	echo "}" >> $@

extensions/initext6.c: extensions/Makefile
	echo "" > $@
	for i in $(EXT6_FUNC); do \
		echo "extern void $${i}_init(void);" >> $@; \
	done
	echo "void init_extensions(void) {" >> $@
	for i in $(EXT6_FUNC); do \
		echo "	$${i}_init();" >> $@; \
	done
	echo "}" >> $@

extensions/lib%.o: extensions/lib%.c
	$(CC) $(CFLAGS) -D_INIT=$*_init -c -o $@ $<

endif
 
EXTRAS += extensions/libipt_targets.man
extensions/libipt_targets.man: $(patsubst %,extensions/libipt_%.man,$(PF_EXT_MAN_ALL_TARGETS))
	@for ext in $(PF_EXT_MAN_TARGETS); do \
	    echo ".SS $$ext" ;\
	    cat extensions/libipt_$$ext.man ;\
	done >extensions/libipt_targets.man
	@if [ -n "$(PF_EXT_MAN_EXTRA_TARGETS)" ]; then \
	    extra=$(PF_EXT_MAN_EXTRA_TARGETS) ;\
	    for ext in $${extra:-""}; do \
		echo ".SS $$ext (not supported, see Patch-O-Matic)" ;\
		cat extensions/libipt_$$ext.man ;\
	    done ;\
       	fi >>extensions/libipt_targets.man

EXTRAS += extensions/libipt_matches.man
extensions/libipt_matches.man: $(patsubst %,extensions/libipt_%.man,$(PF_EXT_MAN_ALL_MATCHES))
	@for ext in $(PF_EXT_MAN_MATCHES); do \
	    echo ".SS $$ext" ;\
	    cat extensions/libipt_$$ext.man ;\
	done >extensions/libipt_matches.man
	@if [ -n "$(PF_EXT_MAN_EXTRA_MATCHES)" ]; then \
	    extra=$(PF_EXT_MAN_EXTRA_MATCHES) ;\
	    for ext in $${extra:-""}; do \
		echo ".SS $$ext (not supported, see Patch-O-Matic)" ;\
		cat extensions/libipt_$$ext.man ;\
	    done ;\
       	fi >>extensions/libipt_matches.man

EXTRAS += extensions/libip6t_targets.man
extensions/libip6t_targets.man: $(patsubst %, extensions/libip6t_%.man, $(PF6_EXT_MAN_ALL_TARGETS))
	@for ext in $(PF6_EXT_MAN_TARGETS); do \
	    echo ".SS $$ext" ;\
	    cat extensions/libip6t_$$ext.man ;\
	done >extensions/libip6t_targets.man
	@if [ -n "$(PF6_EXT_MAN_EXTRA_TARGETS)" ]; then \
	    extra=$(PF6_EXT_MAN_EXTRA_TARGETS) ;\
	    for ext in $${extra:-""}; do \
		echo ".SS $$ext (not supported, see Patch-O-Matic)" ;\
		cat extensions/libip6t_$$ext.man ;\
	    done ;\
       	fi >>extensions/libip6t_targets.man

EXTRAS += extensions/libip6t_matches.man
extensions/libip6t_matches.man: $(patsubst %, extensions/libip6t_%.man, $(PF6_EXT_MAN_ALL_MATCHES))
	@for ext in $(PF6_EXT_MAN_MATCHES); do \
	    echo ".SS $$ext" ;\
	    cat extensions/libip6t_$$ext.man ;\
	done >extensions/libip6t_matches.man
	@if [ -n "$(PF6_EXT_MAN_EXTRA_MATCHES)" ]; then \
	    extra=$(PF6_EXT_MAN_EXTRA_MATCHES) ;\
	    for ext in $${extra:-""}; do \
		echo ".SS $$ext (not supported, see Patch-O-Matic)" ;\
		cat extensions/libip6t_$$ext.man ;\
	    done ;\
       	fi >>extensions/libip6t_matches.man

$(DESTDIR)$(LIBDIR)/iptables/libipt_%.so: extensions/libipt_%.so
	@[ -d $(DESTDIR)$(LIBDIR)/iptables ] || mkdir -p $(DESTDIR)$(LIBDIR)/iptables
	cp $< $@

$(DESTDIR)$(LIBDIR)/iptables/libip6t_%.so: extensions/libip6t_%.so
	@[ -d $(DESTDIR)$(LIBDIR)/iptables ] || mkdir -p $(DESTDIR)$(LIBDIR)/iptables
	cp $< $@
