#ifndef __TCP_H__
#define __TCP_H__
/* Copyright 2000-2002, Texas Instruments, Inc./Telogy Networks */
/*
 * tinytcp.h - header file for tinytcp.c
 *
 * Copyright (C) 1986, IMAGEN Corporation
 *  "This code may be duplicated in whole or in part provided that [1] there
 *   is no commercial gain involved in the duplication, and [2] that this
 *   copyright notice is preserved on all copies.  Any other duplication
 *   requires written notice of the author."
 *
 * Note: the structures herein must guarantee that the
 *       code only performs word fetches, since the
 *       imagenether card doesn't accept byte accesses.
 */

#define TRUE        (1==1)
#define true        (1==1)
#define FALSE       (1==2)
#define false       (1==2)
//#define NULL        0               /* An empty value */
#define NIL         0               /* The distinguished empty pointer */
//#define FTP_SERVER_SUPPORT 1
/* Useful type definitions */
//typedef void (*procref)();

/* Canonically-sized data */
typedef unsigned int longword;      /* 32 bits */
typedef unsigned short word;        /* 16 bits */
typedef char byte;         			/*  8 bits */
typedef byte octet;                 /*  8 bits, for TCP */

extern unsigned int ftpcpufreq;
extern unsigned int ticdiv;
extern ulong get_timer(ulong base);

#define clock_ValueRough() get_timer(0) //(t_get()/ticdiv)

/* protocol address definitions */
typedef longword in_HwAddress;
typedef word eth_HwAddress[3];

/* The Ethernet header */
typedef struct {
    eth_HwAddress   destination __attribute__ ((packed));
    eth_HwAddress   source __attribute__ ((packed));
    word            type;
} eth_Header;

/* The Internet Header: */
typedef struct {
    word            vht;    /* version, hdrlen, tos */
    word            length;
    word            identification;
    word            frag;
    word            ttlProtocol;
    word            checksum;
    in_HwAddress    source __attribute__ ((packed));
    in_HwAddress    destination __attribute__ ((packed));
} in_Header;

#define in_GetHdrlen(ip)  ((wfix((ip)->vht) >> 8) & 0xf)
#define in_GetHdrlenBytes(ip)  ((wfix((ip)->vht) >> 6) & 0x3c)

#define in_GetTTL(ip)      (wfix((ip)->ttlProtocol) >> 8)
#define in_GetProtocol(ip) (wfix((ip)->ttlProtocol) & 0xff)


typedef struct {
    word            srcPort;
    word            dstPort;
    longword        seqnum;
    longword        acknum;
    word            flags;
    word            window;
    word            checksum;
    word            urgentPointer;
} tcp_Header;


#define tcp_FlagFIN     0x0001
#define tcp_FlagSYN     0x0002
#define tcp_FlagRST     0x0004
#define tcp_FlagPUSH    0x0008
#define tcp_FlagACK     0x0010
#define tcp_FlagURG     0x0020
#define tcp_FlagDO      0xF000
#define tcp_GetDataOffset(tp) (wfix((tp)->flags) >> 12)

/* The TCP/UDP Pseudo Header */
typedef struct {
    in_HwAddress    src __attribute__ ((packed));
    in_HwAddress    dst __attribute__ ((packed));
    octet           mbz;
    octet           protocol;
    word            length;
    word            checksum;
} tcp_PseudoHeader;

/*
 * TCP states, from tcp manual.
 * Note: close-wait state is bypassed by automatically closing a connection
 *       when a FIN is received.  This is easy to undo.
 */
enum {
	TCP_STATE_LISTEN		= 0,      /* listening for connection */
	TCP_STATE_SYN_SENT		= 1,      /* syn sent, active open */
	TCP_STATE_SYN_RCVD		= 2,      /* syn received, synack+syn sent. */
	TCP_STATE_ESTABLISHED	= 3,      /* established */
	TCP_STATE_FIN_WAIT_1	= 4,      /* sent FIN */
	TCP_STATE_CLOSE_WAIT	= 5,      /* sent FIN, received FIN (waiting for FINACK) */
	TCP_STATE_FIN_WAIT_2	= 6,      /* sent FIN, received FINACK */
	TCP_STATE_LAST_ACK		= 7,      /* fin received, finack+fin sent */
	TCP_STATE_TIME_WAIT		= 8,      /* dally after sending final FINACK */
	TCP_STATE_CLOSED		= 9       /* finack received */
};

/*
 * TCP Socket definition
 */
#define tcp_MaxData 2048              /* maximum bytes to buffer on output */

