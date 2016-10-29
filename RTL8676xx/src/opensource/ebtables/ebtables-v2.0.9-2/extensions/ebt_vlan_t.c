/* ebt_vlan
 *
 * Authors:
 * ouyangdi  <fireor@126.com>
 *
 * May, 2008
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "../include/ebtables_u.h"
#include <linux/netfilter_bridge/ebt_vlan_t.h>

static int vlan_supplied;

#define VLAN_TARGET '1'
#define VLAN_SET  '2'
#define VLAN_PRIORITY '3'
#define VLAN_HEADER '4'
#define VLAN_UNTAG '5'
#define VLAN_REDIR '6'
static struct option opts[] =
{
	{"vlan-target",required_argument,0,VLAN_TARGET},
	{ "vlan-set" , required_argument, 0, VLAN_SET},
	{"vlan-priority",required_argument, 0, VLAN_PRIORITY},
	/* an oldtime messup, we should have always used the scheme
	 * <extension-name>-<option> */
	{ "vlan-header"    , no_argument, 0, VLAN_HEADER},
	{ "vlan-untag"    , no_argument, 0, VLAN_UNTAG},
	{"redirect-dev",required_argument, 0, VLAN_REDIR},
	{ 0 }
};

static void print_help()
{
	printf(
	"vlan target options:\n"
	" --vlan-set value     : Set vlan value,0-4095\n"
	" --vlan-priority prioriry : set vlan priority,0-7\n"
	" --vlan-header : Add vlan tag to frame's header\n"
	" --vlan-untag : Delete valn tag from frame's header\n"
	" --redirect-dev: Set packets sent device\n"
	" --vlan-target target : ACCEPT, DROP, RETURN or CONTINUE\n");
}

static void init(struct ebt_entry_target *target)
{
	struct ebt_vlan_t_info *vlaninfo =
	   (struct ebt_vlan_t_info *)target->data;

	vlaninfo->target = EBT_ACCEPT;
	vlaninfo->vlan = 0;
	vlaninfo->cmd = 0;
	vlaninfo->redir_ifindex = -1;
	vlan_supplied = 0;
}

/*mod by pengyao 2010116*/
#define OPT_VLAN_TARGET     (0x01<<0)
#define OPT_VLAN_SET        (0x01<<1)
#define OPT_VLAN_PRIORITY   (0x01<<2)
#define OPT_VLAN_HEADER     (0x01<<3)
#define OPT_VLAN_UNTAG      (0x01<<4)
#define OPT_VLAN_REDIR      (0x01<<5)

static int parse(int c, char **argv, int argc,
   const struct ebt_u_entry *entry, unsigned int *flags,
   struct ebt_entry_target **target)
{
	struct ebt_vlan_t_info *vlaninfo =
	   (struct ebt_vlan_t_info *)(*target)->data;
	char *end;

