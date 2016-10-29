#include "mld_inc.h"
#include "mld.h"
#include "mld_func.h"

/***********************************************************************
*               MLD Proxy reporter management API
***********************************************************************/
mld_rep_t*
mld_rep_create(struct in6_addr * pSrcaddr)
{
  mld_rep_t* rep;

  if ((rep = (mld_rep_t*) malloc(sizeof(mld_rep_t)))) {

    bzero(rep, sizeof(mld_rep_t));
    memcpy(&rep->mldr_addr, pSrcaddr, sizeof(struct in6_addr));
    rep->mldr_next  = NULL;
    rep->mldr_srcs = NULL;
  }

  return rep;

} /* mld_rep_create */
#if 0
int
mld_rep_count(mld_group_t *gp)
{
  mld_rep_t *re;
  int count=0;

  if(!gp)
    return 0;
     
  assert (gp != NULL);
  for (re=gp->mldg_members;re != NULL;re=(mld_rep_t *)re->mldr_next)
    count++;

  return count;
       
} /* mld_rep_count */
#endif
void
mld_rep_cleanup(mld_group_t *gp, mld_rep_t * rep)
{
  mld_rep_t *re;
  mld_rep_t *temp_rep;

  assert(rep != NULL);
  assert (gp != NULL);

  if(!gp->mldg_members)
    return;

  if (rep != gp->mldg_members){
    for (re = gp->mldg_members; (mld_rep_t *)re->mldr_next != rep; 
                                            re=(mld_rep_t *)re->mldr_next);   

    temp_rep = (mld_rep_t *)re->mldr_next;

    if(IN6_ARE_ADDR_EQUAL(temp_rep->mldr_addr.s6_addr32, 
                                    rep->mldr_addr.s6_addr32)) {
      re->mldr_next = rep->mldr_next;
      free(rep);
    }
  }
  else {

    /* delete the head */
    re=gp->mldg_members;
    gp->mldg_members = (mld_rep_t *)re->mldr_next;
    free(re);
  }

  return;
} /* mld_rep_cleanup */


mld_rep_t*
mld_group_rep_lookup(mld_group_t *gp, struct in6_addr * pSrcaddr)
{
  mld_rep_t *rep;
  
  assert(gp != NULL);

  if(!gp->mldg_members)
    return NULL;

  for (rep = (mld_rep_t *) gp->mldg_members; rep; rep = (mld_rep_t *)rep->mldr_next)
  {
    if (IN6_ARE_ADDR_EQUAL(rep->mldr_addr.s6_addr32, pSrcaddr->s6_addr32))
        return rep;
  }

  return NULL; 
} /* mld_group_rep_lookup */

mld_rep_t*
mld_group_rep_add(mld_group_t *gp, struct in6_addr * pSrcaddr)
{
  mld_rep_t* rep;

  assert(gp != NULL);

  /* Return the source if it's already present */
  if ((rep = mld_group_rep_lookup(gp, pSrcaddr)))
    return rep;

  /* Create the source and add to the set */ 
  if ((rep = mld_rep_create(pSrcaddr))) {
    rep->mldr_next = gp->mldg_members;
    gp->mldg_members = rep;
  }

  return rep;
} /* mld_group_rep_add */

mld_src_t *
mld_group_rep_lookup_src(mld_rep_t *rep, int filter)
{
  mld_rep_src_t *rsrc = NULL;

  assert(rep != NULL);

  if(!rep->mldr_srcs)
    return NULL;

  for (rsrc =rep->mldr_srcs; rsrc != NULL; rsrc=rsrc->mldr_src_next) {
    if(filter == rsrc->mldr_rep_src->mlds_fmode)
        return rsrc->mldr_rep_src;
  }

  return NULL;
} /* mld_group_rep_lookup_src */

/***********************************************************************
*               MLD Proxy source reporter management API
***********************************************************************/
mld_src_rep_t*
mld_src_rep_create(mld_rep_t *rep)
{
  mld_src_rep_t* srep;

  if ((srep = (mld_src_rep_t*) malloc(sizeof(mld_src_rep_t)))) {
    bzero(srep, sizeof(mld_src_rep_t));
    srep->mlds_src_rep = rep;
    srep->mlds_rep_next = NULL;
  }

  return srep;
} /* mld_src_rep_create */

