/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : mktree.c
 文件描述 :
            1、该工具主要用来将文本格式的节点列表转换成节点树的格式,一般情况下,pdf
               文件中的节点列表均可转换成文本格式.
            2、
               文本的格式应该为:
               Name                             Type            Writable    Default
               例如:
               SRTPKeyingMethods                string(256)     -           -
               PSTNFailOver                     boolean         W           False
               .VoiceService.{i}.Capabilities.  object          -           -
               没有的值以减号代替,值中间以TAB或者空格分隔
            3、调整PRE_TAB_COUNT宏或者使用第三个参数可以增加初始的缩进TAB数量
            4、需要注意手工删除由编辑器换行在路径和节点中误添加的"-"和上下标
            5、
               使用方法:
                 ./mktree filenamein.txt filenameout.xml [PreTabCount]
 函数列表 :
 修订记录 :
          1 创建 : huangjidong
            日期 : 2008-7-15
            描述 : mktree 1.0

**********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_COUNT 1024
#define MAX_OBJ_LEN 512
#define PRE_TAB_COUNT 0   /*最开始前面的TAB数,可更改*/

typedef struct _ST_NODE
{
    char *pcNode;
    struct _ST_NODE *pstPre;
    struct _ST_NODE *pstNxt;
}ST_NODE;


#define APPEND_LIST(head, tail, node) \
{ \
    if (NULL != tail) \
    { \
        tail->pstNxt = node; \
        node->pstPre = tail; \
    } \
    else \
    { \
        head = node; \
    } \
    tail = node; \
}

void free_chain (ST_NODE * chain)
{
    ST_NODE *current = NULL;

    while (chain)
    {
        if(chain->pcNode)
            free(chain->pcNode);
        current = chain;
        chain = chain->pstNxt;
        free (current);
    }
    chain = NULL;
}

int safe_strcmp(const char *s1, const char *s2)
{
    if(!s1 && !s2)
        return 0;
    if(!s1)
        return -1;
    if(!s2)
        return 1;
    return strcmp(s1, s2);
}

ST_NODE *parse_node(const char *ccNodeStr)
{
    char *pcSrc;
    char *pcNode;
    ST_NODE *pstHead = NULL;
    ST_NODE *pstTail = NULL;
    ST_NODE *pstNode = NULL;

    if(!ccNodeStr)
        return NULL;

    pcSrc= strdup(ccNodeStr);
    pcNode = strtok(pcSrc, ".{}");

    if(pcNode)
    do
    {
        pstNode = malloc(sizeof(ST_NODE));
        memset(pstNode, 0, sizeof(ST_NODE));
        APPEND_LIST(pstHead, pstTail, pstNode);
        pstNode->pcNode = strdup(pcNode);
    }
    while(NULL != (pcNode = strtok(NULL, ".{}")));

    free(pcSrc);
    return pstHead;
}

