/*----------------------------------------------------------------------*
<:copyright-broadcom 
 
 Copyright (c) 2005 Broadcom Corporation 
 All Rights Reserved 
 No portions of this material may be reproduced in any form without the 
 written permission of: 
          Broadcom Corporation 
          16215 Alton Parkway 
          Irvine, California 92619 
 All information contained in this document is Broadcom Corporation 
 company private, proprietary, and trade secret. 
 
:>
 *----------------------------------------------------------------------*
 * File Name  : tr64defs.h
 *
 *----------------------------------------------------------------------*/

#ifndef TR64_DEFS_H
#define TR64_DEFS_H

//#include "bcmcfm.h"

#if defined(__cplusplus)
extern "C" {
#endif
extern void tr64_start(void);
#if defined(__cplusplus)
}
#endif

#include "typedefs.h"

#define TR64_DSLFORUM_SCHEMA      "dslforum-org"
#define TR64_NUMBER_EVENT_QUEUE   40
#define TR64_LAN_INTF_NAME        "br0"
#define TR64_PID_FILE             "/var/run/tr64.pid"
#define TR64_STATE_PERSISTENT_TOKEN   "tr64_state"
#define TR64_DSLF_CONFIG          "dslf-config"
#define TR64_DSLF_RESET           "dslf-reset"
#define TR64_DSLF_RESET_PWD       "admin"
#define TR64_REALM                "IgdAuthentication"
#define TR64_DOMAIN               "/"
//robin, 20090921
#ifdef WEBUI_PLANET
#define TR64_ROOT_FRIENDLY_NAME   "ADN-4000"
#elif DLINK_UPNP
#define TR64_ROOT_FRIENDLY_NAME   "DLink ADSL Router";
#else
#define TR64_ROOT_FRIENDLY_NAME   "TW ADSL Router"
#endif

#define TR64_ROOTDEV_XML_NAME     "devicedesc.xml"

//robin, 20090921
#ifdef WEBUI_PLANET
#define DEV_MFR   	       "PLANET"
#define DEV_MFR_URL            "http://www.planet.com.tw/"
#define DEV_MODEL_DESCRIPTION  "802.11n Wireless ADSL 2/2+ Router"
#define DEV_MODEL              "ADN-4000"
#define DEV_MODEL_NO           "2.0"
#define DEV_MODEL_URL          "http://www.planet.com.tw/"
#elif DLINK_UPNP
#define DEV_MFR   	       "Dlink"
#define DEV_MFR_URL            "http://www.dlink.com/"
#define DEV_MODEL_DESCRIPTION  "Dlink ADSL router"
#define DEV_MODEL              "DSL-2640B"
#define DEV_MODEL_NO           "1.0"
#define DEV_MODEL_URL          "http://www.dlink.com/"
#else
#define DEV_MFR   	       "DSL"
#define DEV_MFR_URL            "http://www.DSL.com/"
#define DEV_MODEL_DESCRIPTION  "single-chip ADSL router"
#define DEV_MODEL              "DSL-963xx"
#define DEV_MODEL_NO           "1.0"
#define DEV_MODEL_URL          "http://www.DSL.com/"
#endif

typedef enum {
   FACTORY,
   NORMAL
} sysPasswordState;

typedef enum {
   IDLE,
   AUTHENTICATING,
   AUTHENTICATED,
   ACCESS_NOT_ALLOW
} authState;

#ifdef UUID_TYPE

/* from draft-mealling-uuid-urn-00.txt */
typedef struct _uuid_t
{
   uint32          time_low;
   uint16          time_mid;
   uint16          time_hi_and_version;
   uint16          clock_seq;
   char            node[6];
} uuid_t;
#else
typedef struct _uuid_t
{
   uint32          time_low;
   uint16          time_mid;
   uint16          time_hi_and_version;
   uint16          clock_seq;
   char            node[6];
} uuid_t;
#endif /* UUID_TYPE */

typedef struct tr64PersistentData
{
   sysPasswordState passwordState;
   char password[64];    
   char provisioningCode[64];
} *pTr64PersistentData, tr64PersistentData;


/* TODO: the following could be shared with TR69 */
typedef enum {
   eNone,
   eDigest,
   eBasic
} eAuthentication;

typedef enum {
   eNoQop,
   eAuth,
   eAuthInt
} eQop;

typedef struct sessionAuth {
   eQop  qopType;
   int      nonceCnt;
   char  *nc;     /* str of nonceCnt */
   char    *nonce;
   char    *orignonce;
   char    *realm;
   char    *domain;
   char    *method;
   char    *cnonce;
   char  *opaque;
   char  *qop;
   char    *user;
   char    *uri;
   char  *algorithm;
   char    *response;
   char    *basic;
   unsigned    char    requestDigest[33];
} sessionAuth;
/* TODO: the above could be shared with TR69 */

/** Macro to push another instance id onto the instance stack. */
#define PUSH_INSTANCE_ID(s, id) \
   do {if ((s)->currentDepth < MAX_MDM_INSTANCE_DEPTH ) { \
         (s)->instance[(s)->currentDepth] = (id);    \
         (s)->currentDepth++; }                       \
   } while(0)

#define TR64C_LOCK_TIMEOUT  (60 * MSECS_IN_SEC)
#endif   // TR64_DEFS_H
