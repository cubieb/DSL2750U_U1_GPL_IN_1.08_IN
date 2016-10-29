/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称 : timer.h
 文件描述 : timer 模块的公共头文件

 修订记录 :
          1 创建 : 詹剑
            日期 : 2008-3-17
            描述 :
            
          2 修改 : 徐晓东
            日期 : 2009-7-6
            描述 : 整理代码和注释
**********************************************************************/

#ifndef  __TIMER_H__
#define  __TIMER_H__

#include <time.h>

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/
#define TMR_EXPRESSION 0
#define TMR_INTERVAL   1

#define TIMER_NAME_LENGTH_MAX 16
#define TIMER_BASE_YEAR    1900

#define TIMER_TIMEOUT_SEND_NEED_NOT 0
#define TIMER_TIMEOUT_SEND_NEED 1

#define TIMER_DELETE_NEED_NOT 0
#define TIMER_DELETE_NEED 1

#define TIMER_RELATION_ARRAY_LEN 6

#define TIMER_MID_ALL 0xffff
#define TIMER_ID_ALL 0xffffffff

/* 365 天中的秒数, 365 x 24 x 60 x 60, 即：34,164,000 秒 */
#define TIMER_YEAR_SECOND 0x02094D20L 

/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
typedef struct tag_ST_TIMER_EXPRESSION
{
    unsigned char ucYear;  /* 从 1900 年开始的年份, 允许通配 0xff */
    unsigned char ucMonth; /* 月，1 ~ 12, 允许通配 0xff, 
                            * 注意：
                            *   按照 Linux 的实现，月份是以 0 开始的，
                            *   参考 struct tm 结构成员 tm_mon 的说明： 
                            *   tm_mon: The number of months since January, in the range 0 to 11.
                            *   所以这里的月份 = Linux 系统时间中的月份 + 1 */
    unsigned char ucDay;    /* 日，1 ~ 31, 允许通配 0xff，闰月的情况由调用者自己处理 */
    unsigned char ucHour;   /* 时，0 ~ 23, 允许通配 0xff */
    unsigned char ucMinute; /* 分，0 ~ 59, 允许通配 0xff */
    unsigned char ucSecond; /* 秒，0 ~ 59, 允许通配 0xff */
    /* 以上六项不能同时为通配情况。 */

    unsigned char ucLastSend;   /* 上一次是否产生超时通知 */
    unsigned char aucPad;
} ST_TIMER_EXPRESSION;

typedef struct tag_ST_TIMER_INTERVAL
{
    time_t shot;                /* 下一次超时时间 */
    unsigned long ulInterval;   /* 定时器间隔时间,不能为0, 最大值为365天中的秒数,
                                   TIMER_YEAR_SECOND(0x02094D20L=34,164,000=365 x 24 x 60 x 60)*/
} ST_TIMER_INTERVAL;

/*描述定时器的结构*/ 
typedef struct tag_ST_TIMER_ENTRY
{    
    char szTimerName[TIMER_NAME_LENGTH_MAX]; /*定时器名称,不能为空字符串*/
    unsigned long ulTimerID;  /*初始化时为0, 注册后为1~0xfffffffe,0xffffffff保留为表示所有定时器*/  
    int iFlags; /*表示定时器类型TMR_EXPRESSION 或 TMR_INTERVAL*/
    union
    {
        ST_TIMER_EXPRESSION stTmrExp;
        ST_TIMER_INTERVAL stTmrInter;
    } unTmrInfo;

    unsigned short usMID; /*模块 ID, 不能为 0x0、0xffff*/
    char acPad[2];
    struct tag_ST_TIMER_ENTRY* pstPre;  /*初始化为 NULL*/
    struct tag_ST_TIMER_ENTRY* pstNext; /*初始化为 NULL*/
} ST_TIMER_ENTRY;

/*****************************************************************************
 *                              Function Prototype                           *
 *****************************************************************************/
/**************************************************************************
功能: 添加一个定时器
参数: pstTimer  定时器结构指针,由调用者负责分配内存和初始化
返回: 成功 -- TBS_SUCCESS(若注册成功, 修改 pstTimer->ulTimerID 的值)
      失败 -- 其它错误码
备注: 1.参数 ST_TIMER_ENTRY *pstTimer 表示的时间允许有两种类型:
        ST_TIMER_EXPRESSION 或 ST_TIMER_INTERVAL;
      2.添加 ST_TIMER_EXPRESSION 类型的定时器时请注意:
        此结构中月份遵从习惯表达法,从 1 开始计数的, 但是在Linux 中表示时间
        的月份是从 0 开始的, 所以如果取Linux系统时间来更新ST_TIMER_EXPRESSION
        结构，要将tm_mon + 1才是习惯上说的月;
      3. 该API只能由logic内的模块调用
***************************************************************************/
int TIMER_AddTimer(ST_TIMER_ENTRY *pstTimer);

/**************************************************************************
功能: 1. 添加一个INTERVAL定时器
      2. 修改已经注册过的定时器
         (根据usMID, pulTimerID, pcTimerName识别已注册过的定时器)
参数: usMID       模块 MID
      pulTimerID  注册成功的TimerID指针,值为NULL时不记录 TimerID
      pcTimerName 定时器名称, 不能为空字符串
      ulInterval  定时器间隔时间, 不能为0, 最大值为365天中的秒数(34164000)
      iFlag       是否重复标志, 0-不重复，即只提醒一次, 非0-重复提醒     
返回: 成功 -- TBS_SUCCESS
      失败 -- 其它错误码
备注: 该API只能由logic内的模块调用
***************************************************************************/
int TIMER_QuickAddTimer(unsigned short usMID, unsigned long *pulTimerID, 
                        const char *pcTimerName, unsigned long ulInterval, int iFlag);

/**************************************************************************
功能: 删除一个模块注册的定时器      
参数: usMID       模块 MID
      ulTimerID   已注册的定时器ID, 定时器ID为TIMER_ID_ALL(0xffffffff) 
                  时删除该模块注册的所有定时器      
返回: 成功 -- TBS_SUCCESS
      失败 -- 其它错误码
备注: 该API只能由logic内的模块调用
***************************************************************************/
int TIMER_DelTimer(unsigned short usMID, unsigned long ulTimerID);

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/
#endif /*__TIMER_H__*/

