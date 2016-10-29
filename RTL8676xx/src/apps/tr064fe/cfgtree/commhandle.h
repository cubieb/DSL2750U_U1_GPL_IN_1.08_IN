/* commhandle.h */

#ifndef __commhandle_h__
#define __commhandle_h__

#ifdef __cplusplus
extern          "C" {
#endif

#include <sys/queue.h>         /*include it for LIST*/


#ifndef ROWSTATUS_ENUMS
#define ROWSTATUS_ENUMS

#define ROWSTATUS_ACTIVE  1
#define ROWSTATUS_NOTINSERVICE  2
#define ROWSTATUS_NOTREADY  3
#define ROWSTATUS_CREATEANDGO  4
#define ROWSTATUS_CREATEANDWAIT  5
#define ROWSTATUS_DESTROY  6

#endif                          /* ROWSTATUS_ENUMS */

/*
说明:请替换自动生成代码的OID前缀为下面的宏。这样一旦需要修改OID挂靠点，只需要修改这里一次。
自动生成的代码，需要修改xxxTable_oids.h，如果是标量则修改xxx.c。
*/

/* TruthValue取值枚举值 */
#ifndef TRUTHVALUE_ENUMS
#define TRUTHVALUE_ENUMS

#define TRUTHVALUE_TRUE  1
#define TRUTHVALUE_FALSE  2

#endif                          /* TRUTHVALUE_ENUMS */

/* CACHE数据在内存驻留时间，单位:秒 */
#define SNMP_CACHE_DEFAULT_TIMEOUT	5

/*****************************************************************************
 * 表结构列处理辅助函数
 *****************************************************************************/
int CMM_GetStrColumn(const char* cInstancePrefix, const char* cNode,
	char* cValue, unsigned int uSize, size_t* pulLen, char **pv, int pvLen);

int CMM_GetIpAddrColumn(const char* cInstancePrefix, const char* cNode,
	unsigned char* cValue, unsigned int uSize, size_t* pulLen, char **pv, int pvLen);

int CMM_SetStrColumn(const char* cInstancePrefix, const char* cNode,
	const char* cValue, size_t uSize, char **pv, int pvLen);

int CMM_GetIntColumn(const char* cInstancePrefix, const char* cNode,
	u_long* pulValue, char **pv, int pvLen);

int CMM_SetIntColumn(const char* cInstancePrefix, const char* cNode,
	u_long ulValue, char **pv, int pvLen);


/****************************************************************************
*                             Linked List Defines                           *
****************************************************************************/
// here are some Linked List MACROS I wanted to use,
// but curiously were not in /usr/includes/sys/queue.h

#ifndef LIST_EMPTY
  #define	LIST_EMPTY(head)	((head)->lh_first == NULL)
#endif

#ifndef LIST_NEXT
  #define	LIST_NEXT(elm, field)	((elm)->field.le_next)
#endif

#ifndef LIST_INSERT_BEFORE
  #define	LIST_INSERT_BEFORE(listelm, elm, field) do {	\
	  (elm)->field.le_prev = (listelm)->field.le_prev;		\
	  LIST_NEXT((elm), field) = (listelm);				\
	  *(listelm)->field.le_prev = (elm);				       \
	  (listelm)->field.le_prev = &LIST_NEXT((elm), field);	\
  } while (0)
#endif

#ifndef LIST_FIRST
  #define	LIST_FIRST(head)	((head)->lh_first)
#endif


/****************************************************************************
*                           Linked List Structure                           *
****************************************************************************/
/*static*/
struct lstNode {
  LIST_ENTRY ( lstNode ) nodes;
  char *data;                                 // pointer to data
};

typedef LIST_HEAD ( , lstNode ) lstList_t;

/*pointer to last,     pointer to newly allocated,   pointer to the node being used momentarily*/
struct lstNode *lstNode, *nwlNode, *hdlNode;

void
TW_addList (char *l, char *data, int len, int refBytes);

void 
TW_flushList ( char *l );

void
TW_copyList( char *d, char *s, int dataLen );


#ifdef __cplusplus
}
#endif

#endif

