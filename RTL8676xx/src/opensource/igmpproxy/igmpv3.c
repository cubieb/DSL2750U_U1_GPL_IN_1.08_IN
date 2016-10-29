#include "igmpproxy.h"
#ifdef _SUPPORT_IGMPV3_

#if 0
#define IGMPV3LOG	printf
#else
#define IGMPV3LOG(...)	while(0){}
#endif

#define IGMPV3_MAX_SRCNUM	64

__u32 gsrctmp[IGMPV3_MAX_SRCNUM];

int igmpv3_query( struct mcft_entry *entry, int srcnum, __u32 *srcfilter );



struct src_entry *add_to_srclist(struct mcft_entry *mcp, __u32 src)
{
	struct src_entry *p;

	if(mcp==NULL) return NULL;

	IGMPV3LOG( "%s> group=%s", __FUNCTION__, inet_ntoa( mcp->grp_addr ) );
	IGMPV3LOG( ", src=%s\n", inet_ntoa( src ) );

	p = mcp->srclist;
	while (p) {
		if (p->srcaddr == src)
			return p;
		p = p->next;
	}

	p = malloc(sizeof(struct src_entry));
	if (!p) {
		return NULL;
	}
	memset( p, 0, sizeof(struct src_entry) );
	p->srcaddr = src;
	p->timer.lefttime = 0;
	p->timer.retry_left = 0;
	//p->mrt_state = 0;
	p->next = mcp->srclist;
	mcp->srclist = p;
	return p;
}

int del_from_srclist(struct mcft_entry *mcp, __u32 src)
{
	struct src_entry **q, *p;

	if(mcp==NULL) return  -1;

	IGMPV3LOG( "%s> group=%s", __FUNCTION__, inet_ntoa( mcp->grp_addr ) );
	IGMPV3LOG( ", src=%s\n", inet_ntoa( src ) );

	q = &mcp->srclist;
	p = *q;
	while (p) {
		if(p->srcaddr == src) {
			*q = p->next;
			free(p);
			return 0;
		}
		q = &p->next;
		p = p->next;
	}

	return 0;
}

struct src_entry * get_specific_src(struct mcft_entry *mcp, __u32 src)
{
	struct src_entry **q, *p;

	if(mcp==NULL) return NULL;

	IGMPV3LOG( "%s> group=%s", __FUNCTION__, inet_ntoa( mcp->grp_addr ) );
	IGMPV3LOG( ", src=%s\n", inet_ntoa( src ) );

	q = &mcp->srclist;
	p = *q;
	while (p) {
		if(p->srcaddr == src)
		{
			return p;
		}
		q = &p->next;
		p = p->next;
	}

	return NULL;
}

int get_srclist_num( struct mcft_entry *mcp )
{
	int ret=0;
	struct src_entry *p;

	if(mcp==NULL) return ret;

	p = mcp->srclist;
	while(p)
	{
		ret++;
		p=p->next;
	}

	IGMPV3LOG( "%s> group:%s has %d source(s)\n", __FUNCTION__, inet_ntoa( mcp->grp_addr ), ret );

	return ret;
}

/*过滤组播地址是无效组播源地址的处理功能 dengyanbing 2012-10-20 */
static struct in_addr ip_upnp_addr      = {0xEFFFFFFA}; /* UPnP / SSDP */
static struct in_addr ip_ntfy_srvr_addr = {0xE000FF87}; /* Notificatoin Server*/

int igmp_join_filter(__u32 group)
{
     if(((htonl(group) & htonl(0xFFFFFF00)) == htonl(0xE0000000)) ||
       (htonl(group) == htonl(ip_upnp_addr.s_addr)) || /* UPnp/SSDP */
       (htonl(group) == htonl(ip_ntfy_srvr_addr.s_addr)))   /* Notification srvr */
    {
        return 0;
    }
    else
    {
        return 1;
    }

} /* igmp_join_filter */
#ifdef CONFIG_IGMPPROXY_MULTIWAN
#ifdef CONFIG_IGMP_PASSTHROUGH
int igmp_add_group( __u32 group, __u32 src) 
#else
int igmp_add_group( __u32 group )
#endif
{
	struct ip_mreq mreq;
	struct IfDesc *up_dp ;
	int ret;
        int idx ;
    /*filter igmp join pakcet */ 
    if (!igmp_join_filter(group))
    {    
	    IGMPV3LOG( "%s> join the group=%s is unvalid join group address\n", __FUNCTION__, inet_ntoa( group ) );
        return -1;
    }

	IGMPV3LOG( "%s> join the group=%s\n", __FUNCTION__, inet_ntoa( group ) );

	/* join multicast group */
	mreq.imr_multiaddr.s_addr = group;
	
#ifdef CONFIG_IGMP_PASSTHROUGH
	if (enable_igmp_passthrough)
		mreq.imr_source.s_addr = src;
	else
		mreq.imr_source.s_addr = 0;
#endif	

	for(idx=0;idx<igmp_up_if_num;idx++){
	   up_dp = getIfByName(igmp_up_if_name[idx]);
	   mreq.imr_interface.s_addr = up_dp->InAdr.s_addr;
	   ret = setsockopt(up_dp->sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&mreq, sizeof(mreq));
	   if(ret)
		printf("setsockopt IP_ADD_MEMBERSHIP %s error return:%s!\n", inet_ntoa(mreq.imr_multiaddr), strerror(errno));
	}
	return ret;
}

int igmp_del_group( __u32 group )
{
	struct ip_mreq mreq;
	struct IfDesc *up_dp ;
	int idx ;
        int ret ;
	IGMPV3LOG( "%s> leave the group=%s\n", __FUNCTION__, inet_ntoa( group ) );

	/* drop multicast group */
	mreq.imr_multiaddr.s_addr = group;

	for(idx=0;idx<igmp_up_if_num;idx++){

	   up_dp = getIfByName(igmp_up_if_name[idx]);
	   mreq.imr_interface.s_addr = up_dp->InAdr.s_addr;
	   ret = setsockopt(up_dp->sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*)&mreq, sizeof(mreq));
	   if(ret)
		printf("setsockopt IP_DROP_MEMBERSHIP %s error! return:%s\n", inet_ntoa(mreq.imr_multiaddr), strerror(errno));
	}

	return ret;
}
#else
#ifdef CONFIG_IGMP_PASSTHROUGH
int igmp_add_group( __u32 group, __u32 src) 
#else
int igmp_add_group( __u32 group )
#endif
{
	struct ip_mreq mreq;
	struct IfDesc *up_dp = getIfByName(igmp_up_if_name);
	int ret;

	IGMPV3LOG( "%s> join the group=%s\n", __FUNCTION__, inet_ntoa( group ) );

	/* join multicast group */
	mreq.imr_multiaddr.s_addr = group;
	mreq.imr_interface.s_addr = up_dp->InAdr.s_addr;
#ifdef CONFIG_IGMP_PASSTHROUGH
	if (enable_igmp_passthrough)
		mreq.imr_source.s_addr = src;
	else
		mreq.imr_source.s_addr = 0;
#endif
	ret = setsockopt(up_dp->sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&mreq, sizeof(mreq));
	if(ret)
		printf("setsockopt IP_ADD_MEMBERSHIP %s error return :%s!\n", inet_ntoa(mreq.imr_multiaddr), strerror(errno));

	return ret;
}

