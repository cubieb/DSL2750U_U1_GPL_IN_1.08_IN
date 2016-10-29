
//#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "om_nameval.h"


#undef strcmp
#undef strncmp

int    safe_strcmp(const char *s1, const char *s2)
{
    if(!s1 && !s2)
        return 0;
    if(!s1)
        return -1;
    if(!s2)
        return 1;
    return strcmp(s1, s2);
}

int    safe_strncmp(const char *s1, const char *s2, int n)
{
    if(!s1 && !s2)
        return 0;
    if(!s1)
        return -1;
    if(!s2)
        return 1;
    return strncmp(s1, s2, n);
}

int safe_strlen(const char *s)
{
    if(!s)
        return 0;

    return strlen(s);
}

int safe_atoi(const char *s)
{
    if(!s)
        return 0;

    return atoi(s);
}

long safe_atol(const char *s)
{
    if(!s)
        return 0;

    return atol(s);
}

int    OM_ValAppend(ST_NAME_VAL **head, char *name, char *value)
{
    ST_NAME_VAL        *kv;

    if(!name)
        return -1;

    /* Add a new variable */
    kv = (ST_NAME_VAL *)malloc(sizeof(ST_NAME_VAL));
    if(!kv)
        return -1;
    memset(kv, 0, sizeof(ST_NAME_VAL));
    NODE_ENQUEUE(*head, kv);
    SET_TEXT(kv->name, name);

    if(value){
        SET_TEXT(kv->value, value);
    } else
        kv->value=NULL;
    return 0;
}

int    OM_ValSet(ST_NAME_VAL **head, char *name, char *value)
{
    ST_NAME_VAL *kv;

    if(!name)
        return -1;

    kv = OM_ValFind(head, name);
    if(!kv){
        /* Add a new variable */
        kv = (ST_NAME_VAL *)malloc(sizeof(ST_NAME_VAL));
        if(!kv)
            return -1;
        memset(kv, 0, sizeof(ST_NAME_VAL));
        NODE_ENQUEUE(*head, kv);
        SET_TEXT(kv->name, name);
    }
    if(value){
        SET_TEXT(kv->value, value);
    } else
        kv->value=NULL;
    return 0;
}

char *OM_ValGet(ST_NAME_VAL **head, char *name)
{
    ST_NAME_VAL *kv;
    if(!name)
        return NULL;

    for(kv=*head; kv; kv = kv->pNext)
        if(kv->name && !safe_strcmp(kv->name, name))
            return kv->value;
    return NULL;
}

ST_NAME_VAL    *OM_ValFind(ST_NAME_VAL **head, char *name)
{
    ST_NAME_VAL *kv;
    if(!name)
        return NULL;

    for(kv=*head; kv; kv = kv->pNext ){
        if(kv->name && !strcmp(kv->name, name)){
            return kv;
        }
    }
    return NULL;
}

int    OM_ValDel(ST_NAME_VAL **head, ST_NAME_VAL *kv)
{
    if(!kv)
        return 0;

    CLEAR_TEXT(kv->name);
    CLEAR_TEXT(kv->value);
    if(head)
        NODE_DEL(*head, kv);
    free(kv);
    return 0;
}

static int OM_ValMatchListInclusive(ST_NAME_VAL* a1, ST_NAME_VAL *a2)
{
    while (a1)
    {
        ST_NAME_VAL * m = OM_ValFind(&a2, a1->name);
        if (!m) return 0;
        if (safe_strcmp(a1->value, m->value)) return 0;
        a1 = a1->pNext;
    }
    return 1;
}

int OM_ValMatchList(ST_NAME_VAL* a1, ST_NAME_VAL* a2)
{
    return (OM_ValMatchListInclusive(a1,a2) &&
            OM_ValMatchListInclusive(a2,a1));
}

int    OM_ValFlush(ST_NAME_VAL **head)
{
    ST_NAME_VAL        *kv;

    if(!head)
        return -1;

    while(*head){
        kv=*head;
        CLEAR_TEXT(kv->name);
        CLEAR_TEXT(kv->value);
        NODE_DEL(*head, kv);
        free(kv);
    }
    return 0;
}

int OM_ValDump(ST_NAME_VAL *ptr)
{
    printf("List: ");
    while(ptr)
    {
        printf("'%s'='%s', ", ptr->name, ptr->value);
        ptr = ptr->pNext;
    }
    printf("\n");
	return 0;
	return 0;
}

