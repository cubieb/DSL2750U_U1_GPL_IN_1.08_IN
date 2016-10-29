#ifndef __HIF_GMAC_H
#define __HIF_GMAC_H

#include <linux/if_ether.h>
#include <adf_os_io.h>

#define ETH_P_ATH               0x88BD
#define ATH_P_MAGBOOT           0x12 /*Magpie is booting*/
#define ATH_P_MAGNORM           0x13 /*Magpie is in default state*/
#define ATH_HLEN                sizeof(struct athhdr)
#define GMAC_HLEN               sizeof(struct gmac_hdr)
/**
 * Change this for handling multiple magpies
 */
#define GMAC_HASH_SHIFT     2 
#define GMAC_TBL_SIZE       (1 << GMAC_HASH_SHIFT)


typedef struct ethhdr     ethhdr_t;

typedef struct athtype{
#if defined (ADF_LITTLE_ENDIAN_MACHINE)
    a_uint8_t    proto:6,
                 res :2;
#elif defined (ADF_BIG_ENDIAN_MACHINE)
    a_uint8_t    res :  2,
                 proto : 6;
#else
#error  "Please fix"
#endif
    a_uint8_t   res_lo;
    a_uint16_t  res_hi;
}__attribute__((packed))  athtype_t;

typedef struct athhdr{
    athtype_t   type;
    a_uint16_t  align_pad;
}athhdr_t;

typedef struct gmac_hdr{
    ethhdr_t    eth;
    athhdr_t    ath;
}gmac_hdr_t;


static inline athhdr_t *
ath_hdr(const struct sk_buff *skb)
{
    return (athhdr_t *)skb->data;
}
static inline a_bool_t
is_ath_header(athhdr_t  *ath, a_uint8_t  sub_type)
{
    return ( ath->type.proto == sub_type);
}

static inline void
ath_put_hdr(struct sk_buff *skb, athhdr_t  *hdr)
{
    athhdr_t  *ath;

    ath  = (athhdr_t *)skb_push(skb, ATH_HLEN);
    ath->type.proto = hdr->type.proto;
}
#endif

