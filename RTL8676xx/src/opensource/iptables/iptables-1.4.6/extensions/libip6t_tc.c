/*
 * Shared library for ip6tables for traffic class matching
 * xuxiaodong <xuxiaodong@twsz.com>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ip6tables.h>
#include <getopt.h>
#include <linux/netfilter_ipv6/ip6t_tc_m.h>
#include <linux/netfilter_ipv6.h>

static void
help(void)
{
	printf(
"tc(traffic class) match v%s options:\n"
"[!] --value value	Match IPV6 traffic class with numerical value\n"
"				This value can be in decimal (ex: 32)\n"
"				or in hex (ex: 0x20)\n"
				, IPTABLES_VERSION
);
}

static const struct option opts[] = {
	{ "value", 1, 0, '1' },
	{ .name = NULL }
};

static int check_inverse(const char option[], int *invert, int *poptind, int argc)
{
	if (option && strcmp(option, "!") == 0) {
		if (*invert)
			xtables_error(PARAMETER_PROBLEM,
				   "Multiple `!' flags not allowed");
		*invert = 1;
		if (poptind) {
			*poptind = *poptind+1;
			if (argc && *poptind > argc)
				xtables_error(PARAMETER_PROBLEM,
					   "no argument following `!'");
		}

		return 1;
	}
	return 0;
}


static void
init(struct ip6t_entry_match *m)
{

}

static void
parse_tc(const char *s, struct ip6t_tc_info *tcinfo)
{
	unsigned int tc;

	if((tc = strtoul(s, NULL, 10)) == -1)
		xtables_error(PARAMETER_PROBLEM,
				"Invalid traffic class value `%s'\n", s);

	if(tc > 255)
		xtables_error(PARAMETER_PROBLEM,
				"Traffic Class '%d' out of range\n", tc);

	tcinfo->tc = tc&0xFF;
	return;
}

static int
parse(int c, char **argv, int invert, unsigned int *flags,
                    const void *entry, struct xt_entry_match **match)
{
	struct ip6t_tc_info *tcinfo = (struct ip6t_tc_info*)(*match)->data;

	switch(c) {
	case '1':
		if(*flags)
			xtables_error(PARAMETER_PROBLEM,
				"TC match: Only use --value ONCE!");

		check_inverse(optarg, &invert, &optind, 0);
		parse_tc(argv[optind - 1], tcinfo);
		if(invert)
			tcinfo->invert = 1;
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
			"tc match: You must specify --value");
}

static void
print_tc(unsigned int tc, int invert, int numeric)
{
	if(invert)
		fputc('!', stdout);

	printf("0x%x ", tc);
}

static void
print(const void *ip, const struct xt_entry_match *match,
                     int numeric)
{
	const struct ip6t_tc_info *tcinfo =  (const struct ip6t_tc_info*)match->data;
	printf(" Traffic Class match ");
	print_tc(tcinfo->tc, tcinfo->invert, numeric);
}

static void
save(const void *ip, const struct xt_entry_match *match)
{
	const struct ip6t_tc_info *tcinfo = (const struct ip6t_tc_info*)match->data;

	printf("--value ");
	print_tc(tcinfo->tc, tcinfo->invert, 1);
}

static struct xtables_match tc = {
    .name       = "tc",
    .version    = XTABLES_VERSION,
    .family     = NFPROTO_IPV6,
    .size       = XT_ALIGN(sizeof(struct ip6t_tc_info)),
    .userspacesize  = XT_ALIGN(sizeof(struct ip6t_tc_info)),
    .help       = help,
    .init       = init,
    .parse      = parse,
    .final_check = final_check,
    .print      = print,
    .save       = save,
    .extra_opts = opts,
};

void _init(void)
{
	xtables_register_match(&tc);
}

