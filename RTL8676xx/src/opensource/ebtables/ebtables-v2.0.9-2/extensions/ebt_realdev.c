/*
 * Description: 802.1Q Virtual LAN match support module specialy for tbs,
 * in tbs, we add 'vlan' field in struct sk_buff for easily handling packet 
   that it's vlan tag has been stripped by vlan protocol handler(vlan_skb_recv).
 * Enables to match 802.1Q:
 * 1) VLAN-tagged frames by VLAN numeric identifier (12 - bits field)
 * 2) Priority-tagged frames by user_priority (3 bits field)
 * 
 * Authors: XuXiaoDong borrowed heavily from ebt_vlan.c
 *          2009-03-26
 *
 * License: GNU GPL 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include "../include/ebtables_u.h"
#include "../include/ethernetdb.h"
#include <linux/netfilter_bridge/ebt_realdev.h>
#include <linux/if_ether.h>

#define GET_BITMASK(_MASK_) vlaninfo->bitmask & _MASK_
#define SET_BITMASK(_MASK_) vlaninfo->bitmask |= _MASK_
#define INV_FLAG(_inv_flag_) (vlaninfo->invflags & _inv_flag_) ? "! " : ""
#define CHECK_IF_MISSING_VALUE if (optind > argc) print_error ("Missing %s value", opts[c].name);
#define CHECK_INV_FLAG(_INDEX_) if (ebt_check_inverse (optarg)) vlaninfo->invflags |= _INDEX_;
#define CHECK_RANGE(_RANGE_) if (_RANGE_) print_error ("Invalid %s range", opts[c].name);

#define REALDEV_IN    0

static struct option opts[] = {
    {"realdev-in", required_argument, NULL,
     REALDEV_IN},
    {NULL}
};

/*
 * option inverse flags definition 
 */

static void print_help()
{
    printf(
    	"realdev target options:\n"
    	" --realdev-in [!] value     : Set nfmark value\n");
}

static void init(struct ebt_entry_match *match)
{
    struct ebt_realdev_info *info =
        (struct ebt_realdev_info *) match->data;

    memset(info, 0, sizeof(struct ebt_realdev_info));
}

static int 
parse(int c,
      char **argv, int argc,
      const struct ebt_u_entry *entry,
      unsigned int *flags, struct ebt_entry_match **match)
{
    struct ebt_realdev_info *info =
        (struct ebt_realdev_info *) (*match)->data;
    char *end;

    switch (c) {
    case REALDEV_IN:
        if (ebt_check_inverse(optarg))
        {
            info->invert |= EBT_REALDEV_OP_IN;
        }

        sprintf(info->physindev, "%s", argv[optind-1]);

		info->bitmask |= EBT_REALDEV_OP_IN;
		*flags |= EBT_REALDEV_OP_IN;
        break;
    
    default:
        return 0;

    }
    return 1;
}

static void final_check(const struct ebt_u_entry *entry,
            const struct ebt_entry_match *match,
            const char *name, unsigned int hookmask, unsigned int time)
{

}

static void
print(const struct ebt_u_entry *entry, const struct ebt_entry_match *match)
{
}

static int
compare(const struct ebt_entry_match *vlan1,
    const struct ebt_entry_match *vlan2)
{
    struct ebt_realdev_info *info1 =
        (struct ebt_realdev_info *) vlan1->data;
    struct ebt_realdev_info *info2 =
        (struct ebt_realdev_info *) vlan2->data;

    if (strcmp(info1->physindev, info2->physindev))
        return 0;

    return 1;
}

static struct ebt_u_match realdev_match = {
    .name       = "realdev",
    .size       = sizeof(struct ebt_realdev_info),
    .help       = print_help,
    .init       = init,
    .parse      = parse,
    .final_check    = final_check,
    .print      = print,
    .compare    = compare,
    .extra_ops  = opts,
};

void _init(void)
{
    ebt_register_match(&realdev_match);
}