	switch (c) {
	case VLAN_TARGET:
		ebt_check_option(flags, OPT_VLAN_TARGET);
                if (FILL_TARGET(optarg, vlaninfo->target))
                        ebt_print_error("Illegal --vlan-target target");
		return 1;

	case VLAN_SET:
		ebt_check_option(flags, OPT_VLAN_SET);
		if (*flags & (OPT_VLAN_HEADER|OPT_VLAN_UNTAG))
			ebt_print_error("--vlan-set cannot be used together with specific --vlan option");
		vlaninfo->vlan = strtoul(optarg, &end, 0);
		if (*end != '\0' || end == optarg)
			ebt_print_error("Bad vlan value '%s'", optarg);
		vlaninfo->cmd |= OPT_VLAN_SET;
		vlan_supplied = 1;
		break;

	case VLAN_PRIORITY:
		ebt_check_option(flags, OPT_VLAN_PRIORITY);
		if (*flags & (OPT_VLAN_HEADER|OPT_VLAN_UNTAG))
			ebt_print_error("--vlan-priority cannot be used together with specific --vlan option");
		vlaninfo->priority	= strtoul(optarg, &end, 0);
		if (*end != '\0' || end == optarg)
			ebt_print_error("Bad vlan priority '%s'", optarg);
		vlaninfo->cmd |= OPT_VLAN_PRIORITY;
		vlan_supplied = 1;
		break;

	case VLAN_HEADER:
		ebt_check_option(flags, OPT_VLAN_HEADER);
		if (*flags & (OPT_VLAN_UNTAG|OPT_VLAN_SET|OPT_VLAN_PRIORITY))
			ebt_print_error("--vlan-header cannot be used together with specific --vlan option");
/*
		if (check_inverse(optarg))
			ebt_print_error("Unexpected `!' after --vlan-header");
*/
		vlaninfo->cmd |= OPT_VLAN_HEADER;
		vlan_supplied = 1;
              break;

	case VLAN_UNTAG:
		ebt_check_option(flags, OPT_VLAN_UNTAG);
		if (*flags & (OPT_VLAN_HEADER|OPT_VLAN_SET|OPT_VLAN_PRIORITY))
			ebt_print_error("--vlan-untag cannot be used together with specific --vlan option");

/*		if (check_inverse(optarg))
			ebt_print_error("Unexpected `!' after --vlan-untag");
*/
		vlaninfo->cmd |= OPT_VLAN_UNTAG;
		vlan_supplied = 1;
              break;

	case VLAN_REDIR:
		ebt_check_option(flags, OPT_VLAN_REDIR);
		vlaninfo->redir_ifindex = if_nametoindex(optarg);
        vlaninfo->cmd |= OPT_VLAN_REDIR;
        vlan_supplied = 1;
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
	struct ebt_vlan_t_info *vlaninfo =
	   (struct ebt_vlan_t_info *)target->data;

	if (time == 0 && vlan_supplied == 0) {
		ebt_print_error("No vlan value supplied");
	}

	if(vlaninfo->vlan > 4095 || vlaninfo->vlan < 0) {
		ebt_print_error("Invaild vlan ID");
	}

	if(vlaninfo->priority > 7 || vlaninfo->priority < 0) {
		ebt_print_error("Invaild vlan priority");
	}

	if(vlaninfo->redir_ifindex == 0)
		ebt_print_error("Invaild redirect device");
}

static void print(const struct ebt_u_entry *entry,
   const struct ebt_entry_target *target)
{
	struct ebt_vlan_t_info *vlaninfo =
	   (struct ebt_vlan_t_info *)target->data;

	if(vlaninfo->cmd & OPT_VLAN_SET)
	{
		printf("--vlan-set 0x%x", vlaninfo->vlan);
	}

	if(vlaninfo->cmd & OPT_VLAN_PRIORITY)
	{
		printf("--vlan-priority 0x%x", vlaninfo->priority);
	}

    if(vlaninfo->cmd & OPT_VLAN_HEADER)
    {
        printf(" --vlan-header");
    }

    if(vlaninfo->cmd & OPT_VLAN_UNTAG)
    {
        printf(" --vlan-untag");
    }

	if(vlaninfo->redir_ifindex > 0) {
		char dev[IFNAMSIZ] = {0};

		if_indextoname(vlaninfo->redir_ifindex, dev);
		printf(" --redirect-dev %s", dev);
	}
	printf(" --vlan-target %s", TARGET_NAME(vlaninfo->target));

		return;
}

static int compare(const struct ebt_entry_target *t1,
   const struct ebt_entry_target *t2)
{
	struct ebt_vlan_t_info *vlaninfo1 =
	   (struct ebt_vlan_t_info *)t1->data;
	struct ebt_vlan_t_info *vlaninfo2 =
	   (struct ebt_vlan_t_info *)t2->data;

	return vlaninfo1->target == vlaninfo2->target &&
	   vlaninfo1->vlan == vlaninfo2->vlan;
}

static struct ebt_u_target vlan_target =
{
	.name		= EBT_VLAN_TARGET,
	.size		= sizeof(struct ebt_vlan_t_info),
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
	ebt_register_target(&vlan_target);
}
