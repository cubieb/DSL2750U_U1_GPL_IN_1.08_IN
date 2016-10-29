#! /usr/bin/make

EXT_FUNC+=802_3 nat arp arpreply ip ip6 standard log redirect vlan vlan_t mark_m mark \
          pkttype stp among limit ulog nflog tbs_vlan_m time ftos classify 
EXT_TABLES+=filter nat broute proute aroute

#T&W begin
include $(ROOTDIR)/.config
EXT_TABLES-y :=
EXT_TABLES-${CONFIG_OPENSOURCE_EBT_PROUTE} += proute
EXT_TABLES-${CONFIG_OPENSOURCE_EBT_BROUTE} += broute
EXT_TABLES-${CONFIG_OPENSOURCE_EBT_AROUTE} += aroute
EXT_TABLES-${CONFIG_OPENSOURCE_EBT_FILTER} += filter
EXT_TABLES-${CONFIG_OPENSOURCE_EBT_NAT} += nat

EXT_FUNC-y += standard
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_802_3} += 802_3
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_AMONG} += among
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_ARP} += arp
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_IP} += ip
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_IPV6} += ip6
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_LIMIT} += limit
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_MARK_M} += mark_m
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_PKTTYPE} += pkttype
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_STP} += stp
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_VLAN} += vlan
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_TBSVLAN_M} += tbs_vlan_m
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_TIME} += time
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_ARPREPLY} += arpreply
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_NAT} += nat
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_MARK} += mark
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_REDIRECT} += redirect
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_FTOS} += ftos
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_WMM_MARK} += wmm_mark
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_VLAN_T} += vlan_t
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_CLASSIFY} += classify
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_LOG} += ulog
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_NFLOG} += nflog
EXT_FUNC-${CONFIG_OPENSOURCE_EBT_REALDEV} += realdev

#T&W end
EXT_OBJS+=$(foreach T,$(EXT_FUNC), extensions/ebt_$(T).o)
EXT_OBJS+=$(foreach T,$(EXT_TABLES), extensions/ebtable_$(T).o)
EXT_LIBS+=$(foreach T,$(EXT_FUNC), extensions/libebt_$(T).so)
EXT_LIBS+=$(foreach T,$(EXT_TABLES), extensions/libebtable_$(T).so)
EXT_LIBSI+=$(foreach T,$(EXT_FUNC), -lebt_$(T))
EXT_LIBSI+=$(foreach T,$(EXT_TABLES), -lebtable_$(T))

extensions/ebt_%.so: extensions/ebt_%.o
	$(CC) -shared -o $@ -lc $< -nostartfiles

extensions/libebt_%.so: extensions/ebt_%.so
	mv $< $@

extensions/ebtable_%.so: extensions/ebtable_%.o
	$(CC) -shared -o $@ -lc $< -nostartfiles

extensions/libebtable_%.so: extensions/ebtable_%.so
	mv $< $@

extensions/ebt_%.o: extensions/ebt_%.c include/ebtables_u.h
	$(CC) $(CFLAGS) $(CFLAGS_SH_LIB) $(PROGSPECS) -c -o $@ $< -I$(KERNEL_INCLUDES)

extensions/ebtable_%.o: extensions/ebtable_%.c
	$(CC) $(CFLAGS) $(CFLAGS_SH_LIB) $(PROGSPECS) -c -o $@ $< -I$(KERNEL_INCLUDES)

