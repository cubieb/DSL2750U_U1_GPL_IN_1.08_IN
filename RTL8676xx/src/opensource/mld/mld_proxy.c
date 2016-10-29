#include "mld_inc.h"
#include "mld_proxy.h"
#include "mld.h"
#include "mld_func.h"

extern int inet_pton(int af, const char *src, void *dst);
extern uint16_t ntohs(uint16_t netshort);
extern const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt);                             

extern int bcmSystemEx(char *cmd, int printflag);
extern void wait_for_interfaces();

mld_router_t router;
mld_interface_t *downstream;
char upstream_interface[10][IFNAMSIZ];
int go_on = 1;
char* pidfile = "/var/run/mld_pid";
char grpinfo[64]="";
/*version and isquerier variable from the config file*/
int version,querier;
int interface_lo = FALSE;

/* same as sockios.h */


void ipv6Addr2Str(char * pchar, struct in6_addr *addr6)
{
  int i;
  
  sprintf(pchar, "%04x:", addr6->s6_addr16[0]);
  for(i = 1; i < 8; i++)
    sprintf(pchar + 5*i,":%04x", addr6->s6_addr16[i]);

  return;
}


void mld_info_print(mld_router_t *router) 
{
#ifdef DEBUG_MLD_PROXY
  mld_interface_t *ifp = NULL;
  mld_group_t *gp = NULL;
  mld_src_t *src = NULL;
  mld_rep_t *rep = NULL;
  mld_src_rep_t *srep = NULL;
  mld_rep_src_t *rsrc = NULL;
  char buf1[64]={0};


  printf("\n-----------nMLD Table-----------\n");  

  for (ifp = router->mldrt_interfaces; ifp; ifp = (mld_interface_t *)ifp->mldi_next) {
    ipv6Addr2Str(buf1, &ifp->mldi_addr); 
    printf("interface addr:%s, version:%d\n", buf1,ifp->mldi_version);
      
    if (ifp->mldi_groups != NULL) {

      for(gp = ifp->mldi_groups; gp; gp = (mld_group_t*)gp->mldg_next) {
        ipv6Addr2Str(buf1, &gp->mldg_addr); 
        printf("{\n  groups:%s   %11s, gp_timer:%d\n", buf1, 
                  (gp->mldg_fmode == MLD_FMODE_INCLUDE)? "INCLUDE":"EXCLUDE", gp->mldg_timer);

        puts("    INCLUDE Sources:");

        if (gp->mldg_in_sources != NULL)
          for (src=gp->mldg_in_sources;src;src=(mld_src_t *)src->mlds_next) {

            ipv6Addr2Str(buf1, &src->mlds_source);             
            printf("      src:%s, src_timer:%d\n", buf1, src->mlds_timer);
            printf("       rep_src:\n");

            if(src->mlds_rep != NULL)
              for(srep=(mld_src_rep_t *)src->mlds_rep; srep != NULL; 
                             srep=(mld_src_rep_t *)srep->mlds_rep_next)
               {
                ipv6Addr2Str(buf1, &srep->mlds_src_rep->mldr_addr); 
                printf("        %s\n", buf1);
                }                             
          }

        puts("    EXCLUDE Sources:");

        if (gp->mldg_ex_sources != NULL) {
          for (src=gp->mldg_ex_sources;src;src=(mld_src_t *)src->mlds_next) {

          ipv6Addr2Str(buf1, &src->mlds_source);          
          printf("      src:%s, src_timer:%d\n", buf1, src->mlds_timer);
          printf("       rep_src:\n");

          if(src->mlds_rep != NULL)
            for(srep=(mld_src_rep_t *)src->mlds_rep; srep != NULL; 
                                   srep=(mld_src_rep_t *)srep->mlds_rep_next)
              {ipv6Addr2Str(buf1, &srep->mlds_src_rep->mldr_addr); 
               printf("        %s\n", buf1);
               }
          }
        }
        
        puts("    mldg_members:");
        if (gp->mldg_members != NULL) {
          for (rep=gp->mldg_members;rep;rep=(mld_rep_t *)rep->mldr_next) {
            ipv6Addr2Str(buf1, &rep->mldr_addr);             
            printf("      rep_src: %s\n", buf1);
            printf("       src:\n");

            if(rep->mldr_srcs != NULL) {
              for(rsrc=(mld_rep_src_t *)rep->mldr_srcs; rsrc != NULL; 
                                 rsrc=(mld_rep_src_t *)rsrc->mldr_src_next)
               {
               ipv6Addr2Str(buf1, &rsrc->mldr_rep_src->mlds_source); 
               printf("        %s\n", buf1);
              }
                
            }
          }
        }
        else printf("\n");
        
        printf("}\n");
      }
    } 
    else
      printf("\n\n");
  }
#endif

  return;
} /* mld_info_print */

void mld_memberdb_info_print(mld_router_t *router)
{
#ifdef DEBUG_MLD_PROXY
  membership_db*          member_db; 

  for (member_db = router->mldrt_membership_db;member_db;
                      member_db=(membership_db *)member_db->next) {
    mld_print_ipv6_addr(&member_db->membership.group);
    printf("fmode %d numsrc %d \n",
                   member_db->membership.fmode,
                   member_db->membership.numsources);
                   mld_print_ipv6_addr(&member_db->membership.sources[0]);
                   printf("\n");
                   mld_print_ipv6_addr(&member_db->membership.sources[1]);
                   printf("\n");
                   mld_print_ipv6_addr(&member_db->membership.sources[2]);
                   printf("\n");
                   mld_print_ipv6_addr(&member_db->membership.sources[3]);
                   printf("\n");
	}
#endif
  return;
} /* mld_memberdb_info_print */