typedef struct _tcp_socket {
    struct _tcp_socket *next;
    word          	ip_type;        /* always set to TCP_PROTO */	
    short           state;          /* connection state */
    void        	(*dataHandler)(struct _tcp_socket *, char *, int, int);    /* called with incoming data */
    eth_HwAddress   hisethaddr;     /* ethernet address of peer */
    in_HwAddress    hisaddr;        /* internet address of peer */
    word            myport, hisport;/* tcp ports for this connection */
    longword        acknum, seqnum; /* data ack'd and sequence num */
    int             timeout;        /* timeout, in milliseconds */
    BOOL            unhappy;        /* flag, indicates retransmitting segt's */
    word            flags;          /* tcp flags word for last packet sent */
    short           dataSize;       /* number of bytes of data to send */
    byte            data[tcp_MaxData]; /* data to send */
} tcp_Socket;

extern eth_HwAddress sed_lclEthAddr;
extern eth_HwAddress sed_ethBcastAddr;
extern in_HwAddress  sin_lclINAddr;
extern in_HwAddress  net_Gateway;
extern in_HwAddress  net_IpMask;

/*
 * ARP definitions
 */
#define arp_TypeEther  1        /* ARP type of Ethernet address */

/* harp op codes */
#define ARP_REQUEST 1
#define ARP_REPLY   2

/*
 * Arp header
 */
typedef struct {
    word            hwType;
    word            protType;
    word            hwProtAddrLen;  /* hw and prot addr len */
    word            opcode;
    eth_HwAddress   srcEthAddr __attribute__ ((packed));
    in_HwAddress    srcIPAddr __attribute__ ((packed));
    eth_HwAddress   dstEthAddr __attribute__ ((packed));
    in_HwAddress    dstIPAddr __attribute__ ((packed));
} arp_Header;

/*
 * Ethernet interface:
 *   sed_WaitPacket(0) => ptr to packet (beyond eth header)
 *                          or NIL if no packet ready.
 *   sed_Receive(ptr) - reenables receive on input buffer
 *   sed_FormatPacket(&ethdest, ethtype) => ptr to packet buffer
 *   sed_Send(packet_length) - send the buffer last formatted.
 */
char *sed_IsPacket(void);
char *sed_FormatPacket(register unsigned char *destEAddr, unsigned long ethType);
int sed_Send(int);
int sed_CheckPacket(unsigned short *recBufLocation, unsigned short expectedType);
void sed_shutdown(void);

//extern tcp_Socket ftp_ctl, ftp_data;
#ifdef __BIG_ENDIAN //#ifdef 1 //EB

#define wfix(a)    (a)
#define lfix(a)    (a)
#define wfix4(a) ( ( *((unsigned short *)&(a)) << 16 ) |( *( (unsigned short *)( (unsigned char *)&(a) + 2 ) ) ))
#define lfix4(a) ( ( *((unsigned short *)&(a)) << 16 ) |( *( (unsigned short *)( (unsigned char *)&(a) + 2 ) ) ))

#else

#define wfix(a)    (a)
#define lfix(a)    (a)
#define wfix4(a) ( ( *((unsigned short *)&(a)) << 16 ) |( *( (unsigned short *)( (unsigned char *)&(a) + 2 ) ) ))
#define lfix4(a) ( ( *((unsigned short *)&(a)) << 16 ) |( *( (unsigned short *)( (unsigned char *)&(a) + 2 ) ) ))


#endif

//extern unsigned int rtim;

#define SOPEN  1
#define SDATA  2
#define SCLOSE 3
#define SABORT 4

/* tinytcp.c */
void tbs_tcp( void );
int tcp_Init(void);
void packet_handler(tcp_Socket *s, byte *dp, int len, int state);
void tcp_Close(tcp_Socket *s);
void tcp_Abort(tcp_Socket *s);
void tcp_Retransmitter(void);
void TcpTimeoutCheck(void);
void tcp_Unthread(tcp_Socket *ds);
int tcp(void (*application)(struct _tcp_socket *, unsigned char *, int, int));
void tcp_Listen(tcp_Socket *s, word port, void (*fun)(struct _tcp_socket *, char *, int, int), int timeout);
int tcp_Write(tcp_Socket *s, byte *dp, int len);
void tcp_Flush(tcp_Socket *s);
void tcp_Handler(in_Header *ip);
void tcp_ProcessData(tcp_Socket *s, tcp_Header *tp, int len);
void tcp_Send(tcp_Socket *s);
int tcp_checksum(void *dp, int length);

/* sed.c */
int sed_Init(BOOL useStaticBuffers);

/* arp.c */
int sar_CheckPacket(arp_Header *ap);
int sar_MapIn2Eth(longword ina, eth_HwAddress *ethap);

#endif

