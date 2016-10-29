/***************************************************************************
*     (c)2008-2009 Broadcom Corporation
*  
*  This program is the proprietary software of Broadcom Corporation and/or its licensors,
*  and may only be used, duplicated, modified or distributed pursuant to the terms and
*  conditions of a separate, written license agreement executed between you and Broadcom
*  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
*  no license (express or implied), right to use, or waiver of any kind with respect to the
*  Software, and Broadcom expressly reserves all rights in and to the Software and all
*  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
*  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
*  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.  
*   
*  Except as expressly set forth in the Authorized License,
*   
*  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
*  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
*  and to use this information only in connection with your use of Broadcom integrated circuit products.
*   
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
*  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
*  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO 
*  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES 
*  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, 
*  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION 
*  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF 
*  USE OR PERFORMANCE OF THE SOFTWARE.
*  
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS 
*  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR 
*  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR 
*  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF 
*  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT 
*  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE 
*  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF 
*  ANY LIMITED REMEDY.
 *
 * $brcm_Workfile: list.h $
 * $brcm_Revision: 2 $
 * $brcm_Date: 8/31/09 3:24p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/list.h $
 * 
 * 2   8/31/09 3:24p dliu
 * SW7405-2482: Add BLST_LAST function
* 
***************************************************************************/

#ifndef LIST_H__
#define LIST_H__

#ifdef WIN32
#pragma warning( disable:4127 )
#endif

/*================== Module Overview =====================================
This modules defines macros to control singly-linked list. 
All operations are typesafe (doesn't required typecasting) and constant time.

This list allow:
  o Insert a new entry at the head of the list
  o Insert a new entry after any element in the list
  o O(1) removal of an entry from the list head
  o O(n) removal of any entry from the list
  o Forward traversal through the list
  o Each element requires one pointers
  o Code size and execution time is the smallest and fastest compared to other lists
  
========================================================================*/

/***************************************************************************
Summary:
    Creates new data type for the list head
    
Description:
    Creates new data type for the list head, this type used to create variable for the lists head.
    User should create new the list head data type for every different element datatype.
    
Input:
    name - name for the new data type (structure)
    type - existing user data type used for element of the list 

Example:    
     BLST_HEAD(block_head, block);
     struct block_head  head;

Returns:
    <none>
****************************************************************************/
#define BLST_HEAD(name, type) struct name { struct type *sl_first, *sl_last;}

/***************************************************************************
Summary:
    Defines links entry
    
Description:
    Defines entrys for the list inside the user structure.for the element.
    
Input:
    type - the datatype for element

Example:
     struct block {
        BLST_ENTRY(block) link;
        char string[256];
     };  

Returns:
    <none>
****************************************************************************/
#define BLST_ENTRY(type) struct { struct type *sl_next; }

/***************************************************************************
Summary:
    Initializes lists head
    
Description:
    Initializes the head of the list. The head shall be initialized before list can be used.
    This macro used for dynamic initialization.
    
Input:
    phead - pointer to the list head

See also:
    BLST_INITIALIZER

Example:
    BLST_INIT(&head);

Returns:
    <none>
****************************************************************************/
#define BLST_INIT(phead)    (phead)->sl_first = (phead)->sl_last = NULL

/***************************************************************************
Summary:
    Initializes lists head
    
Description:
    Initializes the head of the list. The head shall be initialized before list can be used.
    This macro used for static initialization.
    
Input:
    head - pointer to the list head

See also:
    BLST_INIT

Example:
    static struct block_head  head = BLST_INITIALIZER(head);

Returns:
    <none>
****************************************************************************/
#define BLST_INITIALIZER(head) { NULL, NULL }


/***************************************************************************
Summary:
    Tests if list is empty
    
Description:
    Tests if list is empty.
    
Input:
    phead - pointer to the list head

Returns:
    true - list empty
    false - list has elements

Example:
    if (BLST_EMPTY(&head) { return ; }

****************************************************************************/
#define BLST_EMPTY(phead) ((phead)->sl_first==NULL)

/***************************************************************************
Summary:
    Returns the lists first element
    
Description:
    Returns pointer to the first element from the list
    
Input:
    phead - pointer to the list head

Returns:
    pointer to the first element from the list.

Example:
    struct block *first=BLST_FIRST(&head);
****************************************************************************/
#define BLST_FIRST(phead) (phead)->sl_first

/***************************************************************************
Summary:
    Returns the lists last element
    
Description:
    Returns pointer to the last element from the list
    
Input:
    phead - pointer to the list tail

Returns:
    pointer to the last element from the list.

Example:
    struct block *first=BLST_LAST(&head);
****************************************************************************/
#define BLST_LAST(phead) (phead)->sl_last