/******************************************************************************
 *
 * mld interface routines 
 *
 *****************************************************************************/

/*
 * mld_interface_t* mld_interface_create()
 *
 * Create and initialize interface record
 */
mld_interface_t*
mld_interface_create(struct in6_addr * pIfaddr, char *ifname, mifi_t index)
{
  mld_interface_t* ifp;
  struct ipv6_mreq mreq;
  //char ra[4];
  int i;
  short flags;
  int status;
  int ifindex = index;
  struct icmp6_filter filter;

  if (((ifp = (mld_interface_t*) malloc(sizeof(mld_interface_t)))) == NULL)
    return NULL;

  bzero(ifp, sizeof(mld_interface_t));

  /* Allocate a buffer to receive mld messages */ 
  ifp->mldi_bufsize = get_interface_mtu(ifname);

  if (ifp->mldi_bufsize == -1)
    ifp->mldi_bufsize = MAX_MSGBUFSIZE;

  /* XXX Should make buffer slightly bigger for ip header */
  if ((ifp->mldi_buf = (char*) malloc(ifp->mldi_bufsize)) == NULL) {
    free(ifp);
    return NULL;
  }	

  bzero(ifp->mldi_buf, ifp->mldi_bufsize);

  /* Initialize all fields */
  memcpy(&ifp->mldi_addr, pIfaddr, sizeof(struct in6_addr));
  strncpy(ifp->mldi_name, ifname, IFNAMSIZ);
  ifp->mldi_index = index;
  if ( strncmp(ifname,"br0",3) == 0) 
  {
    ifp->mldi_type = DOWNSTREAM;
    ifp->mldi_socket = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);/* Create a raw mld socket */
  }
  else 
  {
    ifp->mldi_type = UPSTREAM;
    ifp->mldi_socket = socket(AF_INET6, SOCK_DGRAM, 0);
  }

  if (ifp->mldi_socket == -1) {
    free(ifp->mldi_buf);
    free(ifp);
    return NULL;
  }

  ifp->mldi_groups = NULL;
  ifp->sch_group_query = NULL;

  ifp->mldi_isquerier = TRUE;
  ifp->mldi_version = MLD_VERSION_1; /*MLD_VERSION_1;*/
  ifp->mldi_qi = MLD_DEF_QI;
  ifp->mldi_qri = MLD_DEF_QRI;
  ifp->mldi_rv = MLD_DEF_RV;
  ifp->mldi_gmi = ifp->mldi_rv * ifp->mldi_qi + ifp->mldi_qri;
  ifp->mldi_ti_qi = 0;
  ifp->mldi_next = NULL;


	/* TODO: Set router alert 
	ra[0] = 148;
	ra[1] = 4;
	ra[2] = 0;
	ra[3] = 0; 

  status = setsockopt(ifp->mldi_socket, IPPROTO_IPV6, IP_OPTIONS, ra, 4);
  if(status < 0)
    perror("Err: IP_OPTIONS"); */

  /* Set reuseaddr, ttl, loopback and set outgoing interface */
  i = 1;

  status = setsockopt(ifp->mldi_socket, SOL_SOCKET, SO_REUSEADDR, 
                                                    (void*)&i, sizeof(i));
  if(status < 0)
    perror("Err: SO_REUSEADDR");

  i = 1;
  status = setsockopt(ifp->mldi_socket, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, 
                                                       (void*)&i, sizeof(i));
  if(status < 0)
    perror("Err: IPV6_MULTICAST_HOPS");

  i = 0;
  status = setsockopt(ifp->mldi_socket, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, 
                                                        (void*)&i, sizeof(i));
  if(status < 0)
    perror("Err: IPV6_MULTICAST_LOOP");

  status = setsockopt(ifp->mldi_socket, IPPROTO_IPV6, IPV6_MULTICAST_IF,
                                                   &ifindex, sizeof(ifindex));
  if(status < 0)
    perror("Err: IPV6_MULTICAST_IF");

  i = 1;
  status = setsockopt(ifp->mldi_socket, IPPROTO_IPV6, IPV6_PKTINFO, 
                                                            &i, sizeof(i));
  if(status < 0)
    perror("Err: IPV6_PKTINFO");

  if ( ifp->mldi_type == DOWNSTREAM ) {
    /* Add membership for MLD_LL_V1_ALL_ROUTERS */
    if((inet_pton(AF_INET6, MLD_LL_V1_ALL_ROUTERS, 
                                          &mreq.ipv6mr_multiaddr)) == -1) {
      perror("can't create mcast address \n");
    }

    mreq.ipv6mr_ifindex = ifp->mldi_index;

    if( setsockopt(ifp->mldi_socket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, 
                          (char *)&mreq, sizeof(struct ipv6_mreq) ) == -1 ) {
      perror("can't join mcast group \n");
    }

    /* Add membership for MLD_LL_V2_ALL_ROUTERS */
    if((inet_pton(AF_INET6, MLD_LL_V2_ALL_ROUTERS, 
                                           &mreq.ipv6mr_multiaddr)) == -1) {
      perror("can't create mcast address \n");
    }

    mreq.ipv6mr_ifindex = ifp->mldi_index;

    if( setsockopt(ifp->mldi_socket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, 
                          (char *)&mreq, sizeof(struct ipv6_mreq) ) == -1 ) {
      perror("can't join mcast group \n");
    }

    /* ICMP filter to receive only MLD messages */
    ICMPV6_FILTER_SETBLOCKALL( &filter );
    ICMPV6_FILTER_SETPASS( ICMPV6_MGM_QUERY, &filter );	
    ICMPV6_FILTER_SETPASS( ICMPV6_MGM_REPORT, &filter );
    ICMPV6_FILTER_SETPASS( ICMPV6_MGM_REDUCTION, &filter );
    ICMPV6_FILTER_SETPASS( ICMPV6_MLD2_REPORT, &filter );
	
    status = setsockopt(ifp->mldi_socket, IPPROTO_ICMPV6, 
                        ICMPV6_FILTER, &filter, sizeof(filter));

    if(status < 0) 
      perror("Err: ICMPV6_FILTER");
    
    /* Tell the kernel this interface belongs to a multicast router */
    k_init_proxy(ifp->mldi_socket);
  }


  /* filter to recieve only ICMP messages */
  mld_info_print(&router);
  mld_memberdb_info_print(&router);
  
	
  /* Set the interface flags to receive all multicast packets */
  ifp->mldi_save_flags = get_interface_flags(ifname);
  if (ifp->mldi_save_flags != -1) {
    set_interface_flags(ifname, ifp->mldi_save_flags | IFF_ALLMULTI);
    /* If IFF_ALLMULTI didn't work, try IFF_PROMISC */
    flags = get_interface_flags(ifname);
    if (((flags & IFF_ALLMULTI) != IFF_ALLMULTI)) 
      set_interface_flags(ifname, ifp->mldi_save_flags | IFF_PROMISC);
  }

  return ifp;
} /* mld_interface_create */

