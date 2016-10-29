  
/******************************************************************************
  Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
  
  文件名称: msg4udp.h
  文件描述: 
  函数列表: //列出主要函数
  修订记录:
         1. 作者: Hansel He < heyiming@twsz.com>
            日期: 2008-8-11
            内容: 创建文件
 ******************************************************************************/


#ifndef __MSG4UDP_H__
#define __MSG4UDP_H__

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif

#include "wps_com.h"

//#define  M4U_DEBUG
/****************************************************************************** 
 *                                 MACRO                                      * 
 ******************************************************************************/
#define  MAX_NODE				16
#define  M4U_SLEEP				200000    /* us */

//#define	 M4U_PRINT(format, args...)		printf("[msg4udp] ==> "format, ##args)
#define	 M4U_PRINT(format, args...)		printf("[%s:%d:%s()] ==> "format, __FILE__, __LINE__, __FUNCTION__, ##args)

#ifdef M4U_DEBUG
#define  M4U_TRACE(format, args...)			M4U_PRINT(format, ##args)
#else
#define  M4U_TRACE(format, args...)
#endif


/* M4U_NODE flag definitions */
#define M4U_NODE_BIND			0x1				/* bind to udp port */

typedef   long  M4U_RET;
/****************************************************************************** 
 *                                 ENUM                                       * 
 ******************************************************************************/


/****************************************************************************** 
 *                                STRUCT                                      * 
 ******************************************************************************/
typedef struct tag_M4U_Node
{
	unsigned short mid;					/* MID of node */
	int flag;	
	int sock;
	struct sockaddr_in srv_addr;
	unsigned long msg_id;						/* last message id (MSGID) */
}M4U_Node;

typedef struct tag_M4U_Nodes
{
	M4U_Node *nodes[MAX_NODE];
}M4U_Nodes;
/****************************************************************************** 
 *                               FUNCTION                                     * 
 ******************************************************************************/
int M4U_RegisterUDP(int MID, char* udp_ip, int udp_port, int bind_port, int flag);
void M4U_UnRegisterUDP(int handle);

/****************************************************************************** 
 *                                 END                                        * 
 ******************************************************************************/

#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif

#endif /* __MSG4UDP_H__ */



