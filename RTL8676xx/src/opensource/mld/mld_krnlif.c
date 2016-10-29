#include "mld_inc.h"
#include "mld.h"
#include "mld_func.h"

extern mld_router_t router;

static void addr_conv(unsigned short *in, char * out)
{
    sprintf(out, "%04x%04x%04x", in[0], in[1], in[2]);
    return;
}

static void addr_conv_ip(struct in6_addr *in, char * out)
{
    sprintf(out, "%04x%04x%04x%04x", in->s6_addr16[0], in->s6_addr16[1], in->s6_addr16[2], in->s6_addr16[3]);
    sprintf(out+16, "%04x%04x%04x%04x", in->s6_addr16[4], in->s6_addr16[5], in->s6_addr16[6], in->s6_addr16[7]);
    return;
}

/*
 * set the source list and the source filter
 * on upstream interface
 */
void set_source_filter(mld_router_t* router,
                       mld_interface_t *ifp,
                       mld_group_t* gp,
                       int fmode,
                       int nsources,
                       struct in6_addr *sources ) 
{

  struct group_filter *gf = NULL;
  struct sockaddr_in6 *grp_addr;
  int i, size;

  if (fmode!=MCAST_INCLUDE && fmode!=MCAST_EXCLUDE) {
    syslog(LOG_NOTICE, "PROXY-ERROR: Wrong filter mode.\n");
    return;
  }

  if((gf = (struct group_filter *)malloc(GROUP_FILTER_SIZE(nsources))) == NULL)
    perror("not enough memory");

  bzero(gf, GROUP_FILTER_SIZE(nsources));

  gf->gf_fmode     = fmode;
  gf->gf_numsrc    = nsources;

  grp_addr = (struct sockaddr_in6 *) &gf->gf_group;
  memcpy(&grp_addr->sin6_addr, &gp->mldg_addr, sizeof(struct in6_addr));
   
  for(i = 0; i < nsources; i++) 
    memcpy(&gf->gf_slist[i], &sources[i], sizeof(struct in6_addr));

  size = GROUP_FILTER_SIZE(nsources);

  for (ifp = router->mldrt_interfaces; ifp; ifp = (mld_interface_t *)ifp->mldi_next) {
    if (ifp->mldi_type == UPSTREAM) {

      gf->gf_interface = ifp->mldi_index;

      if(setsockopt(router->mldrt_up_socket, IPPROTO_IPV6, 
                                             MCAST_MSFILTER, gf, size) < 0) {
        syslog(LOG_NOTICE, "setsockopt IP_MSFILTER"); 
        perror("setsockopt MCAST_MSFILTER"); 
      }
    }
  }
  
  if(gf)
    free(gf);

  return;

} /* set_source_filter */


/*
 * Open and init the multicast routing in the kernel.
 */
void k_init_proxy(int socket) 
{

  int v = 1;
    
  if (setsockopt(socket, IPPROTO_IPV6, MRT6_INIT, (void*)&v, sizeof(int)) < 0)
    perror("setsockopt - MRT6_INIT");
  
  return;
} /* k_init_proxy */


/*
 * Stops the multicast routing in the kernel.
 */

void k_stop_proxy(int socket) 
{
    
  if (setsockopt(socket, IPPROTO_IPV6, MRT6_DONE, (char *)NULL, 0) < 0) {
    syslog(LOG_NOTICE, "setsockopt - MRT6_DONE");
    perror("setsockopt - MRT6_DONE");
  }

  return;
} /*k_stop_proxy */

/* 
 * Add a virtual interface to the kernel 
 * using the pimd API:MRT6_ADD_VIF
 * 
 */

int k_proxy_add_mif (int socket, mifi_t mifi) 
{

  struct mif6ctl mc;
  int error;
	
  bzero(&mc, sizeof(struct mif6ctl));

  mc.mif6c_mifi = mifi;
  mc.mif6c_flags = 0;
  mc.vifc_threshold = 0;
  mc.vifc_rate_limit = 0;
  mc.mif6c_pifi = mifi;

  if ((error=setsockopt(socket, IPPROTO_IPV6, MRT6_ADD_MIF, (char *)&mc, sizeof(mc))) <0) {
    syslog(LOG_NOTICE, "setsockopt - MRT6_ADD_MIF");
    perror("failed MRT6_ADD_MIF");
    return FALSE;
  }

  return TRUE;
} /* k_proxy_add_mif */

int k_proxy_del_mif (int socket, mifi_t mifi) 
{
  mifi_t vifi = mifi;
  int error;
	

  if ((error=setsockopt(socket, IPPROTO_IPV6, MRT6_DEL_MIF, (char *)&vifi, sizeof(mifi_t))) <0) {
    syslog(LOG_NOTICE, "setsockopt - MRT6_DEL_MIF");
    perror("failed MRT6_DEL_MIF");
    return FALSE;
  }

  return TRUE;
} /* k_proxy_del_mif */



