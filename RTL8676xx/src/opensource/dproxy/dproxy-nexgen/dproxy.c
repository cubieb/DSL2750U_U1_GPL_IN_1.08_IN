
// Copyright 2005, Texas Instruments Incorporated
//
// This program has been modified from its original operation by Texas Instruments
// to do the following:
//
// 1. Prevent caching when AAAA query responses are received and don't lookup cache
//    when AAAA queries are received from clients 
// 2. Allow error response with rsp code 1 & 3 to client 
// 3. Cache only if a valid address or name is received in the response. There was
//    a possibility of caching incorrect values in ip and cname variables  if the 
//    response is success, but there was no name or host address in the response
// 4. Use port 53 only for listening to DNS request and to send error response to
//    the clients. Use ephemeral port for forwarding request to server. Also
//    use ephemeral port for listening to responses from DNS server
// 5. NSP Policy Routing Framework
//
// THIS MODIFIED SOFTWARE AND DOCUMENTATION ARE PROVIDED
// "AS IS," AND TEXAS INSTRUMENTS MAKES NO REPRESENTATIONS
// OR WARRENTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO, WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE SOFTWARE OR
// DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS,
// COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.
// See The GNU General Public License for more details.
//
// These changes are covered under version 2 of the GNU General Public License,
// dated June 1991.
//-------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdarg.h>
#include <signal.h>
#include <syslog.h>     
#include <sys/stat.h>
#include <netdb.h>

#include "dproxy.h"
#include "dns_decode.h"
#include "cache.h"
#include "conf.h"
#include "new_conf.h"
#include "dns_list.h"
#include "dns_construct.h"
#include "dns_io.h"
//#include <linux/pr.h>
/*****************************************************************************/
/*****************************************************************************/


#define LOCAL_IPV4_DNS_ADDR  "127.0.0.1"

int dns_main_quit;
int sock_in;
int sock_in_any;
int sock_in6;
int sock_in6_any;
int *sock_fd[MAX_SOCK_FD_NUM] = {NULL};
int fakeDNS;
int protoV6 = 0;//0-不支持IPv6?1-支持Ipv6
int warning;    /*支持中国联通网关异常信息提示功能*/
fd_set rfds;
int maxfd;
ST_LAN_INFO g_defaultLanInfo;
dns_request_t *dns_request_list;
struct stat conf_stat;
/*****************************************************************************/
int is_connected()
{
	return 1;
}
/*****************************************************************************/
#define IPV6_V6ONLY		26

#define DNS_HANDLD_NUM		60