int igmp_del_group( __u32 group )
{
	struct ip_mreq mreq;
	struct IfDesc *up_dp = getIfByName(igmp_up_if_name);
	int ret;

	IGMPV3LOG( "%s> join the group=%s\n", __FUNCTION__, inet_ntoa( group ) );

	/* drop multicast group */
	mreq.imr_multiaddr.s_addr = group;
	mreq.imr_interface.s_addr = up_dp->InAdr.s_addr;
	ret = setsockopt(up_dp->sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*)&mreq, sizeof(mreq));
	if(ret)
		printf("setsockopt IP_DROP_MEMBERSHIP %s error return :%s!\n", inet_ntoa(mreq.imr_multiaddr), strerror(errno));
	return ret;
}
#endif

#ifdef CONFIG_IGMPPROXY_MULTIWAN
int igmp_add_mr( __u32 group, __u32 src, int enable )
{
	struct MRouteDesc	mrd;
        int idx ;
	/* add multicast routing entry */
	mrd.OriginAdr.s_addr = src;
	mrd.SubsAdr.s_addr = 0;
	mrd.McAdr.s_addr = group;


	memset(mrd.TtlVc, 0, sizeof(mrd.TtlVc));
	mrd.TtlVc[igmp_down_if_idx] = enable;

	IGMPV3LOG( "%s> group:%s", __FUNCTION__, inet_ntoa(mrd.McAdr) );
	IGMPV3LOG( ", src:%s, enable:%d\n", inet_ntoa(mrd.OriginAdr), enable );
	for(idx=0;idx<igmp_up_if_num;idx++)
	{
	      mrd.InVif = igmp_up_if_idx[idx];
              addMRoute(&mrd);
	}
	return (1);
}

int igmp_del_mr( __u32 group, __u32 src )
{
	struct MRouteDesc	mrd;
	int ret=0;
        int idx;

	/* delete multicast routing entry */
	mrd.OriginAdr.s_addr = src;
	mrd.McAdr.s_addr = group;

	memset(mrd.TtlVc, 0, sizeof(mrd.TtlVc));

	IGMPV3LOG( "%s> group:%s", __FUNCTION__, inet_ntoa(mrd.McAdr) );
	IGMPV3LOG( ", src:%s\n", inet_ntoa(mrd.OriginAdr) );
	for(idx=0;idx<igmp_up_if_num;idx++){
           mrd.InVif = igmp_up_if_idx[idx];
	   delMRoute(&mrd);
	}
	return ret;
}
#else
int igmp_add_mr( __u32 group, __u32 src, int enable )
{
	struct MRouteDesc	mrd;

	/* add multicast routing entry */
	mrd.OriginAdr.s_addr = src;
	mrd.SubsAdr.s_addr = 0;
	mrd.McAdr.s_addr = group;

	mrd.InVif = igmp_up_if_idx;
	memset(mrd.TtlVc, 0, sizeof(mrd.TtlVc));
	mrd.TtlVc[igmp_down_if_idx] = enable;

	IGMPV3LOG( "%s> group:%s", __FUNCTION__, inet_ntoa(mrd.McAdr) );
	IGMPV3LOG( ", src:%s, enable:%d\n", inet_ntoa(mrd.OriginAdr), enable );

	return (addMRoute(&mrd));
}

int igmp_del_mr( __u32 group, __u32 src )
{
	struct MRouteDesc	mrd;
	int ret=0;

	/* delete multicast routing entry */
	mrd.OriginAdr.s_addr = src;
	mrd.McAdr.s_addr = group;
	mrd.InVif = igmp_up_if_idx;
	memset(mrd.TtlVc, 0, sizeof(mrd.TtlVc));

	IGMPV3LOG( "%s> group:%s", __FUNCTION__, inet_ntoa(mrd.McAdr) );
	IGMPV3LOG( ", src:%s\n", inet_ntoa(mrd.OriginAdr) );

	delMRoute(&mrd);
	return ret;
}
#endif

#ifdef CONFIG_IGMPPROXY_MULTIWAN
int igmp_set_srcfilter( struct mcft_entry *p )
{
	struct ip_msfilter *imsfp;
	int	size,i;
	struct IfDesc *up_dp ;
	__u32 group;
	struct src_entry *s;
	int idx;
         if(p==NULL)	return -1;


    //        up_dp = getIfByName(igmp_up_if_name[idx]);

	   /*use the "send_buf buffer*/
	   imsfp = (struct ip_msfilter *)send_buf;
	   imsfp->imsf_multiaddr=p->grp_addr;
//	   imsfp->imsf_interface=up_dp->InAdr.s_addr;
	   imsfp->imsf_fmode=p->filter_mode;
	   imsfp->imsf_numsrc=0;
	   IGMPV3LOG( "%s> maddr:%s", __FUNCTION__, inet_ntoa(imsfp->imsf_multiaddr) );
	   IGMPV3LOG( ", if:%s, fmode:%d\n", inet_ntoa(imsfp->imsf_interface), imsfp->imsf_fmode  );

	   i=0;
	   s=p->srclist;
	     while(s)
	   {
		IGMPV3LOG( "%s>try to match=> fmode:%d, timer:%d, slist:%s\n", __FUNCTION__, p->filter_mode, s->timer.lefttime, inet_ntoa(s->srcaddr) );
		if( ((p->filter_mode==MCAST_INCLUDE) && (s->timer.lefttime>0)) ||
		    ((p->filter_mode==MCAST_EXCLUDE) && (s->timer.lefttime==0)) )
		{
			imsfp->imsf_slist[i] = s->srcaddr;
			IGMPV3LOG( "%s> slist:%s\n", __FUNCTION__, inet_ntoa(imsfp->imsf_slist[i]) );
			i++;
		}
		s=s->next;
	   }
	   imsfp->imsf_numsrc=i;
	   size = IP_MSFILTER_SIZE( i );
	   IGMPV3LOG( "%s> numsrc:%d, size:%d\n", __FUNCTION__, imsfp->imsf_numsrc, size );


	    for(idx=0;idx<igmp_up_if_num;idx++)
          {
                  up_dp = getIfByName(igmp_up_if_name[idx]);
                  imsfp->imsf_interface=up_dp->InAdr.s_addr;
		 if (setsockopt(up_dp->sock, IPPROTO_IP, IP_MSFILTER, imsfp,size) < 0 )
	        {
		        printf("setsockopt IP_MSFILTER return :%s\n", strerror(errno));
        	return -1;
	       }
	  }
	return 0;
}
#else
int igmp_set_srcfilter( struct mcft_entry *p )
{
	struct ip_msfilter *imsfp;
	int	size,i;
	struct IfDesc *up_dp = getIfByName(igmp_up_if_name);
	__u32 group;
	struct src_entry *s;

	if(p==NULL)	return -1;

	/*use the "send_buf buffer*/
	imsfp = (struct ip_msfilter *)send_buf;
	imsfp->imsf_multiaddr=p->grp_addr;
	imsfp->imsf_interface=up_dp->InAdr.s_addr;
	imsfp->imsf_fmode=p->filter_mode;
	imsfp->imsf_numsrc=0;
	IGMPV3LOG( "%s> maddr:%s", __FUNCTION__, inet_ntoa(imsfp->imsf_multiaddr) );
	IGMPV3LOG( ", if:%s, fmode:%d\n", inet_ntoa(imsfp->imsf_interface), imsfp->imsf_fmode  );

	i=0;
	s=p->srclist;
	while(s)
	{
		IGMPV3LOG( "%s>try to match=> fmode:%d, timer:%d, slist:%s\n", __FUNCTION__, p->filter_mode, s->timer.lefttime, inet_ntoa(s->srcaddr) );
		if( ((p->filter_mode==MCAST_INCLUDE) && (s->timer.lefttime>0)) ||
		    ((p->filter_mode==MCAST_EXCLUDE) && (s->timer.lefttime==0)) )
		{
			imsfp->imsf_slist[i] = s->srcaddr;
			IGMPV3LOG( "%s> slist:%s\n", __FUNCTION__, inet_ntoa(imsfp->imsf_slist[i]) );
			i++;
		}
		s=s->next;
	}
	imsfp->imsf_numsrc=i;
	size = IP_MSFILTER_SIZE( i );
	IGMPV3LOG( "%s> numsrc:%d, size:%d\n", __FUNCTION__, imsfp->imsf_numsrc, size );

	if (setsockopt(up_dp->sock, IPPROTO_IP, IP_MSFILTER, imsfp,size) < 0 )
	{
		perror("setsockopt IP_MSFILTER");
        	return -1;
	}

	return 0;
}
#endif
int check_src_set( __u32 src, struct src_entry *srclist )
{
	struct src_entry *p;
	for( p=srclist; p!=NULL; p=p->next )
	{
		if ( src == p->srcaddr )
			return 1;
	}
	return 0;
}