/*
 * void mld_interface_cleanup()
 *
 * Cleanup an interface 
 */
void
mld_interface_cleanup(mld_interface_t* ifp)
{
  mld_group_t* gp,*gp2;
  mld_src_t *src = NULL;

  assert(ifp != NULL);

  /* Cleanup all groups */
  gp=ifp->mldi_groups;
  ifp->mldi_groups = NULL;

  while(gp != NULL) {
    gp2=gp;
    gp=gp->mldg_next;

    /*tell kernel to cleanup group*/
    if ( ifp->mldi_type == DOWNSTREAM){    
      for(src = gp2->mldg_in_sources; src; src=src->mlds_next) {
          k_proxy_del_mfc (ifp->mldi_socket, &src->mlds_source, &gp2->mldg_addr);
      }
      for(src = gp2->mldg_ex_sources; src; src=src->mlds_next) {
          k_proxy_del_mfc (ifp->mldi_socket, &src->mlds_source, &gp2->mldg_addr);
      }
    }   
    
    free(gp2);
  }

  k_proxy_del_mif(ifp->mldi_socket, ifp->mldi_index);
  
  /* Tell the kernel the multicast router is no more */
  if ( ifp->mldi_type == DOWNSTREAM)
    k_stop_proxy(ifp->mldi_socket);
  
  close(ifp->mldi_socket);

  free(ifp->mldi_buf);

  /* If we managed to get/set the interface flags, revert */
  if (ifp->mldi_save_flags != -1)
    set_interface_flags(ifp->mldi_name, ifp->mldi_save_flags);
  
  
  free(ifp);
} /* mld_interface_cleanup */

/*
 * mld_group_t* mld_interface_group_add()
 *
 * Add a group to the set of groups of an interface
 */
mld_group_t*
mld_interface_group_add(mld_router_t* router, mld_interface_t *ifp, 
                                                 struct in6_addr * pGroupaddr)
{
  mld_group_t* gp;
  struct ipv6_mreq mreq;

  assert(ifp != NULL);

  /* Return the group if it's already present */
  if ((gp = mld_interface_group_lookup(ifp, pGroupaddr)))
    return gp;

  /* Create the group and add to the set */ 
  if ((gp = mld_group_create(pGroupaddr))) {
    gp->mldg_next = ifp->mldi_groups;
    ifp->mldi_groups = gp;

    /* adding to all upstream interfaces */
    for (ifp = router->mldrt_interfaces; ifp; ifp = ifp->mldi_next) {
      /*add group to the set of groups of upstream interface*/
      if (ifp->mldi_type == UPSTREAM && (VALID_ADDR(pGroupaddr))) {
        memcpy(&mreq.ipv6mr_multiaddr, pGroupaddr, sizeof(struct in6_addr));
        mreq.ipv6mr_ifindex =ifp->mldi_index;

        /* upstream_interface=ifp, no need to look it up by ifp->mldi_addr*/
        /*upstream_interface = mldrt_interface_lookup(router,&ifp->mldi_addr);*/
        if (mld_interface_group_lookup(ifp, pGroupaddr) == NULL) {
          if (setsockopt(router->mldrt_up_socket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, 
                            (void *) &mreq,  sizeof(struct ipv6_mreq)) < 0) { 
              perror("IP_ADD_MEMBERSHIP failed");
            }
          }
       } 
    } 
  } 