int dns_init()
{
	struct sockaddr_in sa_in;
#ifdef USE_IPV6
	struct sockaddr_in6 sa_in6;
	int i = 0;
#endif
        
	/* Clear it out */
	memset((void *)&sa_in, 0, sizeof(sa_in));
	
#ifdef USE_IPV6
    /*open ipv6 socket*/
    memset((void *)&sa_in6, 0, sizeof(sa_in6));
#define IN6ADDR_ANY_INITITIALIZE { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } } }
	static const struct in6_addr in6addr_any_init = IN6ADDR_ANY_INITITIALIZE;

	sock_in6 = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if( sock_in6 < 0 ){
		debug_perror("Could not create sock_in6 socket\n");
		exit(1);
	}

    /*set socket option*/
    i = 1;
	if (setsockopt(sock_in6, SOL_SOCKET, SO_REUSEADDR,(void*) &i,sizeof(i) ) < 0) {
	    perror( "setsockopt SO_REUSEADDR" );
	    exit(1);
    }

    /*disable ipv4 compatiblily*/
	if (setsockopt(sock_in6, IPPROTO_IPV6, IPV6_V6ONLY, (void*) &i,sizeof(i)) < 0) {
	    perror( "setsockopt IPV6_V6ONLY" );
	    exit(1);
	}
    
	sa_in6.sin6_family = AF_INET6;
	sa_in6.sin6_addr = in6addr_any_init;
	sa_in6.sin6_port = htons(PORT);

	/* bind() the socket to the interface */
	if (bind(sock_in6, (struct sockaddr *)&sa_in6, sizeof(struct sockaddr_in6)) < 0) {
		debug_perror("dns_init: bind: Could not bind ipv6 socket to port 53\n");
		exit(1);
	}

	/* Use ANY port to forward request to the DNS server */
	memset((void *)&sa_in6, 0, sizeof(sa_in6));
	sock_in6_any = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if( sock_in6_any < 0 ){
		debug_perror("Could not create sock_in6_any socket\n");
		exit(1);
	}

	/*set socket option*/
	if (setsockopt(sock_in6_any, SOL_SOCKET, SO_REUSEADDR,(void*) &i,sizeof(i) ) < 0) {	
	    perror( "setsockopt SO_REUSEADDR" );
	    exit(1);
    }

    /*disable ipv4 compatiblily*/
	if (setsockopt(sock_in6_any, IPPROTO_IPV6, IPV6_V6ONLY, (void*) &i,sizeof(i)) < 0) {
	    perror( "setsockopt IPV6_V6ONLY" );
	    exit(1);
	}
	
	sa_in6.sin6_family = AF_INET6;
	sa_in6.sin6_addr = in6addr_any_init;
	sa_in6.sin6_port = 0;

	/* bind() the socket to the interface */
	if (bind(sock_in6_any, (struct sockaddr *)&sa_in6, sizeof(struct sockaddr_in6)) < 0) {
		debug_perror("dns_init: bind: Could not bind ipv6 socket to ANY port\n");
		exit(1);
	}
#endif

    /*open ipv4 socket*/
    sock_in = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if( sock_in < 0 ) {
	    debug_perror("Could not create sock_in socket\n");
	    exit(1);
    }

    sa_in.sin_family = AF_INET;
    sa_in.sin_addr.s_addr = INADDR_ANY;
    sa_in.sin_port = htons(PORT);
  
    /* bind() the socket to the interface */
    if (bind(sock_in, (struct sockaddr *)&sa_in, sizeof(struct sockaddr_in)) < 0){
	    debug_perror("dns_init: bind: Could not bind ipv4 socket to port 53\n");
	    exit(1);
    }

    /* Use ANY port to forward request to the DNS server */
    memset((void *)&sa_in, 0, sizeof(sa_in));
    sock_in_any = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if( sock_in_any < 0 ) {
	    debug_perror("Could not create sock_in_any socket\n");
	    exit(1);
    }

    sa_in.sin_family = AF_INET;
    sa_in.sin_addr.s_addr = INADDR_ANY;
    sa_in.sin_port = 0; /* Use any port */
  
    /* bind() the socket to the interface */
    if (bind(sock_in_any, (struct sockaddr *)&sa_in, sizeof(struct sockaddr_in)) < 0) {
	    debug_perror("dns_init: bind: Could not bind to ipv4 socket to ANY port\n");
	    exit(1);
    }

#if 0
   /* Adding setsockopt to set a mark value for a particular socket. */
  if (setsockopt(dns_any_sock, SOL_IP, POLICY_ROUTING_MARK, (void *)&pr_mark, sizeof(pr_mark)) != 0)
  {
    printf ("Failed to set PR mark for socket %d\n", dns_any_sock);
  }
#endif

    dns_main_quit = 0;
    dns_request_list = NULL;

    sock_fd[0] = &sock_in;
    sock_fd[1] = &sock_in_any;
#ifdef USE_IPV6
    sock_fd[2] = &sock_in6;
    sock_fd[3] = &sock_in6_any;
#endif
    
    /*清空dns 缓存*/
    cache_purge(g_stConfig.nPurgeTime);
    return 1;
}

