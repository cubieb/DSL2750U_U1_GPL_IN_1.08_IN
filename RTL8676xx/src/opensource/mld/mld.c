#include "mld_inc.h"
#include "mld.h"
#include "mld_func.h"

/*
 * void mld_group_handle_allow
 * handle an allow report for a group
 *
 */

extern void mld_update_multi(mld_router_t *mldrt,
             mld_group_t *gp, 
             membership_db *member,
             mld_rep_t *rep); 

extern int inet_pton(int af, const char *src, void *dst);

void mld_group_handle_allow(mld_router_t *router,
                            mld_interface_t *ifp,
                            mld_group_t *gp,
                            int numsrc,
                            struct in6_addr *sources,
                            mld_rep_t *rep,
                            char *gpinfo) 
{

  mld_src_t *src;
  int i;
  membership_db member;
  mld_src_rep_t * srep = NULL;
  mld_rep_src_t * rsrc = NULL;
  struct in6_addr * pSourcesAddr6 = sources;

  /* find reporter -> match src - do action */
  for(i=0;i<numsrc;i++, pSourcesAddr6++) 
  {
    if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_INCLUDE)))
    {
      /* add the reporter */
      srep=mld_group_src_rep_add(src,rep);
      rsrc = mld_group_rep_src_add(rep, src);
      mld_update_snooping_info(gp, SNOOP_IN_ADD, gpinfo, &src->mlds_source);
      gp->mldg_timer = MLD_GMI;
      src->mlds_timer = MLD_GMI;
      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {         
        k_proxy_chg_mfc(router->mldrt_socket,pSourcesAddr6, 
                                      &gp->mldg_addr, ifp->mldi_index);
      }
    }
    else if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_EXCLUDE)))
    {
      mld_src_rep_cleanup(src, rep);
      mld_rep_src_cleanup(rep, src);
      if(mld_rep_src_count(rep) == 0) {
        mld_rep_cleanup(gp, rep);
        mld_update_snooping_info(gp, SNOOP_EX_CLEAR, gpinfo, &src->mlds_source);
      }
      if(mld_src_rep_count(src) == 0) {
        k_proxy_del_mfc (router->mldrt_socket, &src->mlds_source, &gp->mldg_addr);
        mld_src_cleanup(gp,src, MLD_FMODE_EXCLUDE); /* immidiate drop */
      }
    }
    /* new source - add */
    else 
    {
      src=mld_group_src_add(gp,pSourcesAddr6, MLD_FMODE_INCLUDE);
      if(src != NULL) {
        srep = mld_group_src_rep_add(src,rep);
        rsrc = mld_group_rep_src_add(rep, src);
        mld_update_snooping_info(gp, SNOOP_IN_ADD, gpinfo, &src->mlds_source);
        gp->mldg_timer = MLD_GMI;
        src->mlds_timer = MLD_GMI;
      }

      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {         
        k_proxy_chg_mfc(router->mldrt_socket,pSourcesAddr6, &gp->mldg_addr, ifp->mldi_index);
      }
    }
  }


  mld_update_group_src_reporters(router, gp);

  if(ifp->mldi_type != UPSTREAM) {
    mld_update_multi(router,gp, &member, rep);       
    if(gp->mldg_v1host_prsnt_timer > 0) {
      set_source_filter(router, ifp, gp, MLD_FMODE_EXCLUDE, 0, NULL);
    }
    else {
      set_source_filter(router, ifp, gp, member.membership.fmode, \
              member.membership.numsources,member.membership.sources);
    }
  }
 
  return;
} /* mld_group_handle_allow */

/*
 * void mld_group_handle_block
 * handle a block report for a group
 *
 */
void mld_group_handle_block(mld_router_t *router,
                            mld_interface_t *ifp,
                            mld_group_t *gp,
                            int numsrc,
                            struct in6_addr * sources,
                            mld_rep_t *rep,
                            char *gpinfo) 
{

  int i;
  membership_db member;
  mld_src_t * src = NULL;
  mld_rep_src_t *rsrc = NULL;
  mld_src_rep_t *srep = NULL;
  struct in6_addr * pSourcesAddr6 = sources;

  /* find reporter -> match src - do action */
  for(i=0;i<numsrc;i++, pSourcesAddr6++) 
  {
    if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_EXCLUDE)))
    {
      /* add the reporter */
      srep=mld_group_src_rep_add(src,rep);
      rsrc = mld_group_rep_src_add(rep, src);
      mld_update_snooping_info(gp, SNOOP_EX_ADD, gpinfo, &src->mlds_source);
      src->mlds_timer = MLD_GMI;
      gp->mldg_timer = MLD_GMI;
      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
        k_proxy_chg_mfc(router->mldrt_socket,pSourcesAddr6, &gp->mldg_addr,\
                                                           ifp->mldi_index);
      }
    }
    else if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_INCLUDE)))
    {
      mld_src_rep_cleanup(src, rep);
      mld_rep_src_cleanup(rep, src);
      if(mld_rep_src_count(rep) == 0) {
        mld_rep_cleanup(gp, rep);
        mld_update_snooping_info(gp, SNOOP_IN_CLEAR, gpinfo, &src->mlds_source);
      }
      if(mld_src_rep_count(src) == 0) {
        k_proxy_del_mfc (router->mldrt_socket, &src->mlds_source, &gp->mldg_addr);
        mld_src_cleanup(gp,src, MLD_FMODE_INCLUDE); 
      }
    }
    /* new source - add */
    else if(mld_group_rep_lookup_src(rep, MLD_FMODE_EXCLUDE))
    {
      gp->mldg_timer = MLD_GMI;
      src=mld_group_src_add(gp, pSourcesAddr6, MLD_FMODE_EXCLUDE);
      if(src != NULL) {
        srep = mld_group_src_rep_add(src,rep);
        rsrc = mld_group_rep_src_add(rep, src);
        mld_update_snooping_info(gp, SNOOP_EX_ADD, gpinfo, &src->mlds_source);
        src->mlds_timer = MLD_GMI;
      }
      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
        k_proxy_chg_mfc(router->mldrt_socket,pSourcesAddr6, &gp->mldg_addr, \
                                                           ifp->mldi_index);
      }
    }
  }

  mld_update_group_src_reporters(router, gp);

  mld_update_multi(router,gp, &member, rep); 

  if(gp->mldg_v1host_prsnt_timer > 0) {
    set_source_filter(router, ifp, gp, MLD_FMODE_EXCLUDE,0,NULL);
  } 
  else  
  {
    set_source_filter(router, ifp, gp, member.membership.fmode,
              member.membership.numsources,member.membership.sources); 
  }

   return;
} /* mld_group_handle_block */