int
mld_src_rep_count(mld_src_t *src)
{
  mld_src_rep_t *srep;
  int count=0;

  assert (src != NULL);

  for (srep=src->mlds_rep; srep != NULL;  srep=srep->mlds_rep_next)
    count++;

  return count;
} /* mld_src_rep_count */

mld_src_rep_t*
mld_group_src_rep_lookup(mld_src_t* src, mld_rep_t* rep) 
{
  mld_src_rep_t *srep = NULL;
  
  assert(src != NULL);

  if(!src->mlds_rep)
    return NULL;

  for (srep =  src->mlds_rep; srep != NULL; srep = srep->mlds_rep_next) {
    if(IN6_ARE_ADDR_EQUAL(srep->mlds_src_rep->mldr_addr.s6_addr32,
                          rep->mldr_addr.s6_addr32));
      return srep;
  }

  return NULL;
} /* mld_group_src_rep_lookup */

void
mld_src_rep_cleanup(mld_src_t* src, mld_rep_t * rep) 
{
  mld_src_rep_t *re;
  mld_src_rep_t *srep;
  mld_src_rep_t *temp_srep;

  assert(rep != NULL);
  assert (src != NULL);

  srep = mld_group_src_rep_lookup(src, rep);

  if(!src->mlds_rep)
    return;

  if(srep != NULL) {
    if (srep != src->mlds_rep) {
      for (re= src->mlds_rep; re->mlds_rep_next != srep; re=re->mlds_rep_next); 

        temp_srep = re->mlds_rep_next;
        if(IN6_ARE_ADDR_EQUAL(temp_srep->mlds_src_rep->mldr_addr.s6_addr32, 
                                 srep->mlds_src_rep->mldr_addr.s6_addr32)) {
          re->mlds_rep_next = srep->mlds_rep_next;
          free(srep);
       }
    }
    else {
      /*delete the head*/
      srep= src->mlds_rep;
      src->mlds_rep = srep->mlds_rep_next;
      free(srep);
    }
  }
} /* mld_src_rep_cleanup */


/*
 * mld_src_rep_t * mld_group_src_rep_add()
 *
 * Add a member to the set of sources of a group
 */
mld_src_rep_t*
mld_group_src_rep_add(mld_src_t *src, mld_rep_t *rep)
{
  mld_src_rep_t* srep;

  assert(src != NULL);

  /* Return the source if it's already present */
  if ((srep = mld_group_src_rep_lookup(src, rep))) 
    return srep;

  /* Create the source and add to the set */ 
  if ((srep = mld_src_rep_create(rep))) {
    srep->mlds_rep_next = src->mlds_rep;
    src->mlds_rep = srep;
  }

  return srep;
} /* mld_group_src_rep_add */

/***********************************************************************
*               MLD Proxy reporter source management API
***********************************************************************/
mld_rep_src_t*
mld_rep_src_create(mld_src_t *src) 
{
  mld_rep_src_t* rsrc;

  if ((rsrc = (mld_rep_src_t*) malloc(sizeof(mld_rep_src_t)))) {
    bzero(rsrc, sizeof(mld_rep_src_t));
    rsrc->mldr_rep_src = src;
    rsrc->mldr_src_next = NULL;
  }

  return rsrc;
} /* mld_rep_src_create */

int
mld_rep_src_count(mld_rep_t *rep) 
{
  mld_rep_src_t *rsrc;
  int count=0;

  assert (rep != NULL);

  for (rsrc =rep->mldr_srcs; rsrc != NULL; rsrc=rsrc->mldr_src_next)
  count++;

  return count;
       
} /* mld_rep_src_count */

mld_rep_src_t*
mld_group_rep_src_lookup(mld_rep_t* rep, mld_src_t* src) 
{

  mld_rep_src_t *rsrc = NULL;
  
  assert(src != NULL);
  assert(rep != NULL);

  for (rsrc = rep->mldr_srcs; rsrc != NULL; rsrc = rsrc->mldr_src_next) {
    if(IN6_ARE_ADDR_EQUAL(rsrc->mldr_rep_src->mlds_source.s6_addr32,
                            src->mlds_source.s6_addr32))
      return rsrc;
  }

  return NULL;
} /* mld_group_rep_src_lookup */