int check_src( __u32 src, __u32 *sources, int numsrc )
{
	int i;
	for (i=0;i< numsrc; i++)
	{
		if( src == sources[i] )
			return 1;
	}
	return 0;
}

void handle_igmpv3_isex( __u32 group, __u32 src, int srcnum, __u32 *grec_src )
{
	struct mcft_entry *mymcp;

	// Mason Yu Test
	//printf("handle_igmpv3_isex\n");

	if(!IN_MULTICAST(ntohl(group)))
		return;
	/* check if it's protocol reserved group */
	if((group&0xFFFFFF00)==0xE0000000)
		return;

	if(!chk_mcft(group))
	{
		//this group is not handled by this proxy
		return;
	}

	mymcp = get_mcft(group);
	if(mymcp)
	{
		switch( mymcp->filter_mode )
		{
		case MCAST_INCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				// Mason Yu Test
				//printf("handle_igmpv3_isex: MCAST_INCLUDE\n");
				//IN(A), IS_EX(B) => EX(A*B, B-A)
				old_set = mymcp->srclist;
				for(i=0;i<srcnum;i++)
				{
					s = add_to_srclist( mymcp, grec_src[i] );
					if(s)
					{	// (B-A)=0
						if( check_src_set( s->srcaddr, old_set )==0 )
						{
							s->timer.lefttime = 0;
							s->timer.retry_left = 0;
							igmp_add_mr( mymcp->grp_addr, s->srcaddr, 0 );
						}
					}
				}

				s = old_set;
				while(s)
				{
					struct src_entry *s_next=s->next;

					//Delete (A-B)
					if( check_src( s->srcaddr, grec_src, srcnum )==0 )
					{
						igmp_del_mr( mymcp->grp_addr, s->srcaddr );
						del_from_srclist( mymcp, s->srcaddr );
					}
					s = s_next;
				}

				//Group Timer=GMI
				mymcp->timer.lefttime = group_query_interval;
				mymcp->timer.retry_left = group_query_count;

				//set the new state
				mymcp->filter_mode = MCAST_EXCLUDE;
				igmp_set_srcfilter( mymcp );
			}
			break;
		case MCAST_EXCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				// Mason Yu Test
				//printf("handle_igmpv3_isex: MCAST_EXCLUDE\n");
#ifdef KEEP_GROUP_MEMBER
				add_user(mymcp, src);
#endif

				//EX(X,Y), IS_EX(A) => EX(A-Y, Y*A)
				old_set = mymcp->srclist;
				for(i=0;i<srcnum;i++)
				{
					s = add_to_srclist( mymcp, grec_src[i] );
					if(s)
					{	// (A-X-Y)=Group Timer
						if( check_src_set( s->srcaddr, old_set )==0 )
						{
							s->timer.lefttime = group_query_interval;
							s->timer.retry_left = group_query_count;
							igmp_add_mr( mymcp->grp_addr, s->srcaddr, 1 );
						}
					}
				}

				s = old_set;
				while(s)
				{
					struct src_entry *s_next=s->next;

					//Delete (X-A), Delete(Y-A)
					if( check_src( s->srcaddr, grec_src, srcnum )==0 )
					{
						igmp_del_mr( mymcp->grp_addr, s->srcaddr );
						del_from_srclist( mymcp, s->srcaddr );
					}
					s = s_next;
				}

				//Group Timer=GMI
				mymcp->timer.lefttime = group_query_interval;
				mymcp->timer.retry_left = group_query_count;

				//set the new state
				mymcp->filter_mode = MCAST_EXCLUDE;
				igmp_set_srcfilter( mymcp );
			}
			break;
		default:
			break;
		}
	}
}


void handle_igmpv3_isin( __u32 group, __u32 src, int srcnum, __u32 *grec_src )
{
	struct mcft_entry *mymcp;

	if(!IN_MULTICAST(ntohl(group)))
		return;
	/* check if it's protocol reserved group */
	if((group&0xFFFFFF00)==0xE0000000)
		return;

	if(!chk_mcft(group))
	{
		//this group is not handled by this proxy
		return;
	}

	mymcp = get_mcft(group);
	if(mymcp)
	{
		switch( mymcp->filter_mode )
		{
		case MCAST_INCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				//printf("handle_igmpv3_isin: MCAST_INCLUDE\n");
				//IN(A), IN(B) => IN(A+B)
				old_set = mymcp->srclist;
				for(i=0;i<srcnum;i++)
				{
					s = add_to_srclist( mymcp, grec_src[i] );
					if(s)
					{	// (B)= GMI
						s->timer.lefttime = group_query_interval;
						s->timer.retry_left = group_query_count;
						if( check_src_set( s->srcaddr, old_set )==0 )
							igmp_add_mr( mymcp->grp_addr, s->srcaddr, 1 );
					}
				}

				//set the new state
				mymcp->filter_mode = MCAST_INCLUDE;
				igmp_set_srcfilter( mymcp );
			}
			break;
		case MCAST_EXCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				//printf("handle_igmpv3_isin: MCAST_EXCLUDE\n");
				//EX(X,Y), IS_IN(A) => EX(X+A, Y-A)
				old_set = mymcp->srclist;
				for(i=0;i<srcnum;i++)
				{
					s = add_to_srclist( mymcp, grec_src[i] );
					if(s)
					{	// (A)= GMI
						s->timer.lefttime = group_query_interval;
						s->timer.retry_left = group_query_count;
						igmp_add_mr( mymcp->grp_addr, s->srcaddr, 1 );
					}
				}

				//set the new state
				mymcp->filter_mode = MCAST_EXCLUDE;
				igmp_set_srcfilter( mymcp );
			}
			break;
		default:
			break;
		}
	}
}