/*
 * void mld_group_toex()
 * Handle a to_ex{} report for a group
 */

void mld_group_handle_toex(mld_router_t *router,
                           mld_interface_t *ifp,
                           mld_group_t *gp,
                           int numsrc,
                           struct in6_addr * sources,
                           mld_rep_t *rep,
                           char *gpinfo) 
{

  int i;
  membership_db member;
  mld_src_t * src = NULL;
  mld_rep_src_t *rsrc = NULL;
  mld_src_rep_t *srep = NULL;
  struct in6_addr saddr;
  struct in6_addr * pSourcesAddr6 = sources;

  /* remove previous entries from this reporter */
  for(rsrc = (mld_rep_src_t *)rep->mldr_srcs; NULL != rsrc; 
           rsrc = (mld_rep_src_t *)rsrc->mldr_src_next) {
    mld_src_rep_cleanup(rsrc->mldr_rep_src, rep);
  }

  /* find reporter -> match src - do action */
  for(i=0;i<numsrc;i++, pSourcesAddr6++) 
  {
    if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_EXCLUDE)))
    {
      /* add the reporter */
      srep=mld_group_src_rep_add(src,rep);
      rsrc = mld_group_rep_src_add(rep, src);
      mld_update_snooping_info(gp, SNOOP_EX_ADD, gpinfo, &src->mlds_source);
      src->mlds_timer = MLD_GMI;
      gp->mldg_timer = MLD_GMI;
      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
        k_proxy_chg_mfc(router->mldrt_socket,pSourcesAddr6, &gp->mldg_addr,
                                                         ifp->mldi_index);
      }
    }
    else if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_INCLUDE)))
    {
      mld_src_rep_cleanup(src, rep);
      mld_rep_src_cleanup(rep, src);
      if(mld_rep_src_count(rep) == 0) {
        mld_rep_cleanup(gp, rep);
        mld_update_snooping_info(gp, SNOOP_IN_CLEAR, gpinfo, &src->mlds_source);
      }
      if(mld_src_rep_count(src) == 0) {
        k_proxy_del_mfc (router->mldrt_socket, &src->mlds_source, &gp->mldg_addr);
        mld_src_cleanup(gp,src, MLD_FMODE_INCLUDE); /* immidiate drop */
      }
    }
    else /* new source - add */
    {
      gp->mldg_timer = MLD_GMI;
      src=mld_group_src_add(gp,pSourcesAddr6, MLD_FMODE_EXCLUDE);
      if(src != NULL) {
        srep = mld_group_src_rep_add(src,rep);
        rsrc = mld_group_rep_src_add(rep, src);
        mld_update_snooping_info(gp, SNOOP_EX_ADD, gpinfo, &src->mlds_source);
        src->mlds_timer = MLD_GMI;
      }

      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
        k_proxy_chg_mfc(router->mldrt_socket,pSourcesAddr6, &gp->mldg_addr,
                                                           ifp->mldi_index);
      }
    }
  }

  if(numsrc == 0) {
    gp->mldg_timer = MLD_GMI;
    bzero(&saddr, sizeof(struct in6_addr));
    src=mld_group_src_add(gp, &saddr, MLD_FMODE_EXCLUDE);
    if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
      k_proxy_chg_mfc(router->mldrt_socket, &saddr, &gp->mldg_addr, 
                                                      ifp->mldi_index);
    }

    if(src != NULL) {
      src->mlds_timer = MLD_GMI;
      srep = mld_group_src_rep_add(src,rep);
      rsrc = mld_group_rep_src_add(rep, src);
      mld_update_snooping_info(gp, SNOOP_EX_ADD, gpinfo, &src->mlds_source);
    }
  }

  mld_update_group_src_reporters(router, gp);

  mld_update_multi(router,gp, &member, rep);         

  if(gp->mldg_v1host_prsnt_timer > 0) {
    set_source_filter(router, ifp, gp, MLD_FMODE_EXCLUDE, 0,NULL);
  } 
  else 
  {
    set_source_filter(router, ifp, gp, member.membership.fmode,
               member.membership.numsources,member.membership.sources);
  }

  return;
} /* mld_group_handle_toex */


/* 
 * void mld_group_toin()
 * handle to_in{} report for a group
 */