  return gp;
} /* mld_interface_group_add */

/*
 * mld_group_t* mld_interface_group_lookup()
 *
 * Lookup a group in the grouptable of an interface
 */
mld_group_t*
mld_interface_group_lookup(mld_interface_t *ifp, struct in6_addr * pGroupaddr)
{
  mld_group_t* gp;

  assert(ifp != NULL);

  for (gp = ifp->mldi_groups; gp; gp = gp->mldg_next)
    if (IN6_ARE_ADDR_EQUAL(gp->mldg_addr.s6_addr32, pGroupaddr->s6_addr32)) {
      return gp;
    }

  return NULL; 
} /* mld_interface_group_lookup */

/*
 * void mld_interface_membership_report_v1()
 *
 * Process a v1 membership report
 */
void
mld_interface_membership_report_v1(mld_router_t* router, 
                                   mld_interface_t* ifp,
                                   struct in6_addr * pSrc, 
                                   mldv1_report_t * report,
                                   int len, 
                                   char *gpinfo) 
{
  mld_group_t* gp;
  mld_rep_t *rep;
  //char cmd[128];

  /* Ignore a report for a non-multicast address */ 
  if (! IN6_IS_ADDR_MULTICAST((report->grp_addr.s6_addr32)))
    return;

  /* Find the group, and if not present, add it */
  if ((gp = mld_interface_group_add(router,ifp, &report->grp_addr)) == NULL)
    return;
        
  /* set the v1 host timer preset */
  gp->mldg_v1host_prsnt_timer = 
               ((report->icmp6_hdr.icmp6_type== ICMPV6_MLD_V1_REPORT ) ? \
                                    MLD_TIMER_BACK_COMP : 0);

  /* find the member and add it if not present*/
  rep=mld_group_rep_add(gp,pSrc);
  rep->mldr_rep_type = report->icmp6_hdr.icmp6_type;

  /* Consider this to be a v2 is_ex{} report */
  mld_group_handle_isex(router,ifp, gp, 0, NULL, rep, gpinfo);

} /* mld_interface_membership_report_v1 */

void
mld_interface_membership_leave_v1(mld_router_t* router,   
                                  mld_interface_t* ifp,
                                  struct in6_addr * pSrc,
                                  mldv1_report_t * report,
                                  int len)
{
  mld_group_t* gp;
  mld_rep_t *rep;
  mld_interface_t* ifi;
  struct ipv6_mreq mreq;
  int count=0;
  int needdrop=0;
  mld_src_t *gp_src = NULL;
  struct in6_addr saddr;
  //membership_db member;

  if (! IN6_MULTICAST(&report->grp_addr)) {
    printf("Ignore non-multicast ...\n");
    return;
  }

  for (ifi = router->mldrt_interfaces; ifi ; ifi = (mld_interface_t *)ifi->mldi_next) {
    if ( ifi->mldi_type == DOWNSTREAM) {
      for(gp = ifi->mldi_groups;gp;gp = gp->mldg_next) {

        if(IN6_ARE_ADDR_EQUAL(gp->mldg_addr.s6_addr32, report->grp_addr.s6_addr32)) {
          rep = mld_group_rep_lookup(gp, pSrc);

          if (rep) {

            bzero(&saddr, sizeof(struct in6_addr));
            gp_src = mld_group_src_lookup(gp, &saddr, MLD_FMODE_EXCLUDE);

              if(gp_src) {
                mld_src_rep_cleanup(gp_src, rep);
                mld_rep_src_cleanup(rep, gp_src);

                if(mld_rep_src_count(rep) == 0) {

                  mld_rep_cleanup(gp, rep);
                  mld_update_snooping_info(gp, SNOOP_EX_CLEAR, grpinfo, &saddr);
                  count = 0;

                  for(rep = gp->mldg_members; rep != NULL; 
                                             rep = rep->mldr_next) {
                    if(rep->mldr_rep_type != ICMPV6_MLD2_REPORT) {
                      count++; 
                    }
                  }

                  if(count == 0) {
                    gp->mldg_v1host_prsnt_timer = 0;
                  }
                }

                if(mld_src_rep_count(gp_src) == 0) { 

                  mld_src_cleanup(gp, gp_src, MLD_FMODE_EXCLUDE);
                  k_proxy_del_mfc (router->mldrt_socket, &saddr, &gp->mldg_addr);

                  if(!gp->mldg_in_sources && !gp->mldg_ex_sources) {
                    gp->mldg_timer = MLD_TIMER_SCALE;
                    mld_group_cleanup(ifi,gp, router);
                    needdrop=1;
                  }
                  else {
                    if(ifp->mldi_type != UPSTREAM) {

                      //update_multi(router,gp, &member, rep);       

                      if(gp->mldg_v1host_prsnt_timer > 0) {

                        set_source_filter(router, ifp, gp, 
                                       MLD_FMODE_EXCLUDE,0,NULL);
                      }
                      else {
                        printf("error data flow!!!\n");
                        /*这是V2版本处理流程

                        set_source_filter(router, ifp, gp, 
                                            member.membership.fmode,
                                            member.membership.numsources,
                                            member.membership.sources);
                                            */
                      }
                    }
                  }
                } 
              }
            }
         }
      } /*for groups */
    } /*DOWNSTREAM */
  } /*for interfaces*/

  if ( needdrop ) {

    for (ifi = router->mldrt_interfaces; ifi ; ifi = (mld_interface_t *)ifi->mldi_next) {

      if ( ifi->mldi_type == UPSTREAM ) {
        memcpy(&mreq.ipv6mr_multiaddr, &report->grp_addr, sizeof(struct in6_addr));
        mreq.ipv6mr_ifindex=ifi->mldi_index;

        if (VALID_ADDR(mreq.ipv6mr_multiaddr.s6_addr32)) {
          if (setsockopt(router->mldrt_up_socket, IPPROTO_IPV6, 
                              IPV6_DROP_MEMBERSHIP,
                              (void *) &mreq, sizeof(mreq)) < 0) {
             perror("IP_drop_MEMBERSHIP failed -1");
          } 
        }
      }
    }
  } /* needdrop */

  return;
} /* mld_interface_membership_leave_v1 */

