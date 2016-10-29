#ifndef __LINUX_BRIDGE_EBT_TBSVLAN_H
#define __LINUX_BRIDGE_EBT_TBSVLAN_H

#define EBT_VLAN_ID   0x01
#define EBT_VLAN_PRIO  0x02
#define EBT_VLAN_MASK (EBT_VLAN_ID | EBT_VLAN_PRIO)
#define EBT_VLAN_MATCH "tbsvlan"

struct ebt_tbs_vlan_info {
    uint16_t id;        /* VLAN ID {1-4095} */
    uint8_t prio;       /* VLAN User Priority {0-7} */
    uint8_t bitmask;        /* Args bitmask bit 1=1 - ID arg,
                               bit 2=1 User-Priority arg*/
    uint8_t invflags;       /* Inverse bitmask  bit 1=1 - inversed ID arg, 
                               bit 2=1 - inversed Pirority arg */
};

#endif
