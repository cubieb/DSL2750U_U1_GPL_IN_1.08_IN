#ifndef _MLD_FUNC_H_
#define _MLD_FUNC_H_

#include "mld_inc.h"
#include "mld_proxy.h"
#include "mld.h"

/***********************************************************************
                    mld_proxy_api.c
***********************************************************************/
mld_rep_t* mld_rep_create( struct in6_addr * pSrcaddr);
int mld_rep_count( mld_group_t *gp);
void mld_rep_cleanup( mld_group_t *gp, mld_rep_t * rep);
mld_rep_t* mld_group_rep_lookup( mld_group_t *gp, struct in6_addr * pSrcaddr);
mld_rep_t* mld_group_rep_add( mld_group_t *gp, struct in6_addr * pSrcaddr);
mld_src_t * mld_group_rep_lookup_src(mld_rep_t *rep, int filter);
mld_src_rep_t* mld_src_rep_create( mld_rep_t *rep);
int mld_src_rep_count( mld_src_t *src);
mld_src_rep_t* mld_group_src_rep_lookup( mld_src_t* src, mld_rep_t* rep);
void mld_src_rep_cleanup( mld_src_t* src, mld_rep_t * rep);
mld_src_rep_t* mld_group_src_rep_add( mld_src_t *src, mld_rep_t *rep);
mld_rep_src_t* mld_rep_src_create( mld_src_t *src);
int mld_rep_src_count( mld_rep_t *rep);
mld_rep_src_t* mld_group_rep_src_lookup( mld_rep_t* rep, mld_src_t* src);
void mld_rep_src_cleanup( mld_rep_t * rep, mld_src_t* src);
mld_rep_src_t* mld_group_rep_src_add( mld_rep_t *rep, mld_src_t *src);
mld_src_t* mld_src_create(struct in6_addr  * psrcaddr);
void mld_src_cleanup( mld_group_t *gp, mld_src_t * src, int filter);
void mld_src_print( mld_src_t* src);
mld_src_t* mld_group_src_lookup( mld_group_t *gp, struct in6_addr * srcaddr, int filter);
mld_src_t* mld_group_src_add( mld_group_t *gp, struct in6_addr * srcaddr, int filter);
mld_group_t* mld_group_create( struct in6_addr * pGroupaddr);
void mld_group_cleanup( mld_interface_t *ifp, mld_group_t* gp, mld_router_t* router);
void mld_group_print( mld_group_t* gp);
void mld_interface_print( mld_interface_t* ifp);
void mldrt_print(mld_router_t* mldrt);

/***********************************************************************
                    mld_proxy.c
***********************************************************************/
void mld_info_print(mld_router_t *router);
void mld_memberdb_info_print(mld_router_t *router);
mld_interface_t* mld_interface_create( struct in6_addr *  pIfaddr, char *ifname, mifi_t index);
void mld_interface_cleanup(mld_interface_t* ifp);
mld_group_t* mld_interface_group_add( mld_router_t* router, mld_interface_t *ifp, struct in6_addr * pGroupaddr);
mld_group_t* mld_interface_group_lookup( mld_interface_t *ifp, struct in6_addr * pGroupaddr);
void mld_interface_membership_report_v1( mld_router_t* router,	mld_interface_t* ifp, struct in6_addr * pSrc, mldv1_report_t * report, int len, char *gpinfo);
void mld_interface_membership_leave_v1( mld_router_t* router,   mld_interface_t* ifp, struct in6_addr * pSrc, mldv1_report_t * report, int len);
void mld_interface_membership_report_v2( mld_router_t* router, mld_interface_t* ifp, struct in6_addr * pSrc, mldv2_report_t* report, int len, char *gpinfo);
int mldrt_init( mld_router_t* mldrt);
mld_interface_t* mldrt_interface_add( mld_router_t* mldrt, struct in6_addr * pIfaddr, char *ifname, mifi_t index);
void mldrt_timer(mld_router_t* mldrt);
void* mldrt_timer_thread(void* arg);
void mldrt_input(mld_router_t* mldrt, mld_interface_t* ifp);
void* mldrt_input_thread(void* arg);
void mldrt_start(mld_router_t* mldrt);
void mldrt_stop(mld_router_t* mldrt);