/*****************************************************************************/
void dns_handle_new_query(dns_request_t *m)
{
    unaddr addr;
    int retval = -1;
    char ipaddr[64] = {0};
	int flag = 0;
    

    if(m->message.question[0].type == A) {
        /* standard query */
        retval = cache_lookup_name( m->cname, m->ip, m->pNode->szCacheFile);
    } else if ( m->message.question[0].type == AAAA ) {
        /* AAAA query*/
        /* This query is not cached */
        if(protoV6 == 0)//IPv6已关闭，所以IPv6的DNS请求返回错误
            retval = 2;
        else
        retval = 0;
        
    } else if( m->message.question[0].type == PTR ) {
        /* reverse lookup */
        retval = cache_lookup_ip( m->ip, m->cname, m->pNode->szCacheFile);
    }

#ifdef CONFIG_TELEFONICA_BRAZIL
    if(fakeDNS==1 && m->srcisipv4 == 1) {
		m->message.question[0].type = A;
		memcpy(m->ip,"192.168.111.222",15);
		retval = 1;
		flag = 1;
    }
#else
	if(fakeDNS==1 && m->protocol == PROTO_IPV4) {
		m->message.question[0].type = A;
		memcpy(m->ip,"192.168.1.222",15);
		retval = 1;
    }
#endif
    debug(".......... %s ---- %s\n", m->cname, m->ip);
 
    switch( retval )
    {
        case 0:
        if ( is_connected() ) {
		    debug("Adding to request list-> id: %d\n", m->message.header.id);
		    dns_request_list = dns_list_add( dns_request_list, m );
		    /*!!! relay the query untouched */
		
            inet_pton((m->protocol == PROTO_IPV4) ? AF_INET : AF_INET6, 
                       m->pNode->szDnsIp[m->protocol][m->ucIndex], (void *)&addr);
				
            debug("Sent Request To %s\n",m->pNode->szDnsIp[m->protocol][m->ucIndex]);
            dns_write_packet((m->protocol == PROTO_IPV4) ? sock_in_any : sock_in6_any,
                              &addr, PORT, m );
        } else {
	        debug("Not connected **\n");
	        dns_construct_error_reply(m);
	        dns_write_packet((m->protocol == PROTO_IPV4) ? sock_in : sock_in6, &m->src_addr, m->src_port, m );
        }
        break;
        case 1:
#ifdef CONFIG_TELEFONICA_BRAZIL
		dns_construct_reply( m,flag );
#else
        dns_construct_reply( m );
#endif
        /*cpeipv4 dns不可用，pc 使用ipv4地址请求，cpe使用ipv6地址请求*/
        if(m->srcisipv4)
        {
                     m->protocol = PROTO_IPV4;
                     dns_write_packet(sock_in,
                                         &m->src_addr, m->src_port, m);
        }
        else
        {
           m->protocol = PROTO_IPV6;
           dns_write_packet(sock_in6, &m->src_addr, m->src_port, m );
        }
        debug("Cache hit -  name %s -ip %s\n", m->cname, m->ip);
        break;

        case 2:
	        debug("AAAA request when IPv4\n");
	        dns_construct_error_reply(m);
	        dns_write_packet((m->protocol == PROTO_IPV4) ? sock_in : sock_in6, &m->src_addr, m->src_port, m );
        break;
            
        default:
        debug("Unknown query type: %d\n", m->message.question[0].type );
    }
}

