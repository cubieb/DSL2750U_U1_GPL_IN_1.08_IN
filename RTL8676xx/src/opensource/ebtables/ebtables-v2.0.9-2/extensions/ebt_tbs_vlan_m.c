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
#include <linux/netfilter_bridge/ebt_tbs_vlan_m.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>

#define GET_BITMASK(_MASK_) vlaninfo->bitmask & _MASK_
#define SET_BITMASK(_MASK_) vlaninfo->bitmask |= _MASK_
#define INV_FLAG(_inv_flag_) (vlaninfo->invflags & _inv_flag_) ? "! " : ""
#define CHECK_IF_MISSING_VALUE if (optind > argc) ebt_print_error ("Missing %s value", opts[c].name);
#define CHECK_INV_FLAG(_INDEX_) if (ebt_check_inverse (optarg)) vlaninfo->invflags |= _INDEX_;
#define CHECK_RANGE(_RANGE_) if (_RANGE_) ebt_print_error ("Invalid %s range", opts[c].name);

#define NAME_VLAN_ID    "id"
#define NAME_VLAN_PRIO  "prio"

#define VLAN_ID    0
#define VLAN_PRIO  1

static struct option opts[] = {
    {EBT_VLAN_MATCH "-" NAME_VLAN_ID, required_argument, NULL,
     VLAN_ID},
    {EBT_VLAN_MATCH "-" NAME_VLAN_PRIO, required_argument, NULL,
     VLAN_PRIO},
    {NULL}
};

/*
 * option inverse flags definition 
 */
#define OPT_VLAN_ID     0x01
#define OPT_VLAN_PRIO   0x02
#define OPT_VLAN_FLAGS  (OPT_VLAN_ID | OPT_VLAN_PRIO)

static void print_help()
{
#define HELP_TITLE "802.1Q TBS VLAN extension"

    printf(HELP_TITLE " options:\n");
    printf("--" EBT_VLAN_MATCH "-" NAME_VLAN_ID " %s" NAME_VLAN_ID
           " : VLAN-tagged frame identifier,0-4095 (integer), default 1\n",
           OPT_VLAN_FLAGS & OPT_VLAN_ID ? "[!] " : "");
    printf("--" EBT_VLAN_MATCH "-" NAME_VLAN_PRIO " %s" NAME_VLAN_PRIO
           " : Priority-tagged frame user_priority, 0-7 (integer), default 0\n",
           OPT_VLAN_FLAGS & OPT_VLAN_PRIO ? "[!] " : "");
}

static void init(struct ebt_entry_match *match)
{
    struct ebt_tbs_vlan_info *vlaninfo =
        (struct ebt_tbs_vlan_info *) match->data;

    vlaninfo->id = 1;
    vlaninfo->prio = 0;
    vlaninfo->invflags = 0;
    vlaninfo->bitmask = 0;
}

static int 
parse(int c,
      char **argv, int argc,
      const struct ebt_u_entry *entry,
      unsigned int *flags, struct ebt_entry_match **match)
{
    struct ebt_tbs_vlan_info *vlaninfo =
        (struct ebt_tbs_vlan_info *) (*match)->data;
    char *end;
    struct ebt_tbs_vlan_info local;

    switch (c) {
    case VLAN_ID:
        ebt_check_option(flags, OPT_VLAN_ID);
        CHECK_INV_FLAG(EBT_VLAN_ID);
        CHECK_IF_MISSING_VALUE;
        local.id = strtoul(argv[optind - 1], &end, 10);
        CHECK_RANGE(local.id > 4095 || local.id < 1 || *end != '\0');
        vlaninfo->id = local.id;
        SET_BITMASK(EBT_VLAN_ID);
        break;

    case VLAN_PRIO:
        ebt_check_option(flags, OPT_VLAN_PRIO);
        CHECK_INV_FLAG(EBT_VLAN_PRIO);
        CHECK_IF_MISSING_VALUE;
        local.prio = strtoul(argv[optind - 1], &end, 10);
        CHECK_RANGE(local.prio > 7 || *end != '\0');
        vlaninfo->prio = local.prio;
        SET_BITMASK(EBT_VLAN_PRIO);
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
    struct ebt_tbs_vlan_info *vlaninfo =
        (struct ebt_tbs_vlan_info *) match->data;

    if (GET_BITMASK(EBT_VLAN_ID)) {
        printf("--%s %s%d ",
               opts[VLAN_ID].name,
               INV_FLAG(EBT_VLAN_ID), vlaninfo->id);
    }

    if (GET_BITMASK(EBT_VLAN_PRIO)) {
        printf("--%s %s%d ",
               opts[VLAN_PRIO].name,
               INV_FLAG(EBT_VLAN_PRIO), vlaninfo->prio);
    }
}

static int
compare(const struct ebt_entry_match *vlan1,
    const struct ebt_entry_match *vlan2)
{
    struct ebt_tbs_vlan_info *vlaninfo1 =
        (struct ebt_tbs_vlan_info *) vlan1->data;
    struct ebt_tbs_vlan_info *vlaninfo2 =
        (struct ebt_tbs_vlan_info *) vlan2->data;

    if (vlaninfo1->bitmask != vlaninfo2->bitmask)
        return 0;

    if (vlaninfo1->invflags != vlaninfo2->invflags)
        return 0;

    if (vlaninfo1->bitmask & EBT_VLAN_ID) {
        if (vlaninfo1->id != vlaninfo2->id)
            return 0;
    };

    if (vlaninfo1->bitmask & EBT_VLAN_PRIO) {
        if (vlaninfo1->prio != vlaninfo2->prio)
            return 0;
    };

    return 1;
}

static struct ebt_u_match tbs_vlan_match = {
    .name       = EBT_VLAN_MATCH,
    .size       = sizeof(struct ebt_tbs_vlan_info),
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
    ebt_register_match(&tbs_vlan_match);
}