void
mld_rep_src_cleanup(mld_rep_t * rep, mld_src_t* src) 
{
  mld_rep_src_t *re;
  mld_rep_src_t *rsrc;
  mld_rep_src_t *temp_rsrc;

  assert(rep != NULL);
  assert (src != NULL);

  rsrc = mld_group_rep_src_lookup(rep, src);

  if(!rep->mldr_srcs)
    return;

  if(rsrc != NULL) {
    if (rsrc != rep->mldr_srcs) {
      for (re = rep->mldr_srcs; re->mldr_src_next != rsrc; re=re->mldr_src_next);      
      temp_rsrc = (mld_rep_src_t *)re->mldr_src_next;

      if(IN6_ARE_ADDR_EQUAL(temp_rsrc->mldr_rep_src->mlds_source.s6_addr32, 
                               rsrc->mldr_rep_src->mlds_source.s6_addr32)) {
      re->mldr_src_next = rsrc->mldr_src_next;
      free(rsrc);
      }
   }
   else {
     /*delete the head*/
     rsrc = rep->mldr_srcs;
     rep->mldr_srcs = rsrc->mldr_src_next;
     free(rsrc);
   }
  }
} /* mld_rep_src_cleanup */


mld_rep_src_t*
mld_group_rep_src_add(mld_rep_t *rep, mld_src_t *src) 
{
  mld_rep_src_t* rsrc;

  assert(src != NULL);
  assert(rep != NULL);

  /* Return the source if it's already present */
  if ((rsrc = mld_group_rep_src_lookup(rep, src))) 
    return rsrc;

  /* Create the source and add to the set */ 
  if ((rsrc = mld_rep_src_create(src))) {
    rsrc->mldr_src_next = rep->mldr_srcs;
    rep->mldr_srcs = rsrc;
  }

  return rsrc;
} /* mld_group_rep_src_add */

/***********************************************************************
*               MLD Proxy source management API
***********************************************************************/
mld_src_t*
mld_src_create(struct in6_addr * pSrcaddr)
{
  mld_src_t* src;

  if ((src = (mld_src_t*) malloc(sizeof(mld_src_t)))) {
    bzero(src, sizeof(mld_src_t));
    memcpy(&src->mlds_source, pSrcaddr, sizeof(struct in6_addr));
    src->mlds_timer = 0;
    src->mlds_fstate = TRUE;
    src->mlds_next  = NULL;
    src->mlds_rep   = NULL;
  }

  return src;
} /* mld_src_create */


/*
清除mldg_in/ex_sources下的src(mld_src_t)结构，
*/
void
mld_src_cleanup(mld_group_t *gp, mld_src_t * src, int filter)
{
  mld_src_t *sr;
  mld_src_t *temp_sr;

  assert(src != NULL);
  assert (gp != NULL);

  if(filter == MODE_IS_INCLUDE) {
    if(!gp->mldg_in_sources)
      return;

    if (src != gp->mldg_in_sources){
     for (sr=gp->mldg_in_sources; sr->mlds_next != src; sr=sr->mlds_next); 
       temp_sr = sr->mlds_next;

      if(IN6_ARE_ADDR_EQUAL(temp_sr->mlds_source.s6_addr32,
                                  src->mlds_source.s6_addr32)) {
         sr->mlds_next = src->mlds_next;
         free(src);
         src = NULL; 
      }
    }/*首指针指向的就是所要去掉的src结构的地址*/
    else {
      /*delete the head*/
      sr=gp->mldg_in_sources;
      gp->mldg_in_sources = sr->mlds_next;
      free(sr);
    }
    LOG((LOG_DEBUG, "mld_src_cleanup: %s\n", inet_ntoa(src->mlds_source)));
  }
  else {
    if(!gp->mldg_ex_sources)
      return;

    if (src != gp->mldg_ex_sources){
      for (sr = gp->mldg_ex_sources; sr->mlds_next != src; sr=sr->mlds_next); 
        temp_sr = (mld_src_t *)sr->mlds_next;

      if(IN6_ARE_ADDR_EQUAL(temp_sr->mlds_source.s6_addr32,
                                  src->mlds_source.s6_addr32)) {
        sr->mlds_next = src->mlds_next;
        free(src);
        src = NULL; 
      }
    } else {
      /*delete the head*/
      sr = gp->mldg_ex_sources;
      gp->mldg_ex_sources = sr->mlds_next;
      free(sr);
    }
    LOG((LOG_DEBUG, "mld_src_cleanup: %s\n", inet_ntoa(src->mlds_source)));
  }
} /* mld_src_cleanup */

