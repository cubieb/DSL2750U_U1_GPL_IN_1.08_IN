/**
 * @file pcp_defs.h
 * @brief wired packet format, is the networking byte-order
 * @author wangjiaming
 * @date 2012-02-01
 */
#ifndef __PCP_DEFS_H_
#define __PCP_DEFS_H_

#include <netdb.h>
#include <netinet/in.h>

/* PCP Port as defined by the draft-ietf-pcp-base-12 */
#define PCP_PORT (44323)

/* 1024 octets is upper boundary, pls refer to line 914*/
#define PCP_MAXIMUM (1024)

#define PCP_DELAY (2)   /* 2 seconds */

#define OPCODE_ANNOUNCE 0
#define OPCODE_MAP 1
#define OPCODE_PEER 2

/*              0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |  Version = 1  |R|   OpCode    |      Reserved (16 bits)       |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |                      Requested Lifetime                       |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |                      Reserved                                 |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |                                                               |
 *             |              PCP Client's IP address (always 128 bits)        |
 *             |                                                               |
 *             |                                                               |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             :                                                               :
 *             :             (optional) opcode-specific information            :
 *             :                                                               :
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             :                                                               :
 *             :             (optional) PCP Options                            :
 *             :                                                               :
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             			Common Request Packet Format
 *
 *
 *              0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |  Version = 1  |R|   OpCode    |   Reserved    |  Result Code  |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |                            Lifetime                           |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |                             Epoch                             |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |                                                               |
 *             |              PCP Client's IP address (always 128 bits)        |
 *             |                                                               |
 *             |                                                               |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             :                                                               :
 *             :             (optional) opcode-specific information            :
 *             :                                                               :
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             :                                                               :
 *             :             (optional) PCP Options                            :
 *             :                                                               :
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             			Common Reponse Packet Format
 *             */ 

/* commom request / response header  */
struct pcphdr { 
	uint8_t version;	/* for now version = 1 */ 

/* #if defined(__LITTLE_ENDIAN_BITFIELD) */
/*       the complier will deal with the big and little endian
 *       uint8_t opcode:7,
 *               rbit:1;
 * #elif defined (__BIG_ENDIAN_BITFIELD) */
	uint8_t rbit:1; 
	uint8_t opcode:7; 

	union {
		uint16_t res; /* for request packet */ 
		struct {
			uint8_t res;
			uint8_t res_code;
		} res_s; /* for response packet */ 
	} res1_u;

	uint32_t lifetime;

	union {
		uint32_t epoch; /* for response packet */ 
		struct in6_addr pcaddr;
	} res2_u;

#define pcp_rescode      res1_u.res_s.res_code
#define pcp_pcaddr      res2_u.pcaddr
#define pcp_epoch        res2_u.epoch
	/*opcode-specific information start here. */
} __attribute__ ((__packed__));

/* 
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  Protocol     |          Reserved (24 bits)                   |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |        Internal Port          |   Suggested External Port     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * :                                                               :
 * : Suggested External IP Address (128bits):
 * :                                                               :
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *       MAP OpCode Request Packet Format
 *  */
struct map_code {
	uint8_t  protol;	/* conform to IANA protocol registry */ 
	uint8_t  res[3];
	uint16_t in_port;
	uint16_t ext_port;
	struct in6_addr addr6;
} __attribute__ ((__packed__));

/*       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |   Protocol    |          Reserved (24 bits)                   |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |        Internal Port          |    Suggested External Port    |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      |           Suggested External IP Address (128 bits)            |
 *      |                                                               |
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |       Remote Peer Port        |     Reserved (16 bits)        |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      |               Remote Peer IP Address (128 bits)               |
 *      |                                                               |
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 *                Figure 11: PEER Opcode Request Packet Format */
struct peer_code {
	uint8_t  protol;	/* conform to IANA protocol registry */ 
	uint8_t  res[3];
	uint16_t in_port;
	uint16_t ext_port;
	struct in6_addr addr6;
	uint16_t rm_port;
	uint16_t res1;
	struct in6_addr rm_addr6;
} __attribute__ ((__packed__));

enum pcp_response/* PCP response code */
{
	NOT_RESPONSE = -1,
	SUCCESS         = 0,
	UNSUPP_VERSION   = 1,
	NOT_AUTHORIZED      = 2,
	MALFORMED_REQUEST    = 3,
	UNSUPP_OPCODE    = 4,
	UNSUPP_OPTION       = 5,
	MALFORMED_OPTION = 6,
	NETWORK_FAILURE = 7,
	NO_RESOURCES  = 8,
	UNSUPP_PROTOCOL  = 9,
	USER_EX_QUOTA    = 10,
	CANNOT_PROVIDE_EXTERNAL  = 11,
	ADDRESS_MISMATCH   = 12,
	EXCESSIVE_REMOTE_PEERS     = 13
};



#endif
