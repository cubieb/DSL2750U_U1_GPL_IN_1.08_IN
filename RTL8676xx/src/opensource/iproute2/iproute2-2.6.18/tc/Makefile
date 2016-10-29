TCOBJ= tc.o tc_core.o tc_util.o tc_qdisc.o tc_class.o tc_filter.o

include ../Config
include $(ROOTDIR)/.config

tc-modules-y :=
tc-modules-${CONFIG_OPENSOURCE_TC_QFIFO} += q_fifo.o
tc-modules-${CONFIG_OPENSOURCE_TC_QSFQ} += q_sfq.o
tc-modules-${CONFIG_OPENSOURCE_TC_QTBF} +=  q_tbf.o
tc-modules-${CONFIG_OPENSOURCE_TC_QPRIO} += q_prio.o
tc-modules-${CONFIG_OPENSOURCE_TC_QCBQ} += q_cbq.o tc_cbq.o
tc-modules-${CONFIG_OPENSOURCE_TC_QRED} += q_red.o tc_red.o
tc-modules-${CONFIG_OPENSOURCE_TC_QGRED} += q_gred.o tc_red.o
tc-modules-${CONFIG_OPENSOURCE_TC_QHTB} += q_htb.o
tc-modules-${CONFIG_OPENSOURCE_TC_QDSMARK} += q_dsmark.o
tc-modules-${CONFIG_OPENSOURCE_TC_QINGRESS} += q_ingress.o
tc-modules-${CONFIG_OPENSOURCE_TC_QHFSC} += q_hfsc.o
tc-modules-${CONFIG_OPENSOURCE_TC_QATM} += q_atm.o
tc-modules-${CONFIG_OPENSOURCE_TC_QNETEM} += q_netem.o

tc-modules-${CONFIG_OPENSOURCE_TC_FBASIC} += f_basic.o
tc-modules-${CONFIG_OPENSOURCE_TC_FU32} += f_u32.o
tc-modules-${CONFIG_OPENSOURCE_TC_FROUTE} += f_route.o
tc-modules-${CONFIG_OPENSOURCE_TC_FFW} += f_fw.o
tc-modules-${CONFIG_OPENSOURCE_TC_FTCINDEX} += f_tcindex.o
tc-modules-${CONFIG_OPENSOURCE_TC_FRSVP} += f_rsvp.o

tc-modules-${CONFIG_OPENSOURCE_TC_ESTIMATOR} += tc_estimator.o m_estimator.o
tc-modules-${CONFIG_OPENSOURCE_TC_MONITOR} += tc_monitor.o

tc-modules-${CONFIG_OPENSOURCE_TC_ACT} += m_action.o
tc-modules-${CONFIG_OPENSOURCE_TC_ACT_POLICE} += m_police.o
tc-modules-${CONFIG_OPENSOURCE_TC_ACT_GACT} += m_gact.o
tc-modules-${CONFIG_OPENSOURCE_TC_ACT_MIRRED} += m_mirred.o
tc-modules-${CONFIG_OPENSOURCE_TC_ACT_IPT} += m_ipt.o
tc-modules-${CONFIG_OPENSOURCE_TC_ACT_PEDIT} += m_pedit.o
tc-modules-${CONFIG_OPENSOURCE_TC_ACT_PEDIT_IP} += p_ip.o
tc-modules-${CONFIG_OPENSOURCE_TC_ACT_PEDIT_ICMP} += p_icmp.o
tc-modules-${CONFIG_OPENSOURCE_TC_ACT_PEDIT_TCP} += p_tcp.o
tc-modules-${CONFIG_OPENSOURCE_TC_ACT_PEDIT_UDP} += p_udp.o

tc-modules-${CONFIG_OPENSOURCE_TC_POLICE} += m_police.o

tc-modules-${CONFIG_OPENSOURCE_TC_EMATCH} += m_ematch.o
tc-modules-${CONFIG_OPENSOURCE_TC_EMATCH} += emp_ematch.yacc.o 
tc-modules-${CONFIG_OPENSOURCE_TC_EMATCH} += emp_ematch.lex.o
tc-modules-${CONFIG_OPENSOURCE_TC_EM_NBYTE} += em_nbyte.o
tc-modules-${CONFIG_OPENSOURCE_TC_EM_CMP} += em_cmp.o
tc-modules-${CONFIG_OPENSOURCE_TC_EM_U32} += em_u32.o
tc-modules-${CONFIG_OPENSOURCE_TC_EM_META} += em_meta.o

TCOBJ += $(tc-modules-y)
CFLAGS += -DCONFIG_GACT -DCONFIG_GACT_PROB -I$(BUILDDIR)/

LDLIBS += -lm -ldl

LDFLAGS += -Wl,-export-dynamic

YACC := bison
LEX := flex

all: tc
tc: $(TCOBJ)

install: all
	mkdir -p $(DESTDIR)$(SBINDIR)
	install -c -m 0755 tc $(DESTDIR)$(SBINDIR)

clean:
	rm -f *.o tc *.so emp_ematch.yacc.h; \
	rm -f emp_ematch.yacc.output

%.yacc.c: %.y
	$(YACC) $(YACCFLAGS) -o $@ $<

%.lex.c: %.l
	$(LEX) $(LEXFLAGS) -o$@ $<
