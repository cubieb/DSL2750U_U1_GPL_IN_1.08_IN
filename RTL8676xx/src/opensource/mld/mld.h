#ifndef _MLD_H_
#define _MLD_H_

#include "mld_inc.h"
#define  MLD_TRACE() printf("##in %s, line:%d\n", __func__, __LINE__);

/******************************************************************************
*                      Definisions
*******************************************************************************/
#define MLDV1_QUERY_SIZE 24 /* Typical MLDv1 query size */
#define MLDV2_QUERY_SIZE 28 /* Typical MLDv2 query size */

#define ICMPV6_MLD_V1V2_QUERY           130
#define ICMPV6_MLD_V1_REPORT            131
#define ICMPV6_MLD_V1_DONE              132
#define ICMPV6_MLD_V2_REPORT		143

/* Node Local Scope */
#define MLD_NL_ALL_NODES               "ff01::1" /* All Nodes in network */
#define MLD_NL_ALL_ROUTERS             "ff01::2" /* All routers in network */

/* Link Local Scope */
#define MLD_LL_ALL_HOSTS               "ff02::1" /* All Nodes in network */
#define MLD_LL_V1_ALL_ROUTERS          "ff02::2" /* All routers in network */
#define MLD_LL_V2_ALL_ROUTERS          "ff02::16" /* All MLDv2 Routers */

/* Site Local Scope */
#define MLD_SL_ALL_ROUTERS             "ff05::2" /* All routers in network */

#define MLD_VERSION_1            1
#define MLD_VERSION_2            2

/* MLD filter definisions */
#define	MLD_FMODE_INCLUDE	 1
#define	MLD_FMODE_EXCLUDE	 0

/* MLD timer definisions */
#define QUERY_TIMEOUT           130
#define	MLD_TIMER_SCALE	        4    /*意味着:该group要去掉*/
#define MLD_DEF_QI	        125
#define MLD_DEF_QRI		10
#define MLD_DEF_RV	       	2
#define MLD_OQPI		((MLD_DEF_RV * MLD_DEF_QI) + MLD_DEF_QRI/2) //255
#define MLD_GMI		((MLD_DEF_RV * MLD_DEF_QI) + MLD_DEF_QRI)      //260
#define	MLD_TIMER_SCALE_SRC	1
#define MLD_TIMER_BACK_COMP    MLD_GMI

/* MLD filter mode definisions */
#define MODE_IS_INCLUDE          1
#define MODE_IS_EXCLUDE          2
#define MODE_TO_INCLUDE          3
#define MODE_TO_EXCLUDE          4
#define MODE_ALLOW_NEW_SRCS      5
#define MODE_BLOCK_OLD_SRCS      6

/******************************************************************************
*                      Structure Definisions
*******************************************************************************/

/* MLDv1 report */
typedef struct _mldv1_report_t 
{
   struct icmp6hdr            icmp6_hdr; /* ICMPv6 header */
   struct in6_addr            grp_addr; /* multicast group addr */
} mldv1_report_t;

typedef mldv1_report_t mldv1_query_t;

/* MLDv2 report record format */
typedef struct _mld_group_record_t 
{
   uint8_t                     mldg_type; /* record type */
   uint8_t                     mldg_datalen; /* auxiliary data len */
   uint16_t                    mldg_numsrc; /* number of sources */
   struct in6_addr             mldg_group; /* multicast group address */
   struct in6_addr             mldg_sources[1]; /* source addresses */
} mld_grp_rec_t;

#define MLD_GROUPREC_HDR_LEN (sizeof(mld_grp_rec_t) - sizeof(struct in6_addr))

/* MLDv2 report */
typedef struct _mldv2_report_t 
{
   uint8_t                     type;
   uint8_t                     code; 
   uint16_t                    cksum;
   uint16_t                    rsvd;
   uint16_t                    mldr_numgrps;  /* number of group records */
   mld_grp_rec_t               mldr_group[1]; /* multicast groups */
} mldv2_report_t;

typedef struct icmp6hdr mld_report_t;

/* MLD Query */
typedef struct _mldv2_query_t 
{
   struct icmp6hdr           icmp6_hdr;
   struct in6_addr           grp_addr;
   uint8_t                   misc;     /* rsvd, suppress & robustness */
   uint8_t                   qqi;   	 /* querier's query interval */
   uint16_t                  num_srcs; /* number of sources */
   struct in6_addr           sources[1];
} mldv2_query_t;

/*******************************相互映射, Begin *******************************
    各个数据结构的指向逻辑: src,  src-rep,  rep, rep-src, src
    
       mld_src_t --> mld_src_rep_t -> mld_rep_t 
            ^                              |
            |<------ mld_rep_src_t <-------|
            
*********************************************************************/
/* MLD source filter type */
typedef struct _mld_src_t 
{
  struct in6_addr         mlds_source;
  int                     mlds_timer;
  /* add a flag to indicate the forwarding state*/	
  int 		              mlds_fstate;
  int                     mlds_fmode;
  struct _mld_src_rep_t*  mlds_rep;
  struct _mld_src_t*      mlds_next;
} mld_src_t;

/* MLD reporter/member type */
typedef struct _mld_rep_t 
{
  unsigned char           mldr_rep_type;   /*实际上是报文类型, 如,ICMPV6_MLD2_REPORT*/
  unsigned char           ucPad[3];
  struct in6_addr         mldr_addr;
  struct _mld_rep_src_t   *mldr_srcs;
  struct _mld_rep_t       *mldr_next;
} mld_rep_t;

/* MLD src reporter */
typedef struct _mld_src_rep_t
{
  mld_rep_t              *mlds_src_rep;
  struct _mld_src_rep_t  *mlds_rep_next;
} mld_src_rep_t;

/* MLD rep source */

typedef struct _mld_rep_src_t 
{
   mld_src_t             *mldr_rep_src;
   struct _mld_rep_src_t *mldr_src_next;
} mld_rep_src_t;
/*************************************相互映射，end*********************/


/* MLD group type */
typedef struct _mld_group_t 
{
  struct in6_addr         mldg_addr;
  int                     mldg_timer;
  int                     mldg_fmode;
  int 		                mldg_version;
  mld_rep_t*              mldg_members;
  mld_src_t*      	      mldg_in_sources;
  mld_src_t*      	      mldg_ex_sources;
  int                     mldg_v1host_prsnt_timer; /*only for V1*/
  struct _mld_group_t*    mldg_next;
} mld_group_t;

/* Scheduled group specifiq query type*/
typedef struct _sch_query_t 
{
  struct in6_addr gp_addr;
  int    mld_retnum;
  int    numsrc;
  struct in6_addr sources[1];
  struct _sch_query_t *sch_next;
} sch_query_t; 

#endif /* _MLD_H_ */
