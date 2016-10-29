//#define STRING_MAX_LEN    2*MAX_LMSG_LEN

//struct string *make_string(char *txt, ...);
//struct string *make_string_exact(char *txt);

#ifndef __OM_NAME_VAL_H__
#define __OM_NAME_VAL_H__

typedef struct tag_ST_NAME_VAL          // 通用值的序列描述数据
{
    char	*name;                     // 名称
    char	*value;                    // 值

    struct tag_ST_NAME_VAL  *pNext;
    struct tag_ST_NAME_VAL  *pPrev;
}ST_NAME_VAL;



#define STRNCPY(dest, src, n)                                    \
    { strncpy(dest, src, n); \
        dest[n-1] = 0; \
    }

/* The following functions are used for doublely linked
 * lists. It will use any structure, as long as it has
 * a pNext and pPrev pointer.
 */

#define NODE_ADD(head, node)    \
    {    node->pNext=head;        \
        node->pPrev=NULL;        \
        if(head) (head)->pPrev=node;    \
        head=node;            \
    }

/* This enqueues a node to the end of a list. As we don't have a temp node,
   we fake it out by using the node's pNext variable, which will be NULL, but
   is of the correct type */
#define NODE_ENQUEUE(head, node)    \
    {    node->pNext=head;                \
        if(node->pNext)                    \
            while(node->pNext->pNext)            \
                node->pNext=node->pNext->pNext;    \
        node->pPrev=node->pNext;                \
        node->pNext=NULL;                \
        if(!head) head=node;                \
        else node->pPrev->pNext=node;                \
    }

#define NODE_DEL(head, node)    \
    {    if(node->pNext) node->pNext->pPrev=node->pPrev;    \
        if(node->pPrev) node->pPrev->pNext=node->pNext;    \
        else head=node->pNext;                \
    }


#define    CLEAR_TEXT(var)            \
    {    if(var) free(var);        \
        var=NULL;                \
    }

#define    SET_TEXT(var, string)            \
/*lint -save -e774*/{    if(var) free(var);        \
        var=NULL;            \
        if(string) var=strdup(string);    \
   }/*lint -restore*/

#define    SET_N_TEXT(var, string, size)        \
/*lint -save -e774*/{if(var != NULL) free(var);        \
        var=NULL;            \
        if(string != NULL) var=strndup(string, size);    \
    }/*lint -restore*/

int     safe_strcmp(const char *s1, const char *s2);
int     safe_strncmp(const char *s1, const char *s2, int n);
int     safe_strlen(const char *s);
int     safe_atoi(const char *s);
long    safe_atol(const char *s);



int     OM_ValAppend (ST_NAME_VAL **head, char *key, char *value);
int     OM_ValSet    (ST_NAME_VAL **head, char *key, char *value);
char   *OM_ValGet    (ST_NAME_VAL **head, char *key);
ST_NAME_VAL  *OM_ValFind(ST_NAME_VAL **head, char *key);
int     OM_ValDel    (ST_NAME_VAL **head, ST_NAME_VAL *kv);
int     OM_ValMatchList(ST_NAME_VAL* a1, ST_NAME_VAL* a2);
int     OM_ValFlush  (ST_NAME_VAL **head);
int     OM_ValDump   (ST_NAME_VAL *ptr);

#endif //__OM_NAME_VAL_H__
