/* Shared library add-on to iptables to add connection limit support. */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ip_conntrack.h>
#include <linux/netfilter_ipv4/ipt_connlimit.h>

/* Function which prints out usage message. */
static void help(void)
{
	 printf(
	"connlimit v%s options:\n"
	"[!] --connlimit-above n match if the number of existing tcp connections is (not) above n\n"
	" --connlimit-mask n group hosts using mask\n"
	"\n", IPTABLES_VERSION);
}

static struct option opts[] = {
	 {"connlimit-above", 1, NULL, '1'},
	 {"connlimit-mask", 1, NULL, '2'},
	 {NULL},
};

/* Function which parses command options; returns true if it
 ate an option */
static int parse(int c, char **argv, int invert, unsigned int *flags,
 const struct ipt_entry *entry, unsigned int *nfcache,
 struct ipt_entry_match **match)
{
	 struct ipt_connlimit_info *info = (void *)(*match)->data;
	 int i;

	 /*
	 * set default mask unless we have already seen a mask option
	 */
	 if (!(*flags & 2))
	 	info->mask = htonl(0xFFFFFFFF);

	 switch (c) {
	 	case '1':
			 check_inverse(optarg, &invert, &optind, 0);
			 info->limit = strtoul(argv[optind-1], NULL, 0);
			 info->inverse = invert;
			 *flags |= 1;
		 break;

		 case '2':
			 i = strtol(argv[optind-1], NULL, 0);
			 if (i < 0 || i > 32)
			 exit_error(PARAMETER_PROBLEM,
			 "--connlimit-mask must be between 0 and 32");

			 if (i == 0)
			 info->mask = 0;
			 else
			 info->mask = htonl(0xFFFFFFFF << (32 - i));

			 *flags |= 2;
		 break;

		 default:
		 	return 0;
	 }

	 return 1;
}

/* Final check */
static void final_check(unsigned int flags)
{
	 if (!(flags & 1))
	 	exit_error(PARAMETER_PROBLEM, "You must specify `--connlimit-above'");
}

static int count_bits(u_int32_t mask)
{
	 int i, bits;

	 for (bits = 0, i = 31; i >= 0; i--) {
		 if (mask & htonl((u_int32_t)1 << i)) {
			 bits++;
			 continue;
	 	}
	 	break;
	 }
	 return bits;
}

/* Prints out the matchinfo. */
static void print(const struct ipt_ip *ip, const struct ipt_entry_match *match,
 int numeric)
{
	 const struct ipt_connlimit_info *info = (const void *)match->data;

	 printf("#conn/%d %s %d ", count_bits(info->mask),
	 info->inverse ? "<" : ">", info->limit);
}

/* Saves the matchinfo in parsable form to stdout. */
static void save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
	 const struct ipt_connlimit_info *info = (const void *)match->data;

	 printf("%s--connlimit-above %u --connlimit-mask %u ",
	 info->inverse ? "! " : "", info->limit,
	 count_bits(info->mask));
}
	
static struct iptables_match connlimit = {
	 .next		= NULL, 
	 .name = "connlimit",
	 .version = IPTABLES_VERSION,
	 .size = IPT_ALIGN(sizeof(struct ipt_connlimit_info)),
	 .userspacesize = offsetof(struct ipt_connlimit_info, data),
	 .help = help,
	 .parse = parse,
	 .final_check = final_check,
	 .print = print,
	 .save = save,
	 .extra_opts = opts,
};

void _init(void)
{
 	register_match(&connlimit);
}