/*****************************************************************************/
void dns_handle_request(dns_request_t *m)
{
    dns_request_t *ptr = NULL;
    char addr[64] = {0};
	int forward_resp=0;
	
    /* request may be a new query or a answer from the upstream server */
    ptr = dns_list_find_by_id( dns_request_list, m );

  	if( ptr != NULL )
  	{
        debug("Found query in list\n");
        /* message may be a response */
    	if( m->message.header.flags.f.question == 1 )
		{
            /* Added by WGX 2010.08.03:
               Handle the case that there is no answer in response */
            debug("ancount:%d\n", m->message.header.ancount);
            debug("time_pending:%d\n", ptr->time_pending);
            if (m->message.header.ancount == 0 && m->protocol == PROTO_IPV4)
            {
    	        ptr->time_pending++;
    	        if( ptr->time_pending > DNS_TIMEOUT ) {
    	            debug("Request timed out\n");
    	            /* send error back */
    	            dns_construct_error_reply(ptr);
                    dns_write_packet((ptr->protocol == PROTO_IPV4) ? sock_in : sock_in6,
                                      &ptr->src_addr, ptr->src_port, ptr);
    	            
    	            dns_request_list = dns_list_remove(dns_request_list, ptr);
    	        }
    	        
    	        if( /*( ptr->time_pending % DNS_SERVER_TIMEOUT == 0) &&*/( ptr->time_pending < DNS_TIMEOUT) )
    	        {
                    ++ptr->ucIndex;
                    if(have_available_dns(ptr) == false)
                    {
    		  	        /* send error back */
    			        debug("All Servers Tried: No Response\n");
    		  	        dns_construct_error_reply(ptr);
                        dns_write_packet((ptr->protocol == PROTO_IPV4) ? sock_in : sock_in6,
                                         &ptr->src_addr, ptr->src_port, ptr);
                                         
    		  	        dns_request_list = dns_list_remove( dns_request_list, ptr );
    	                debug("Sent a DNS error message to Client \n");
    	            }
    	            else
    	            {
    	                /* Create the new server IP address and relay the query untouched */
                        inet_pton((ptr->protocol == PROTO_IPV4) ? AF_INET : AF_INET6, 
                                   ptr->pNode->szDnsIp[ptr->protocol][ptr->ucIndex], (void *)&addr);	                				    
        	            /*!!!*/
    	                debug("Didn't get a response from last server\n");
                        debug("Sending a new request to %s\n", 
                              ptr->pNode->szDnsIp[ptr->protocol][ptr->ucIndex]);
                        /* Send the new request to the next name server */
    	                dns_write_packet((ptr->protocol == PROTO_IPV4) ? sock_in_any : sock_in6_any,
    	                                  &addr, PORT, ptr );
    	            }                
                }
                
                return;
            }
            /* Add end */
            
			switch( m->message.header.flags.f.rcode )
			{
				case DNS_NO_ERROR: 
				{
					/* Append to the cache if not a AAAA query and cache flag indicates
                       something available for cache 
                    */
					if((m->message.question[0].type != AAAA ) &&
					   (m->cache)) 
					{
						debug("Cache append: %s --->%s, %s\n",
						       m->cname, m->ip, ptr->pNode->szCacheFile);
	                    cache_name_append( m->cname, m->ip, ptr->pNode->szCacheFile);
					}
					forward_resp=1;
					break;
				}
				case DNS_FMT_ERROR:
				{
					forward_resp=1;
					break;
				}
				case DNS_NAME_ERR:
				{
					if(m->message.header.flags.f.authorative)
					{
						forward_resp=1;
					}
					break;
				}
				default:
				/* do nothing */
					break;
			}

			if (m->protocol == PROTO_IPV4) {
                inet_ntop(AF_INET, &m->src_addr.in_addr, addr, sizeof(addr));
            }
            else {
                inet_ntop(AF_INET6, &m->src_addr.in6_addr, addr, sizeof(addr));
            }

            debug("answer from %s\n", addr);
            
			if (forward_resp) {

                 /* pc 使用ipv4地址请求，cpe使用ipv4地址响应*/
                 if(ptr->srcisipv4)
                 {
                     m->protocol = PROTO_IPV4;
                     dns_write_packet(sock_in,
                                         &ptr->src_addr, ptr->src_port, m);
                 }
                 else
                 { 
                    m->protocol = PROTO_IPV6;
                    dns_write_packet(sock_in6,
   				                 &ptr->src_addr, ptr->src_port, m);
                 }
				 
   				debug("Rsp Code: %d - Replying with answer from %s\n",
   				      (int)(m->message.header.flags.f.rcode), addr);
   				dns_request_list = dns_list_remove( dns_request_list, ptr );
			}
			else {
   				debug("Rsp Code: %d - answer from %s dropped\n",
   				      (int)(m->message.header.flags.f.rcode), addr);
			}
    	}
		else {
            debug("Duplicate query\n");
        }
  	}
  	else
	{
		if(m->pNode != NULL)
		{
        	dns_handle_new_query( m );
		}
    }
}

/*****************************************************************************/
int have_available_dns(dns_request_t *dns_req)
{
    if (dns_req->ucIndex > NUM_OF_DNS)
        return false;

    if (dns_req->protocol == PROTO_IPV4)
    {
        if (dns_req->pNode->szDnsIp[PROTO_IPV4][dns_req->ucIndex][0] == '\0')
            return false;
    }
    else
    {
        if (dns_req->pNode->szDnsIp[PROTO_IPV6][dns_req->ucIndex][0] == '\0')
            return false;
    }

    return true;
}