#if 0
void
mld_src_print(mld_src_t* src)
{
  printf("\t\tsource:%-16s  %d\n", 
	 (char *)inet_ntoa(src->mlds_source),src->mlds_timer);
}
#endif

mld_src_t* 
mld_group_src_lookup(mld_group_t *gp, struct in6_addr * pSrcaddr, int filter)
{
  mld_src_t *src;
  
  if (gp == NULL)
    return NULL;
  
  if(filter == MODE_IS_INCLUDE) {
    for (src = gp->mldg_in_sources; src != NULL; src = src->mlds_next) {
      if (IN6_ARE_ADDR_EQUAL(src->mlds_source.s6_addr32, pSrcaddr->s6_addr32))
      return src;
    }
  }
  else {
    for (src = gp->mldg_ex_sources; src != NULL; src = src->mlds_next) {
      if (IN6_ARE_ADDR_EQUAL(src->mlds_source.s6_addr32, pSrcaddr->s6_addr32))
        return src;
    }
  }
  return NULL; 
} /* mld_group_src_lookup */

/*
 * mld_src_t* mld_group_src_add()
 *
 * Add a source to the set of sources of a group
 */
mld_src_t*
mld_group_src_add(mld_group_t *gp, struct in6_addr * pSrcaddr, int filter)
{
  mld_src_t* src;

  assert(gp != NULL);

  if(filter == MODE_IS_INCLUDE) {
    /* Return the source if it's already present */
    if((src = mld_group_src_lookup(gp, pSrcaddr, filter)))
      return src;

    /* Create the source and add to the set */ 
    if ((src = mld_src_create(pSrcaddr))) {
      src->mlds_fmode = MODE_IS_INCLUDE;
      src->mlds_next = gp->mldg_in_sources;
      gp->mldg_in_sources = src;
    }
    return src;
  }
  else {
    /* Return the source if it's already present */
    if((src = mld_group_src_lookup(gp, pSrcaddr, filter)))
      return src;

    /* Create the source and add to the set */ 
    if ((src = mld_src_create(pSrcaddr))) {        
      src->mlds_fmode = MODE_IS_EXCLUDE;
      src->mlds_next = gp->mldg_ex_sources;
      gp->mldg_ex_sources = src;
    }
    return src;
  }
} /* mld_group_src_add */



/***********************************************************************
*               MLD Proxy group management API
***********************************************************************/
/*
 * mld_group_t* mld_group_create()
 *
 * Create a group record
 */
mld_group_t* mld_group_create(struct in6_addr * pGroupaddr) 
{

  mld_group_t* gp;

  if ((gp = (mld_group_t*) malloc(sizeof(mld_group_t)))) {
    bzero(gp, sizeof(mld_group_t));
    memcpy(&gp->mldg_addr, pGroupaddr, sizeof(struct in6_addr));
    gp->mldg_fmode = MLD_FMODE_EXCLUDE;
    gp->mldg_version = MLD_VERSION_1;/*default version is V1*/
    gp->mldg_timer = 0;
    gp->mldg_in_sources = NULL;
    gp->mldg_ex_sources = NULL;
    gp->mldg_members = NULL;
    gp->mldg_next = NULL;
    gp->mldg_v1host_prsnt_timer = 0;
      return gp;
  } 
  else
    return NULL;
} /* mld_group_create */

void
mld_group_cleanup(mld_interface_t *ifp, mld_group_t* gp, mld_router_t* router)
{
  mld_group_t *g;
  //struct in6_addr saddr;

  assert(gp != NULL);
  assert(ifp != NULL);

  //bzero(&saddr, sizeof(struct in6_addr));
  //k_proxy_del_mfc (router->mldrt_socket, &saddr , &gp->mldg_addr);

  if (ifp->mldi_groups != gp) {
    g=ifp->mldi_groups;

    while(g->mldg_next != gp)		
      g=g->mldg_next;

      g->mldg_next=gp->mldg_next;		
      free(gp);
      gp = NULL;
  }
  else {
    /*delete the head*/
    g=ifp->mldi_groups;
    ifp->mldi_groups = g->mldg_next;
    free(g);
  } 

  mld_info_print(router);
  mld_memberdb_info_print(router);

  return;
} /* mld_group_cleanup */