void mld_group_handle_toin(mld_router_t *router,
                           mld_interface_t *ifp,
                           mld_group_t *gp,
                           int numsrc,
                           struct in6_addr * sources,
                           mld_rep_t *rep,
                           char *gpinfo) 
{

  int i;
  membership_db member;
  mld_src_t * src = NULL;
  mld_rep_src_t *rsrc = NULL;
  mld_src_rep_t *srep = NULL;
  struct in6_addr saddr;
  struct in6_addr * pSourcesAddr6 = sources;

  /* remove previous entries from this reporter */
  for(rsrc = (mld_rep_src_t *)rep->mldr_srcs; rsrc != NULL; 
                  rsrc = (mld_rep_src_t *)rsrc->mldr_src_next) {
    mld_src_rep_cleanup(rsrc->mldr_rep_src, rep);
  }

  /* find reporter -> match src - do action */
  for(i=0;i<numsrc;i++, pSourcesAddr6++) 
  {
    if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_INCLUDE)))
    {
      /* add the reporter */
      srep=mld_group_src_rep_add(src,rep);
      rsrc = mld_group_rep_src_add(rep, src);
      mld_update_snooping_info(gp, SNOOP_IN_ADD, gpinfo, &src->mlds_source);
      src->mlds_timer = MLD_GMI;
      gp->mldg_timer = MLD_GMI;
      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
        k_proxy_chg_mfc(router->mldrt_socket,pSourcesAddr6,&gp->mldg_addr,
                                                         ifp->mldi_index);
      }
    }
    else if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_EXCLUDE)))
    {
      mld_src_rep_cleanup(src, rep);
      mld_rep_src_cleanup(rep, src);
      if(mld_rep_src_count(rep) == 0) {
        mld_rep_cleanup(gp, rep);
        mld_update_snooping_info(gp, SNOOP_EX_CLEAR, gpinfo, &src->mlds_source);
      }
      if(mld_src_rep_count(src) == 0) {
        k_proxy_del_mfc (router->mldrt_socket, &src->mlds_source, &gp->mldg_addr);
        mld_src_cleanup(gp,src, MLD_FMODE_EXCLUDE); /* immidiate drop */
      }
    }
    else /* new source - add */
    {
      gp->mldg_timer = MLD_GMI;
      src=mld_group_src_add(gp, pSourcesAddr6, MLD_FMODE_INCLUDE);
      if(src != NULL) {
        srep = mld_group_src_rep_add(src,rep);
        rsrc = mld_group_rep_src_add(rep, src);
        mld_update_snooping_info(gp, SNOOP_IN_ADD, gpinfo, &src->mlds_source);
        src->mlds_timer = MLD_GMI;
      }

      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
        k_proxy_chg_mfc(router->mldrt_socket, pSourcesAddr6, &gp->mldg_addr,
                                                         ifp->mldi_index);
      }
    }
  }

  if(numsrc == 0) {
    bzero(&saddr, sizeof(struct in6_addr));
    src = mld_group_src_lookup(gp, &saddr, MLD_FMODE_EXCLUDE);
    if(src) {
      mld_src_rep_cleanup(src, rep);
      mld_rep_src_cleanup(rep, src);
      if(mld_rep_src_count(rep) == 0) {
        mld_rep_cleanup(gp, rep);
        mld_update_snooping_info(gp, SNOOP_EX_CLEAR, gpinfo, &src->mlds_source);
      }
      if(mld_src_rep_count(src) == 0) {
        mld_src_cleanup(gp, src, MLD_FMODE_EXCLUDE);
        k_proxy_del_mfc (router->mldrt_socket, &saddr, &gp->mldg_addr);
      }
    }
  }

  mld_update_group_src_reporters(router, gp);

  if (ifp->mldi_type != UPSTREAM) {
    mld_update_multi(router,gp, &member, rep);         
    if(gp->mldg_v1host_prsnt_timer > 0) {
      set_source_filter(router, ifp, gp, MLD_FMODE_EXCLUDE, 0,NULL);
    } 
    else 
    {
      set_source_filter(router, ifp, gp, member.membership.fmode,
              member.membership.numsources,member.membership.sources);
    }
  }		    
} /* mld_group_handle_toin */
  
/*
 * void mld_group_handle_isex()
 *
 * Handle a is_ex{A} report for a group 
 * the report have only one source
 */
