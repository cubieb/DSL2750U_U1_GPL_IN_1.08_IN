/******************************************************************************
 *    (c)2009 Broadcom Corporation
 * 
 * This program is the proprietary software of Broadcom Corporation and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.  
 *  
 * Except as expressly set forth in the Authorized License,
 *  
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *  
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO 
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES 
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, 
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION 
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF 
 * USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS 
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR 
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR 
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT 
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE 
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF 
 * ANY LIMITED REMEDY.
 *
 * $brcm_Workfile: ScanDir.c $
 * $brcm_Revision: 3 $
 * $brcm_Date: 7/24/09 10:47a $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/LinuxOSL/ScanDir.c $
 * 
 * 3   7/24/09 10:47a ajitabhp
 * PR55165: Warnings while compiling as Library.
 * 
 * 2   7/21/09 3:14p ajitabhp
 * PR55165: Linux OSL New Scan item was not returning the item to be
 *  scanned.
 * 
 *****************************************************************************/
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ScanDir.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

typedef enum _SCAN_ITEM_RESULT_{
    SCAN_ITEM_FILE=0,
    SCAN_ITEM_DIR=1,
    SCAN_ITEM_IGNORE=2
}SCAN_ITEM_RESULT;

typedef struct _SCAN_ITEM_
{
    char *DirPathToScan;         /* Full Path Of the Directory For Scanning*/
    struct _SCAN_ITEM_ *next;    /* List pointing to next directory to scan*/
}SCAN_ITEM,
*PSCAN_ITEM;

static
PSCAN_ITEM 
new_scan_item(char * PathToScan)
{
    PSCAN_ITEM pNewItem=NULL;
    pNewItem = (PSCAN_ITEM) malloc(sizeof(SCAN_ITEM));
    if(pNewItem)
    {
        pNewItem->DirPathToScan = strdup(PathToScan);
        cmsUtl_strcpy(pNewItem->DirPathToScan,PathToScan);
        pNewItem->next=NULL;
    }
    return pNewItem;
}

static 
void
free_scan_item(PSCAN_ITEM item)
{
    free(item->DirPathToScan);
    free(item);
}

static
void
insert_list_tail(PSCAN_ITEM *pHead, PSCAN_ITEM item)
{
    PSCAN_ITEM temp = *pHead;
    if(!*pHead)
    {
        *pHead = item;
        return;
    }

    while(temp)
    {
        if(temp->next)
            temp = temp->next;
        else{
            temp->next = item;
            break;
        }
    }
}    

static
PSCAN_ITEM
remove_list_head(PSCAN_ITEM *pHead)
{
    PSCAN_ITEM temp = *pHead;

    if(!*pHead) return NULL;
    else *pHead = temp->next;

    return temp;
}

static
char
is_list_empty(PSCAN_ITEM *pHead)
{
    if(!*pHead) return TRUE;
    return FALSE;
}

static 
SCAN_ITEM_RESULT
NeedItemScan(struct stat *FileStats,
             char *FileName,
             char *Extension)
{
    printf("=======%s  %d          %s\n",
            (S_ISDIR(FileStats->st_mode)) ? "<Dir >":"<File>",
            (int)FileStats->st_size,FileName);

    if(!FileName)
        return SCAN_ITEM_IGNORE;

    if(!S_ISDIR(FileStats->st_mode))
    {
        /*Match the Extension here. If matches, then return SCAN_ITEM_FILE else return SCAN_ITEM_IGNORE*/
        return SCAN_ITEM_FILE;
    }

    if( (S_ISDIR(FileStats->st_mode)) && 
        (strcmp(FileName,".") ) &&
        (strcmp(FileName,"..")) )
    {
        /*This is a new Directory to Scan*/
        return SCAN_ITEM_DIR;
    }

    return SCAN_ITEM_IGNORE;
}

static
char 
HandleScanResult(SCAN_ITEM_RESULT     ScanRes,
                 char                 ScanSubDir,
                 PSCAN_ITEM         *Head,
                 char                 *FileName,
                 char                 *FullFilePath,
                 off_t                FileSz,
                 PSCAN_ITEM         scan_item,
                 NEW_FILE_FOUND     pfnFoundFile,
                 void                *cb_context)
{
    FOUND_FILE_INFO        FileInfo;

    if(!FileName || !Head)
        return FALSE;

    if(ScanRes == SCAN_ITEM_IGNORE)
        return TRUE; /*This is not a error*/

    if(ScanRes == SCAN_ITEM_DIR)
    {
        if(ScanSubDir)
        {
            PSCAN_ITEM  NewItem=NULL;
            NewItem = new_scan_item(FullFilePath);    
            insert_list_tail(Head,NewItem);
        }
    }

    if(ScanRes == SCAN_ITEM_FILE)
    {
        /*Invoke the callback if provided and the file matched the extension*/
        FileInfo.FileName      = FileName;    
        FileInfo.FullFilePath = FullFilePath;
        FileInfo.FileSize = FileSz;
        pfnFoundFile(cb_context,&FileInfo);
    }

    return TRUE;
}

char
ScanDir(char *StartDir,            /* The Root for starting the Scan*/
        char ScanSubDir,        /* Scan The Sub Directories*/
        char *Extension,        /* The Files To Report eg. mpg*/
        NEW_FILE_FOUND pfnFoundFile,    /* The callback function when file is found*/
        void    *cb_context)
{
    PSCAN_ITEM    Head=NULL,scan_item;        
    struct dirent **namelist=NULL;
    struct stat FileStat;
    char FullPath[MAX_PATH];
    SCAN_ITEM_RESULT ScanRes;
    int i,n;

      if(!StartDir) return FALSE;
    
    scan_item = new_scan_item(StartDir);
    insert_list_tail(&Head,scan_item);

    while(!is_list_empty(&Head))
    {
        scan_item = remove_list_head(&Head);
        if(!scan_item)
        {
            printf("List Cannot Be NULL Here!!");
            free_scan_item(scan_item);
            return FALSE;
        }

        printf("Scanning %s\n",scan_item->DirPathToScan);
        n = scandir(scan_item->DirPathToScan,&namelist,0,0); 
        if(n < 0)
        {
            printf("Scanning Failure\n");
            return 0;
        }

        for(i=0;i < n; i++)
        {
            sprintf(FullPath,"%s%s%s",
                    scan_item->DirPathToScan,"/",namelist[i]->d_name);

            if(stat(FullPath,&FileStat) < 0)
                continue;

            ScanRes = NeedItemScan(&FileStat,    
                                     namelist[i]->d_name, 
                                     Extension);

            HandleScanResult(ScanRes,
                             ScanSubDir,
                             &Head,
                             namelist[i]->d_name,
                             FullPath,
                             FileStat.st_size,
                             scan_item,
                             pfnFoundFile,
                             cb_context);
        }    

        free_scan_item(scan_item);
    }

    return TRUE;
}

/***********************************************************
 * Enable this code when you want to debug scan dir 
 * seperately.
char    
NewFileFound(void *Context,PFOUND_FILE_INFO pFileInfo)        
{
    return TRUE;
}

int main()
{
    
    FOUND_FILE_INFO FoundFile;
    ScanDir("/home/ajitabhp/Desktop/test",TRUE,NULL,NewFileFound,(void *)0x12345);    
}
***********************************************************/

