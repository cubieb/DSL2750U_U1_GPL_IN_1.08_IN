#include "mld_inc.h"
#include "mld.h"
#include "mld_func.h"

#define _MLD_PATH_PROCNET_IFINET6           "/proc/net/if_inet6"

int log_level;

extern char upstream_interface[10][IFNAMSIZ];

extern int inet_pton(int af, const char *src, void *dst);

void wait_for_interfaces() 
{
  int fd = 0;
  struct ifreq iface;
  int i = 0;
  int try = 0;
    
  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    return;
    
  for (i=0; i<10; i++) {
    if (strlen(upstream_interface[i]) > 1) {
      strncpy(iface.ifr_name, upstream_interface[i], IFNAMSIZ);
        while ((ioctl(fd, SIOCGIFADDR,   &iface) < 0) && (try < 6)) {
          while (sleep(1) > 0)
            try++;
        }  
     }
  } // sleep for 6 sec max.
    
  close(fd);
} /* wait_for_interfaces */

char *
myif_indextoname(int sockfd, unsigned int ifindex,char *ifname) 
{
  struct ifreq ifr;
  int status;

  memset(&ifr,0,sizeof(struct ifreq));
  ifr.ifr_ifindex = ifindex;
	
  status = ioctl(sockfd,SIOCGIFNAME,&ifr);
	
  if (status < 0) {
    /*printf("ifindex %d has no device \n",ifindex);*/
    return NULL;
  }
  else
    return strncpy(ifname,ifr.ifr_name,IFNAMSIZ);
} /* myif_indextoname */

int
upstream_interface_lookup(char *s) 
{

  int i;
	
  for (i=0; i<10; i++) 
    if (strcmp(s,upstream_interface[i]) == 0)
      return 1;
  return 0;
} /* upstream_interface_lookup */

#if 0 
/*目前不被调用，另外所传递的变量与内核处理过程不一致，故不能使用，
其实mld_interface_create中已经使用到该命令字了*/

/*
 * Set/reset the IP_MULTICAST_LOOP. Set/reset is specified by "flag".
 */
void k_set_loop(int socket, int flag) 
{

  u_char loop;

  loop = flag;

  if (setsockopt(socket, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
                                      (char *)&loop, sizeof(loop)) < 0)
    printf("setsockopt IP_MULTICAST_LOOP %u\n", loop);
} /* k_set_loop */

/*
 * Set the IP_MULTICAST_IF option on local interface ifa.
 */
void k_set_if(int socket, struct in6_addr ifa) 
{

  struct in6_addr adr;

  memcpy(&adr, &ifa, sizeof(struct in6_addr));
  if (setsockopt(socket, IPPROTO_IPV6, IPV6_MULTICAST_IF,
                                      (char *)&adr, sizeof(adr)) < 0)
    printf("setsockopt IP_MULTICAST_IF \n");
} /*k_set_if */
#endif

/*
 * void debug(int level)
 *
 * Write to stdout
 */
void debug(int level, const char* fmt, ...) 
{
  va_list args;

  if (level < log_level)
    return;

  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
} /* debug */

/*
 * u_short in_cksum(u_short *addr, int len)
 *
 * Compute the inet checksum
 */
unsigned short in_cksum(unsigned short *addr, int len) 
{

  int nleft = len;
  int sum = 0;
  unsigned short *w = addr;
  unsigned short answer = 0;

  while (nleft > 1) {
    sum += *w++;
    nleft -= 2;
  }

  if (nleft == 1) {
    *(unsigned char*)(&answer) = *(unsigned char*)w;
    sum += answer;
  }

  sum = (sum >> 16) + (sum & 0xffff);
  answer = ~sum;

  return (answer);
} /* in_cksum */

/*
 * interface_list_t* get_interface_list(short flags, short unflags)
 *
 * Get the list of interfaces with an address from family af, and whose flags 
 * match 'flags' and don't match 'unflags'. 
 */
interface_list_t* get_interface_list(short flags, short unflags) 
{

  char *p, buf[IFNAMSIZ];
  interface_list_t *ifp, *ifprev, *list;
  struct ifreq ifr;
  int sockfd6;
  int i, err;
  struct sockaddr_in6 psa6;

  sockfd6 = socket(PF_INET6, SOCK_DGRAM, 0);
  if (sockfd6 <= 0)
    return NULL;

  list = ifp = ifprev = NULL;

  for (i=2; i < MAXM6IFS; i++) {
    p = myif_indextoname(sockfd6, i, buf);
    if (p == NULL) {
      continue;
    }

    /* skip the one user did not enable from webUI */
    if ( upstream_interface_lookup(p) == 0 && strncmp(p,"br0",3) != 0)
      continue;

    strncpy(ifr.ifr_name, p, IFNAMSIZ);

    err = mld_get_ipv6_addr_by_ifidx(i, &psa6);
    if(err < 0)
      puts("error can't get ip addr by idx");

    err = ioctl(sockfd6, SIOCGIFFLAGS, (void*)&ifr);
    if (err == -1) {
      continue;
    }

    if (((ifr.ifr_flags & flags) != flags) ||
        ((ifr.ifr_flags & unflags) != 0)) {
      continue; 
    }

    ifp = (interface_list_t*) malloc(sizeof(*ifp));
    if (ifp) {
      ifp->ifl_index =i;
      strncpy(ifp->ifl_name, ifr.ifr_name, IFNAMSIZ);
      memcpy(&ifp->ifl_addr6, &psa6, sizeof(struct sockaddr_in6));
      ifp->ifl_next = NULL;

      if (list == NULL)
        list = ifp;

      if (ifprev != NULL)
        ifprev->ifl_next = ifp;
        ifprev = ifp;
    }
  }

  close(sockfd6);
  return list;
} /*get_interface_list */