void
mld_group_handle_isex(mld_router_t* router,
                      mld_interface_t* ifp,
                      mld_group_t* gp,
                      int numsrc,
                      struct in6_addr * sources,
                      mld_rep_t *rep, 
                      char *gpinfo) 
{

  int i;
  membership_db member;
  mld_src_t * src = NULL;
  mld_rep_src_t *rsrc = NULL;
  mld_src_rep_t *srep = NULL;
  struct in6_addr saddr;
  struct in6_addr * pSourcesAddr6 = sources;

  /* Reset timer */
  gp->mldg_timer = MLD_GMI; /* ifp->mldi_qi = GMI : GMI = (RBV * QI) + QRI */
       
  /* find reporter -> match src - do action */
  for(i=0;i<numsrc;i++, pSourcesAddr6++) 
  {
    if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_EXCLUDE)))
    {
      /* add the reporter */
      srep=mld_group_src_rep_add(src,rep);
      rsrc = mld_group_rep_src_add(rep, src);
      mld_update_snooping_info(gp, SNOOP_EX_ADD, gpinfo, &src->mlds_source);
      src->mlds_timer = MLD_GMI;
      gp->mldg_timer = MLD_GMI;
      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
        k_proxy_chg_mfc(router->mldrt_socket,pSourcesAddr6, &gp->mldg_addr,
                                                         ifp->mldi_index);
      }
    }
    else if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_INCLUDE)))
    {
      mld_src_rep_cleanup(src, rep);
      mld_rep_src_cleanup(rep, src);
      if(mld_rep_src_count(rep) == 0) {
        mld_rep_cleanup(gp, rep);
        mld_update_snooping_info(gp, SNOOP_IN_CLEAR, gpinfo, &src->mlds_source);
      }
      if(mld_src_rep_count(src) == 0) {
        k_proxy_del_mfc (router->mldrt_socket, &src->mlds_source, &gp->mldg_addr);
        mld_src_cleanup(gp,src, MLD_FMODE_INCLUDE); /* immidiate drop */
      }
    }
    else /* new source - add */
    {
      gp->mldg_timer = MLD_GMI;
      src=mld_group_src_add(gp,pSourcesAddr6, MLD_FMODE_EXCLUDE);
      if(src != NULL) {
        src->mlds_timer = MLD_GMI;
        srep = mld_group_src_rep_add(src,rep);
        rsrc = mld_group_rep_src_add(rep, src);
        mld_update_snooping_info(gp, SNOOP_EX_ADD, gpinfo, &src->mlds_source);
      }

      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
        k_proxy_chg_mfc(router->mldrt_socket,pSourcesAddr6,&gp->mldg_addr,
                                                          ifp->mldi_index);
      }
    }
  }

  if(numsrc == 0) {
    gp->mldg_timer = MLD_GMI;
    bzero(&saddr, sizeof(struct in6_addr));
    src=mld_group_src_add(gp, &saddr, MLD_FMODE_EXCLUDE);
    if((ifp->mldi_type != UPSTREAM) &&(VALID_ADDR(&gp->mldg_addr))) {
      k_proxy_chg_mfc(router->mldrt_socket,&saddr,&gp->mldg_addr, ifp->mldi_index);
    }
    
    if(src != NULL) {
      src->mlds_timer = MLD_GMI;
      srep = mld_group_src_rep_add(src,rep);
      rsrc = mld_group_rep_src_add(rep, src);
      mld_update_snooping_info(gp, SNOOP_EX_ADD, gpinfo, &src->mlds_source);
    }
  }

  mld_update_group_src_reporters(router, gp);

  if (ifp->mldi_type != UPSTREAM) {
    mld_update_multi(router,gp, &member, rep);        
    if(gp->mldg_v1host_prsnt_timer > 0) {
      set_source_filter(router, ifp, gp, MLD_FMODE_EXCLUDE, 0,NULL);
    }
    else 
    {
      set_source_filter(router, ifp, gp, member.membership.fmode,
                member.membership.numsources,member.membership.sources);
    }
  }
  return;
} /* mld_group_handle_isex */

/*
 * void mld_group_handle_isin()
 *
 * Handle a is_in{A} report for a group 
 * the report have only one source
 */
void
mld_group_handle_isin(mld_router_t* router,
                      mld_interface_t* ifp,
                      mld_group_t* gp,
                      int numsrc,
                      struct in6_addr * sources,
                      mld_rep_t *rep,
                      char *gpinfo) 
{

  int i;
  membership_db member;
  mld_src_t * src = NULL;
  mld_rep_src_t *rsrc = NULL;
  mld_src_rep_t *srep = NULL;
  struct in6_addr saddr;
  struct in6_addr * pSourcesAddr6 = sources;
  
  /* Reset timer */
  /* ifp->mldi_qi = GMI : GMI = (RBV * QI) + QRI */
  gp->mldg_timer = MLD_GMI; 
  /* find reporter -> match src - do action */
  for(i=0;i<numsrc;i++, pSourcesAddr6++) 
  {
    if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_INCLUDE)))
    {
      /* add the reporter */
      srep=mld_group_src_rep_add(src,rep);
      rsrc = mld_group_rep_src_add(rep, src);
      mld_update_snooping_info(gp, SNOOP_IN_ADD, gpinfo, &src->mlds_source);
      src->mlds_timer = MLD_GMI;
      gp->mldg_timer = MLD_GMI;
      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
        k_proxy_chg_mfc(router->mldrt_socket,pSourcesAddr6, &gp->mldg_addr,
                                                           ifp->mldi_index);
      }
    }
    else if((src = mld_group_src_lookup(gp, pSourcesAddr6, MLD_FMODE_EXCLUDE)))
    {
      mld_src_rep_cleanup(src, rep);
      mld_rep_src_cleanup(rep, src);
      if(mld_rep_src_count(rep) == 0) {
        mld_rep_cleanup(gp, rep);
        mld_update_snooping_info(gp, SNOOP_EX_CLEAR, gpinfo, &src->mlds_source);
      }
      if(mld_src_rep_count(src) == 0) {
        k_proxy_del_mfc (router->mldrt_socket, &src->mlds_source, &gp->mldg_addr);
        mld_src_cleanup(gp,src, MLD_FMODE_EXCLUDE); /* immidiate drop */
      }
    }
    else /* V2, new source - add */
    {
      gp->mldg_timer = MLD_GMI;
      src=mld_group_src_add(gp,pSourcesAddr6, MLD_FMODE_INCLUDE);
      
      if(src != NULL) {
        src->mlds_timer = MLD_GMI;        
        srep = mld_group_src_rep_add(src,rep);
        rsrc = mld_group_rep_src_add(rep, src);        
        mld_update_snooping_info(gp, SNOOP_IN_ADD, gpinfo, &src->mlds_source);
      }
      
      if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
        k_proxy_chg_mfc(router->mldrt_socket,pSourcesAddr6, &gp->mldg_addr, ifp->mldi_index);
      }
    }
  }
  
  /*v1*/
  if(numsrc == 0) {
    bzero(&saddr, sizeof(struct in6_addr));
    src = mld_group_src_lookup(gp, &saddr, MLD_FMODE_EXCLUDE);

    if(src) {
      mld_src_rep_cleanup(src, rep);
      mld_rep_src_cleanup(rep, src);
      if(mld_rep_src_count(rep) == 0) {
        mld_rep_cleanup(gp, rep);
        mld_update_snooping_info(gp, SNOOP_EX_CLEAR, gpinfo, &src->mlds_source);
      }
      if(mld_src_rep_count(src) == 0) {
        mld_src_cleanup(gp, src, MLD_FMODE_EXCLUDE);
        k_proxy_del_mfc (router->mldrt_socket, &saddr, &gp->mldg_addr);
      }
    }
  }

  mld_update_group_src_reporters(router, gp);
  if (ifp->mldi_type != UPSTREAM) {	  
    mld_update_multi(router,gp, &member, rep);   
    if(gp->mldg_v1host_prsnt_timer > 0) {/*V1*/
      set_source_filter(router, ifp, gp, MLD_FMODE_EXCLUDE, 0,NULL);
    } 
    else 
    {/*V2*/
      set_source_filter(router, ifp, gp, member.membership.fmode,
             member.membership.numsources,member.membership.sources);
    }
  }
} /* mld_group_handle_isin */