void
mld_interface_membership_report_v2(mld_router_t* router,
                                   mld_interface_t* ifp,
                                   struct in6_addr * pSrc,
                                   mldv2_report_t* report,
                                   int len, 
                                   char *gpinfo) 
{
  mld_group_t* gp;
  mld_rep_t *rep;
  u_short numsrc;
  u_short numgrps;
  u_char type;
  u_short i;
  mld_grp_rec_t *grp_rec = NULL;

  numgrps = ntohs(report->mldr_numgrps);
  grp_rec = report->mldr_group;

  for (i=0; i < numgrps; i++) {

    /* Ignore a report for a non-multicast address */ 
    if (! IN6_MULTICAST(grp_rec->mldg_group.s6_addr32))
      return;

    /* Find the group, and if not present, add it */
    if ((gp = mld_interface_group_add(router, ifp, &grp_rec->mldg_group)) == NULL)
      return;

    /* find the source of the report and add it if not present*/
    rep=mld_group_rep_add(gp,pSrc);    
    rep->mldr_rep_type = report->type;

    /* Find the group record type */
    type   = (u_char)grp_rec->mldg_type;
    numsrc = ntohs(grp_rec->mldg_numsrc);

    switch(type) {
      case MODE_IS_INCLUDE: /* an IS_IN report */
        mld_group_handle_isin(router, ifp, gp, numsrc, 
                            (struct in6_addr *) &grp_rec->mldg_sources, 
                            rep, gpinfo);
        break;

      case MODE_IS_EXCLUDE: /* an IS_EX report */
        mld_group_handle_isex(router, ifp, gp, numsrc, 
                              (struct in6_addr *) &grp_rec->mldg_sources, 
                              rep, gpinfo);
        break;

      case MODE_TO_INCLUDE: /* an TO_IN report */
        mld_group_handle_toin(router, ifp, gp, numsrc,
                              (struct in6_addr *)&grp_rec->mldg_sources, 
                              rep, gpinfo);
        break;

      case MODE_TO_EXCLUDE: /* an TO_EX report */
        mld_group_handle_toex(router, ifp, gp, numsrc,
                              (struct in6_addr *)&grp_rec->mldg_sources, 
                              rep, gpinfo);
        break;

      case MODE_ALLOW_NEW_SRCS: /* an ALLOW report */
        mld_group_handle_allow(router, ifp, gp, numsrc, 
                               (struct in6_addr *)&grp_rec->mldg_sources, 
                               rep, gpinfo);
        break;

      case MODE_BLOCK_OLD_SRCS: /* an BLOCK report */
        mld_group_handle_block(router, ifp, gp, numsrc,
                               (struct in6_addr *)&grp_rec->mldg_sources, 
                               rep, gpinfo);
        break;

      default:
        printf("default type\n"); 
    }

    grp_rec = (mld_grp_rec_t *)((u_char *)grp_rec 
                                + (MLD_GROUPREC_HDR_LEN 
                                 + (numsrc * sizeof(struct in6_addr))));
  }

  return;
} /* mld_interface_membership_report_v2 */


/******************************************************************************
 *
 * mld router routines
 *
 *****************************************************************************/

/*
 * int mldrt_init()
 *
 * Initialize mld router
 */
int
mldrt_init(mld_router_t* mldrt) 
{
  mldrt->mldrt_interfaces = NULL;
  mldrt->mldrt_thr_timer = 0;
  mldrt->mldrt_thr_input = 0;
  mldrt->mldrt_flag_timer = 0;
  mldrt->mldrt_flag_input = 0;

  /*
   * create datagram socket to tell mld host about reports to send on upstream 
   * interface
   */
  mldrt->mldrt_up_socket = socket( AF_INET6, SOCK_DGRAM, 0 );

  if( mldrt->mldrt_up_socket < 0) {
    perror("can't open upstream socket");
  }

  /*create raw socket to update mfc and vif table*/
  mldrt->mldrt_socket = socket(AF_INET6, SOCK_DGRAM, 0);
  if (mldrt->mldrt_socket < 0) 
    perror("can't open mld socket");

  return 1;
} /* mldrt_init */