void handle_igmpv3_toin( __u32 group, __u32 src, int srcnum, __u32 *grec_src )
{
	struct mcft_entry *mymcp;

	if(!IN_MULTICAST(ntohl(group)))
		return;
	/* check if it's protocol reserved group */
	if((group&0xFFFFFF00)==0xE0000000)
		return;

	if(!chk_mcft(group))
	{
		if (srcnum == 0) //mean to repeatly leave message, drop it , added by chenzhihua
			return;
		mymcp = add_mcft(group, src);
		if(!mymcp) return;
		mymcp->igmp_ver = IGMP_VER_3;
#ifdef CONFIG_IGMP_PASSTHROUGH					
		igmp_add_group( group, src);
#else
		igmp_add_group( group );
#endif
	}

	mymcp = get_mcft(group);
	if(mymcp)
	{
		switch( mymcp->filter_mode )
		{
		case MCAST_INCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				// Mason Yu Test
				//printf("handle_igmpv3_toin: MCAST_INCLUDE\n");
#ifdef KEEP_GROUP_MEMBER
				if ( srcnum == 0 ) {
					int count;
					count = del_user(mymcp, src);
					if  ((count == 0) && enable_fast_leaving){// no member, drop it!
						del_mr(mymcp->grp_addr);
						del_mcft(mymcp->grp_addr);
						break;
					}
				}else
					add_user(mymcp, src);
#endif
				//IN(A), TO_IN(B) => IN(A+B)
				old_set = mymcp->srclist;
				for(i=0;i<srcnum;i++)
				{
					s = add_to_srclist( mymcp, grec_src[i] );
					if(s)
					{	// (B)= GMI
						s->timer.lefttime = group_query_interval;
						s->timer.retry_left = group_query_count;
						if( check_src_set( s->srcaddr, old_set )==0 )
							igmp_add_mr( mymcp->grp_addr, s->srcaddr, 1 );
					}
				}

				//send Q(G,A-B)
				i=0;
				s = old_set;
				while(s)
				{
					if( check_src( s->srcaddr, grec_src, srcnum )==0 )
					{
						gsrctmp[i]=s->srcaddr;
						i++;
						if(i==IGMPV3_MAX_SRCNUM) break;
					}
					s = s->next;
				}
				if(i>0) igmpv3_query( mymcp, i, gsrctmp );

				//set the new state
				mymcp->filter_mode = MCAST_INCLUDE;
				igmp_set_srcfilter( mymcp );
			}
			break;
		case MCAST_EXCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				// Mason Yu Test
				//printf("handle_igmpv3_toin: MCAST_EXCLUDE and srcnum=%d\n", srcnum);
#ifdef KEEP_GROUP_MEMBER
				if ( srcnum == 0 ) {
					int count;
					count = del_user(mymcp, src);
					if  ((count == 0) && enable_fast_leaving){// no member, drop it!
						del_mr(mymcp->grp_addr);
						del_mcft(mymcp->grp_addr);
						break;
					}
				}else
					add_user(mymcp, src);
#endif
				//EX(X,Y), TO_IN(A) => EX(X+A, Y-A)
				old_set = mymcp->srclist;
				for(i=0;i<srcnum;i++)
				{
					s = add_to_srclist( mymcp, grec_src[i] );
					if(s)
					{	// (A)= GMI
						s->timer.lefttime = group_query_interval;
						s->timer.retry_left = group_query_count;
						igmp_add_mr( mymcp->grp_addr, s->srcaddr, 1 );
					}
				}

				//send Q(G,X-A)
				i=0;
				s = old_set;
				while(s)
				{
					if( s->timer.lefttime>0 )
					{
						if( check_src( s->srcaddr, grec_src, srcnum )==0 )
						{
							gsrctmp[i]=s->srcaddr;
							i++;
							if(i==IGMPV3_MAX_SRCNUM) break;
						}
					}
					s = s->next;
				}
				if(i>0) igmpv3_query( mymcp, i, gsrctmp );
#ifdef KEEP_GROUP_MEMBER
				if (!enable_fast_leaving)
				{
#endif				
					if (srcnum == 0 && mymcp->user_count == 0)
					{
					/* 当组中最后一个成员退出时，网关根据最后成员查询间隔和最后成员查询次数发查询报文。 */
						mymcp->timer.lefttime = lm_query_interval;
						mymcp->timer.retry_left = lm_query_count;
					}else
					{
						//send Q(G)
						if( mymcp->igmp_ver==IGMP_VER_3 )
							igmpv3_query( mymcp, 0, NULL );
						else
							igmp_query(ALL_SYSTEMS, mymcp->grp_addr, group_query_response_interval);
					}						
#ifdef KEEP_GROUP_MEMBER					
				}
#endif				
				//set the new state
				mymcp->filter_mode = MCAST_EXCLUDE;
				igmp_set_srcfilter( mymcp );
			}
			break;
		default:
			break;
		}
	}
}


void handle_igmpv3_toex( __u32 group, __u32 src, int srcnum, __u32 *grec_src )
{
	struct mcft_entry *mymcp;

	if(!IN_MULTICAST(ntohl(group)))
		return;
	/* check if it's protocol reserved group */
	if((group&0xFFFFFF00)==0xE0000000)
		return;

	if(!chk_mcft(group))
	{
		mymcp = add_mcft(group, src);
		if(!mymcp) return;
		mymcp->igmp_ver = IGMP_VER_3;
#ifdef CONFIG_IGMP_PASSTHROUGH					
		igmp_add_group( group, src);
#else
		igmp_add_group( group );
#endif
	}

	mymcp = get_mcft(group);
	if(mymcp)
	{
		switch( mymcp->filter_mode )
		{
		case MCAST_INCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				//printf("handle_igmpv3_toex: MCAST_INCLUDE\n");
#ifdef KEEP_GROUP_MEMBER
				add_user(mymcp, src);
#endif
				//IN(A), TO_EX(B) => EX(A*B, B-A)
				old_set = mymcp->srclist;
				for(i=0;i<srcnum;i++)
				{
					s = add_to_srclist( mymcp, grec_src[i] );
					if(s)
					{	// (B-A)=0
						if( check_src_set( s->srcaddr, old_set )==0 )
						{
							s->timer.lefttime = 0;
							s->timer.retry_left = 0;
							igmp_add_mr( mymcp->grp_addr, s->srcaddr, 0 );
						}
					}
				}

				s = old_set;
				while(s)
				{
					struct src_entry *s_next=s->next;

					//Delete (A-B)
					if( check_src( s->srcaddr, grec_src, srcnum )==0 )
					{
						igmp_del_mr( mymcp->grp_addr, s->srcaddr );
						del_from_srclist( mymcp, s->srcaddr );
					}
					s = s_next;
				}

				//send Q(G,A*B)
				i=0;
				s = mymcp->srclist;
				while(s)
				{
					if( s->timer.lefttime > 0 )
					{
						gsrctmp[i]=s->srcaddr;
						i++;
						if(i==IGMPV3_MAX_SRCNUM) break;
					}
					s=s->next;
				}
				if(i>0) igmpv3_query( mymcp, i, gsrctmp );


				//Group Timer=GMI
				mymcp->timer.lefttime = group_query_interval;
				mymcp->timer.retry_left = group_query_count;

				//set the new state
				mymcp->filter_mode = MCAST_EXCLUDE;
				igmp_set_srcfilter( mymcp );
			}
			break;
		case MCAST_EXCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				//printf("handle_igmpv3_toex: MCAST_EXCLUDE\n");
#ifdef KEEP_GROUP_MEMBER
				add_user(mymcp, src);
#endif
				//EX(X,Y), TO_EX(A) => EX(A-Y, Y*A)
				old_set = mymcp->srclist;
				for(i=0;i<srcnum;i++)
				{
					s = add_to_srclist( mymcp, grec_src[i] );
					if(s)
					{	// (A-X-Y)=Group Timer
						if( check_src_set( s->srcaddr, old_set )==0 )
						{
							s->timer.lefttime = mymcp->timer.lefttime;
							s->timer.retry_left = group_query_count;
							igmp_add_mr( mymcp->grp_addr, s->srcaddr, 1 );
						}
					}
				}

				s = old_set;
				while(s)
				{
					struct src_entry *s_next=s->next;

					//Delete (X-A), Delete(Y-A)
					if( check_src( s->srcaddr, grec_src, srcnum )==0 )
					{
						igmp_del_mr( mymcp->grp_addr, s->srcaddr );
						del_from_srclist( mymcp, s->srcaddr );
					}
					s = s_next;
				}

				//send Q(G,A-Y)
				i=0;
				s = mymcp->srclist;
				while(s)
				{
					if( s->timer.lefttime > 0 )
					{
						gsrctmp[i]=s->srcaddr;
						i++;
						if(i==IGMPV3_MAX_SRCNUM) break;
					}
					s=s->next;
				}
				if(i>0) igmpv3_query( mymcp, i, gsrctmp );

				//Group Timer=GMI
				mymcp->timer.lefttime = group_query_interval;
				mymcp->timer.retry_left = group_query_count;

				//set the new state
				mymcp->filter_mode = MCAST_EXCLUDE;
				igmp_set_srcfilter( mymcp );
			}
			break;
		default:
			break;
		}
	}
}