/***************************************************************************/
/*									   */
/*   			Timer management routines                          */
/************************************************************************* */

/*
 * void mldrt_timer_querier(mld_interface_t *ifp)
 * handle the other querier timer
 *
 */
void mldrt_timer_querier(mld_interface_t *ifp) 
{

  if (ifp->mldi_oqp > 0)
    --ifp->mldi_oqp;

  if (ifp->mldi_oqp == 0)
    ifp->mldi_isquerier = TRUE;
} /* mldrt_timer_querier */

/*
 * void mldrt_timer_group(mld_router_t* router, mld_interface_t *ifp)
 *
 * handle the  groups timers for this router
 *
 */
void mldrt_timer_group(mld_router_t* router,mld_interface_t *ifp) 
{

  mld_group_t *gp;
  //mld_router_t* mldrt;
  //struct ipv6_mreq mreq;
  //mld_interface_t *ifp1;
  //struct in6_addr saddr;
  
  for(gp=ifp->mldi_groups;gp;gp=gp->mldg_next) {
    if ((IN6_ARE_ADDR_EQUAL(gp->mldg_addr.s6_addr32, MLD_LL_V1_ALL_ROUTERS)) ||
        (IN6_ARE_ADDR_EQUAL(gp->mldg_addr.s6_addr32, MLD_LL_V2_ALL_ROUTERS))) {
         continue;
    }

    /*decrement backward compatiblity timers */
    if(gp->mldg_v1host_prsnt_timer > 0)
       --gp->mldg_v1host_prsnt_timer;

    /*decrement group timer*/
    if (gp->mldg_timer > 0)
      --gp->mldg_timer;
    
    /*handle group timer*/
    if (gp->mldg_timer == 0){
    	/* no more listeners to group */
  	  /*all source timers expired*/
  	  LOG((LOG_DEBUG,"all source timer expired delete group\n"));
       #if 0 /*set_source_filter will do this job*/
      //bzero(&saddr, sizeof(struct in6_addr));
      //k_proxy_del_mfc (router->mldrt_socket, &saddr, &gp->mldg_addr);

  	  mldrt = router;
        
  	  /*deleate group from the set of groups of upstream interface if all downstream memberships are down*/          
      /*Tell all upstream interfaces that we are gone */
      for (ifp1 = router->mldrt_interfaces; ifp1; ifp1 = ifp1->mldi_next) {
        if (ifp1->mldi_type == UPSTREAM && (VALID_ADDR(&gp->mldg_addr))) {
          memcpy(&mreq.ipv6mr_multiaddr, &gp->mldg_addr, sizeof(struct in6_addr));
          mreq.ipv6mr_ifindex = ifp1->mldi_index;
         
          if (setsockopt(router->mldrt_up_socket, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP,
                         (void *) &mreq, sizeof(mreq)) < 0) { 
             syslog(LOG_NOTICE, "timer_group, perror:setsockopt - IP_DROP_MEMBERSHIP");
             perror("timer_group,IP_drop_MEMBERSHIP failed");
          }
        } /*if UPSTREAM */ 
        
      } /*for */ 
      
      #endif
       
      mld_group_cleanup(ifp,gp, router);
    }
  }
}


	  
/*
 * void mldrt_timer_source(mld_router_t* router, mld_interface_t *ifp)
 *
 * handle source timers
 */
