/*
 * Shared library for ip6tables for Flow Label matching
 * xuxiaodong <xuxiaodong@twsz.com>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ip6tables.h>
#include <getopt.h>
#include <linux/netfilter_ipv6/ip6t_fl.h>
#include <linux/netfilter_ipv6.h>

static void
help(void)
{
	printf(
"fl(flow label) match v%s options:\n"
"[!] --flowlabel value	Match IPV6 flow label with numerical value\n"
"				This value can be in decimal (ex: 32)\n"
"				or in hex (ex: 0x20)\n"
				, IPTABLES_VERSION
);
}

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


static const struct option opts[] = {
	{ "flowlabel", 1, 0, '1' },
	{ .name = NULL }
};

static void
init(struct xt_entry_match *m)
{

}

#define IP6T_FL_MAX 0x000FFFFF
static void
parse_fl(const char *s, struct ip6t_fl_info *flinfo)
{
	unsigned int flowlabel;

	if((flowlabel = strtoul(s, 0, 16)) == -1)
		xtables_error(PARAMETER_PROBLEM,
				"Invalid Flow Label value `%s'\n", s);

	if(flowlabel > IP6T_FL_MAX)
		xtables_error(PARAMETER_PROBLEM,
				"Flow Label '%d' out of range\n", flowlabel);

	flinfo->flowlabel = flowlabel&IPV6_FLOW_LABLE_MASK;
	return;
}

static int
parse(int c, char **argv, int invert, unsigned int *flags,
                    const void *entry, struct xt_entry_match **match)
{
	struct ip6t_fl_info *flinfo = (struct ip6t_fl_info*)(*match)->data;

	switch(c) {
	case '1':
		if(*flags)
			xtables_error(PARAMETER_PROBLEM,
				"DSCP match: Only use --flowlabel ONCE!");

		check_inverse(optarg, &invert, &optind, 0);
		parse_fl(argv[optind - 1], flinfo);
		if(invert)
			flinfo->invert = 1;
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
			"fl match: You must specify --flowlabel");
}

static void
print_fl(unsigned int flowlabel, int invert, int numeric)
{
	if(invert)
		fputc('!', stdout);

	printf("0x%x ", flowlabel);
}

static void
print(const void *ip, const struct xt_entry_match *match,
                     int numeric)
{
	const struct ip6t_fl_info *flinfo =  (const struct ip6t_fl_info*)match->data;
	printf("Flow Label match ");
	print_fl(flinfo->flowlabel, flinfo->invert, numeric);
}

static void
save(const void *ip, const struct xt_entry_match *match)
{
	const struct ip6t_fl_info *flinfo = (const struct ip6t_fl_info*)match->data;

	printf("--flowlabel ");
	print_fl(flinfo->flowlabel, flinfo->invert, 1);
}

static struct xtables_match fl = {
    .name       = "fl",
    .version    = XTABLES_VERSION,
    .family     = NFPROTO_IPV6,
    .size       = XT_ALIGN(sizeof(struct ip6t_fl_info)),
    .userspacesize  = XT_ALIGN(sizeof(struct ip6t_fl_info)),
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
	xtables_register_match(&fl);
}