/*
 * void mldrt_destroy()
 *
 * Cleanup the router 
 */
static void
mldrt_destroy(mld_router_t* mldrt)
{
  mld_interface_t *ifp, *ifp2;

  for (ifp = mldrt->mldrt_interfaces; ifp;) {
    ifp2 = ifp;
    ifp = ifp->mldi_next;
    mld_interface_cleanup(ifp2);
  }

  LOG((LOG_DETAIL, "destroy mld router ...\n"));
  mldrt_stop(mldrt);
	
} /* mldrt_destroy */

/*
 * interface_t* mldrt_interface_lookup()
 *
 * Lookup a group, identified by the interface address
 */
static mld_interface_t*
mldrt_interface_lookup(mld_router_t* mldrt, struct in6_addr * pIfaddr)
{
  mld_interface_t *ifp;

  for (ifp = mldrt->mldrt_interfaces; ifp; ifp = (mld_interface_t *)ifp->mldi_next) {

    if(IN6_ARE_ADDR_EQUAL(ifp->mldi_addr.s6_addr32, pIfaddr->s6_addr32)) {
      return ifp;
    }
  }

  return NULL;
} /* mldrt_interface_lookup */

/*
 * interface_t* mldrt_interface_lookup_index()
 *
 * Lookup a group, identified by the interface index
 */
static mld_interface_t*
mldrt_interface_lookup_index(mld_router_t* mldrt, int ifp_index) 
{

  mld_interface_t *ifp;


  for (ifp = mldrt->mldrt_interfaces; ifp; ifp = (mld_interface_t *)ifp->mldi_next)
    {
    if (ifp->mldi_index == ifp_index)
        {
          return ifp;
        }
    }

  return NULL;
} /* mldrt_interface_lookup_index */

/*
 * struct mld_interface_t* mldrt_interface_add()
 *
 * Add an interface to the interfacetable
 */
mld_interface_t*
mldrt_interface_add(mld_router_t* mldrt, 
                    struct in6_addr * pIfaddr, 
                    char *ifname,
                    mifi_t index) 
{

  mld_interface_t *ifp;

  /* Return the interface if it's already in the set */
  if ((ifp = mldrt_interface_lookup(mldrt, pIfaddr))) {
    return ifp;
  }

  /* Create the interface and add to the set */
  if ((ifp = mld_interface_create(pIfaddr, ifname,index))) {
    ifp->mldi_next = mldrt->mldrt_interfaces;
    mldrt->mldrt_interfaces = ifp;
  }

  return ifp;
} /* mldrt_interface_add */


/*
 * void mldrt_timergroup(mld_router_t* mldrt)
 *
 * Decrement timers and handle whatever has to be done when one expires
 */
void
mldrt_timer(mld_router_t* mldrt) 
{

  mld_interface_t* ifp;
  struct in6_addr zero;
  
  memset(&zero, 0, sizeof(struct in6_addr));
  
  /* Handle every interface */
  for (ifp = mldrt->mldrt_interfaces; ifp; ifp = (mld_interface_t *)ifp->mldi_next) {
    /* If we're the querier for this network, handle all querier 
     * duties */
    if ( ifp->mldi_type == DOWNSTREAM ) {
      if (ifp->mldi_isquerier == TRUE) {
        /* Deal with the general query */
        if (--ifp->mldi_ti_qi <= 0) {
          ifp->mldi_ti_qi = ifp->mldi_qi;
          mldrt_membership_query(mldrt, ifp, &zero, NULL, 0, 0);
        }
      } 
      else {
        /* If not the querier, deal with other-querier-present timer*/
        mldrt_timer_querier(ifp);	
      }
      
      /*handle source timer*/
      mldrt_timer_source(mldrt,ifp);

      /*handle group timer*/
      mldrt_timer_group(mldrt,ifp);     

      /*handle scheduled query*/
      send_sh_query(mldrt,ifp);
    }
  }
} /* mldrt_timer */

void*
mldrt_timer_thread(void* arg) 
{

  mld_router_t* mldrt = (mld_router_t*) arg;

  if (mldrt->mldrt_flag_timer) {
    mldrt_timer(mldrt);
  }

  usleep(1000000);

  return NULL;
} /* mldrt_timer_thread */

/*
 * void mldrt_input()
 *
 * Process an incoming mld message
 */