void mldrt_timer_source (mld_router_t* router,mld_interface_t *ifp)
{
    mld_group_t *gp;
    mld_src_t   *src;
    //int flag;
    //struct ipv6_mreq mreq;
    //mld_router_t* mldrt;
    //mld_interface_t *ifp1;

    for (gp=ifp->mldi_groups; gp ;gp=gp->mldg_next) {
        for (src=gp->mldg_in_sources;src;src=(mld_src_t *)src->mlds_next){
            /*decrement source timer*/
            if (src->mlds_timer > 0)	
                --src->mlds_timer;

            /*handle source timer*/
            if (src->mlds_timer > 0 ){
                /* suggest to forward traffic from source */
                /* TO DO: add a flag to the source record*/	
                if (src->mlds_fstate == FALSE){
                    src->mlds_fstate= TRUE;

                    /*tell the kernel to forward traffic from this source*/
                    if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
                        k_proxy_chg_mfc(router->mldrt_socket,&src->mlds_source, &gp->mldg_addr,ifp->mldi_index);
                    }
                }
            }else {
                if (src->mlds_timer == 0){
                    /*suggest to stop forwarding traffic from source*/
                    if (src->mlds_fstate == TRUE){
                        src->mlds_fstate = FALSE;

                        /*tell the kernel to stop forwarding traffic from this source*/
                        if((ifp->mldi_type != UPSTREAM) &&(VALID_ADDR(&gp->mldg_addr))) {
                            k_proxy_chg_mfc(router->mldrt_socket,&src->mlds_source, &gp->mldg_addr,ifp->mldi_index); 
                        }
                    }

                    k_proxy_del_mfc (router->mldrt_socket, &src->mlds_source, &gp->mldg_addr);
                    mld_src_cleanup(gp,src, MLD_FMODE_INCLUDE);
                }
            }	
        } 

        for (src=gp->mldg_ex_sources;src;src=(mld_src_t *)src->mlds_next){
            /*decrement source timer*/
            if (src->mlds_timer > 0)	
            --src->mlds_timer;
            
            /*handle source timer*/
            if (src->mlds_timer > 0 ){
                /* suggest to forward traffic from source */
                /* TO DO: add a flag to the source record*/	
                if (src->mlds_fstate == FALSE){
                    src->mlds_fstate= TRUE;
                    
                    //printf("forward traffic from source: %s\n", (char *)inet_ntoa(src->mlds_source));
                    /*tell the kernel to forward traffic from this source*/
                    if((ifp->mldi_type != UPSTREAM) && (VALID_ADDR(&gp->mldg_addr))) {
                        k_proxy_chg_mfc(router->mldrt_socket,&src->mlds_source, &gp->mldg_addr,ifp->mldi_index);
                    }
                }
            }else {
                if (src->mlds_timer == 0){
                    /*suggest to stop forwarding traffic from source*/
                    if (src->mlds_fstate == TRUE){
                    src->mlds_fstate = FALSE;
                    
                    //printf("stop forwarding traffic from source, timer = 0: %s\n", (char *) inet_ntoa(src->mlds_source));
                    /*tell the kernel to stop forwarding traffic from this source*/
                    if((ifp->mldi_type != UPSTREAM) &&  (VALID_ADDR(&gp->mldg_addr))) {
                        k_proxy_chg_mfc(router->mldrt_socket,&src->mlds_source, &gp->mldg_addr,ifp->mldi_index); 
                        }
                    }
                    
                    k_proxy_del_mfc (router->mldrt_socket, &src->mlds_source, &gp->mldg_addr);
                    mld_src_cleanup(gp,src, MLD_FMODE_EXCLUDE);
                }
            }	
        } 
        
        if ((gp->mldg_in_sources == NULL) && 
        (gp->mldg_ex_sources == NULL) && (gp->mldg_v1host_prsnt_timer <= 0)) {
        
            #if 0  /*set_source_filter will do this job*/
            mldrt = router;
            /*deleate group from the set of groups of upstream interface if all downstream memberships are down*/
            
            /*Tell all upstream interfaces that we are gone */
            for (ifp1 = router->mldrt_interfaces; ifp1; ifp1 = ifp1->mldi_next) {
                if (ifp1->mldi_type == UPSTREAM && (VALID_ADDR(&gp->mldg_addr))) {
                    memcpy(&mreq.ipv6mr_multiaddr, &gp->mldg_addr, sizeof(struct in6_addr));
                    mreq.ipv6mr_ifindex=ifp1->mldi_index;
                    
                    if (setsockopt(router->mldrt_up_socket, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP,
                        (void *) &mreq, sizeof(mreq)) < 0) { 
                        syslog(LOG_NOTICE, "timer_source, error:setsockopt - IP_DROP_MEMBERSHIP\n");
                        perror("timer_source,IP_drop_MEMBERSHIP failed");
                        //exit(1);
                    }               
                   
                } /*if UPSTREAM */ 
            } /*for */ 

            router = mldrt;
            #endif
             
            /*delete group*/
            mld_group_cleanup(ifp,gp, router);
        }			      			      
    }
}

/*****************************************************************************
 *
 *   				Querry Routines
 *
 *****************************************************************************/
#if 0

/*
 * create a new scheduled entry
 */
sch_query_t *mld_sch_create( struct in6_addr gp) 
{

  sch_query_t *sh;

  if ((sh=((sch_query_t *)malloc(sizeof(sch_query_t))))){
    bzero(sh, sizeof(sch_query_t));
    memcpy(&sh->gp_addr, &gp, sizeof(struct in6_addr));
    sh->sch_next = NULL;
  }

  return sh;	
}

/*
 * add a scheduled query entry
 */
void sch_group_specq_add(mld_interface_t *ifp,
                         struct in6_addr gp,
                         struct in6_addr *sources,
                         int numsrc) 
{

  sch_query_t* sch;
  int i;

  if (numsrc != 0){
    /*create the schedule entry*/
    sch=mld_sch_create(gp);

    /*set the retransmissions number*/
    sch->mld_retnum = MLD_DEF_RV - 1;
    sch->numsrc = numsrc;

    for (i=0;i<numsrc;i++)
      memcpy(&sch->sources[i], &sources[i], sizeof(struct in6_addr));
      sch->sch_next = (sch_query_t *)ifp->sch_group_query;
      ifp->sch_group_query = sch;
    }
    else
      return;
}
		

/*
 * send a query to a specific group
 *
 */
void send_group_specific_query(
		mld_router_t *router,
		mld_interface_t *ifp,
		mld_group_t *gp)
{
  int SRSP=FALSE;
#if 0
  if (gp->mldg_timer > MLD_TIMER_SCALE)
    SRSP = TRUE;
  else
#endif
    gp->mldg_timer = MLD_TIMER_SCALE;

  /*send a group specific query imediately*/
  mldrt_membership_query(router,ifp,&gp->mldg_addr,NULL,0,SRSP);

  /*schedule retransmission*/
  sch_group_specq_add(ifp,gp->mldg_addr,NULL,0); 

  return;
} 

/*
 * send a group and source specific query
 */
