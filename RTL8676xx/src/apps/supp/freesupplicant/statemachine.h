#ifndef _STATEMACHINE_H
#define _STATEMACHINE_H

#include <unistd.h>

struct state_info{
	char init;
	char ifup;
	char supp_state;
	char eap_req;
	char eap_res;
	char eap_suc;
	char eap_fail;
	char start_count;
	char max_start;
	char send_id;
	char receive_id;
	char eapol_ver;
	char md5_challenge;
	struct md5_values *md5_v;
};

// Offsets in to frames where data is located.
#define OFFSET_PAST_MAC       12
#define OFFSET_TO_EAPOL       14
#define OFFSET_TO_EAP         18
#define OFFSET_TO_DATA        23

struct eapol_header{
	unsigned short  ether_type;
	char proto_ver;
	char type;
	unsigned short len;
}__attribute__((__packed__));

//eapol type
#define EAPOL_PACKET 0
#define EAPOL_START 1
#define EAPOL_LOGOFF 2
#define EAPOL_KEY 3
#define EAPOL_ASF 4

struct eap_header{
	char code;
	char identifier;
	unsigned short len;
	char type;
}__attribute__((__packed__));
//eap code
#define EAP_REQ 1
#define EAP_RES 2
#define EAP_SUC 3
#define EAP_FAIL 4

#define EAP_REQUEST_ID     1
#define EAP_REQUEST_AUTH   2
#define EAP_REQUEST_NOTIFY 3

//supp_state
#define INITIALIZE 1
#define AUTHENTICATING 2
#define AUTHENTICATED 3
#define HELD 4

#ifndef TRUE 
#define TRUE 1
#endif

#ifndef FALSE 
#define FALSE 0
#endif

extern struct state_info state;
#endif
