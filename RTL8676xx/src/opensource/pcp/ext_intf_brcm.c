#include "glbvars.h"
#include "ext_intf.h"
#include "cms_util.h"
#include "cms_msg.h"
#include "pcp.h"
#include "options.h"
#include "timer.h"

#define PCP_INIT "INIT"
#define PCP_ACTIVE "ACTIVE"
#define PCP_FAIL "FAIL"

enum PCP_STATE_CODE {
	PCP_INIT_CODE = 0,
	PCP_ACTIVE_CODE = 2,
	PCP_FAIL_CODE = 3
};

enum PCP_OPTION
{
	THIRD_PARTY_CODE = 1,
	PREFER_FAILURE_CODE = 2,
	FILTER_CODE = 3
};

typedef struct
{
   UINT16 protocol;     /**< Have we been assigned dns server addresses ? */
   UINT16 in_port;
   UINT16 ext_port;
   struct in6_addr addr6;
} PCPMap_t;

typedef struct
{
   UINT16 protocol; 
   UINT16 in_port;
   UINT16 ext_port;
   UINT16 rm_port;
   struct in6_addr addr6;
   struct in6_addr rm_addr6;
} PCPPeer_t;

typedef struct
{
   UINT16 code;     /**< option code */
   UINT16 length;
} PCPOption_t;

static void *msgHandle = NULL;
static int comm_fd;
extern pcp_t p;

void ext_intf_init() {
   if ((cmsMsg_init(EID_PCP, &msgHandle)) != CMSRET_SUCCESS)
   {
      exit(1);
   }
}

void ext_intf_fd(int *max_fd, fd_set *rfds) {
	cmsMsg_getEventHandle(msgHandle, &comm_fd);
	FD_SET(comm_fd, rfds);
	if (comm_fd > *max_fd)
		*max_fd = comm_fd;
}

int ext_event(fd_set *r)
{
	CmsMsgHeader *msg = NULL;
	PCPMsgBody *pcp_msg = NULL;
	PCPPeer_t *peer = NULL;
	struct pcp_oper *oper;

	if (FD_ISSET(comm_fd, r)) {
		if ((cmsMsg_receive(msgHandle, &msg)) != CMSRET_SUCCESS)
			return -1;
	}
    
	switch (msg->type) {
	case CMS_MSG_IPV6_PCP:
		pcp_msg = (PCPMsgBody *)(msg+1);
		switch (pcp_msg->code) {
		case O_MAP:
		{
			int len = 0;
			uint8_t *position = NULL;
			PCPMap_t *map = (PCPMap_t *)(pcp_msg+1);
			position = (uint8_t *)map;
			map_t *mt = NULL;
			struct pcp_option *opt = NULL;
			PCPOption_t *tmp = NULL;
			len = msg->dataLength - sizeof(PCPMsgBody) -sizeof(PCPMap_t);
			if (len < 0)
				goto fail;

			TAILQ_FOREACH(oper, &oper_hd, link) {
				if (oper->o_type == O_MAP) {
					mt = (map_t *)oper->data;
					if ((mt->protol == map->protocol) &&
							(mt->in_port == map->in_port) &&
							(mt->ext_port == map->ext_port) &&
							!strncmp((const char *)&mt->addr6, (const char *)&map->addr6, sizeof(struct in6_addr))) {
					    if (pcp_msg->action == PCP_DEL) {
                            /* 如果是当前运行的pcp，则应该释放当前指针 */
                            if (p.timer &&
                                (oper == p.timer->expire_data)) {
                                p.timer = NULL;
                            }
                            TAILQ_REMOVE(&oper_hd, oper, link);
                            pcp_remove_timer(&oper->timer);
                            free(oper);
                        }
                        else {
                            pcp_remove_timer(&oper->timer);
    						oper->o_state = pcp_msg->state;
    						mt->ext_port = map->ext_port;
    						oper->lifetime == pcp_msg->lifetime;
                            mt->addr6 = map->addr6;
                            pcp_add_timer(&oper->timer, &oper);
                        }

#if 0
						if ((oper->o_state == PCP_ACTIVE_CODE) && (pcp_msg->lifetime == 0)) {
							oper->o_state = PCP_INIT_CODE; /* delete request */ 
							pcp_remove_timer(&oper->timer);
						}
						else
							mt->addr6 = map->addr6;
#endif

						break;
					}
				}
			}
			if ((oper == NULL) && (pcp_msg->action == PCP_ADD)) { /* can not find the related operation */ 
				oper = malloc(sizeof(struct pcp_oper));
				if (oper == NULL)
					goto fail;
				else {
					memset(oper, 0, sizeof(struct pcp_oper));
					oper->o_type = O_MAP;
					oper->o_state = pcp_msg->state;
					oper->lifetime = pcp_msg->lifetime;
					if ((mt = malloc(sizeof(map_t))) == NULL) {
						free(oper);
						goto fail;
					}
					else {
						memset(mt, 0, sizeof(map_t));
						SLIST_INIT(&mt->opt_h);
						mt->protol = map->protocol;
						mt->in_port = map->in_port;
						mt->ext_port = map->ext_port;
						in6_addr_assign(&mt->addr6, &map->addr6);
						oper->data = mt;
					}
				}
				//TAILQ_INSERT_TAIL(&oper_hd, oper, link);
			//}
			position = (uint8_t *)(map+1);
			while((len -= sizeof(PCPOption_t)) >= 0) {
				tmp = (PCPOption_t *)(position);
				len -= tmp->length;
				if (len >= 0) {
					opt = (struct pcp_option *)malloc_option(tmp->code, tmp->length, /*(tmp+1)*/
                        (tmp->length == 0)?NULL:(tmp+1));
                    add_option(O_MAP, &mt->opt_h, opt);
					position += sizeof(PCPOption_t) + tmp->length;
				}
				else
					break;
			}
            TAILQ_INSERT_TAIL(&oper_hd, oper, link);
            }
			break;
		}
		case O_PEER:
		{
			int len = 0;
			uint8_t *position = NULL;

			peer = (PCPPeer_t *)(pcp_msg+1);
			position = peer;
			peer_t *pt = NULL;
			struct pcp_option *opt = NULL;
			PCPOption_t *tmp = NULL;
			len = msg->dataLength - sizeof(PCPMsgBody) -sizeof(PCPPeer_t);
			if (len < 0)
				goto fail;
			TAILQ_FOREACH(oper, &oper_hd, link) {
				if (oper->o_type == O_PEER) {
					pt = (peer_t *)oper->data;
					if ((pt->protol == peer->protocol) &&
							(pt->in_port == peer->in_port) &&
							(pt->ext_port == peer->ext_port) &&
							(pt->rm_port == peer->rm_port) &&
							in6_addr_equal(&pt->rm_addr6, &peer->rm_addr6)){
					    if (pcp_msg->action == PCP_DEL) {
                            /* 如果是当前运行的pcp，则应该释放当前指针 */
                            if (p.timer &&
                                (oper == p.timer->expire_data)) {
                                p.timer = NULL;
                            }

                            TAILQ_REMOVE(&oper_hd, oper, link);
                            pcp_remove_timer(&oper->timer);
                            free(oper);
                        }
                        else {
                            pcp_remove_timer(&oper->timer);
    						oper->o_state = pcp_msg->state;
    						pt->ext_port = peer->ext_port;
    						in6_addr_assign(&pt->addr6, &peer->addr6);
                            pcp_add_timer(&oper->timer, &oper);
                        }
                        break;
					}
				}
			}
			if ((oper == NULL) && (pcp_msg->action == PCP_ADD)) { /* can not find the related operation */
				oper = malloc(sizeof(struct pcp_oper));
				if (oper == NULL)
					goto fail;
				else {
					memset(oper, 0, sizeof(struct pcp_oper));
					oper->o_type = O_PEER;
					oper->o_state = pcp_msg->state;
					oper->lifetime = pcp_msg->lifetime;
					if ((pt = malloc(sizeof(peer_t))) == NULL) {
						free(oper);
						goto fail;
					}
					else {
						memset(pt, 0, sizeof(peer_t));
						SLIST_INIT(&pt->opt_h);
						pt->protol = peer->protocol;
						pt->in_port = peer->in_port;
						pt->ext_port = peer->ext_port;
						pt->rm_port = peer->rm_port;
						in6_addr_assign(&pt->addr6, &peer->addr6);
						in6_addr_assign(&pt->rm_addr6, &peer->rm_addr6);
						oper->data = pt;
					}
				}
				//TAILQ_INSERT_TAIL(&oper_hd, oper, link);
			//}
			position = (peer+1);

			while((len -= sizeof(PCPOption_t)) >= 0) {
				tmp = (PCPOption_t *)(position);
				len -= tmp->length;
				if (len >= 0) {
					opt = malloc_option(tmp->code, tmp->length, (tmp->length == 0)?NULL:(tmp+1)/*(tmp+1)*/);
					add_option(O_PEER, &pt->opt_h, opt);
					position += sizeof(PCPOption_t) + tmp->length;
				}
				else
					break;
			}
            TAILQ_INSERT_TAIL(&oper_hd, oper, link);
            }
			break;
		}
		default:
			;
		}
	default:
		;
	}
	CMSMEM_FREE_BUF_AND_NULL_PTR(msg);
	return PCP_OK;

fail:
		CMSMEM_FREE_BUF_AND_NULL_PTR(msg);
	return -1;
}

