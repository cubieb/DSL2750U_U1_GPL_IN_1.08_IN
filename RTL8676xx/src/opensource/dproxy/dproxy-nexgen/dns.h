#include <asm/byteorder.h>
#include "new_conf.h"

#define NUM_RRS 10
/*****************************************************************************/
struct dns_rr{//
  char name[NAME_SIZE];
  uint16 type;
  uint16 class;
  uint32 ttl;
  uint16 rdatalen;
  char data[NAME_SIZE];
};
/*****************************************************************************/
union header_flags {
  uint16 flags;
  struct {
#if defined(__LITTLE_ENDIAN_BITFIELD)
    unsigned short int rcode:4;
    unsigned short int unused:3;
    unsigned short int recursion_avail:1;
    unsigned short int want_recursion:1;
    unsigned short int truncated:1;
    unsigned short int authorative:1;
    unsigned short int opcode:4;
    unsigned short int question:1;
#elif  defined(__BIG_ENDIAN_BITFIELD)
    unsigned short int question:1;
    unsigned short int opcode:4;
    unsigned short int authorative:1;
    unsigned short int truncated:1;
    unsigned short int want_recursion:1;
    unsigned short int recursion_avail:1;
    unsigned short int unused:3;
    unsigned short int rcode:4;
#else
#error  "Adjust your <asm/byteorder.h> defines"
#endif

  } f;
};

/**************************************/
typedef union {
    struct in_addr in_addr;
    struct in6_addr in6_addr;
} unaddr;

typedef union {
    struct sockaddr sa;
    struct sockaddr_in sa_in;
    struct sockaddr_in6 sa_in6;
} unsockaddr;

/*****************************************************************************/
struct dns_header_s{
  uint16 id;
  union header_flags flags;
  uint16 qdcount;
  uint16 ancount;
  uint16 nscount;
  uint16 arcount;
};
/*****************************************************************************/
struct dns_message{
  struct dns_header_s header;
  struct dns_rr question[NUM_RRS];
  struct dns_rr answer[NUM_RRS];
};
/*****************************************************************************/
typedef struct dns_request_s{
  char cname[NAME_SIZE];
  char ip[64];
  int cache; 
  int ttl;
  int time_pending; /* request age in seconds */

  /* the actual dns request that was recieved */
  struct dns_message message;

  /* where the request came from */
  unaddr src_addr;  
  int protocol;
  int src_port;

  /* the orginal packet */
  char original_buf[MAX_PACKET_SIZE];
  int numread;
  int srcisipv4;
  char *here;

  /* next node in list */
  struct dns_request_s *next;
	/* 
	 * added by lichuang(2009.03.25)
	 * 增加记录LAN信息的数据
	 */
  ST_LAN_INFO* pNode;		/* 对应LAN信息的指针 */
  unsigned char ucIndex;	/* 当前使用的DNS地址的索引, 也就是LAN_INFO中的szDnsIp数组索引 */
}dns_request_t;

/*****************************************************************************/
/* TYPE values */
enum{ A = 1,      /* a host address */
	NS,       /* an authoritative name server */
	MD,       /* a mail destination (Obsolete - use MX) */
	MF,       /* */
	CNAME,    /* the canonical name for an alias */
	SOA,      /* marks the start of a zone of authority  */
	MB,       /* a mailbox domain name (EXPERIMENTAL) */
	MG,       /* */
	MR,       /* */
	NUL,      /* */
	WKS,      /* a well known service description */
	PTR,      /* a domain name pointer */
	HINFO,    /* host information */
	MINFO,    /* mailbox or mail list information */
	MX,       /* mail exchange */
	TXT,      /* text strings */

	AAAA = 0x1c /* IPv6 A */
	};

/* CLASS values */
enum{
  IN = 1,         /* the Internet */
    CS,
    CH,
    HS
};

/* OPCODE values */
enum{
  QUERY,
    IQUERY,
    STATUS
};

/* Response codes */
enum {
	DNS_NO_ERROR,
	DNS_FMT_ERROR,
	DNS_SRVR_FAIL,
	DNS_NAME_ERR,
	DNS_NOT_IMPLEMENTED,
	DNS_REFUSED
};
	
