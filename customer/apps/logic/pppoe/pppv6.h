
#ifndef _PPPV6_H_
#define _PPPV6_H_

#define PPP_KEY_LOCAL_LLA   "LocalLLA"
#define PPP_KEY_REMOTE_LLA  "RemoteLLA"
#define PPP_V6_IS_ONDEMAND  "Ondemand"
#define PPPV6_INFO_STAT_VALID   "Valid"
#define PPPV6_INFO_STAT_INVALID "Invalid"

#define PPPV6_PREFIX_INFO       "IPv6SitePrefixInfo."
#define PPPV6_STR_IPV6_CONFIG   "X_TWSZ-COM_IPv6Config."
#define PPPV6_SITE_PREFIX       "IPv6SitePrefix"
#define PPPV6_PREFIX_STAT_NODE  "PrefixStatus"
#define PPPV6_PREFERRED_LIFETIME_NODE "PreferredLifeTime"
#define PPPV6_VALID_LIFETIME_NODE "ValidLifeTime"
#define PPPV6_TELE_PREFIX       "X_TELEFONICA-ES_IPv6SitePrefix"
#define PPPV6_PREFIX_LEN        "IPv6SitePrefixLength"
#define PPPV6_DNS_SVRS          "IPv6DNSServers"
#define PPPV6_IPV6_ADDR         "IPv6Address"
#define PPPV6_ADDR_PREFIX_LEN   "PrefixLength"
#define PPPV6_ADDR_STATUS       "AddressStatus"

#define PPP_ONDEMAND_GLOBAL  "2001::1234"
#define PPP_ONDEMAND_PREFIX  "2000::"
#define PPP_ONDEMAND_DNSINFO "3000::4567"

enum {
    PROTO_TYPE_UNKNOWN,
    PROTO_TYPE_IPV4,
    PROTO_TYPE_IPV6,
    PROTO_TYPE_IPV4_6
};

extern int PPPOE_GetProtoType(ST_PPPOE_Instance *pstPPPInst, int *piProtoType);
extern int PPPOE_GetLocalLLAdr(void *PPPInstancePara,const char *pszOutStr);
extern int PPPOE_GetRemoteLLAdr(void *PPPInstancePara,const char *pszOutStr);
extern int PPPOE_Ip6cpUpHandler(void *PPPInstancePara, const char *pszOutStr);
extern int PPPOE_Pppv6Terminate(ST_PPPOE_Instance *PPPInstance, int iClearConf);
extern int PPPOE_Ip6cpDownHandler(void *PPPInstancePara, const char *pszOutStr);
extern int PPPOE_Ip6cpRejectHandler(void *PPPInstancePara, const char *pszOutStr);
extern int PPPOE_GetNodeValue(ST_PPPOE_Instance * PPPInstance, const char * pszName, char * pszValue);
extern int PPPOE_SetNodeValue(const ST_PPPOE_Instance * pstInstance, const char * pszName, const char * pszValue);
//extern void PPPOE_InitRetryCnt(ST_PPPOE_Instance *PPPInstance);

#endif /*_PPPV6_H_*/