void handle_igmpv3_allow( __u32 group, __u32 src, int srcnum, __u32 *grec_src )
{
	struct mcft_entry *mymcp;

	if(!IN_MULTICAST(ntohl(group)))
		return;
	/* check if it's protocol reserved group */
	if((group&0xFFFFFF00)==0xE0000000)
		return;

	if(!chk_mcft(group))
	{
		mymcp = add_mcft(group, src);
		if(!mymcp) return;
		mymcp->igmp_ver = IGMP_VER_3;
#ifdef CONFIG_IGMP_PASSTHROUGH					
		igmp_add_group( group, src);
#else
		igmp_add_group( group );
#endif
	}

	mymcp = get_mcft(group);
	if(mymcp)
	{
		switch( mymcp->filter_mode )
		{
		case MCAST_INCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				//printf("handle_igmpv3_allow: MCAST_INCLUDE\n");
				//IN(A), ALLOW(B) => IN(A+B)
				old_set = mymcp->srclist;
				for(i=0;i<srcnum;i++)
				{
					s = add_to_srclist( mymcp, grec_src[i] );
					if(s)
					{	// (B)= GMI
						s->timer.lefttime = group_query_interval;
						s->timer.retry_left = group_query_count;
						if( check_src_set( s->srcaddr, old_set )==0 )
							igmp_add_mr( mymcp->grp_addr, s->srcaddr, 1 );
					}
				}

				//set the new state
				mymcp->filter_mode = MCAST_INCLUDE;
				igmp_set_srcfilter( mymcp );
			}
			break;
		case MCAST_EXCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				//printf("handle_igmpv3_allow: MCAST_EXCLUDE\n");
				//EX(X,Y), ALLOW(A) => EX(X+A, Y-A)
				old_set = mymcp->srclist;
				for(i=0;i<srcnum;i++)
				{
					s = add_to_srclist( mymcp, grec_src[i] );
					if(s)
					{	// (A)= GMI
						s->timer.lefttime = group_query_interval;
						s->timer.retry_left = group_query_count;
						igmp_add_mr( mymcp->grp_addr, s->srcaddr, 1 );
					}
				}

				//set the new state
				mymcp->filter_mode = MCAST_EXCLUDE;
				igmp_set_srcfilter( mymcp );
			}
			break;
		default:
			break;
		}
	}
}

void handle_igmpv3_block( __u32 group, __u32 src, int srcnum, __u32 *grec_src )
{
	struct mcft_entry *mymcp;

	if(!IN_MULTICAST(ntohl(group)))
		return;
	/* check if it's protocol reserved group */
	if((group&0xFFFFFF00)==0xE0000000)
		return;

	if(!chk_mcft(group))
	{
		mymcp = add_mcft(group, src);
		if(!mymcp) return;
		mymcp->igmp_ver = IGMP_VER_3;
#ifdef CONFIG_IGMP_PASSTHROUGH					
		igmp_add_group( group, src);
#else
		igmp_add_group( group );
#endif
	}

	mymcp = get_mcft(group);
	if(mymcp)
	{
		switch( mymcp->filter_mode )
		{
		case MCAST_INCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				// Mason Yu Test
				//printf("handle_igmpv3_block: MCAST_INCLUDE\n");

				//IN(A), BLOCK(B) => IN(A)
				//send Q(G,A*B)
				i=0;
				s = mymcp->srclist;
				while(s)
				{
					if( check_src( s->srcaddr, grec_src, srcnum )==1 )
					{
						gsrctmp[i]=s->srcaddr;
#if 0
#ifdef KEEP_GROUP_MEMBER

						int count;
						count = del_user(mymcp, src);
						if ((count == 0) && enable_fast_leaving) {// no member, drop it!
							//del_mr(mymcp->grp_addr);
							igmp_del_group(mymcp->grp_addr);
							igmp_del_mr( mymcp->grp_addr, s->srcaddr );
							del_mcft(mymcp->grp_addr);
							return;
						}
#endif
#endif
						i++;
						if(i==IGMPV3_MAX_SRCNUM) break;
					}
					s=s->next;
				}
				if(i>0) igmpv3_query( mymcp, i, gsrctmp );


			}
			break;
		case MCAST_EXCLUDE:
			{
				int i;
				struct src_entry *s, *old_set;

				//printf("handle_igmpv3_block: MCAST_EXCLUDE\n");

				//EX(X,Y), BLOCK(A) => EX( X+(A-Y), Y )
				old_set = mymcp->srclist;
				for(i=0;i<srcnum;i++)
				{
					s = add_to_srclist( mymcp, grec_src[i] );
					if(s)
					{	// (A-X-Y)=Group Timer
						if( check_src_set( s->srcaddr, old_set )==0 )
						{
							s->timer.lefttime = mymcp->timer.lefttime;
							s->timer.retry_left = group_query_count;
							igmp_add_mr( mymcp->grp_addr, s->srcaddr, 1 );
						}
					}
				}

				//send Q(G,A-Y)
				i=0;
				s = mymcp->srclist;
				while(s)
				{
					if( s->timer.lefttime > 0 )
					{
						if( check_src( s->srcaddr, grec_src, srcnum )==1 )
						{
							gsrctmp[i]=s->srcaddr;
#if 0
#ifdef KEEP_GROUP_MEMBER

							int count;
							count = del_user(mymcp, src);
							if ((count == 0) && enable_fast_leaving){// no member, drop it!
								//del_mr(mymcp->grp_addr);
								igmp_del_group(mymcp->grp_addr);
								igmp_del_mr( mymcp->grp_addr, s->srcaddr );
								del_mcft(mymcp->grp_addr);
								return;
							}
#endif
#endif
							i++;
							if(i==IGMPV3_MAX_SRCNUM) break;
						}
					}
					s=s->next;
				}
				if(i>0) igmpv3_query( mymcp, i, gsrctmp );

				//set the new state
				mymcp->filter_mode = MCAST_EXCLUDE;
				igmp_set_srcfilter( mymcp );
			}
			break;
		default:
			break;
		}
	}
}

