/*  Copyright (C) 1996, 2000 N.M. Maclaren
    Copyright (C) 1996, 2000 The University of Cambridge

This includes all of the code needed to handle Berkeley sockets.  It is way
outside current POSIX, unfortunately.  It should be easy to convert to a system
that uses another mechanism.  It does not currently use socklen_t, because
the only system that the author uses that has it is Linux. */



#include "header.h"
#include "internet.h"
#include <fcntl.h>

#define SOCKET
#include "kludges.h"
#undef SOCKET



/* The code needs to set some variables during the open, for use by later
functions. */

static int initial = 1;
int   descriptors[MAX_SOCKETS];
static struct sockaddr_in here[MAX_SOCKETS], there[MAX_SOCKETS];

#if 0
void display_in_hex (const void *data, int length) {
    int i;

    for (i = 0; i < length; ++i)
        fprintf(stderr,"%.2x",((const unsigned char *)data)[i]);
}
#endif

int valid_socket (int which)
{
    if (which < 0 || which >= MAX_SOCKETS || descriptors[which] < 0)
        return 0;
    else
        return 1;
}

/***********************************************************************/
void open_socket (int which, char *hostname, int timespan) {

	/* Locate the specified NTP server, set up a couple of addresses and open a
	socket. */

    int port, k;
    struct in_addr address, anywhere, everywhere;

	/* Initialise and find out the server and port number.  Note that the port
	number is in network format. */

    /* 第一次调用open_socket时对套接字描述符数组进行初始化 */
	
    if (initial)
    {
		for (k = 0; k < MAX_SOCKETS; ++k) 
			descriptors[k] = -1;
    }
	
    initial = 0;
	
    if (which < 0 || which >= MAX_SOCKETS || descriptors[which] >= 0)
        fatal(0,"socket index out of range or already open",NULL);

    if (verbose > 2) 
		fprintf(stderr,"Looking for the socket addresses\n");
	
    if(0 != find_address(&address,&anywhere,&everywhere,&port,hostname,timespan))
    {
        return;
    }
	
    if (verbose > 2) 
	{
        fprintf(stderr,"Internet address: address=%s, ", inet_ntoa(address));
        fprintf(stderr,"anywhere=%s, ", inet_ntoa(anywhere));
        fprintf(stderr,"everywhere=%s\n", inet_ntoa(everywhere));
    }

	/* Set up our own and the target addresses.  Note that the target address will
	be reset before use in server mode. */

    memset(&here[which],0,sizeof(struct sockaddr_in));
    here[which].sin_family = AF_INET;
    here[which].sin_port = (operation == op_listen || operation == op_server ? port : 0);
    here[which].sin_addr = anywhere;
    
    memset(&there[which],0,sizeof(struct sockaddr_in));
    there[which].sin_family = AF_INET;
    there[which].sin_port = port;
    there[which].sin_addr = (operation == op_broadcast ? everywhere : address);

	if (verbose > 2)
	{
        fprintf(stderr,"Initial sockets: here=%s/%hu, ", inet_ntoa(here[which].sin_addr), ntohs(here[which].sin_port));
        fprintf(stderr,"there=%s/%hu\n", inet_ntoa(there[which].sin_addr), ntohs(there[which].sin_port));
    }

	/* Allocate a local UDP socket and configure it. */

    errno = 0;
	
    if ((descriptors[which] = socket(AF_INET,SOCK_DGRAM,0)) < 0 ||
        bind(descriptors[which], (struct sockaddr *)&here[which], sizeof(here[which])) < 0
       )
        fatal(1,"unable to allocate socket for NTP",NULL);

    if (operation == op_broadcast) 
	{
        errno = 0;
        k = setsockopt(descriptors[which],SOL_SOCKET,SO_BROADCAST,(void *)&k,sizeof(k));

		if (k != 0) 
			fatal(1,"unable to set permission to broadcast",NULL);
    }
}

/***********************************************************************/
void write_socket (int which, void *packet, int length) {

/* Any errors in doing this are fatal - including blocking.  Yes, this leaves a
server vulnerable to a denial of service attack. */

    int k;

    if (which < 0 || which >= MAX_SOCKETS || descriptors[which] < 0)
    {
        fprintf(stderr, "write_socket: socket index out of range or not open\n");
        return;
    }
        
    errno = 0;
    k = sendto(descriptors[which],packet,(size_t)length,0,
            (struct sockaddr *)&there[which],sizeof(there[which]));
    if (k != length) {
        //fatal(1,"unable to send NTP packet",NULL);
        fprintf(stderr, "unable to send NTP packet\n");
        return;
    }
    
    if (verbose > 2)
        fprintf(stderr ,"write_socket: send 1 packet %d bytes to %s/%hu\n", 
                length, inet_ntoa(there[which].sin_addr), ntohs(there[which].sin_port));

    return;
}