int dns_main_loop()
{
  struct timeval tv;
  int retval;
  dns_request_t m;
  dns_request_t *ptr, *next;
  int purge_time = g_stConfig.nPurgeTime / 60;
  struct stat temp_stat ; 
  unaddr addr;
  int i = 0;
  int dnsHandleCount = 0;
  int dnsIdle = 0;

  while( !dns_main_quit ){
    /* set the one second time out */
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    FD_ZERO( &rfds );
    maxfd = -1;
    memset(&m, 0, sizeof(m));

    for (i = 0; i < MAX_SOCK_FD_NUM; i++) {
        if (sock_fd[i] != NULL) {
            FD_SET(*sock_fd[i],&rfds);
            if (*sock_fd[i] > maxfd)
                maxfd = *sock_fd[i];
        }
    }
    
    retval = select( maxfd+1, &rfds, NULL, NULL, &tv );
    if (retval < 0) {
        if ( errno == EINTR || errno == EAGAIN )
            continue;

        perror( "select" );
        exit( 1 );
    }
    else if (retval > 0) {
        /* data is now available */
	    if (FD_ISSET(sock_in, &rfds)) {
            if (-2 == dns_read_packet(sock_in, &m, PROTO_IPV4, 1)) {
                dns_construct_error_reply(&m);
                dns_write_packet(sock_in, &m.src_addr, m.src_port, &m);
            }
            else {
                dns_handle_request( &m );
            }
        }
        else if (FD_ISSET(sock_in_any, &rfds)) {
            dns_read_packet(sock_in_any, &m, PROTO_IPV4, 0);
            dns_handle_request( &m );
        }
#ifdef USE_IPV6
        else if (FD_ISSET(sock_in6, &rfds)) {
            if (-2 == dns_read_packet(sock_in6, &m, PROTO_IPV6, 1)) {
                dns_construct_error_reply(&m);
                dns_write_packet(sock_in6, &m.src_addr, m.src_port, &m); 
            }
            else {
                dns_handle_request( &m );
            }
        }
        else if (FD_ISSET(sock_in6_any, &rfds)) {
            dns_read_packet(sock_in6_any, &m, PROTO_IPV6, 0);
            dns_handle_request( &m );
        }
#endif
        m.time_pending = 0;

        dnsHandleCount++;
    }
    else 
    {
       dnsIdle = 1;
    }

   /*强制处理一定数量的dns报文后，处理无响应的报文
   * 避免bt下载刚开始时，大量的dns请求导致dproxy进程
   * 占用大量的内存。
   * 强制处理 后，可以保证无效的dns请求保持在3*DNS_HANDLD_NUM
   * 的数量范围内，可以提供dns链表查询效率
   */
   if(dnsIdle || (dnsHandleCount > DNS_HANDLD_NUM))
   {
       if(dnsIdle)
       {
          dnsIdle = 0;
       }

       if(dnsHandleCount > DNS_HANDLD_NUM)
       {
          dnsHandleCount = 0;
       }
       
        /* select time out */
        /* If resolv.conf has changed read it again*/
        if( !stat(g_stConfig.szConfigFile, &temp_stat))
        {
            if( conf_stat.st_ctime != temp_stat.st_ctime)
            {
                load_config(g_stConfig.szConfigFile);
                conf_stat = temp_stat;
            }
        }
        
        ptr = dns_request_list;
        while( ptr ) {
	        next = ptr->next;
	        /* Resend query to a new nameserver if response
	         * has not been received from the current nameserver */
	
	        ptr->time_pending++;
	        if( ptr->time_pending > DNS_TIMEOUT ) {
	            debug("Request timed out\n");
	            /* send error back */
	            dns_construct_error_reply(ptr);
                dns_write_packet((ptr->protocol == PROTO_IPV4) ? sock_in : sock_in6,
                                  &ptr->src_addr, ptr->src_port, ptr);
	            
	            dns_request_list = dns_list_remove(dns_request_list, ptr);
	        }
	        
	        if( ( ptr->time_pending % DNS_SERVER_TIMEOUT == 0) &&( ptr->time_pending < DNS_TIMEOUT) )
	        {
                ++ptr->ucIndex;
                if(have_available_dns(ptr) == false)
                {
		  	        /* send error back */
			        debug("All Servers Tried: No Response\n");
		  	        dns_construct_error_reply(ptr);
                    dns_write_packet((ptr->protocol == PROTO_IPV4) ? sock_in : sock_in6,
                                     &ptr->src_addr, ptr->src_port, ptr);
                                     
		  	        dns_request_list = dns_list_remove( dns_request_list, ptr );
	                debug("Sent a DNS error message to Client \n");
	            }
	            else
	            {
	                /* Create the new server IP address and relay the query untouched */
                    inet_pton((ptr->protocol == PROTO_IPV4) ? AF_INET : AF_INET6, 
                               ptr->pNode->szDnsIp[ptr->protocol][ptr->ucIndex], (void *)&addr);	                				    
    	            /*!!!*/
	                debug("Didn't get a response from last server\n");
                    debug("Sending a new request to %s\n", 
                          ptr->pNode->szDnsIp[ptr->protocol][ptr->ucIndex]);
                    /* Send the new request to the next name server */
	                dns_write_packet((ptr->protocol == PROTO_IPV4) ? sock_in_any : sock_in6_any,
	                                  &addr, PORT, ptr );
	            }
	        }

	        ptr = next;
        }

        /* purge cache */
        purge_time--;
        if( !purge_time ){
            cache_purge(g_stConfig.nPurgeTime);
            purge_time = g_stConfig.nPurgeTime / 60;
        }
    }
  }
  return 0;
}