void
mldrt_input(mld_router_t* mldrt, 
            mld_interface_t* ifp) 
{

  struct msghdr msg;
  struct iovec iov;
  struct cmsghdr *cmsg;
  char *ctrl = NULL;
  int if_index = 0;
  struct in6_pktinfo *info = NULL;
  int n;
  int badmsg=0;
  mld_interface_t *ifpi;
  struct sockaddr_in6 sa;
  char src_addr[INET6_ADDRSTRLEN];
  char grp_addr[INET6_ADDRSTRLEN];
  struct icmp6hdr *icmp;
  mldv2_query_t *mld_qry;
  mldv1_report_t *mldv1;
  mldv2_report_t *mldv2;
  char *offset;
  //char cmd[128];


  ctrl = (char *)malloc(MAXCTRLSIZE);

  /* Read the mld message */
  memset( &iov, 0, sizeof(struct iovec) );
  iov.iov_base = (char *)ifp->mldi_buf;
  iov.iov_len = ifp->mldi_bufsize;

  memset(&msg, 0, sizeof(struct msghdr) );
  msg.msg_iov = &iov;
  msg.msg_iovlen= 1;
  msg.msg_control = ctrl;
  msg.msg_controllen = MAXCTRLSIZE;
  msg.msg_name = &sa;
  msg.msg_namelen = sizeof(struct sockaddr_in6);
        
  n = recvmsg(ifp->mldi_socket,&msg,MSG_WAITALL);

  if (n <= sizeof(struct icmp6hdr)) {
    free(ctrl);
    return; 
  }

  for(cmsg=CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg =CMSG_NXTHDR(&msg,cmsg)) {
    if ((cmsg->cmsg_level == IPPROTO_IPV6) &&(cmsg->cmsg_type == IPV6_PKTINFO)) {
      info = (struct in6_pktinfo *)CMSG_DATA(cmsg);
      if_index = info->ipi6_ifindex;
    } 
    else {
      badmsg=1;
    }
  }

  if(ctrl)
    free(ctrl);

  if ( badmsg ) {
    downstream->mldi_ti_qi=10;
    downstream->mldi_isquerier=TRUE;
    return;
  }

  /* Set pointer to start of report */
  offset = (char *)ifp->mldi_buf;
  icmp = (struct icmp6hdr *)offset;
  mldv1 = (mldv1_report_t *) offset;
  mldv2 = (mldv2_report_t *) offset;
  mld_qry = (mldv2_query_t *) offset;

#ifdef DEBUG_MLD_PROXY
  printf("Dumping %d bytes recevied message on index %d type %d ifp->index=%d\n",
                  n, if_index, ifp->mldi_type, ifp->mldi_index);
  dumpBuf((char *)(offset), n);
#endif

  if(NULL == (inet_ntop( AF_INET6, &(sa.sin6_addr.s6_addr), src_addr, 
                                             INET6_ADDRSTRLEN))) {
    perror("wrong address");
    return;
  }

  MLD_DEBUG("Rcvd %d bytes sent by %s from if %d\n", 
                                         n, src_addr, info ? info->ipi6_ifindex: 0);          
  MLD_DEBUG("ICMPv6 type %d code %d \n\n\n", icmp->icmp6_type, icmp->icmp6_code);

        
  /*lookup the network interface from which the packet arrived*/
  ifpi = mldrt_interface_lookup_index(mldrt,if_index);

  if (ifpi == NULL)
    return;

  /* Handle the message */
  switch (icmp->icmp6_type) {

    case ICMPV6_MGM_QUERY:

      inet_ntop(AF_INET6, &(mld_qry->grp_addr), grp_addr, INET6_ADDRSTRLEN);

      MLD_DEBUG("Group Address %s\n", grp_addr);
              
      if(n == MLDV1_QUERY_SIZE)
      {
        MLD_DEBUG("MLD v1 query");
      }
      else if(MLDV2_QUERY_SIZE)
      {
        MLD_DEBUG("MLD v2 query");
      }
      break;
              
    case ICMPV6_MGM_REPORT:

      MLD_DEBUG("MLDv1 Report");

      inet_ntop(AF_INET6, &(mldv1->grp_addr), grp_addr, INET6_ADDRSTRLEN);

      MLD_DEBUG("Group Address %s\n", grp_addr);

      if(IN6_IS_ADDR_MC_GLOBAL(&(mldv1->grp_addr)))
        MLD_DEBUG("Global scope");
      else if(IN6_IS_ADDR_MC_SITELOCAL(&mldv1->grp_addr))
        MLD_DEBUG("Site Local");
      else if(IN6_IS_ADDR_MC_LINKLOCAL(&mldv1->grp_addr))
        MLD_DEBUG("Link Local");
      else
        MLD_DEBUG("Invalid group");

      bzero(grpinfo, 64);
      ioctl(ifp->mldi_socket, SIOCGRECVIF, grpinfo);
      /* add group membership */
      mld_interface_membership_report_v1(&router, ifp, &sa.sin6_addr, 
                                                              mldv1, n, grpinfo);
      break;

    case ICMPV6_MGM_REDUCTION:
      MLD_DEBUG("MLDv1 Done");
      inet_ntop(AF_INET6, &(mldv1->grp_addr), grp_addr, INET6_ADDRSTRLEN);

      MLD_DEBUG("Group Address %s\n", grp_addr);
      bzero(grpinfo, 64);
      ioctl(ifp->mldi_socket, SIOCGRECVIF, grpinfo);
      mld_interface_membership_leave_v1(&router,   ifp, &sa.sin6_addr,
                                        mldv1, n);
      break;

    case ICMPV6_MLD2_REPORT:
      MLD_DEBUG("MLDv2 report\n");

      bzero(grpinfo, 64);
      ioctl(ifp->mldi_socket, SIOCGRECVIF, grpinfo);
      MLD_DEBUG("grpinfo=%s\n", grpinfo);
      mld_interface_membership_report_v2(&router, ifp, &sa.sin6_addr,
                                         mldv2, n, grpinfo); 
      break;

    default:
      MLD_DEBUG("Unknown MLD message type \n");
  }

  mld_info_print(&router);
  mld_memberdb_info_print(&router);

  return;
} /* mldrt_input */

