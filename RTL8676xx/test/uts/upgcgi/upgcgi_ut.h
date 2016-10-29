
//#include "common.h"


 /*---------------------------------- 桩函数-----------------------------*/
/* 桩函数的通用实现, 如果变量fail_##FUNC_NAME 为正数，
  * 表示失败几次后成功，如果为负数，表示成功几次后失败*/
#define STUB_COMM_CODE(FUNC_NAME, ERROR_RETURN)     \
    if ( fail_##FUNC_NAME > 0 )   \
    {   \
        fail_##FUNC_NAME--;   \
        return ERROR_RETURN;   \
    }   \
   \
    if ( fail_##FUNC_NAME < 0 )   \
    {   \
        fail_##FUNC_NAME++;   \
        if ( fail_##FUNC_NAME == 0 )   \
        {   \
            fail_##FUNC_NAME = 1;   \
        }   \
    }


/* 调用MSG_CreateMessage 返回的消息*/
ST_MSG *s_pstCreateMsg = NULL;



int fail_MSG_CreateMessage = 0;
ST_MSG * MSG_CreateMessage(unsigned long ulSize)
{
    STUB_COMM_CODE(MSG_CreateMessage, NULL);
    memset(s_pstCreateMsg, 0, sizeof(ST_MSGHEAD));
    return s_pstCreateMsg;
}


int fail_MSG_ReleaseMessage = 0;
void MSG_ReleaseMessage(ST_MSG *pstMsg)
{
    STUB_COMM_CODE(MSG_ReleaseMessage, ;);
}


int fail_MSG_SendMessage = 0;
int MSG_SendMessage(ST_MSG * pstMsg)
{
    STUB_COMM_CODE(MSG_SendMessage, TBS_FAILED);
    return TBS_SUCCESS;
}

int fail_MSG_ReceiveMessage = 0;
int msg_result=0;
MSG_RET MSG_ReceiveMessage(ST_MSG **ppstMsg, unsigned long lSecond)
{
    STUB_COMM_CODE(MSG_ReceiveMessage, TBS_FAILED);
    
    /* 初始化s_pstTestMsg */
    *ppstMsg = (ST_MSG *)malloc(sizeof(ST_MSG) + sizeof(unsigned long));
    if (NULL == (*ppstMsg))
    {
        printf("receive msg fail.\n");
        return TBS_FAILED;
    }

    memset(*ppstMsg, 0, sizeof(ST_MSG) +  sizeof(unsigned long));
    (*ppstMsg)->stMsgHead.ulBodyLength =  sizeof(unsigned long);
    
    *( (unsigned long *)((*ppstMsg)->szMsgBody) ) = msg_result;
    return TBS_SUCCESS;
}

int fail_MSG_RegModule = 0;
MSG_RET MSG_RegModule(unsigned short usMID, FUN_RECEIVE funReceive)
{
    STUB_COMM_CODE(MSG_RegModule, TBS_FAILED);
    return TBS_SUCCESS;
}


int fail_MSG_UnregModule = 0;
MSG_RET MSG_UnregModule(unsigned short usMID, FUN_RECEIVE funReceive)
{
    STUB_COMM_CODE(MSG_UnregModule, TBS_FAILED);
    return TBS_SUCCESS;
}

int fail_open = 0;
int stub_open(const char *pathname, int flag)
{
    int src=0;
    if (0 != fail_open)
    {
        fail_open = 0;
        return -1;
    }
    else
    {
    	src = open(pathname, flag);
     	return src;
    }
}

int read_result = 0;
int stub_read(int fd, void *buf, size_t count)
{
    if (0 == read_result)
    {        
        return 1;
    }
    else
    {
    	return -1;
    }
}

int fail_fopen = 0;
FILE *stub_fopen(const char *path, const char *mode)
{
    if (0 != fail_fopen)
    {
        fail_fopen = 0;
        return NULL;
    }
    else
    {
        return fopen(path, mode);
    }
}

int fork_result = 0;
int  stub_fork()
{    
    return fork_result;
}


int post_content_index=0;

BOOL get_line_from_post(char *sPtr, int maxLen)
{
	switch(post_content_index)
	{
		
	case 0:
		strcpy(sPtr,"#");
		return TRUE;
	
	case 1:
		post_content_index=0;
		strcpy(sPtr,"ok");
		return TRUE;
	
	
		
	case 2:
		post_content_index=1;
		strcpy(sPtr,"remain");
		return TRUE;
		
	case 3:
		return 0;
		
	case 4:
		post_content_index=3;
		return TRUE;
		
	default:
		break;
		
	}
	
	return TRUE;
	
}

int var_name_index =0;
void get_var_name_from_post(char *sPtr, int maxLen)
{
	switch(var_name_index)
	{
	case 0:
	strcpy(sPtr,"firmware");
	break;
	
	case 1:
	strcpy(sPtr,"config");
	break;	
		
	case 2:
	strcpy(sPtr,"downloadConfig");
	break;	
	
		
	case 3:
	strcpy(sPtr,"notsupported");
	break;	
	
	default:
	strcpy(sPtr,"firmware");
	break;
		
	}
	
	return;	
	
}

void remove_CRLF_and_save_buffer(char *sPtr, long *nSize, char *pSavePtr)
{
	return;

}

void remove_trailing_CRLF(char *sPtr, int length)
{
	return;
}


int content_type_index=0;
int get_content_type(char *sPtr, int maxLen)
{
	int iType;
	
	
	switch(content_type_index)
	{
	case 0:
	iType = TYPE_TEXT;
	break;
	
	case 1:
	iType = TYPE_BINARY;
	break;	
	
	default:
	iType = TYPE_TEXT;
	break;
		
	}
	
	return iType ;
}

int  fail_fprintf=0;
int stub_fprintf(FILE *stream, const char *format, ...)
{
    STUB_COMM_CODE(fprintf, TBS_FAILED);
    return TBS_SUCCESS;
}

int fail_fclose=0;
int stub_fclose(FILE *fp)
{
	if (0 != fail_fclose)
    	{
        	fail_fclose = 0;
        	return TBS_SUCCESS;
    	}
    	else
    	{
        	return fclose(fp);
    	}
}