int igmpv3_query( struct mcft_entry *entry, int srcnum, __u32 *srcfilter )
{
    struct igmpv3_query	*igmpv3;
    struct sockaddr_in	sdst;
    struct IfDesc 	*dp = getIfByName(igmp_down_if_name);
    __u32	grp=0;
    int		i,totalsize=0;

    if(entry) grp=entry->grp_addr;

    igmpv3            = (struct igmpv3_query *)send_buf;
    igmpv3->type      = 0x11;
    igmpv3->code      = lm_query_interval;
    igmpv3->csum      = 0;
    igmpv3->group     = grp;
    igmpv3->resv      = 0;
    igmpv3->suppress  = 1;
    igmpv3->qrv       = 2;
    igmpv3->qqic      = group_query_interval;
    igmpv3->nsrcs     = srcnum;
    IGMPV3LOG( "%s> send to group:%s, src:", __FUNCTION__, inet_ntoa( grp ) );
    for(i=0;i<srcnum;i++)
    {
    	igmpv3->srcs[i] = srcfilter[i];
    	IGMPV3LOG( "(%s)", inet_ntoa( igmpv3->srcs[i] ) );
    }
    totalsize	      = sizeof(struct igmpv3_query)+igmpv3->nsrcs*sizeof(__u32);
    igmpv3->csum      = in_cksum((u_short *)igmpv3, totalsize );
    IGMPV3LOG( "\n" );


    bzero(&sdst, sizeof(struct sockaddr_in));
    sdst.sin_family = AF_INET;
    if(grp)
    	sdst.sin_addr.s_addr = grp;
    else
    	sdst.sin_addr.s_addr = ALL_SYSTEMS;

    if (sendto(dp->sock, igmpv3, totalsize, 0, (struct sockaddr *)&sdst, sizeof(sdst)) < 0)
    {
	printf("igmpv3_query> sendto error, from %s ", inet_ntoa(dp->InAdr.s_addr));
	printf("to %s\n", inet_ntoa(sdst.sin_addr.s_addr));
    }

    return 0;
}



void handle_group_timer(void)
{
	struct mcft_entry *p,*next;

	p = mcpq;
	next = NULL;
	while( p!=NULL )
	{
		next = p->next;

		if( p->timer.lefttime )
		{
			p->timer.lefttime--;
			if( (p->timer.lefttime==0) && (p->timer.retry_left!=0) )
			{
				p->timer.retry_left--;
				//if( p->timer.retry_left )
				//{
					IGMPV3LOG("%s> GROUP TIMEOUT, send Query to group:%s\n", __FUNCTION__, inet_ntoa(p->grp_addr) );

					p->timer.lefttime = lm_query_interval;
					if( p->igmp_ver==IGMP_VER_3 )
						igmpv3_query( p, 0, NULL );
					else
						igmp_query(ALL_SYSTEMS, p->grp_addr, group_query_response_interval);
				//}

			}
#ifdef CONFIG_APPS_LOGIC_STATIC_IGMP
			if ((p->timer.lefttime == 0) && (p->is_static == 1))
			{
				p->timer.lefttime = group_query_interval;
				p->timer.retry_left = group_query_count;
				IGMPV3LOG("static group 0x%x timeout, set to init station!\n", p->grp_addr);
			}
#endif				
			switch( p->filter_mode )
			{
			case MCAST_INCLUDE:
				break;
			case MCAST_EXCLUDE:
				if( p->timer.lefttime==0 )
				{
					struct src_entry *s, *s_next;

					IGMPV3LOG("%s> group:%s is timeout(EXCLUDE mode)\n", __FUNCTION__, inet_ntoa(p->grp_addr) );

					s=p->srclist;
					while(s)
					{
						s_next=s->next;
						if( s->timer.lefttime==0 )
						{
							//remove this source
							igmp_del_mr( p->grp_addr, s->srcaddr );
							del_from_srclist( p, s->srcaddr );
						}
						s=s_next;
					}

					if( p->srclist )
					{
						IGMPV3LOG("%s> group:%s changes to INCLUDE mode\n", __FUNCTION__, inet_ntoa(p->grp_addr) );

						p->filter_mode=MCAST_INCLUDE;
						igmp_set_srcfilter( p );
					}else{
						IGMPV3LOG("%s> remove group:%s\n", __FUNCTION__, inet_ntoa(p->grp_addr) );

						//delete this group record
						if(p->mrt_state)
						{
							igmp_del_mr( p->grp_addr, 0 );
							p->mrt_state=0;
						}
						igmp_del_group( p->grp_addr );
						del_mcft( p->grp_addr );
					}
				}
				break;
			default:
				break;
			}

		}

		p = next;
	}
}

