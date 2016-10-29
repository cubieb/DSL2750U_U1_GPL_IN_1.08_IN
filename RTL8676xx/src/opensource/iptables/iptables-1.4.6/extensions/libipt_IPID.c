/* Shared library add-on to iptables for the IPID target
 * This program is distributed under the terms of GNU GPL
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <xtables.h>


#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ipt_IPID.h>

#define IPT_IPID_USED	1

static void IPID_init(struct ipt_entry_target *t, unsigned int *nfcache) 
{
}

static void IPID_help(void) 
{
	printf("IPID target v%s options\n", XTABLES_VERSION);
	printf("--ipid-pace value\n");
	printf("--ipid-chaotic value\n");
	
}

static int IPID_parse(int c, char **argv, int invert, unsigned int *flags,
		const struct ipt_entry *entry,
		struct ipt_entry_target **target)
{
	struct ipt_IPID_info *info = (struct ipt_IPID_info *) (*target)->data;
	unsigned int value;
	
	if (*flags & IPT_IPID_USED) {
		xtables_error(PARAMETER_PROBLEM, 
				"Can't specify IPID option twice");
	}
	if (!optarg)
		xtables_error(PARAMETER_PROBLEM,
			"IPID: You must specify a value");
	if (xtables_check_inverse(optarg,&invert,NULL,0,argv))
		xtables_error(PARAMETER_PROBLEM,
				"IPID: unexpected '!'");
	if(string_to_number(optarg,0,65535,&value) == -1)
		xtables_error(PARAMETER_PROBLEM,
				"IPID: Expected value between 0 and 65535");
	switch (c) {
		case '1':
		info->mode = IP_IPID_PACE;
		info->ipid = value;
		break;
		case '2':
		info->mode = IP_IPID_CHAOTIC;
		break;
	}
	*flags |= IPT_IPID_USED;

	return 1;
}

static void IPID_check(unsigned int flags)
{
	if (!(flags & IPT_IPID_USED))
		xtables_error(PARAMETER_PROBLEM,
				"IPID: You must specify an action");
}

static void IPID_save(const struct ipt_ip *ip,
		const struct ipt_entry_target *target)
{
	const struct ipt_IPID_info *info = 
		(struct ipt_IPID_info *) target->data;
	switch(info->mode){
		case IP_IPID_PACE:
		printf("--ipid-pace %u",info->ipid);
		break;
		case IP_IPID_CHAOTIC:
		printf("--ipid-chaotic 0");
		break;
	}
	
}

static void IPID_print(const struct ipt_ip *ip,
		const struct ipt_entry_target *target, int numeric)
{
	const struct ipt_IPID_info *info =
		(struct ipt_IPID_info *) target->data;

	printf("IPID ");
	
	switch(info->mode){
		case IP_IPID_PACE:
		printf("pace is %u ", info->ipid);
		break;
		case IP_IPID_CHAOTIC:
		printf("chaotic");
		break;
	}
}

static struct option IPID_opts[] = {
	{ "ipid-pace", 1, 0, '1' },
	{ "ipid-chaotic", 1, 0, '2' },
	{ 0 }	
};

static struct xtables_target IPID_mt_reg = {
	.next		    = NULL, 
	.name		    = "IPID",
	.version	    = XTABLES_VERSION,
	.size		    = XT_ALIGN(sizeof(struct ipt_IPID_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct ipt_IPID_info)),
	.help		    = IPID_help,
	.init		    = IPID_init,
	.parse		    = IPID_parse,
	.final_check	= IPID_check,
	.print		    = IPID_print,
	.save		    = IPID_save,
	.extra_opts	    = IPID_opts 
};

void _init(void)
{
	xtables_register_target(&IPID_mt_reg);
}