/******************************************************************************/
int read_socket (int which, void *packet, int length, int waiting) {

/* Read a packet and return its length or -1 for failure.  Only incorrect
length and timeout are not fatal. */

    struct sockaddr_in scratch, *ptr;
    int n;
    int k;
    struct  timeval to;
	int active_fds;
	fd_set  fds;
    
    if (which < 0 || which >= MAX_SOCKETS || descriptors[which] < 0)
    {
        fprintf(stderr,"read_socket: socket index out of range or not open\n");
        return -1;
	}
	        
	to.tv_sec = waiting;
    to.tv_usec = 0;

    FD_ZERO(&fds);
    FD_SET(descriptors[which],&fds);    

    /* 判断套接字是否可读 */                                                                                                                                                                                
    active_fds = select(descriptors[which]+1, &fds, NULL, NULL, &to);
    if(active_fds < 0)
    {
        fprintf(stderr, "fds < 0.\n");
	    return -1;
    }                                                                                                                                                                               
    if( (to.tv_sec == 0) && (to.tv_usec == 0))
    {
	    printf("time out.\n");
	    return -1;
    }

    /* 如果可读则从套接字中读取数据 */
    if (operation == op_server)
        memcpy(ptr = &there[which],&here[which],sizeof(struct sockaddr_in));
    else
        memcpy(ptr = &scratch,&there[which],sizeof(struct sockaddr_in));
    n = sizeof(struct sockaddr_in); //result-value
    errno = 0;
    k = recvfrom(descriptors[which],packet,(size_t)length,0, (struct sockaddr *)ptr,&n);

    /* Now issue some low-level diagnostics. */

    if (k <= 0) fatal(1,"unable to receive NTP packet from server",NULL);
    if (verbose > 2)
        fprintf(stdout,"read_socket: receive 1 packet %d bytes from %s/%hu\n",
                        k, inet_ntoa(ptr->sin_addr), ntohs(ptr->sin_port));
    
    return k;
}

/*****************************************************************************/
int flush_socket (int which) {

/* Get rid of any outstanding input, because it may have been hanging around
for a while.  Ignore packet length oddities and return the number of packets
skipped. */

    struct sockaddr_in scratch;
    int n;
    char buffer[256];
    int flags, count = 0, total = 0, k;

    if (verbose > 2) fprintf(stdout, "descriptors[%d]=%d\n", which, descriptors[which]);
    if (which < 0 || which >= MAX_SOCKETS || descriptors[which] < 0)
    {
        fprintf(stderr,"flush_socket: socket index out of range or not open\n");
        return 0;
    }
    	
    if (verbose > 2) fprintf(stderr,"Flushing outstanding packets\n");
    errno = 0;
    if ((flags = fcntl(descriptors[which],F_GETFL,0)) < 0 ||
        fcntl(descriptors[which],F_SETFL,flags|O_NONBLOCK) == -1)
    {
        fprintf(stderr, "flush_socket: unable to set non-blocking mode\n");
        return 0;
    }

    while (1) {
        n = sizeof(struct sockaddr_in);
        errno = 0;
        k = recvfrom(descriptors[which],buffer,256,0, (struct sockaddr *)&scratch,&n);
        if (k < 0)
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;

        ++count;
        total += k;
    }
    errno = 0;
    if (fcntl(descriptors[which],F_SETFL,flags) == -1)
    {
        fprintf(stderr, "flush_socket: unable to restore blocking mode\n");
        return 0;
	}
    if (verbose > 2)
        fprintf(stderr,"flush_socket: Flushed %d packets totalling %d bytes\n",count,total);

    return count;
}

/*******************************************/
void close_socket (int which) {

/* There is little point in shielding this with a timeout, because any hangs
are unlikely to be interruptible.  It can get called when the sockets haven't
been opened, so ignore that case. */

    if (which < 0 || which >= MAX_SOCKETS)
        fatal(0,"close_socket: socket index out of range",NULL);
    if (descriptors[which] < 0) return;
    errno = 0;
    if (close(descriptors[which])) fatal(1,"unable to close NTP socket",NULL);
}