#if 0
/*****************************************************************************/
void debug_perror( char * msg ) {
	debug( "%s : %s\n" , msg , strerror(errno) );
}
/*****************************************************************************/
void debug(char *fmt, ...)
{
#define MAX_MESG_LEN 1024
  
  va_list args;
  char text[ MAX_MESG_LEN ];
  
  sprintf( text, "[ %d ]: ", getpid());
  va_start (args, fmt);
  vsnprintf( &text[strlen(text)], MAX_MESG_LEN - strlen(text), fmt, args);
  va_end (args);
  
  if( config.debug_file[0] ){
    FILE *fp;
    fp = fopen( config.debug_file, "a");
    if(!fp){
      syslog( LOG_ERR, "could not open log file %m" );
      return;
    }
    fprintf( fp, "%s", text);
    fclose(fp);
  }
  
  /** if not in daemon-mode stderr was not closed, use it. */
  if( ! config.daemon_mode ) {
    fprintf( stderr, "%s", text);
  }
}
#endif

/*****************************************************************************
 * print usage informations to stderr.
 * 
 *****************************************************************************/
void usage(char * program , char * message ) {
  fprintf(stderr,"%s\n" , message );
  fprintf(stderr,"usage : %s [-c <config-file>] [-d] [-h] [-P]\n", program );
  fprintf(stderr,"\t-c <config-file>\tread configuration from <config-file>\n");
  fprintf(stderr,"\t-d \t\trun in debug (=non-daemon) mode.\n");
  fprintf(stderr,"\t-P \t\tprint configuration on stdout and exit.\n");
  fprintf(stderr,"\t-h \t\tthis message.\n");
}
/*****************************************************************************
 * get commandline options.
 * 
 * @return 0 on success, < 0 on error.
 *****************************************************************************/