void send_group_src_specific_q(
			mld_router_t *router,
			mld_interface_t *ifp,
			mld_group_t *gp,
			struct in6_addr * sources,
			int numsrc)
{
  int i;
  mld_src_t *src;
  struct in6_addr * pSrcAddr6 = sources;

  if (gp != NULL) {
    for (i=0;i < numsrc; i++, pSrcAddr6++) {
      src=mld_group_src_lookup(gp,pSrcAddr6, MLD_FMODE_INCLUDE);
      if ((src != NULL) && (mld_src_rep_count(src)) == 0)
        src->mlds_timer = MLD_TIMER_SCALE;
      else
        return;
    }

  /*schedule retransmission*/
  sch_group_specq_add(ifp,gp->mldg_addr,sources,numsrc);
  }

  return;
} 


/*
 * handle a reception of membership query
 *
 */
void receive_membership_query(mld_interface_t *ifp,
			      struct in6_addr * gp,
			      struct in6_addr *sources,
			      u_long src_query,
			      int numsrc,
			      int srsp )
{
  mld_group_t *gr;
  mld_src_t *src;
  int i;
  struct in6_addr * pSrcAddr6 = sources;

#if 0
  TODO: for now we assume there are no other queriers on this LAN and we are the one and the upstream guy.
  if (src_query < ifp->mldi_addr.s_addr){ /* another querier is present with lower IP adress*/
    ifp->mldi_oqp=MLD_OQPI;/*set the Other-Querier-Present timer to OQPI*/
    ifp->mldi_isquerier = FALSE;
  }
#endif

  if ( srsp == FALSE) { /* Supress Router-Side Processing flag not set*/
    gr=mld_interface_group_lookup(ifp,gp);

    if (gr != NULL) {
      if (numsrc == 0) {
        /*group specific query*/
        /* group timer is lowered to LMQI*/
        gr->mldg_timer = MLD_TIMER_SCALE;
      }
      else {
        /*group and source specific query*/
        for (i=0;i < numsrc; i++, pSrcAddr6++) {
          src=mld_group_src_lookup(gr,pSrcAddr6, MLD_FMODE_INCLUDE);

          if (src != NULL)
            src->mlds_timer= MLD_TIMER_SCALE;
        }
      }
    }
  }
} 

#endif


/*
 * void mldrt_membership_query()
 *
 * Send a membership query on the specified interface, to the specified group.
 * Include sources if they are specified and the router version of the 
 * interface is mld version 2.
 */
void
mldrt_membership_query(mld_router_t* mldrt, 
                       mld_interface_t* ifp,
                       struct in6_addr *group, 
                       struct in6_addr *sources, 
                       int numsrc, 
                       int SRSP) 
{
  mldv1_query_t mld;
  int offset = 2;
  int sockfd = ifp->mldi_socket;
//  unsigned int ifindex = ifp->mldi_index;	
 // struct sockaddr_in6 src;
  struct sockaddr_in6 dst;
  struct msghdr msgh;
  struct cmsghdr *cmsgh;
  struct iovec iovector;
  char control[128];
  char ra[8] = { IPPROTO_ICMPV6, 0, IPV6_TLV_PADN, 0,
			IPV6_TLV_ROUTERALERT, 2, 0, 0 };

  if ( ifp->mldi_type == UPSTREAM) {
    return;
  }

  memset( &dst, 0, sizeof(struct sockaddr_in6) );
  if( (inet_pton(AF_INET6, MLD_LL_ALL_HOSTS, &dst.sin6_addr)) == -1) {
    perror("inet_pton: error\n");
  }   
  dst.sin6_family = AF_INET6;
	
  /* the MLD v1 query message */
  memset( &mld, 0, sizeof(mldv1_query_t) );
  mld.icmp6_hdr.icmp6_type = ICMPV6_MLD_V1V2_QUERY;
  mld.icmp6_hdr.icmp6_code = 0;
  mld.icmp6_hdr.icmp6_cksum = 0;
	
  memset( &iovector, 0, sizeof(struct iovec) );
  iovector.iov_base = &mld;
  iovector.iov_len = sizeof(mldv1_query_t);
	
  memset( &msgh, 0, sizeof(struct msghdr) );
  msgh.msg_name = &dst;
  msgh.msg_namelen = sizeof(struct sockaddr_in6);
  msgh.msg_iov = &iovector;
  msgh.msg_iovlen = 1;
  msgh.msg_control = control;
  msgh.msg_controllen = 128;
	
  /* set ancillary data options + copy hop-by-hop option */
  cmsgh = CMSG_FIRSTHDR(&msgh);
  cmsgh->cmsg_len = CMSG_LEN(8);
  cmsgh->cmsg_level = IPPROTO_IPV6;
  cmsgh->cmsg_type = IPV6_HOPOPTS;
  memcpy( (void *)CMSG_DATA(cmsgh), (void *)ra, sizeof(ra) );
  msgh.msg_controllen = cmsgh->cmsg_len;
	
  /* ask kernel to compute ICMP checksum */
  if (setsockopt(sockfd, IPPROTO_IPV6,IPV6_CHECKSUM, &offset, 
                                              sizeof(offset)) < 0) {  
    perror("IPV6_CHECKSUM: failed");
  }

#ifdef DEBUG_MLD_PROXY
  printf("Sending Query size \n");
  dumpBuf((char *)&mld, sizeof(mldv1_query_t));
#endif

  if((sendmsg( sockfd, &msgh, MSG_DONTROUTE )) <= 0) {
    perror("Error with sendmsg");
  }

  return;
} /*mldrt_membership_query*/

/*
 * void sch_query_cleanup(ifp,sch)
 * cleanup a scheduled record query from an inteeface
 *
 */
