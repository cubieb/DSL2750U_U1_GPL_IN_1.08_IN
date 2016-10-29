/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: adslstart.c
 文件描述: 启动RTL8672 ADSL

 修订记录:
       1 作者 : Xuan Guanglei
         日期 : 2009-07-28
         描述 : 创建
**********************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>

#include "tbserror.h"
#include "flash_layout.h"
#include "adslstart.h"

#define RTL8672_DSL_DEVICE  "/dev/adsl0"
static FILE* adslFp = NULL;

/*************************************************************************
功能: DSL设备打开
参数: adslFp:设备句柄
返回: 0:打开失败
             1:打开成功
备注: 
*************************************************************************/
static char open_adsl_drv()
{
    if ((adslFp = fopen(RTL8672_DSL_DEVICE, "r")) == NULL) 
    {
        printf("Failed to open %s\n",RTL8672_DSL_DEVICE);
        return 0;
    }
    return 1;
}

/*************************************************************************
功能: DSL设备关闭
参数: adslFp:设备句柄
返回: 0:打开失败
             1:打开成功
备注: 
*************************************************************************/
static void close_adsl_drv()
{
    if(adslFp)
    {
         fclose(adslFp);
    }
    adslFp = NULL;
}

/*************************************************************************
功能: DSL IOCTRL操作
参数: id:   ioctrl ID
             *rValue:传递值
             len:长度
返回: 0:操作失败
             1:操作成功
备注: 
*************************************************************************/
static char adsl_drv_set(unsigned int id, void *rValue, unsigned int len)
{
    obcif_arg myarg;
    
    if(open_adsl_drv()) 
    {        
        myarg.argsize = (int) len;
        myarg.arg = (int) (rValue);

        if (ioctl(fileno(adslFp), id, &myarg) < 0) 
        {
            printf("ADSL ioctl failed! id=%x\n",id );            
            close_adsl_drv();
            
            return 0;
        };

        close_adsl_drv();
        return 1;
    }
    return 0;
}

/*************************************************************************
功能: 按照模式启动RTL8672 ADSL
参数: dsl_mode:  DSL模式
             olr:  
返回: 错误码
备注: 
*************************************************************************/
#define TELEFONICA_DEFAULT_CFG
#ifdef TELEFONICA_DEFAULT_CFG
static unsigned char rtl86782_DSL_LastModeGet()
{
    char buf[4];
    unsigned short Len = 4;
    unsigned char dslMode =0;
    int ret;
    ret = app_item_get(buf, "lastdslmode",&Len);
    if(0 ==ret)
    {
        dslMode =((unsigned char)atoi(buf));
    }
    return(dslMode);
}

static void setDsl_PMSConfig()
{
#if 1
	int pms_cfg[13];
	char buf[16];
	char str[16];
	int i;
	int j =0;
 	int Len =16;

 	memset(&pms_cfg[0], 0, sizeof(int)*13);
    for(i =0; i < 13;  i++)
	{
	    sprintf(str, "DslPmsConfig%d", i);
        if(0 == app_item_get(buf, str, &Len))
        {
            pms_cfg[i] =atoi(buf);
            j++;
        }
        else
        {
            break;
        }
	}
	if(j)
	{
	    adsl_drv_set(RLCM_SET_ADSL_PMS_CONFIG, (void *)&pms_cfg[0], sizeof(int)*13);
	}
#endif
}
#endif