/*
 * Del an MFC entry from the kernel
 * using pimd API:MRT6_DEL_MFC
 */

int k_proxy_del_mfc (int socket, 
                     struct in6_addr * pSource, 
                     struct in6_addr * pGroup) 
{
	
  struct mf6cctl mc;
  mld_interface_t *ifp;
  mld_router_t* mldrt=&router;

  memset(&mc,0,sizeof(struct mf6cctl));
  for (ifp = mldrt->mldrt_interfaces; ifp; ifp = (mld_interface_t *)ifp->mldi_next) {    
    if (ifp->mldi_type == UPSTREAM) {

      mc.mf6cc_origin.sin6_family = AF_INET6;
      memcpy(&mc.mf6cc_origin.sin6_addr, pSource, sizeof(struct in6_addr));
      mc.mf6cc_mcastgrp.sin6_family = AF_INET6;
      memcpy(&mc.mf6cc_mcastgrp.sin6_addr, pGroup, sizeof(struct in6_addr));
      mc.mf6cc_parent = ifp->mldi_index;

      if (setsockopt(socket, IPPROTO_IPV6, MRT6_DEL_MFC, 
                                             (char *)&mc, sizeof(mc)) < 0) {
        syslog(LOG_NOTICE, "setsockopt- MRT6_DEL_MFC\n");
        perror("setsockopt- MRT6_DEL_MFC");
        return(FALSE);
      }
    }
  }

  return(TRUE);
} /* k_proxy_del_mfc */

/*
 * Install and modify a MFC entry in the kernel (S,G,interface address)
 * using pimd API: MRT6_AD_MFC
 */
int 
k_proxy_chg_mfc(int socket, 
                struct in6_addr * pSource, 
                struct in6_addr * pGroup, 
                mifi_t outmif /*,int fstate*/) 
{

  struct mf6cctl mc;
  mld_interface_t *ifp;
  mld_router_t* mldrt=&router;

  memset(&mc,0,sizeof(struct mf6cctl));
  for (ifp = mldrt->mldrt_interfaces; ifp; ifp = (mld_interface_t *)ifp->mldi_next) {
    if (ifp->mldi_type == UPSTREAM) {

      mc.mf6cc_origin.sin6_family = AF_INET6;
      memcpy(&mc.mf6cc_origin.sin6_addr, pSource, sizeof(struct in6_addr));
      mc.mf6cc_mcastgrp.sin6_family = AF_INET6;
      memcpy(&mc.mf6cc_mcastgrp.sin6_addr, pGroup, sizeof(struct in6_addr));      
      mc.mf6cc_parent = ifp->mldi_index;
      IF_SET(outmif, &(mc.mf6cc_ifset));

      if (setsockopt(socket, IPPROTO_IPV6, MRT6_ADD_MFC, 
                                         (char *)&mc, sizeof(mc)) < 0) {
        syslog(LOG_NOTICE,"setsockopt- MRT_ADD_MFC\n");
        perror("setsockopt- MRT6_ADD_MFC\n");
        return(FALSE);
      }
    }
  }

  return(TRUE);
} /* k_proxy_chg_mfc */

/*gpinfo=br1 vport4 333300000016,000c291fbbd5*/
void mld_update_snooping_info(mld_group_t *gp, 
                              int mode, 
                              char *gpinfo,
                              struct in6_addr *src_ip) 
{

    char cmd[128];
    unsigned short tmp[3];
    char Contain[64];
    char mcast_info[32];
    char ip_char[64];
    char * pos = NULL;

    if(!gp || !gpinfo || !strstr(gpinfo, "br0")) {
      return;
    } 

    bzero(Contain, sizeof(Contain));
    bzero(mcast_info, sizeof(mcast_info));
    bzero(ip_char, sizeof(ip_char));
    
    strcpy(Contain, gpinfo);
    addr_conv_ip(src_ip, ip_char);
  
    tmp[0] = 0x3333;
    tmp[1] = gp->mldg_addr.s6_addr16[6];
    tmp[2] = gp->mldg_addr.s6_addr16[7];
    addr_conv(tmp, mcast_info);

    /*mac_info=001122334455,001122334455,  mac_mcast,mac_host*/
    pos = Contain + 12;
    while(*pos != ',')
        pos++;    
        
    /*override the mac_mcast*/
    memcpy(pos - 12, mcast_info, 12);   
  
    if((mode == SNOOP_IN_CLEAR) || (mode == SNOOP_EX_CLEAR)) {
      sprintf(cmd, "brctl mldclearportsnooping %s,%d,%s 1> /dev/null", Contain, mode, ip_char);
    }
    else if ((mode == SNOOP_IN_ADD) || (mode == SNOOP_EX_ADD)) {
      sprintf(cmd, "brctl mldsetportsnooping %s,%d,%s 1> /dev/null", Contain, mode, ip_char);
    }
    system(cmd);   

    return;
} /* mld_update_snooping_info */