int main(int argc, char **argv)
{
    char *pcName = NULL;
    char *pcType = NULL;
    char *pcWritable = NULL;
    char *pcDefault = NULL;
    char acBuf[MAX_LINE_COUNT+1] = {0};
    char acOutBuf[MAX_LINE_COUNT+1] ={0};
    FILE *fpIn = NULL, *fpOut = NULL;
    int i = 0, j = 0;
    int iPreTab = PRE_TAB_COUNT;
    char acObjectOld[MAX_OBJ_LEN+1] = {0};
    char acObjectNew[MAX_OBJ_LEN+1]={0};
    ST_NODE *pstOld = NULL, *pstNew = NULL, *pstFreeOld = NULL, *pstFreeNew = NULL;
    int iFileEnd = 0;

    if(argc < 3 || argc > 4)
    {
        printf("Usage:\n\tmktree filenamein.txt filenameout.xml\n\t\n\tText node file shoud include Name,Type,Writable,and Default.\n");
        exit(0);
    }
    else
    {
        /*打开输入输出文件*/
        fpIn = fopen(argv[1], "r");
        fpOut = fopen(argv[2], "w");
    }
    if(argc == 4)
    {
        iPreTab = atol(argv[3]);
    }

    if(!fpIn || !fpOut)
    {
        printf("File open error, Please check and try again.\n");
        exit(0);
    }

    while(fgets(acBuf, MAX_LINE_COUNT, fpIn))
    {
        /*该解析过程同时去掉了全角字符的引号*/
        /*获得节点名*/
        LAB_FILE_END:
        pcName = strtok(acBuf, " \t\r\n濉板”");
        if(pcName)
        {
            /*获得节点类型*/
            pcType = strtok(NULL, " \t\r\n濉板”");
            if(pcType)
            {
                /*获得节点可写属性*/
                pcWritable = strtok(NULL, " \t\r\n濉板”");
                if(pcWritable)
                {
                    /*获得节点默认值*/
                    if(strcasecmp(pcType, "object") != 0)
                    {
                        pcDefault = strtok(NULL, " \t\r\n濉板”");
                        if(!pcDefault)
                        {
                            continue;
                        }
                    }
                    else
                    {
                        /*如果是对象节点的话,进行对象解析过程*/
                        pcDefault = "";
                        strcpy(acObjectNew, pcName);
                        /*比较新对象和旧对象的不同*/
                        pstFreeNew = pstNew = parse_node(acObjectNew);
                        pstFreeOld = pstOld = parse_node(acObjectOld);
                        if(pstOld != NULL)
                        {
                            while(pstOld && pstNew)
                            {
                                if(safe_strcmp(pstNew->pcNode, pstOld->pcNode) != 0)break;
                                pstNew=pstNew->pstNxt;
                                pstOld=pstOld->pstNxt;
                            }
                        }

                        if(pstOld)
                        {
                            /*如果减少了路径,则将减少的对象关闭*/
                            for(i = 0; pstOld->pstNxt != NULL; i++)pstOld=pstOld->pstNxt;
                            for(; i >= 0 ; i--)
                            {
                                iPreTab--;
                                for(j = 0; j < iPreTab; j++)acOutBuf[j]='\t';
                                sprintf(acOutBuf+iPreTab, "</%s>\n", pstOld->pcNode);
                                printf("%s", acOutBuf);
                                /*输出到文件*/
                                if(fputs(acOutBuf, fpOut)<0)
                                {
                                    printf("file write error.\n");
                                }
                                pstOld=pstOld->pstPre;
                            }
                        }
                        if(pstNew)
                        {
                            if(strcasecmp(pcWritable, "w") == 0)
                            {
                                pcWritable = " Writable=\"1\"";
                            }
                            else if(strcasecmp(pcWritable, "0") == 0)
                            {
                                pcWritable = " Writable=\"0\"";
                            }
                            else
                            {
                                /*定义该宏可将没有明确声明为W或0属性的节点不设置Writable属性*/
                                #ifdef EXPLICIT_WRITABLE
                                pcWritable = "";
                                #else
                                pcWritable = " Writable=\"0\"";
                                #endif
                            }
                            /*如果增加了路径,则将其加为对象*/
                            while(pstNew != NULL)
                            {
                                for(j = 0; j < iPreTab; j++)acOutBuf[j]='\t';
                                sprintf(acOutBuf+iPreTab, "<%s type=\"%s\"%s>\n", \
                                    pstNew->pcNode, pcType, pcWritable);
                                printf("%s", acOutBuf);
                                /*输出到文件*/
                                if(fputs(acOutBuf, fpOut)<0)
                                {
                                    printf("file write error.\n");
                                }
                                pstNew=pstNew->pstNxt;
                                iPreTab++;
                            }
                        }
                        free_chain(pstFreeNew);
                        free_chain(pstFreeOld);
                        strcpy(acObjectOld, acObjectNew);
                        continue;
                    }
                }
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }
        }
        else
        {
            continue;
        }

        /*转换默认值*/
        if(strcasecmp(pcDefault, "false") == 0)
        {
            pcDefault = "0";
        }
        else if(strcasecmp(pcDefault, "true") == 0)
        {
            pcDefault = "1";
        }
        else if(strcasecmp(pcDefault, "<empty>") == 0)
        {
            pcDefault = "";
        }

        if(strcasecmp(pcWritable, "w") == 0)
        {
            pcWritable = " Writable=\"1\"";
        }
        else if(strcasecmp(pcWritable, "0") == 0)
        {
            pcWritable = " Writable=\"0\"";
        }
        else
        {
            /*定义该宏可将没有明确声明为W或0属性的节点不设置Writable属性*/
            #ifdef EXPLICIT_WRITABLE
            pcWritable = "";
            #else
            pcWritable = " Writable=\"0\"";
            #endif
        }

        /*输出节点固定格式*/
        for(j = 0; j < iPreTab; j++)acOutBuf[j]='\t';
        sprintf(acOutBuf+iPreTab, "<%s type=\"%s\"%s>%s</%s>\n", \
            pcName, pcType, pcWritable,strcmp(pcDefault, "-") == 0?"":pcDefault, pcName);
        printf("%s", acOutBuf);

        /*输出到文件*/
        if(fputs(acOutBuf, fpOut)<0)
        {
            printf("file write error.\n");
        }
    }

    if(iFileEnd == 0)
    {
        iFileEnd = 1;
        strcpy(acBuf, ".    object  -   -");
        goto LAB_FILE_END;
    }

    if(fpOut)
    {
        fclose(fpOut);
    }
    if(fpIn)
    {
        fclose(fpIn);
    }
    exit(0);
}