static int DslSet(unsigned short dsl_mode, char olr )
{
    char inp;
    int xmode, axB, axM, axL;
    int adslmode =0;
    int val;
    char tone[64];

    adslmode=(int)(dsl_mode & (ADSL_MODE_GLITE|ADSL_MODE_T1413|ADSL_MODE_GDMT));
    adsl_drv_set(RLCM_PHY_START_MODEM, (void *)&adslmode, 4);
#ifdef TELEFONICA_DEFAULT_CFG
{
	unsigned char vChar;
	vChar =rtl86782_DSL_LastModeGet();
	if(vChar & 0x04)	//if bit2 is 1, set 0 to adsl last opmode
		vChar = 0;
	adsl_drv_set(RLCM_SET_ADSL_LAST_OPMode, (void *)&vChar, 1);
}
#endif

    if (dsl_mode & ADSL_MODE_ANXB) 
    {   
        // Annex B
        axB = 1;
        axL = 0;
        axM = 0;
    }
    else 
    {   
        // Annex A
        axB = 0;
        if (dsl_mode & ADSL_MODE_ANXL)	
        {
            // Annex L
            axL = 3; // Wide-Band & Narrow-Band Mode            
        }        
        else
        {
            axL = 0;
        }
        
        if (dsl_mode & ADSL_MODE_ANXM)	
        {
            // Annex M
            axM = 1;
        }        
        else
        {
            axM = 0;
        }        
    }

    adsl_drv_set(RLCM_SET_ANNEX_B, (void *)&axB, 4);
    adsl_drv_set(RLCM_SET_ANNEX_L, (void *)&axL, 4);
    adsl_drv_set(RLCM_SET_ANNEX_M, (void *)&axM, 4);
    	
    xmode=0; 
    if (dsl_mode & (ADSL_MODE_GLITE|ADSL_MODE_T1413|ADSL_MODE_GDMT))
    {
        xmode |= 1; // ADSL1
    }
    
    
    if (dsl_mode & ADSL_MODE_ADSL2)
    {
        xmode |= 2;	// ADSL2
    }
    
    
    if (dsl_mode & ADSL_MODE_ADSL2P)
    {
        xmode |= 4;	// ADSL2+
    }  
    
    adsl_drv_set(RLCM_SET_XDSL_MODE, (void *)&xmode, 4);

    adsl_drv_set(RLCM_SET_ADSL_MODE, (void *)&adslmode, 4);	

    // for Telefonica
#ifdef TELEFONICA_DEFAULT_CFG
    setDsl_PMSConfig();
#endif
    	
    // set OLR Type
    val = (int)(olr & 3);
    if (val == 2) 
    {
        // SRA (should include bitswap)
        val = 3;
    }
    
    adsl_drv_set(RLCM_SET_OLR_TYPE, (void *)&val, 4);

    //enable 1-bit bitloading
    unsigned char pData[5];
    pData[0] = 1;   //set data
                    //index = 0
    pData[1] = 0;   //index bit 8~15
    pData[2] = 0;   //index bit 0~7
    pData[3] = 1;   //要set的data length = 1 bytes
                    //set data: 1 1bit,0 no 1bit
    pData[4] = (olr & 4)? 1: 0;   
    adsl_drv_set(RLCM_GET_SET_ADSL_PHY_DATA, (void *)pData, 5);
    
    // set Tone mask
    ////mib_get(MIB_ADSL_TONE, (void *)tone);
    memset(tone, 0, sizeof(tone));       /*此值由原版BSP获得*/

    adsl_drv_set(RLCM_LOADCARRIERMASK, (void *)tone, GET_LOADCARRIERMASK_SIZE);

    ////mib_get(MIB_ADSL_HIGH_INP, (void *)&inp);
    inp=0;      /*此值由原版BSP获得*/

    xmode = inp;
    
    adsl_drv_set(RLCM_SET_HIGH_INP, (void *)&xmode, 4);
    
    /*
    if (mib_get(MIB_ADSL_MODE, (void *)&dsl_mode) != 0)
    {
    snprintf(mode, 3, "%u", (int)dsl_mode);
    mode[2] = '\0';
    }
    va_cmd(ADSLCTRL, 2, 1, "startAdsl", mode);
    */

    // new_hibrid
    ////mib_get(MIB_DEVICE_TYPE, (void *)&inp);

    inp=3;      /*此值由原版BSP获得*/

    xmode = inp;
    if (xmode == 1) 
    {
        xmode = 1052;
        adsl_drv_set(RLCM_SET_HYBRID, (void *)&xmode, 4);
    }
    else if (xmode == 2) 
    {
        xmode = 2099;
        adsl_drv_set(RLCM_SET_HYBRID, (void *)&xmode, 4);
    }
    else if (xmode == 3) 
    {
        xmode = 3099;
        adsl_drv_set(RLCM_SET_HYBRID, (void *)&xmode, 4);
    }
    else if (xmode == 4) 
    {
        xmode = 4052;
        adsl_drv_set(RLCM_SET_HYBRID, (void *)&xmode, 4);
    }
    else if (xmode == 5) 
    {
        xmode = 5099;
        adsl_drv_set(RLCM_SET_HYBRID, (void *)&xmode, 4);
    }

    return TBS_SUCCESS;
}

/*************************************************************************
功能: 启动RTL8672 ADSL
参数: argc  参数个数
             argv  参数数据
返回: 错误码
备注: 此进程由logic中的DSL模块启动
*************************************************************************/
int main(int argc, char *argv[])
{
    unsigned short dsl_mode;
    char olr;

    if( argc !=3 )
    {
        return TBS_PARAM_ERR;
    }
         
    dsl_mode = (unsigned short)atoi(argv[1]);
    olr = (unsigned short)atoi(argv[2]);
    
    return DslSet(dsl_mode, olr );
}

/*********************************************end file********************************/
