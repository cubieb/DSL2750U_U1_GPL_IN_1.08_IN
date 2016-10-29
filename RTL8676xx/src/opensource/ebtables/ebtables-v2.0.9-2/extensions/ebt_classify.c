/* ebt_classify
 *
 * Authors: XuXiaodong
 *
 * 2009-03-25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <linux/types.h>
#include <linux/pkt_sched.h>
#include "../include/ebtables_u.h"
#include <linux/netfilter_bridge/ebt_classify.h>

static int class_supplied;

#define CLASS_SETVAL  '1'
#define CLASS_TARGET  '2'
static struct option opts[] =
{
	{ "set-class" , required_argument, 0,  CLASS_SETVAL},
	{ "class-target", required_argument, 0, CLASS_TARGET},
	{ 0 }
};

static void print_help()
{
	printf(
	"classify target options:\n"
	"--set-class [MAJOR:MINOR]  (Set skb->priority value)\n"
	"--class-target target : ACCEPT, DROP, RETURN or CONTINUE\n");
}

static void init(struct ebt_entry_target *target)
{
	struct ebt_classify_target_info *clinfo =
	   (struct ebt_classify_target_info *)target->data;

	clinfo->priority= 0;
	clinfo->target = EBT_CONTINUE;
	class_supplied = 0;
}

int string_to_priority(const char *s, unsigned int *p)
{
	unsigned int i, j;

	if (sscanf(s, "%x:%x", &i, &j) != 2)
		return 1;
	
	*p = TC_H_MAKE(i<<16, j);
	return 0;
}

#define OPT_SET_CLASS  0x01
#define OPT_CLASS_TARGET 0x02

static int parse(int c, char **argv, int argc,
   const struct ebt_u_entry *entry, unsigned int *flags,
   struct ebt_entry_target **target)
{
	struct ebt_classify_target_info *clinfo =
	   (struct ebt_classify_target_info *)(*target)->data;

	switch (c) {
	case CLASS_SETVAL:
		ebt_check_option(flags, OPT_SET_CLASS);
		if (string_to_priority(optarg, &clinfo->priority))
			ebt_print_error("Bad class value `%s'", optarg);		
		class_supplied = 1;
		break;
	case CLASS_TARGET:
		ebt_check_option(flags, OPT_CLASS_TARGET);
		if (FILL_TARGET(optarg, clinfo->target))
			ebt_print_error("Illegal --class-target target");
		break;	
	 default:
		return 0;
	}
	return 1;
}

static void final_check(const struct ebt_u_entry *entry,
   const struct ebt_entry_target *target, const char *name,
   unsigned int hookmask, unsigned int time)
{
	struct ebt_classify_target_info *clinfo =
	   (struct ebt_classify_target_info *)target->data;

	if (time == 0 && class_supplied == 0)
		ebt_print_error("No class value supplied");
		
	if (BASE_CHAIN && clinfo->target == EBT_RETURN)
		ebt_print_error("--class-target RETURN not allowed on base chain");		
}

static void print_class(unsigned int priority)
{
	printf("%x:%x ", TC_H_MAJ(priority)>>16, TC_H_MIN(priority));
}

static void print(const struct ebt_u_entry *entry,
   const struct ebt_entry_target *target)
{
	struct ebt_classify_target_info *clinfo =
	   (struct ebt_classify_target_info *)target->data;

	printf("--set-class ");
	print_class(clinfo->priority);
	
	if (clinfo->target == EBT_ACCEPT)
		return;
	printf(" --class-target %s", TARGET_NAME(clinfo->target));	
}


static int compare(const struct ebt_entry_target *t1,
   const struct ebt_entry_target *t2)
{
	struct ebt_classify_target_info *clinfo1 =
	   (struct ebt_classify_target_info *)t1->data;
	struct ebt_classify_target_info *clinfo2 =
	   (struct ebt_classify_target_info *)t2->data;

	return clinfo1->target == clinfo2->target &&
	   clinfo1->priority == clinfo2->priority;
}


static struct ebt_u_target classify_target =
{
	.name		= EBT_CLASSIFY_TARGET,
	.size		= sizeof(struct ebt_classify_target_info),
	.help		= print_help,
	.init		= init,
	.parse		= parse,
	.final_check	= final_check,
	.print		= print,
	.compare	= compare,
	.extra_ops	= opts,
};

void _init(void)
{
	ebt_register_target(&classify_target);
}