void ext_onotify(struct pcp_oper *oper)
{
	CmsRet ret;
	CmsMsgHeader *msg;
	uint8_t *msgBuf;
	int len = sizeof(PCPMsgBody);

	switch (oper->o_type) {
	case O_MAP:
		len = sizeof(PCPMap_t);
	case O_PEER:
		len = sizeof(PCPPeer_t);
	}

	msg->src = MAKE_SPECIFIC_EID(getpid(), EID_PCP);
	msg->dst = EID_SSK;
	msg->type = CMS_MSG_IPV6_PCP;
	msg->wordData = 0;
	msg->dataLength  = len;
	msg = (CmsMsgHeader *)msgBuf;

	len = sizeof(CmsMsgHeader);

	PCPMsgBody *pcp_msg =  (PCPMsgBody *)(&msgBuf[len]);
	pcp_msg->code = O_MAP;
	pcp_msg->state = oper->o_state;
	pcp_msg->lifetime = oper->lifetime;

	len += sizeof(PCPMsgBody);

	switch (oper->o_type) {
		case O_MAP:
			{
				PCPMap_t *map =  (PCPMap_t *)(&msgBuf[len]);
				map_t *mt = (map_t *)oper->data;
				map->protocol = mt->protol;
				map->in_port = mt->in_port;
				map->ext_port = mt->ext_port;
				map->addr6 = mt->addr6;
			}
		case O_PEER:
			{
				PCPPeer_t *peer =  (PCPPeer_t *)(&msgBuf[len]);
				peer_t *pt = (peer_t *)oper->data;
				peer->protocol = pt->protol;
				peer->in_port = pt->in_port;
				peer->ext_port = pt->ext_port;
				peer->addr6 = pt->addr6;
				peer->rm_port = pt->rm_port;
				peer->rm_addr6 = pt->rm_addr6;
			}

	}

	ret = cmsMsg_send(msgHandle, msg);
	CMSMEM_FREE_BUF_AND_NULL_PTR(msgBuf);
}