void handle_src_timer(void)
{
	struct mcft_entry *p,*next;

	p = mcpq;
	next = NULL;
	while( p!=NULL )
	{
		struct src_entry *s, *src_next;
		int	change_sf=0;

		next = p->next;
		s = p->srclist;
		src_next = NULL;
		while( s )
		{
			src_next = s->next;

			if( s->timer.lefttime )
			{
				s->timer.lefttime--;
				if( (s->timer.lefttime==0) && (s->timer.retry_left!=0) )
				{
					s->timer.retry_left--;
					//if( s->timer.retry_left )
					//{
						IGMPV3LOG("%s> SRC TIMEOUT, send Query to group:%s", __FUNCTION__, inet_ntoa(p->grp_addr) );
						IGMPV3LOG(", src:%s\n", __FUNCTION__, inet_ntoa(s->srcaddr) );

						s->timer.lefttime = lm_query_interval;
						igmpv3_query( p, 1, &s->srcaddr );
					//}
				}


				switch( p->filter_mode )
				{
				case MCAST_INCLUDE:
					if( s->timer.lefttime )
					{
						//forward this src
						if(p->mrt_state)
						{
							IGMPV3LOG("%s> stop all sources for group:%s\n", __FUNCTION__, inet_ntoa(p->grp_addr) );
							igmp_del_mr( p->grp_addr, 0 );
							p->mrt_state=0;
						}
					}else{
						IGMPV3LOG("%s> remove src:%s", __FUNCTION__, inet_ntoa(s->srcaddr) );
						IGMPV3LOG(" from group:%s\n", __FUNCTION__, inet_ntoa(p->grp_addr) );

						//==0, stop this src
						igmp_del_mr( p->grp_addr, s->srcaddr );
						del_from_srclist( p, s->srcaddr );
						//NO MORE SOURCE, DELETE GROUP RECORD
						change_sf=1;
					}
					break;
				case MCAST_EXCLUDE:
					if( s->timer.lefttime )
					{
						//forward this src
						if(p->mrt_state)
						{
							IGMPV3LOG("%s> stop all sources for group:%s\n", __FUNCTION__, inet_ntoa(p->grp_addr) );
							igmp_del_mr( p->grp_addr, 0 );
							p->mrt_state=0;
						}
					}else{
						IGMPV3LOG("%s> stop forwarding src:%s", __FUNCTION__, inet_ntoa(s->srcaddr) );
						IGMPV3LOG(" for group:%s\n", __FUNCTION__, inet_ntoa(p->grp_addr) );

						//==0, stop this src, do not remove record
						igmp_add_mr( p->grp_addr, s->srcaddr, 0 );
						change_sf=1;
					}
					break;
				default:
					break;
				}
			}
			s = src_next;
		}

		//set the new state
		if(change_sf)	igmp_set_srcfilter( p );

		//EX( {}, X )
		if( (p->filter_mode==MCAST_EXCLUDE) && (p->srclist!=NULL) )
		{
			int allsrcinex=1;
			s = p->srclist;
			while(s)
			{
				if(s->timer.lefttime>0)
				{
					allsrcinex=0;
					break;
				}
				s=s->next;
			}
			if(allsrcinex==1)
			{
				if( p->mrt_state==0 )
				{
					IGMPV3LOG("%s> forward all sources for group:%s\n", __FUNCTION__, inet_ntoa(p->grp_addr) );

					igmp_add_mr( p->grp_addr, 0, 1 );
					p->mrt_state = 1;
				}
			}
		}

		//for empty condition
		if( p->srclist==NULL )
		{
			switch( p->filter_mode )
			{
			case MCAST_INCLUDE:
				//not foreward all source
				//delete this group record
				if(p->mrt_state)
				{
					IGMPV3LOG("%s> stop all sources for group:%s\n", __FUNCTION__, inet_ntoa(p->grp_addr) );

					igmp_del_mr( p->grp_addr, 0 );
					p->mrt_state=0;
				}
				IGMPV3LOG("%s> remove group:%s\n", __FUNCTION__, inet_ntoa(p->grp_addr) );
				igmp_del_group( p->grp_addr );
				del_mcft( p->grp_addr );
				break;
			case MCAST_EXCLUDE:
				//forward all source
				if( p->mrt_state==0 )
				{
					IGMPV3LOG("%s> forward all sources for group:%s\n", __FUNCTION__, inet_ntoa(p->grp_addr) );

					igmp_add_mr( p->grp_addr, 0, 1 );
					p->mrt_state = 1;
				}
				break;
			default:
				break;
			}
		}

		p = next;
	}
}



static struct timeval start_time={0,0};
static int init_stat_time=0;
void igmpv3_timer(void)
{
	struct timeval cur_time;

	if( init_stat_time==0 )
	{
		gettimeofday( &start_time, NULL );
		init_stat_time = 1;
		return;
	}



	gettimeofday( &cur_time, NULL );

	/*由于当前每次循环执行时间小于0.1秒，导致每秒会多执行一次循环，
	从而造成计时增加10%左右，所以对一秒的判断提前0.1秒*/
	if( (cur_time.tv_sec > (start_time.tv_sec+1)) ||
	    ((cur_time.tv_sec == (start_time.tv_sec+1)) && ((cur_time.tv_usec+100000) >=start_time.tv_usec)) )
	{
		//suppose 1 second passed
		//printf( "." );fflush(NULL);
#ifdef IGMP_GENERAL_QUERY		
		handle_general_timer();
#endif
		handle_group_timer();
		handle_src_timer();

		start_time.tv_sec = cur_time.tv_sec;
		start_time.tv_usec = cur_time.tv_usec;
	}

	return;
}


#ifdef CONFIG_APPS_LOGIC_STATIC_IGMP

struct c_static_group
{
	struct c_static_group *next;
	__u32 group;
	__u32 source;
};

struct c_static_group *prev_sglst = NULL;

int add_static_group(__u32 group, __u32 src)
{
	struct mcft_entry *mymcp;
	
	mymcp = get_mcft(group);
	if (mymcp == NULL)
	{
		mymcp = add_mcft(group, src);
		if(!mymcp) return -1;
#ifdef CONFIG_IGMP_PASSTHROUGH					
		igmp_add_group( group, src);
#else
		igmp_add_group( group );
#endif
	}

	mymcp->is_static = 1;
	mymcp->igmp_ver = IGMP_VER_2;

	IGMPV3LOG("group 0x%x set to static, with src 0x%x\n", group, src);

	//Report => IS_EX( {} )
	handle_igmpv3_isex(group, src, 0, NULL );
	return 1;
}


int del_static_group(__u32 group, __u32 src)
{
	struct mcft_entry *mymcp;
	
	mymcp = get_mcft(group);
	if ((mymcp == NULL) || (mymcp->is_static == 0))
		return -1;

	mymcp->is_static = 0;
	handle_igmpv3_toin( group,src, 0, NULL );
	return 1;
}

void del_all_prev_static_group()
{
	struct c_static_group *p = prev_sglst;
	struct c_static_group *q;
	while(p != NULL)
	{
		q = p->next;
		del_static_group(p->group, p->source);
		free(p);
		p = q;
	}
	prev_sglst = NULL;
}

void add_to_sglst(__u32 group, __u32 souce)
{
	struct c_static_group *p = prev_sglst;
	struct c_static_group *n;
	
	while (p != NULL)
	{
		if ((p->group == group) && (p->source == souce))
			break;
		p = p->next;
	}
	if (p == NULL)
	{
		n = malloc(sizeof(struct c_static_group));
		if (n == NULL) return;
		memset(n, 0, sizeof(struct c_static_group));

		n->group = group;
		n->source = souce;
		n->next = prev_sglst;
		prev_sglst = n;
	}
}

#define STATIC_GROUP_FILE	"/tmp/static_group"

void get_static_group_file(int signum)
{
	FILE * fd;
	char linebuf[255];
	char group[20] = {0}, source[20] = {0}, upif[20] = {0}, downif[20] = {0}, phyif[20] = {0};

	if((fd = fopen(STATIC_GROUP_FILE, "r")) == NULL)
	{
		IGMPV3LOG("open static group file failed: %s\n!", STATIC_GROUP_FILE);
		return;
	}

	del_all_prev_static_group();

	printf("get static group config:\n");
	while(!feof(fd))
	{
		memset(linebuf, 0, sizeof(linebuf));
		fgets(linebuf, sizeof(linebuf), fd);
		
		if (sscanf(linebuf, "%s %s %s %s %s", group, source, upif, downif, phyif) > 0)
		{
			printf("	group = %s, source = %s, upif = %s, downif = %s, phyif = %s\n", group, source, upif, downif, phyif);
			add_to_sglst(inet_addr(group),inet_addr(source));
			add_static_group(inet_addr(group),inet_addr(source));
		}
	}

	fclose(fd);

	return;
}

#endif