int get_options( int argc, char ** argv ) 
{
    int c = 0;
    int not_daemon = 0;
    int want_printout = 0;
    char * progname = argv[0];

    while( (c = getopt( argc, argv, "c:dhPf:w:p:")) != -1 ) {
        switch(c) {
            case 'c':
				if (0 > load_config(optarg))
				{
					printf("load_config error\n");
					return -1;
				}
                if(g_stConfig.szConfigFile)
                    stat(g_stConfig.szConfigFile, &conf_stat);
                break;
            case 'd':
                not_daemon = 1;
                break;
            case 'h':
                usage(progname,"");
                return -1;
            case 'P':
                want_printout = 1;
                break;
            case 'p':
            {
				if(memcmp(optarg,"ipv6",4)==0)
                    protoV6 = 1;
            }
                break;
			case 'f':
			{
				if(memcmp(optarg,"1",1)==0)
					fakeDNS = 1;
				else
					fakeDNS = 0;
				printf("Load DNS is Fake mode = %d!!\n",fakeDNS);
	            break;
			}
			/*支持中国联通网关异常信息提示功能*/
			case 'w':
			{
				if(memcmp(optarg,"1",1)==0)
					warning = 1;
				else
					warning = 0;
				
	            break;
			}
            default:
                usage(progname,"");
                return -1;
        }
    }

    /** unset daemon-mode if -d was given. */
    if( not_daemon ) {
		g_stConfig.ucDeamon = 0;
    }

    if( want_printout ) {
        exit(0);
    }
    return 0;
}

/*****************************************************************************
 * get default Dns List.
 * 
 * @return 0 on success, < 0 on error.
 *****************************************************************************/
int load_defaultDns(ST_LAN_INFO *pInfo) 
{			
   FILE *fp_resolv = NULL;
   char resv_buff[256] = {0};
   char resv_dns[64] = {0};
   int ret = -1;
   int i = 0, j = 0;
   debug("come in \n");
   if (!pInfo)
   {
      return -2;
   }
   memset(pInfo, 0, sizeof(ST_LAN_INFO));
   
   strcpy(pInfo->szName, "default");

   snprintf(pInfo->szCacheFile, sizeof(pInfo->szCacheFile)
					,"/var/cache/%s.cache", "default");
   
   pInfo->pNext = NULL;

   pInfo->ipPro = 0;
   
   debug("szCacheFile=%s  \n", pInfo->szCacheFile);
   fp_resolv = fopen("/etc/resolv.conf", "r");
   if(fp_resolv != NULL) 
   {
      while(fgets(resv_buff, sizeof(resv_buff), fp_resolv))
      {
         memset(resv_dns, 0, sizeof(resv_dns));
         ret = sscanf(resv_buff, "nameserver %s", resv_dns);
         debug("..................resv_dns=%s \n", resv_dns);
         if(1 == ret) 
         {

            if (strchr(resv_dns, '.')) 
            {
               /*IPv4*/
               if (i < NUM_OF_DNS)
                  strcpy(pInfo->szDnsIp[PROTO_IPV4][i++], resv_dns);

               if(strcmp(resv_dns, "0.0.0.0"))
               {
                 pInfo->ipPro |= IPV4_ENABLE ;
               }
            }
            else
            {
               if (j < NUM_OF_DNS)
                  strcpy(pInfo->szDnsIp[PROTO_IPV6][j++], resv_dns);
               
               pInfo->ipPro |= IPV6_ENABLE ;
            }
            
            if(i >= NUM_OF_DNS && j >= NUM_OF_DNS)
            {
               break;
            }
         }
      }
   }
   
   fclose(fp_resolv);
   return 0;
}


/*****************************************************************************/
void sig_hup (int signo)
{
  signal(SIGHUP, sig_hup); /* set this for the next sighup */
	load_config(g_stConfig.szConfigFile);
}
/*****************************************************************************/
int main(int argc, char **argv)
{

  /* get commandline options, load config if needed. */
  if(get_options( argc, argv ) < 0 ) {
	  exit(1);
  }

  signal(SIGHUP, sig_hup);

  /*初始化socket配置*/  
  dns_init();

  /*读取默认的dns信息*/
  load_defaultDns(&g_defaultLanInfo);

  if (g_stConfig.ucDeamon) {
    /* Standard fork and background code */
    switch (fork()) {
	 case -1:	/* Oh shit, something went wrong */
		debug_perror("fork\n");
		exit(-1);
	 case 0:	/* Child: close off stdout, stdin and stderr */
		close(0);
		close(1);
		close(2);
		break;
	 default:	/* Parent: Just exit */
		exit(0);
    }
  }

  dns_main_loop();

  return 0;
}