/***************************************************************************
Summary:
    Returns next element from the lists
    
Description:
    Returns pointer to the next element from the list
    
Input:
    elm - pointer to the list element
    field - name of the elements link field

Returns:
    pointer to the next element from the list

Example:
    struct block *second=BLST_NEXT(first);
****************************************************************************/
#define BLST_NEXT(pitem, field) (pitem->field.sl_next)

/***************************************************************************
Summary:
    Inserts element into the list
    
Description:
    Inserts new element into the head of the list.
    
Input:
    head - pointer to the list head
    elm - pointer to the new element
    field - name of the elements link field

Returns:
    <none>

Example:
    BLST_INSERT_HEAD(&head, new_block, link);
****************************************************************************/
#define BLST_INSERT_HEAD(phead, pitem, field) do { \
                 if ( (phead)->sl_last == NULL) (phead)->sl_last = (pitem); \
               (pitem)->field.sl_next = (phead)->sl_first; \
                (phead)->sl_first = (pitem); \
               } while(0)


/***************************************************************************
Summary:
    Inserts element into the list
    
Description:
Description:
    Inserts new element into the tail list.
    
Input:
    head - pointer to the list head
    elm - pointer to the new element
    field - name of the elements link field

Returns:
    <none>

Example:
    BLST_INSERT_TAIL(&head, new_block, link);
****************************************************************************/
#define BLST_INSERT_TAIL(phead, pitem, field) do {                \
                (pitem)->field.sl_next = NULL;                        \
                if ((phead)->sl_last) {                                \
                        (phead)->sl_last->field.sl_next = (pitem);    \
                } else {                                            \
                        (phead)->sl_first = (pitem);                \
                }                                                    \
                (phead)->sl_last = (pitem);                            \
               } while(0)

/***************************************************************************
Summary:
    Inserts element into the list
    
Description:
    Inserts new element after existing element.
    
Input:
    head - pointer to the list head
    elm - pointer to the element from the list
    new_elm - pointer to the new element
    field - name of the elements link field

Returns:
    <none>

Example:
    BLST_INSERT_AFTER(&head, first, second, link);
****************************************************************************/
#define BLST_INSERT_AFTER(head, elm, new_elm, field) do { \
        (new_elm)->field.sl_next = (elm)->field.sl_next; \
        (elm)->field.sl_next = new_elm; \
      } while(0)


/***************************************************************************
Summary:
    Removes element from the list
    
Description:
    Removes element from the head of the list.
    
Input:
    head - pointer to the list head
    field - name of the elements link field

See also:
    BLST_REMOVE

Returns:
    <none>

Example:
    BLST_REMOVE_HEAD(&head, link);
****************************************************************************/
#define BLST_REMOVE_HEAD(phead, field) (phead)->sl_first = (phead)->sl_first->field.sl_next


/***************************************************************************
Summary:
    Removes element from the list
    
Description:
    Removes element from the of the list. This implementation is O(n), 
    where n it's position of the element in the list
    
Input:
    head - pointer to the list head
    elm - pointer to the list element
    type - datatype for an element of the list
    field - name of the elements link field

See also:
    BLST_REMOVE_HEAD

Returns:
    <none>

Example:
    BLST_REMOVE(&head, first, block, link);
****************************************************************************/
#define BLST_REMOVE(phead, elm, type, field) do { \
        if ((phead)->sl_first == (elm)) { \
             BLST_REMOVE_HEAD(phead, field);\
        } else { \
            struct type *cur; \
                                    \
            for (cur=(phead)->sl_first; cur->field.sl_next != (elm); cur = cur->field.sl_next) {} \
            cur->field.sl_next=cur->field.sl_next->field.sl_next; \
        } \
    } while(0)

/***************************************************************************
Summary:
    Removes element from the list
    
Description:
    Removes next element from the list.
    
Input:
    head - pointer to the list head
    elm - pointer to the element from the list
    field - name of the elements link field

See also:
    BLST_REMOVE

Returns:
    <none>

Example:
    BLST_REMOVE_NEXT(elm, link);
****************************************************************************/
#define BLST_REMOVE_NEXT(head, elm, field) \
           elm->field.sl_next=elm->field.sl_next->field.sl_next; 

