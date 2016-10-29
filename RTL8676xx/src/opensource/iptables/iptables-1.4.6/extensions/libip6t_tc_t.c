/*
 * Shared library for ip6tables Traffic Class mangling
 *
 * (C) 2010 XuXiaodong <xuxiaodong@twsz.com>
 *
 * This code is released under the GNU GPL
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <ip6tables.h>
#include <linux/netfilter_ipv6/ip6t_tc_t.h>

static void
help(void)
{
	printf(
"TC v%s options:\n"
"  --set-value value   Set the Traffic Class field in the packet header to value.\n"
"\n",
				IPTABLES_VERSION);
};

static struct option opts[] =
{
	{"set-value", 1, 0, '1'},
	{ .name = NULL }
};

static void
parse_tc(const char *s, struct ip6t_TC_info *tcinfo)
{
	unsigned int tc;

	if((tc = strtoul(s, NULL, 10)) == -1)
		xtables_error(PARAMETER_PROBLEM,
			"Invalid TC `%s'\n", s);

	if(tc> 255)
		xtables_error(PARAMETER_PROBLEM,
			"TC `%d' out of range\n", tc);

	tcinfo->tc = (u_int8_t)tc;
	return;
}

static int
parse(int c, char **argv, int invert, unsigned int *flags,
                            const void *e, struct xt_entry_target **target)
{
	struct ip6t_TC_info *tcinfo =
		(struct ip6t_TC_info*)(*target)->data;

	switch(c) {
	case '1':
		if(*flags)
			xtables_error(PARAMETER_PROBLEM,
				"TC target: Only use --set-value ONCE !");
		parse_tc(optarg, tcinfo);
		*flags = 1;
		break;

	default:
		return 0;
	}

	return 1;
}

static void
final_check(unsigned int flags)
{
	if(!flags)
		xtables_error(PARAMETER_PROBLEM,
			"TC target: Parameter --set-value required");
}

static void
print_tc(u_int8_t tc, int numeric)
{
	printf("0x%02x ", tc);
}

static void
print(const void *ip, const struct xt_entry_target *target,
                 int numeric)
{
	const struct ip6t_TC_info *tcinfo =
		(const struct ip6t_TC_info*)target->data;

	printf(" TC set ");
	print_tc(tcinfo->tc, numeric);
}

static void
save(const void *ip, const struct xt_entry_target *target)
{
	const struct ip6t_TC_info *tcinfo =
		(const struct ip6t_TC_info*)target->data;

	printf("--set-value 0x%02x ", tcinfo->tc);
}

static struct xtables_target tc = {
    .name       = "TC",
    .version    = XTABLES_VERSION,
    .family     = NFPROTO_IPV6,
    .size       = XT_ALIGN(sizeof(struct ip6t_TC_info)),
    .userspacesize  = XT_ALIGN(sizeof(struct ip6t_TC_info)),
    .help       = help,
    .final_check = final_check,
    .parse      = parse,
    .print      = print,
    .save       = save,
    .extra_opts = opts,

};

void _init(void)
{
	xtables_register_target(&tc);
}
