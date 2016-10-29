
#ifndef __ALG_H__
#define __ALG_H__
//#define DEBUG 1

#ifdef DEBUG
#define FW_DEBUG(a...)    printf(a)
#define FW_DEBUG1(a...)    printf(a)
#else
#define FW_DEBUG(a...)
#define FW_DEBUG1(a...)
#endif
#define PROC_MODULE  "/proc/modules"

#define MACROS_NOT_DEFINED
#ifdef MACROS_NOT_DEFINED
typedef int INT32;
typedef char CHAR;
typedef unsigned char UCHAR;
#endif



typedef enum {
            NAT_ALG = 1,
            SIP_ALG,
            AMANDA_ALG,
            H323_ALG,
            IRC_ALG,
            PPTP_ALG,
            /*GRE_ALG,
            ICMP_ALG, */
            SNMP_ALG,
            TFTP_ALG,
//            MMS_ALG,
            NETBIOS_ALG,
            IPSEC_ALG,
            NULL_ALG
        }algs_e;           

typedef struct alg_s
{
    char *pcAlg;
    algs_e iAlgNo;
    char *pcModName;
}alg_t;



static alg_t algs[] = { 
                        { "ftp",NAT_ALG , "ip_nat_ftp"},
                        { "sip",SIP_ALG , "ip_nat_sip"},
                        { "amanda",AMANDA_ALG , "ip_nat_amanda"},
                        { "h323"  ,H323_ALG, "ip_nat_h323"},
                        { "irc" , IRC_ALG , "ip_nat_irc"},
                        { "pptp",PPTP_ALG,"ip_nat_pptp"},
                        /*{ "gre", GRE_ALG,"ip_nat_proto_gre"},
                        { "icmp",ICMP_ALG,"ip_nat_proto_icmp"}, */
                        { "snmp",SNMP_ALG,"ip_nat_snmp_basic"},
                        { "tftp",TFTP_ALG,"ip_nat_tftp"},
 //                       { "mms",MMS_ALG,"ip_nat_mms"},
                        { "netbios",NETBIOS_ALG,"ip_conntrack_netbios_ns"},
                        { "ipsec",IPSEC_ALG,"ip_nat_ipsec"},
                        { 0, NULL_ALG,0}
                      };
#define MAX_ALGS sizeof(algs)/sizeof(alg_t)

#endif