/***********************************************************************
                    mld.c
***********************************************************************/
void mld_group_handle_allow(
			mld_router_t *router,
			mld_interface_t *ifp,
			mld_group_t *gp,
			int numsrc,
			struct in6_addr *sources,
			mld_rep_t *rep,
			char *gpinfo);

void mld_group_handle_block(
			mld_router_t *router,
			mld_interface_t *ifp,
			mld_group_t *gp,
			int numsrc,
			struct in6_addr *sources,
			mld_rep_t *rep,
			char *gpinfo);

void mld_group_handle_toex(
			    mld_router_t *router,
			    mld_interface_t *ifp,
			    mld_group_t *gp,
			    int numsrc,
			    struct in6_addr *sources,
			    mld_rep_t *rep,
			    char *gpinfo);

void mld_group_handle_toin(
			    mld_router_t *router,
			    mld_interface_t *ifp,
			    mld_group_t *gp,
			    int numsrc,
			    struct in6_addr *sources,
			    mld_rep_t *rep,
			    char *gpinfo);
void
mld_group_handle_isex(
			mld_router_t* router,
			mld_interface_t* ifp,
			mld_group_t* gp,
			int numsrc,
			struct in6_addr *sources,
			mld_rep_t *rep, 
			char *gpinfo);

void
mld_group_handle_isin(
			mld_router_t* router,
			mld_interface_t* ifp,
			mld_group_t* gp,
			int numsrc,
			struct in6_addr *sources,
			mld_rep_t *rep,
			char *gpinfo);

void mldrt_timer_querier(mld_interface_t *ifp);
void mldrt_timer_group(mld_router_t* router,mld_interface_t *ifp);
void mldrt_timer_source (mld_router_t* router,mld_interface_t *ifp);

void send_sh_query(mld_router_t *router, mld_interface_t *ifp);
void mld_update_group_src_reporters(mld_router_t *router, 
                                     mld_group_t *gp);
void
mldrt_membership_query(mld_router_t* mldrt, 
                       mld_interface_t* ifp,
                       struct in6_addr *group, 
                       struct in6_addr *sources, 
                       int numsrc, 
                       int SRSP);

/***********************************************************************
                    mld_util.c
***********************************************************************/
int mld_get_ipv6_addr_by_ifidx(int if_index, struct sockaddr_in6 *saddr6);
void dumpBuf(char *buf, int len);
void mld_print_ipv6_addr(struct in6_addr *addr6);
interface_list_t* get_interface_list(short flags, short unflags);
void free_interface_list(interface_list_t *ifl);
short get_interface_flags(char *ifname);
short set_interface_flags(char *ifname, short flags);
int get_interface_mtu(char *ifname);


/***********************************************************************
                    mld_krnlif.c
***********************************************************************/
void set_source_filter(
                        mld_router_t* router,
                        mld_interface_t *ifp,
                        mld_group_t* gp,
                        int fmode,
                        int nsources,
                        struct in6_addr *sources);
void k_init_proxy(int socket);
void k_stop_proxy(int socket);
int k_proxy_add_mif (int socket, mifi_t mifi);
int k_proxy_del_mif(int socket, mifi_t mifi);
int k_proxy_del_mfc (int socket, struct in6_addr * pSource, struct in6_addr * pGroup);
int k_proxy_chg_mfc(int socket, struct in6_addr * source, struct in6_addr * pGroup, mifi_t outvif);
void mld_update_snooping_info(mld_group_t *gp, 
                              int mode, 
                              char *gpinfo,
                              struct in6_addr *src_ip);

#endif /* _MLD_FUNC_H_ */