#if 0
void
mld_group_print(mld_group_t* gp)
{
  mld_src_t *src;
  mld_rep_t *rep;
  char ipv6_addr[INET6_ADDRSTRLEN];

  printf("  %-16s %s %d\n", 
	 (char *)inet_ntoa(gp->mldg_addr),
	 (gp->mldg_fmode == MLD_FMODE_EXCLUDE) ? "exclude" : "include",
	 gp->mldg_timer);
  if (gp->mldg_in_sources != NULL)
    for (src=gp->mldg_in_sources;src;src=(mld_src_t *)src->mlds_next)
       inet_ntop(AF_INET6, &src->mlds_source, ipv6_addr, INET6_ADDRSTRLEN);
      printf("in source : %s timer : %d\n", (char *)ipv6_addr, src->mlds_timer);
  if (gp->mldg_ex_sources != NULL)
    for (src=gp->mldg_ex_sources;src;src=(mld_src_t *)src->mlds_next)
       inet_ntop(AF_INET6, &src->mlds_source, ipv6_addr, INET6_ADDRSTRLEN);
     
      printf("ex source : %s timer : %d\n", (char *)ipv6_addr, src->mlds_timer);
   if (gp->mldg_members != NULL)
    for (rep=gp->mldg_members;rep;rep=(mld_rep_t *)rep->mldr_next)
       inet_ntop(AF_INET6, &rep->mldr_addr, ipv6_addr, INET6_ADDRSTRLEN);
      printf("member : %s \n",(char *)ipv6_addr);
}


/***********************************************************************
*               MLD Proxy membership management API
***********************************************************************/
/*
 * create entry in the membership database
 */

membership_db*
mld_create_membership(struct in6_addr group,
                  int fmode,
                  int numsources,
                  struct in6_addr sources[MAX_ADDRS]) 
{

  membership_db* member;
  int i;

  if ((member = (membership_db*) malloc(sizeof(membership_db)))) {

    bzero(member, sizeof(membership_db));
    member->membership.group = group;
    member->membership.fmode = fmode;
    member->membership.numsources = numsources;

    for(i=0;i<numsources;i++)
      memcpy(&member->membership.sources[i], &sources[i], 
                                             sizeof(struct in6_addr));
      member->next = NULL;
      return member;
  }
  else
    return NULL;
} /* mld_create_membership */


/*
 * lookup for a group entry in the membership database
 */

membership_db*
mld_find_membership(membership_db *membership,struct in6_addr group) 
{

  membership_db* memb;

  for(memb=membership;memb;memb=memb->next)
    if(IN6_ARE_ADDR_EQUAL(memb->membership.group.s6_addr32, group.s6_addr32))
      return memb;
  return NULL;
} /* mld_find_membership */

/*
 * deleate group entry from membership database
 */

membership_db*
mld_delete_membership(mld_router_t* mldrt,struct in6_addr group) 
{

  membership_db *member;
  membership_db *memb;

  member = mld_find_membership(mldrt->mldrt_membership_db,group);

  if((mldrt != NULL) && (member != NULL)) {

    if(IN6_ARE_ADDR_EQUAL(
                    mldrt->mldrt_membership_db->membership.group.s6_addr32, 
                    group.s6_addr32)) {

      memb = mldrt->mldrt_membership_db;
      while(!IN6_ARE_ADDR_EQUAL(memb->next->membership.group.s6_addr32, 
                                                           group.s6_addr32))
        memb = memb->next;

      memb->next = member->next;
      free(member);
    }
    else { /*deleate the head*/
      memb = mldrt->mldrt_membership_db;
      mldrt->mldrt_membership_db = memb->next;
      free(memb);
    }
  }

  return NULL; /* TODO */
} /* mld_delete_membership */
#endif


/*
 * order the sources in a in a source list
 去掉源地址为零的源地址数组
 */