int igmpv3_accept(int recvlen, struct IfDesc *dp)
{
	register __u32 src, dst, group;
	struct iphdr *ip;
	struct igmphdr *igmp;
	int ipdatalen, iphdrlen;
	struct mcft_entry *mymcp;

	if (recvlen < sizeof(struct iphdr))
	{
		LOG(LOG_WARNING, 0, "received packet too short (%u bytes) for IP header", recvlen);
		return 0;
	}

	ip  = (struct iphdr *)recv_buf;
	src = ip->saddr;
	dst = ip->daddr;

	if(!IN_MULTICAST(ntohl(dst)))	/* It isn't a multicast */
		return -1;
	if(chk_local(src)) 	/* It's our report looped back */
		return -1;
	if(dst == ALL_PRINTER)	/* It's MS-Windows UPNP all printers notify */
		return -1;

	//pkt_debug(recv_buf);

	iphdrlen  = ip->ihl << 2;
	ipdatalen = ip->tot_len;

	igmp    = (struct igmphdr *)(recv_buf + iphdrlen);
	group   = igmp->group;

	/* determine message type */
	IGMPV3LOG("\n%s> receive IGMP type [%x] from %s to ", __FUNCTION__, igmp->type, inet_ntoa(ip->saddr));
	IGMPV3LOG("%s\n", inet_ntoa(ip->daddr));
	switch (igmp->type) {
		case IGMP_HOST_MEMBERSHIP_QUERY:
			/* Linux Kernel will process local member query, it wont reach here */
			break;

		case IGMP_HOST_MEMBERSHIP_REPORT:
		case IGMPV2_HOST_MEMBERSHIP_REPORT:
			{
				IGMPV3LOG("%s> REPORT(V1/V2), group:%s\n", __FUNCTION__, inet_ntoa(group) );
				if(!chk_mcft(group))
				{
					mymcp = add_mcft(group, src);
					if(!mymcp) return -1;
					mymcp->igmp_ver = IGMP_VER_2;
#ifdef CONFIG_IGMP_PASSTHROUGH					
					igmp_add_group( group, src);
#else
					igmp_add_group( group );
#endif

					//Group Timer=GMI
					mymcp->timer.lefttime = group_query_interval;
					mymcp->timer.retry_left = group_query_count;

					//set the new state
					mymcp->filter_mode = MCAST_EXCLUDE;
					igmp_set_srcfilter( mymcp );
				}

				mymcp = get_mcft(group);
				if(mymcp) mymcp->igmp_ver = IGMP_VER_2;

				//Report => IS_EX( {} )
				handle_igmpv3_isex( group,src, 0, NULL );
			}
			break;
 		case IGMPV3_HOST_MEMBERSHIP_REPORT:
		     {
			struct igmpv3_report *igmpv3;
			struct igmpv3_grec *igmpv3grec;
			__u16 rec_id;

			IGMPV3LOG("%s> REPORT(V3)\n", __FUNCTION__ );
			igmpv3 = (struct igmpv3_report *)igmp;
			//printf( "recv IGMPV3_HOST_MEMBERSHIP_REPORT\n" );
			//printf( "igmpv3->type:0x%x\n", igmpv3->type );
			//printf( "igmpv3->ngrec:0x%x\n", ntohs(igmpv3->ngrec) );

			rec_id=0;
			igmpv3grec =  &igmpv3->grec[0];
			while( rec_id < ntohs(igmpv3->ngrec) )
			{
				int srcnum;
				//printf( "igmpv3grec[%d]->grec_type:0x%x\n", rec_id, igmpv3grec->grec_type );
				//printf( "igmpv3grec[%d]->grec_auxwords:0x%x\n", rec_id, igmpv3grec->grec_auxwords );
				//printf( "igmpv3grec[%d]->grec_nsrcs:0x%x\n", rec_id, ntohs(igmpv3grec->grec_nsrcs) );
				//printf( "igmpv3grec[%d]->grec_mca:%s\n", rec_id, inet_ntoa(igmpv3grec->grec_mca) );

				group = igmpv3grec->grec_mca;
				srcnum = ntohs(igmpv3grec->grec_nsrcs);

				switch( igmpv3grec->grec_type )
				{
				case IGMPV3_MODE_IS_INCLUDE:
					IGMPV3LOG("%s> IS_IN, group:%s, srcnum:%d\n", __FUNCTION__, inet_ntoa(group), srcnum );
					handle_igmpv3_isin( group,src, srcnum, igmpv3grec->grec_src );
					break;
				case IGMPV3_MODE_IS_EXCLUDE:
					IGMPV3LOG("%s> IS_EX, group:%s, srcnum:%d\n", __FUNCTION__, inet_ntoa(group), srcnum );
					handle_igmpv3_isex( group,src, srcnum, igmpv3grec->grec_src );
					break;
				case IGMPV3_CHANGE_TO_INCLUDE:
					IGMPV3LOG("%s> TO_IN, group:%s, srcnum:%d\n", __FUNCTION__, inet_ntoa(group), srcnum );
					handle_igmpv3_toin( group,src, srcnum, igmpv3grec->grec_src );
					break;
				case IGMPV3_CHANGE_TO_EXCLUDE:
					IGMPV3LOG("%s> TO_EX, group:%s, srcnum:%d\n", __FUNCTION__, inet_ntoa(group), srcnum );
					handle_igmpv3_toex( group,src, srcnum, igmpv3grec->grec_src );
					break;
				case IGMPV3_ALLOW_NEW_SOURCES:
					IGMPV3LOG("%s> ALLOW, group:%s, srcnum:%d\n", __FUNCTION__, inet_ntoa(group), srcnum );
					handle_igmpv3_allow( group,src, srcnum, igmpv3grec->grec_src );
					break;
				case IGMPV3_BLOCK_OLD_SOURCES:
					IGMPV3LOG("%s> BLOCK, group:%s, srcnum:%d\n", __FUNCTION__, inet_ntoa(group), srcnum );
					handle_igmpv3_block( group,src, srcnum, igmpv3grec->grec_src );
					break;
				default:
					IGMPV3LOG("%s> Unknown Group Record Types [%x]\n", __FUNCTION__, igmpv3grec->grec_type );
					break;
				}

				rec_id++;
				//printf( "count next: 0x%x %d %d %d %d\n", igmpv3grec, sizeof( struct igmpv3_grec ), igmpv3grec->grec_auxwords, ntohs(igmpv3grec->grec_nsrcs), sizeof( __u32 ) );
				igmpv3grec = (struct igmpv3_grec *)( (char*)igmpv3grec + sizeof( struct igmpv3_grec ) + (igmpv3grec->grec_auxwords+ntohs(igmpv3grec->grec_nsrcs))*sizeof( __u32 ) );
				//printf( "count result: 0x%x\n", igmpv3grec );
			}
			break;
		     }
		case IGMP_HOST_LEAVE_MESSAGE :
			IGMPV3LOG("%s> LEAVE(V2), group:%s\n", __FUNCTION__, inet_ntoa(group) );
			if(chk_mcft(group))
			{
				//Leave => TO_IN( {} )
				handle_igmpv3_toin( group,src, 0, NULL );
			}
			break;
		default:
			IGMPV3LOG("%s> receive IGMP Unknown type [%x]\n", __FUNCTION__, igmp->type );
			break;
    }
    return 0;
}


#endif /*_SUPPORT_IGMPV3_*/