void sch_query_cleanup(mld_interface_t *ifp,
		       sch_query_t *sch)
{
  sch_query_t *sh;

  if (sch != ifp->sch_group_query) {
    for (sh=ifp->sch_group_query;sh->sch_next != sch;sh=sh->sch_next);
      sh->sch_next = sch->sch_next;
      free(sch);
    }
    else { /*delete the head*/
      sh=ifp->sch_group_query;
      ifp->sch_group_query = sh->sch_next;
      free(sh);
    }
} /* sch_query_cleanup */	

/*
 * void construct_set()
 * construct two sets of sources with source timer lower than LMQI
 * et another with source timer greater than LMQI
 */
void construct_set( mld_interface_t *ifp,
		    sch_query_t *sch,
		    struct in6_addr *src_inf_lmqi,
		    struct in6_addr *src_sup_lmqi,
		    int *numsrc1,
		    int *numsrc2)
{
  mld_src_t *src;
  mld_group_t *gp;
  int i,numsr1,numsr2;
  /*src_sup_lmqi=NULL;
  src_inf_lmqi=NULL;*/
	
  numsr1 = numsr2 = 0;

  for (i=0;i < sch->numsrc; i++) {
  /*lookup the group of the source*/
    if ((gp=mld_interface_group_lookup(ifp,&sch->gp_addr))== NULL){
      *numsrc1 = numsr1;
      *numsrc1 = numsr2;
      return;
    }

    /*lookup the record source in the group*/
    if ((src=mld_group_src_lookup(gp,&sch->sources[i], MLD_FMODE_INCLUDE))
                                                               ==NULL) {
      *numsrc1 = numsr1;
      *numsrc1 = numsr2;
      return;
    }

    if (src->mlds_timer > MLD_TIMER_SCALE) {
      memcpy(&src_sup_lmqi[numsr1],&src->mlds_source, sizeof(struct in6_addr));
      numsr1++;
    }
    else {
      memcpy(&src_inf_lmqi[numsr2], &src->mlds_source, 
                                             sizeof(struct in6_addr));
      numsr2++;
    }
  }
	
  *numsrc1 =numsr1;
  *numsrc2 =numsr2;

  return;
} /* construct_set */

/*
 * send_sh_query(router,ifp)
 * send scheduled query on an interface
 *
 */		
void send_sh_query(mld_router_t *router,
		   mld_interface_t *ifp)
{
  sch_query_t *sch;
  struct in6_addr src_inf_lmqi;
  struct in6_addr src_sup_lmqi;
  int numsrc1;
  int numsrc2;
  mld_group_t *gp;

  if (ifp->sch_group_query != NULL) {
    for (sch=ifp->sch_group_query;sch;sch=sch->sch_next){

      /*trait query per query*/
      if (sch->numsrc == 0) {
        /*group specifq query*/
        if (sch->mld_retnum >0) { /*another query yet*/
          if ((gp=mld_interface_group_lookup(ifp,&sch->gp_addr))==NULL) {
            return;
          }

          if (gp->mldg_timer > MLD_TIMER_SCALE)/* group timer > LMQI*/
            mldrt_membership_query(router,ifp,&sch->gp_addr,NULL,0,1);
          else
            mldrt_membership_query(router,ifp,&sch->gp_addr,NULL,0,0);

          --sch->mld_retnum; 
        } 
        else { /*number retransmission = 0*/
          /*delete the query record*/
          sch_query_cleanup(ifp,sch);
        }
      } 
      else {
        /*group and source specifiq query*/
        if (sch->mld_retnum > 0 ) {

          construct_set(ifp,sch,&src_inf_lmqi,&src_sup_lmqi,&numsrc1,&numsrc2);

          /*send query of source with timer > LMQI*/
          if (numsrc2 != 0) {
            mldrt_membership_query(router,ifp,&sch->gp_addr,
                                              &src_inf_lmqi,numsrc2,0);	
          }

          if (numsrc1  != 0) {
            mldrt_membership_query(router,ifp,&sch->gp_addr,&src_sup_lmqi,
                                                                numsrc1,1);
          }

          --sch->mld_retnum;
        }
        else /*retransmission =0*/
          sch_query_cleanup(ifp,sch);	
      }
    }
  }	
} /* send_sh_query */


/*
清查一个group下的各个src下，就残留的rep
*/
void mld_update_group_src_reporters(mld_router_t *router, 
                                     mld_group_t *gp)
{
  mld_src_t *src = NULL;
  mld_src_t *src_next = NULL;
  mld_rep_t *rep = NULL;
  mld_rep_t *rep_next = NULL;
  

  for(src = gp->mldg_in_sources; src; ) {
    src_next = (mld_src_t *)src->mlds_next;
    if(mld_src_rep_count(src) == 0) {
      k_proxy_del_mfc (router->mldrt_socket, &src->mlds_source, &gp->mldg_addr);
      mld_src_cleanup(gp, src, MLD_FMODE_INCLUDE);
    }
    src = src_next;
  }

  for(src = gp->mldg_ex_sources; src; ) {
    src_next = (mld_src_t *)src->mlds_next;
    if(mld_src_rep_count(src) == 0) {
      k_proxy_del_mfc (router->mldrt_socket, &src->mlds_source, &gp->mldg_addr);
      mld_src_cleanup(gp, src, MLD_FMODE_EXCLUDE);
    }
    src = src_next;
  }

  for(rep = gp->mldg_members; rep; ) {
    rep_next = (mld_rep_t *)rep->mldr_next;
    if(mld_rep_src_count(rep) == 0) {
      mld_rep_cleanup(gp, rep);
    }
    rep = rep_next;
  }

  /* null sources, set the grup for removal */
  if(!gp->mldg_in_sources  && !gp->mldg_ex_sources &&
     (gp->mldg_v1host_prsnt_timer <= 0)) {
    gp->mldg_timer = MLD_TIMER_SCALE;
  }

  return;

} /* mld_update_group_src_reporters*/