void mld_order_sources_in_source_list(int * pnSources,struct in6_addr *sources) 
{

  int i,j;
  struct in6_addr sr[MLD_MAX_NUM_SRCS];

  for(i=0; i < MLD_MAX_NUM_SRCS; i++) {
    memcpy(&sr[i], &sources[i], sizeof(struct in6_addr));
    bzero(&sources[i], sizeof(struct in6_addr));
  }

  j = 0;
  for(i=0; i < MLD_MAX_NUM_SRCS; i++) {
    if (sr[i].s6_addr32[0] != 0) {
      memcpy(&sources[i], &sr[i], sizeof(struct in6_addr));
      j++;
    }
  }

  *pnSources = j;

  return;
} /* mld_order_sources_in_source_list */

/*
update membership of one group, 
PARAMETER member是输出，这是set_source_filter要用到的，但是，这个变量很快就失效，在timer中并不会用到
*/
void 
mld_update_multi(mld_router_t *mldrt,
             mld_group_t *gp, 
             membership_db *member,
             mld_rep_t *rep) 
{

  int i,k;
  mld_src_t *src;
  struct in6_addr saddr;
  
  if((rep && (rep->mldr_rep_type != ICMPV6_MLD_V2_REPORT)) &&
     (gp->mldg_v1host_prsnt_timer > 0)) {
    return;
  }

  bzero(member, sizeof(membership_db));

  /* find sources with valid number of reporters */ 
  bzero(&saddr, sizeof(struct in6_addr));

  src=mld_group_src_lookup(gp, &saddr, MLD_FMODE_EXCLUDE);

  if(src) {
    memcpy(&member->membership.group, &gp->mldg_addr, sizeof(struct in6_addr));
    member->membership.fmode = MLD_FMODE_EXCLUDE;
    member->membership.numsources = 0;
    gp->mldg_fmode = MLD_FMODE_EXCLUDE;
    return;
  }
  else if(gp->mldg_ex_sources) {

    /* (union ??? of all  EX) - IN */
    for(src=gp->mldg_ex_sources, i=0; 
                         ((NULL != src) && (i < MLD_MAX_NUM_SRCS)); 
                         src = src->mlds_next, i++) {
      memcpy(&member->membership.sources[i], &src->mlds_source, sizeof(struct in6_addr));
    } 
    /*mldg_ex_sources中的源地址也在mldg_in_sources链表中时，要去掉*/
    for(k=0;k<MLD_MAX_NUM_SRCS;k++) {
      for(src=gp->mldg_in_sources; NULL != src; src = src->mlds_next) {
        if(IN6_ARE_ADDR_EQUAL(member->membership.sources[k].s6_addr32, 
                              src->mlds_source.s6_addr32))
          memset(&member->membership.sources[k], 0, sizeof(struct in6_addr));
      }
    }

    mld_order_sources_in_source_list(&(member->membership.numsources),  member->membership.sources);

    memcpy(&member->membership.group, &gp->mldg_addr, sizeof(struct in6_addr));
    member->membership.fmode = MLD_FMODE_EXCLUDE;
    gp->mldg_fmode = MLD_FMODE_EXCLUDE;
    return;
    }
    else if(gp->mldg_in_sources) {
      /* union of IN */
      for(src=gp->mldg_in_sources, i=0; ((NULL != src) && (i < MLD_MAX_NUM_SRCS)); 
                         src = src->mlds_next, i++) {
        memcpy(&member->membership.sources[i], &src->mlds_source, 
                                               sizeof(struct in6_addr));
      }

      mld_order_sources_in_source_list(&(member->membership.numsources), 
                                       member->membership.sources);
      memcpy(&member->membership.group, &gp->mldg_addr, sizeof(struct in6_addr));
      member->membership.fmode = MLD_FMODE_INCLUDE;
      gp->mldg_fmode = MLD_FMODE_INCLUDE;
      return;
  } 
  else {
    /* no sources - should leave the group*/
    memcpy(&member->membership.group, &gp->mldg_addr, sizeof(struct in6_addr));
    member->membership.fmode = MLD_FMODE_INCLUDE;
    member->membership.numsources = 0;
    gp->mldg_fmode = MLD_FMODE_INCLUDE;
    return;
  }

  return;
} /* mld_update_multi */
