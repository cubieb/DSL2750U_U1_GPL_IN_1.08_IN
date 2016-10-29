/* Shared library add-on to iptables to add MARK target support. */
/*在一条iptables规则中同时支持对mark值进行set/and/or操作
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
/* For 64bit kernel / 32bit userspace */
#include "../include/linux/netfilter_ipv4/ipt_MULTIMARK.h"

/* Function which prints out usage message. */
static void
help(void)
{
	printf(
"MULTIMARK target v%s options:\n"
"  --set-multimark value                   Set nfmark value\n"
"  --and-multimark value                   Binary AND the nfmark with value\n"
"  --or-multimark  value                   Binary OR  the nfmark with value\n"
"  --set-multirmark  value                 Set nfrmark value\n"
"\n",
IPTABLES_VERSION);
}

static struct option opts[] = {
	{ "set-multimark", 1, 0, '1' },
	{ "and-multimark", 1, 0, '2' },
	{ "or-multimark", 1, 0, '3' },
	{ "set-multimark", 1, 0, '4'},
	{ 0 }
};

/* Initialize the target. */
static void
init(struct xt_entry_target *t)
{
}

/* Function which parses command options; returns true if it
   ate an option */
static int
parse_v1(int c, char **argv, int invert, unsigned int *flags,
                         const void *entry, struct xt_entry_target **target)
{
    unsigned int uMarkOpt = 0;
    unsigned int mark = 0;

	struct ipt_mark_multiopt_target_info_v1 *markinfo
		= (struct ipt_mark_multiopt_target_info_v1 *)(*target)->data;

    switch (c) {
    case '1':
    case '2':
    case '3':
    case '4':

        break;
    default:
        return 0;
    }

    mark = strtoul(optarg, NULL, 16);
    //if (xtables_strtoui(optarg, NULL, &mark, 0, 0xffffffff))
    //    xtables_error(PARAMETER_PROBLEM, "Bad MARK value `%s'", optarg);

	switch (c) {
	case '1':
        markinfo->setmark = mark;
        markinfo->mode |= IPT_MARK_MULTIOPT_SET;
        uMarkOpt = IPT_MARK_MULTIOPT_SET;
		break;
	case '2':
        markinfo->andmark = mark;
        markinfo->mode |= IPT_MARK_MULTIOPT_AND;
        uMarkOpt = IPT_MARK_MULTIOPT_AND;
		break;
	case '3':
        markinfo->ormark = mark;
		markinfo->mode |= IPT_MARK_MULTIOPT_OR;
        uMarkOpt = IPT_MARK_MULTIOPT_OR;
		break;
	case '4':
        markinfo->setrmark = mark;
		markinfo->mode |= IPT_RMARK_MULTIOPT_SET;
        uMarkOpt = IPT_RMARK_MULTIOPT_SET;
		break;
	default:
		return 0;
	}

	if (*flags & uMarkOpt)
		xtables_error(PARAMETER_PROBLEM,
			   "MULTIMARK target: Can't specify --set-multimark/--and-multimark/--or-multimark twice");

	*flags |= uMarkOpt;
	return 1;
}


static void
print_mark(unsigned int mark)
{
	printf("0x%x ", mark);
}

/* Prints out the targinfo. */
static void
print_v1(const void *ip, const struct xt_entry_target *target,
                 int numeric)
{
	const struct ipt_mark_multiopt_target_info_v1 *markinfo =
		(const struct ipt_mark_multiopt_target_info_v1 *)target->data;
    printf(" MULTIMARK ");

    if(markinfo->mode & IPT_MARK_MULTIOPT_SET)
    {
        printf(" --set-multimark ");
        print_mark(markinfo->setmark);
    }
    if(markinfo->mode & IPT_MARK_MULTIOPT_AND)
    {
        printf(" --and-multimark ");
        print_mark(markinfo->andmark);
    }
    if(markinfo->mode & IPT_MARK_MULTIOPT_OR)
    {
        printf(" --or-multimark ");
        print_mark(markinfo->ormark);
    }
    if(markinfo->mode & IPT_RMARK_MULTIOPT_SET)
    {
        printf(" --set-multirmark ");
        print_mark(markinfo->setrmark);
    }

}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save_v1(const void *ip, const struct xt_entry_target *target)
{
	const struct ipt_mark_multiopt_target_info_v1 *markinfo =
		(const struct ipt_mark_multiopt_target_info_v1 *)target->data;

    if(markinfo->mode & IPT_MARK_MULTIOPT_SET)
    {
        printf(" --set-multimark ");
        print_mark(markinfo->setmark);
    }
    if(markinfo->mode & IPT_MARK_MULTIOPT_AND)
    {
        printf(" --and-multimark ");
        print_mark(markinfo->andmark);
    }
    if(markinfo->mode & IPT_MARK_MULTIOPT_OR)
    {
        printf(" --or-multimark ");
        print_mark(markinfo->ormark);
    }
    if(markinfo->mode & IPT_RMARK_MULTIOPT_SET)
    {
        printf(" --set-multirmark ");
        print_mark(markinfo->setrmark);
    }
}


static
struct xtables_target mark_v1 = {
    .name       = "MULTIMARK",
    .version    = XTABLES_VERSION,
    .family     = NFPROTO_IPV4,
    .size       = XT_ALIGN(sizeof(struct ipt_mark_multiopt_target_info_v1)),
    .userspacesize  = XT_ALIGN(sizeof(struct ipt_mark_multiopt_target_info_v1)),
    .help       = help,
    .init       = init,
    .parse      = parse_v1,
    .print      = print_v1,
    .save       = save_v1,
    .extra_opts = opts,
};


void _init(void)
{
	xtables_register_target(&mark_v1);
}