/***************************************************************************
Summary:
    Adds new entry to the dictionary
    
Description:
    Adds new element to the dictionary implemented as sordted single linked list
    
Input:
    head - pointer to the list head
    elm - pointer to the element from the list
    type - datatype for an element of the list
    key - name of the key element
    link - name of the elements link field
    duplicate - label that would be used (gotoed) if duplicate element is found

See also:
    BLST_DICT_FIND
    BLST_DICT_REMOVE

Returns:
    <none>

Example:
    BLST_DICT_ADD(head, elm, block, key, link, err_duplicate);
****************************************************************************/
#define BLST_DICT_ADD(head, elm, type, key, link, duplicate) do { \
    struct type *i, *prev; \
    for(prev=NULL,i=BLST_FIRST((head));i!=NULL;prev=i,i=BLST_NEXT(i, link)) { \
        if((elm)->key > i->key) {  break;} \
        if((elm)->key == i->key) { goto duplicate; } \
    } \
    if(prev) { BLST_INSERT_AFTER((head), prev, (elm), link);} \
    else { BLST_INSERT_HEAD(head, (elm), link);}\
    } while(0)

/***************************************************************************
Summary:
    Finds element in the dictionary
    
Description:
    Finds element in the dictinary with matching key
    
Input:
    head - pointer to the list head
    elm - pointer to the element from the list
    key - value of key to find
    field - name of the key element
    link - name of the elements link field

See also:
    BLST_DICT_ADD
    BLST_DICT_REMOVE

Returns:
    elm = NULL, if element not found in the list
    elm - pointer to the found element

Example:
    BLST_DICT_FIND(head, elm, 123, key, link);
****************************************************************************/
#define BLST_DICT_FIND(head, elm, key, field, link) do { \
    for((elm) = BLST_FIRST(head); (elm)!=NULL ; (elm) = BLST_NEXT((elm), link)) { \
        if(key == (elm)->field )  { break; } \
        else if( key > (elm)->field ) { (elm) = NULL;break; } } } while(0)

/***************************************************************************
Summary:
    Finds element in the dictionary
    
Description:
    Finds element in the dictinary with matching string keys. This only work
    if the key is a string and will do a strcmp on the keys
    
Input:
    head - pointer to the list head
    elm - pointer to the element from the list
    key - value of key to find
    field - name of the key element
    link - name of the elements link field

See also:
    BLST_DICT_ADD
    BLST_DICT_REMOVE

Returns:
    elm = NULL, if element not found in the list
    elm - pointer to the found element

Example:
    BLST_DICT_FIND_BY_STR(head, elm, 123, key, link);
****************************************************************************/
#define BLST_DICT_FIND_BY_STR(head, elm, key, field, link) do { \
    for((elm) = BLST_FIRST(head); (elm)!=NULL ; (elm) = BLST_NEXT((elm), link)) { \
    if(0 == strcmp(key, (elm)->field)) { break; } \
        } } while(0)


/***************************************************************************
Summary:
    Removed element in the dictionary
    
Description:
    Removes element in the dictinary with matching key
    
Input:
    head - pointer to the list head
    elm - pointer to the element from the list
    type - datatype for an element of the list
    key - value of key to find
    field - name of the key element
    link - name of the elements link field

See also:
    BLST_DICT_ADD
    BLST_DICT_FIND

Returns:
    elm = NULL, if element not found in the list
    elm - pointer to the found element

Example:
    BLST_DICT_REMOVE(head, elm, 123, block, key, link);
****************************************************************************/
#define BLST_DICT_REMOVE(head, elm, key, type, field, link) do { \
    struct type *prev; \
    for(prev=NULL,(elm)=BLST_FIRST((head));(elm)!=NULL;prev=elm, elm=BLST_NEXT((elm), link)) { \
        if( (key) == (elm)->field ) { \
            if(prev) { BLST_REMOVE_NEXT((head), prev, link); } \
            else { BLST_REMOVE_HEAD((head), link);} break; \
        } else if( key > (elm)->field ) { (elm) = NULL;break; } \
    } } while(0)

/***************************************************************************
Summary:
    Removed element in the dictionary
    
Description:
    Removes element in the dictinary with matching key
    
Input:
    head - pointer to the list head
    elm - pointer to the element from the list
    type - datatype for an element of the list
    key - value of key to find
    field - name of the key element
    link - name of the elements link field

See also:
    BLST_DICT_ADD
    BLST_DICT_FIND

Returns:
    elm = NULL, if element not found in the list
    elm - pointer to the found element

Example:
    BLST_DICT_REMOVE(head, elm, 123, block, key, link);
****************************************************************************/
#define BLST_DICT_REMOVE_BY_STR(head, elm, key, type, field, link) do { \
    struct type *prev; \
    for(prev=NULL,(elm)=BLST_FIRST((head));(elm)!=NULL;prev=elm, elm=BLST_NEXT((elm), link)) { \
        if(0 == strcmp(key, (elm)->field)) { \
            if(prev) { BLST_REMOVE_NEXT((head), prev, link); } \
            else { BLST_REMOVE_HEAD((head), link);} break; \
        } \
    } } while(0)




#endif /* BUTIL_SLIST_H */