/*
 * void* mldrt_input_thread(void* arg)
 *
 * Wait on all interfaces for packets to arrive
 * mld_router_t router;
 */
void*
mldrt_input_thread(void* arg) 
{

  mld_router_t* mldrt = (mld_router_t*) arg;
  mld_interface_t* ifp;
  fd_set rset;
  int n, maxfd;
  struct timeval to;


  /* Add the sockets from all interfaces to the set */
  FD_ZERO(&rset);
  maxfd = 0;

  for (ifp = mldrt->mldrt_interfaces; ifp; ifp = (mld_interface_t *)ifp->mldi_next) {
    if ( ifp->mldi_type == DOWNSTREAM) {
      FD_SET(ifp->mldi_socket, &rset);
      if (maxfd < ifp->mldi_socket)
        maxfd = ifp->mldi_socket;
    }
  }

  if (maxfd == 0) {
    syslog(LOG_NOTICE,"mld:no interface available\n");
    return NULL;
  }

  /* Wait for data on all sockets */
      
  to.tv_sec = 0;
  to.tv_usec = 0;

  n = select(maxfd+1, &rset, NULL, NULL, &to);

  if (n > 0) {

    for (ifp = mldrt->mldrt_interfaces; ifp; ifp = (mld_interface_t *)ifp->mldi_next) {
      if ( ifp->mldi_type == DOWNSTREAM) { 
        if (FD_ISSET(ifp->mldi_socket, &rset))
          mldrt_input(mldrt, ifp);
            if (--n == 0)
              break;
      }
    } 
  } 

   return NULL;
} /* mldrt_input_thread */

/*
 * void mldrt_start(mld_router_t* mldrt)
 *
 * Start the threads of this router 
 */
void
mldrt_start(mld_router_t* mldrt) 
{

  /* Return if already running */
  if (mldrt->mldrt_running)
    return;

  if (!interface_lo) {
    mldrt->mldrt_flag_timer = 1;
  }


  /* Create and start input handling (thread) */
  mldrt->mldrt_flag_input = 1;

  mldrt->mldrt_running = 1;

  return;
} /* mldrt_start */ 

/*
 * void mldrt_stop(mld_router_t* mldrt)
 *
 * Stop the threads of this router 
 */
void
mldrt_stop(mld_router_t* mldrt) 
{

  /* Return if not running */ 
  if (! mldrt->mldrt_running)
    return;

  /* Signal threads to stop */
  mldrt->mldrt_flag_timer = 0;
  mldrt->mldrt_flag_input = 0;

  /* Wait for the threads to finish */
  /* XXX Should use attach */ 
  mldrt->mldrt_running = 0;
  sleep(3);

  return;
} /* mldrt_stop */


/* signal handler */
void
done(int sig) 
{

  mldrt_destroy(&router);
  unlink(pidfile);
  exit(0);
}

#ifdef BUILD_STATIC
int mld_main (int argc,char **argv)
#else
int main (int argc,char **argv)
#endif
{
  struct sockaddr_in6 *psin;
  interface_list_t *ifl, *ifp;
  mld_interface_t *ifc;
  int i;
  

  argc--; /* skip mld */
  argv++;

  if(0 == argc){
    syslog(LOG_NOTICE,"mld no upstream interface!\n");
    return 1;
  }
    
  bzero(upstream_interface, sizeof(upstream_interface));

  /* get all upstream interfaces */
  for (i=0; i<argc; i++) 
    strcpy(upstream_interface[i],*argv++);	

  openlog("mld",LOG_PID|LOG_NDELAY,LOG_USER);

  syslog(LOG_NOTICE,"mld started!\n");

  log_level = LOG_DEBUG;
	
  /* Initialize */
  signal(SIGUSR1, done);
  signal(SIGKILL, done);
  signal(SIGABRT, done);
  signal(SIGTERM, done);
  signal(SIGHUP, done);

  /* Create and initialize the router */
  querier=1;
  version=MLD_VERSION_1;

  mldrt_init(&router);  
  
  /* Add all the multicast enabled ipv4 interfaces */
  ifl = get_interface_list(IFF_MULTICAST, IFF_LOOPBACK);/*acturally, AF_INET is no use for SUPPORT_NO_IPV4.*/

  /*添加接口 在/proc/net/ip6_mr_vif中可以看到*/
  for (ifp = ifl; ifp; ifp = ifp->ifl_next) {
    psin = (struct sockaddr_in6*) &ifp->ifl_addr6;
    mldrt_interface_add(&router, &psin->sin6_addr, ifp->ifl_name,ifp->ifl_index);
    k_proxy_add_mif(router.mldrt_socket,ifp->ifl_index);
  }

  free_interface_list(ifl);

  /* Start the router */
  mldrt_start(&router);

  for (ifc = (&router)->mldrt_interfaces; ifc; ifc = (mld_interface_t *)ifc->mldi_next) {
    if ( ifc->mldi_type == DOWNSTREAM ) {
      downstream = ifc;
    }
  }           

  while (go_on) {
    mldrt_input_thread(&router);
    
    mldrt_timer_thread(&router);
  }

  /* Done */
  done(0);

  return 0;
}