/*
 * void free_interface_list(interface_list_t *ifl)
 *
 * Free a list of interfaces
 */
void free_interface_list(interface_list_t *ifl) 
{
  interface_list_t *ifp = ifl;

  while (ifp) {
    ifl = ifp;
    ifp = ifp->ifl_next;
    free(ifl);
  }
} /* free_interface_list */

/*
 * short get_interface_flags(char *ifname)
 *
 * Get the value of the flags for a certain interface 
 */
short get_interface_flags(char *ifname) 
{

  struct ifreq ifr;
  int sockfd, err;

  sockfd = socket(PF_INET6, SOCK_DGRAM, 0);
  if (sockfd <= 0)
    return -1;

  strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
  err = ioctl(sockfd, SIOCGIFFLAGS, (void*)&ifr);
  close(sockfd);
  if (err == -1)
    return -1;
  return ifr.ifr_flags; 
} /* get_interface_flags */

/*
 * short set_interface_flags(char *ifname, short flags)
 *
 * Set the value of the flags for a certain interface 
 */
short set_interface_flags(char *ifname, short flags) 
{

  struct ifreq ifr;
  int sockfd, err;

  sockfd = socket(PF_INET6, SOCK_DGRAM, 0);

  if (sockfd <= 0)
    return -1;

  strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
  ifr.ifr_flags = flags; 
  err = ioctl(sockfd, SIOCSIFFLAGS, (void*)&ifr);
  close(sockfd);
  if (err == -1)
    return -1;
  return 0; 
} /* set_interface_flags */

/*
 * short get_interface_flags(char *ifname)
 *
 * Get the value of the flags for a certain interface 
 */
int get_interface_mtu(char *ifname) 
{

  struct ifreq ifr;
  int sockfd, err;

  sockfd = socket(PF_INET6, SOCK_DGRAM, 0);
  if (sockfd <= 0)
    return -1;

  strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
  err = ioctl(sockfd, SIOCGIFMTU, (void*)&ifr);
  close(sockfd);
  if (err == -1)
    return -1;
  return ifr.ifr_mtu; 
} /* get_interface_mtu */



int mld_get_ipv6_addr_by_ifidx(int if_index, 
                               struct sockaddr_in6 *saddr6) 
{
  FILE *f;
  char addr6p[8][5];
  char addr6[40], devname[20];
  struct sockaddr_in6 sap;
  int if_idx, scope, plen, dad_status;
  int status = -1;

  if ((f = fopen(_MLD_PATH_PROCNET_IFINET6, "r")) != NULL) {
    while (fscanf(f, "%4s%4s%4s%4s%4s%4s%4s%4s %02x %02x %02x %02x %20s\n",
                      addr6p[0], addr6p[1], addr6p[2], addr6p[3], addr6p[4],
                      addr6p[5], addr6p[6], addr6p[7], &if_idx, &plen, &scope,
                      &dad_status, devname) != EOF) {

      if (if_index == if_idx) {

        sprintf(addr6, "%s:%s:%s:%s:%s:%s:%s:%s",
                        addr6p[0], addr6p[1], addr6p[2], addr6p[3],
                        addr6p[4], addr6p[5], addr6p[6], addr6p[7]);

        inet_pton(AF_INET6, addr6, (struct sockaddr *) &sap.sin6_addr);
        sap.sin6_family = AF_INET6;
        memcpy(saddr6, &sap, sizeof(struct sockaddr_in6));
        status = 0;
        break;
      }
    }
    fclose(f);
  }

  return status;
} /* mld_get_ipv6_addr_by_ifidx */

void mld_print_ipv6_addr(struct in6_addr *addr6) 
{

  int i;

  printf("%04x:", addr6->s6_addr16[0]);
  for(i = 1; i < 8; i++)
    printf(":%04x", addr6->s6_addr16[i]);

 return;
} /* mld_print_ipv6_addr */

#ifdef DEBUG_MLD_PROXY
void dumpBuf(char *buf, int len) 
{
  int i;
  short *buf1 = (short *)buf;

  puts("==================buffer start====================== ");

  for(i = 1; i<= (len/2)+1; i++) {
    printf("%04x ", htons(buf1[i-1]));
    if(i%8 == 0) {
      printf("\n");
    }
  }
  puts("\n==================buffer end  ====================== ");

  return;
} /* dumpBuf */
#endif
