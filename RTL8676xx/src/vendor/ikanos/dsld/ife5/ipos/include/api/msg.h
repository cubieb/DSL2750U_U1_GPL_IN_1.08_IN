
/******************************************************************************
  Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

  文件名称: tbsmsg.h
  文件描述: 公共消息定义
  函数列表:
  修订记录:
         1. 作者: 武萌
            日期: 2007-9-5
            内容: 创建文件
 ******************************************************************************/

#ifndef __TBS_MSG_H__
#define __TBS_MSG_H__

#include "tbstype.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif


/* MSG模块错误码基地 */
#define MSG_ERR_ADDR                (MID_CCP << 16)

#define MSG_OK   0

/* 通过mid得到pid */
#define MID2PID(mid) ((mid) >> 8)

/* MSG模块错误码 */
enum
{
    MSG_MALLOC_FAIL = MSG_ERR_ADDR + 1,   /* 内存分配失败 */
    MSG_PARA_INVALID,                     /* 参数非法 */
    MSG_MID_INVALID,                      /* MID非法 */
    MSG_ERR_SOCKET,                       /* 套接字错误 */
    MSG_PROC_NOT_FOUND,                   /* 消息处理函数没有找到 */
    MSG_SEND_FAIL,                        /* 消息发送失败 */
    MSG_DSTMID_UNREGED,                   /* 目的模块没有注册 */
    MSG_NOT_FULL,                         /* 进程外消息包不完整 */
    MSG_PART_NOEFFECT,                    /* 无效消息片 */
    MSG_PART_INVALID,                     /* 非法消息片 */

    MSG_TIMEOUT,                          /* 接收消息超时 */
    MSG_PART_LIST_FULL,                   /* 片段链表已满 */

	MSG_BME_PORT_NOT_READY, 
	MSG_BME_PORT_NOT_IN_SHOWTIME,
	MSG_BME_PORTNUM_ERROR   
};


/* 消息头 */
typedef struct tag_ST_MSGHEAD
{
    unsigned short usSrcMID;       /* 源模块MID */
    unsigned short usDstMID;       /* 目的模块MID */
    unsigned long  ulMsgID;        /* 消息ID----应用程序自定义 */
    unsigned short usMsgType;      /* 消息类型----应用程序自定义 */
    unsigned short usFirstMID;     /* 消息的最先发出者 */
    unsigned long  ulBodyLength;   /* 消息体长度 */
}ST_MSGHEAD;

typedef struct
{
    unsigned short usSrcMID;       /*源模块MID*/
    unsigned short usDstMID;       /*目的模块MID*/
    unsigned long ulMsgID;        /*消息ID----应用程序自定义*/
    unsigned short usMsgType;      /*消息类型----应用程序自定义*/
    unsigned short usReserve;      /*保留*/
    unsigned long ulBodyLength;   /*消息体长度*/
    unsigned long Result;
}ST_ACKMSGHEAD;

/* 消息 */
typedef struct tag_ST_MSG
{
    ST_MSGHEAD  stMsgHead;
    char        szMsgBody[0];
}ST_MSG;


/******************************** VDSL **********************************/
typedef struct {
    unsigned long Result;
    unsigned short BMEID;
    unsigned short PortID;
    unsigned char PortStatus;
} port_status_brief_t;

typedef struct {
    ST_MSGHEAD stMsgHead;
    port_status_brief_t port_status_brief;
}msg_port_status_brief_t;

typedef struct {
    unsigned long Result;
    unsigned short BMEID;
    unsigned short PortID;
    unsigned long DsLineRate;
    unsigned long UsLineRate;
    unsigned long FastDsPayloadRate;
    unsigned long SlowDsPayloadRate;
    unsigned long FastUsPayloadRate;
    unsigned long SlowUsPayloadRate;
    unsigned char FastDsNumParityBytes;
    unsigned char SlowDsNumParityBytes;
    unsigned char FastUsNumParityBytes;
    unsigned char SlowUsNumParityBytes;
    unsigned short DsDelay;
    unsigned short UsDelay;
    unsigned short TransmitPower;
    unsigned short DsTrainingMargin;
    unsigned short DsAttainableRate;
    unsigned short DsAttainableLineRate;
    unsigned short SlowDsLineProtection;
    unsigned short UsTrainingMargin;
    unsigned short UsAttainableRate;
    unsigned short UsAttainableLineRate;
    unsigned short SlowUsLineProtection;
    unsigned char NeItuVendorId[8];
    unsigned char FeItuVendorId[8];
    unsigned short FeTransmitPower;
    unsigned short LoopLength;
    unsigned short ghsNearEndLoopLength;
    unsigned short ghsFarEndLoopLength;
    unsigned short SlowUsFFTProtection;
    unsigned short SlowDsFFTProtection;
    unsigned char FastDsCodeWordSize;
    unsigned char SlowDsCodeWordSize;
    unsigned char FastUsCodeWordSize;
    unsigned char SlowUsCodeWordSize;
    unsigned char DsInterleaverDepth;
    unsigned char UsInterleaverDepth;
    unsigned char DsInterleaverBlkLen;
    unsigned char UsInterleaverBlkLen;
    unsigned char CurrentFramingMode;
    unsigned char BandType;
    unsigned char NumOfBandsUsDs;
    unsigned char LineType[20];
} port_status_t;

typedef struct {
    ST_MSGHEAD stMsgHead;
    port_status_t port_status;
}msg_port_status_t;

typedef struct {
    unsigned long Result;
    unsigned short BMEID;
    unsigned short PortID;
    unsigned long EnetTxFrameCount;
    unsigned long EnetOctXmitCount;
    unsigned long TxPauseFrameCount;
    unsigned long BroadCastFrameCount;
    unsigned long EnetDropFrameCount;
    unsigned long AllignErrorCount;
    unsigned long LongFrameCount;
    unsigned long RxErrorCount;
    unsigned long EnetCrcErrorCount;
    unsigned long CarrierSenseErrorCount;
    unsigned long EnetRxFrameCount;
    unsigned long EnetOctRcvCount;
    unsigned long RxPauseFrameCount;
    unsigned long RxHdlcCount;
    unsigned long RxHdecCount;
    unsigned long RxHdscCount;
    unsigned long RxHdmcCount;
    unsigned long RxHdDefTransCount;
    unsigned long RxHdlcErrorFrameCount;
    unsigned long RxHdlcInvalidFrameCount;
    unsigned long RxHdlcDropFrameCount;
    unsigned short NeLOS;
    unsigned short NeLOF;
    unsigned short NeLOM;
    unsigned short NeESE;
    unsigned short NeNCD_S;
    unsigned short NeLCD_S;
    unsigned short NeNCD_F;
    unsigned short NeLCD_F;
    unsigned short FeLOS;
    unsigned short FeRDI;
    unsigned short FeLPR;
    unsigned short FeLOM;
    unsigned short FeESE;
    unsigned short FeNCD_S;
    unsigned short FeLCD_S;
    unsigned short FeNCD_F;
    unsigned short FeLCD_F;
    unsigned short FePO;
    unsigned short NeSES;
    unsigned short NeES;
    unsigned int NeFECS;
    unsigned int NeLOSS;
    unsigned int NeUnavl;
    unsigned int FeSES;
    unsigned int FeES;
    unsigned int FeFECS;
    unsigned int FeLOSS;
    unsigned int FeUnavl;
    unsigned int NeFEC_S;
    unsigned int NeFEC_F;
    unsigned int NeCRC_S;
    unsigned int NeCRC_F;
    unsigned int NeHEC_S;
    unsigned int NeHEC_F;
    unsigned int FeFEC_S;
    unsigned int FeFEC_F;
    unsigned int FeCRC_S;
    unsigned int FeCRC_F;
    unsigned int FeHEC_S;
    unsigned int FeHEC_F;
    unsigned int numUSRcell_S;
    unsigned int numUSRcell_F;
    unsigned int numIDLEcell_S;
    unsigned int numIDLEcell_F;
    unsigned int numIDLEcellBe_S;
    unsigned int numIDLEcellBe_F;
    unsigned int numTxBlk_S;
    unsigned int numTxBlk_F;
    unsigned int numRxBlk_S;
    unsigned int numRxBlk_F;
    unsigned int NeFECUnCrr_S;
    unsigned int NeFECUnCrr_F;
    unsigned short sixPtLineHold;
    unsigned short numReInit;
    unsigned short numInitFailure;
    unsigned char NeBER_S;
    unsigned char NeBER_F;
} vdsl_status_t;

typedef struct {
    ST_MSGHEAD stMsgHead;
    vdsl_status_t vdsl_status;
} msg_vdsl_status_t;

typedef struct {
    unsigned long Result;
    unsigned short BMEID;
    unsigned short PortID;	
    long SnrMargin;
    unsigned long Attn;
    long SnrAvgMargin;
    long SnrAvg;
}port_snr_status_t;

typedef struct {
    ST_MSGHEAD stMsgHead;
    port_snr_status_t port_snr_status;
}msg_port_snr_status_t;

typedef struct {
    unsigned long Result;
    unsigned char VersionStr[250];
} bme_version_t;

typedef struct {
    ST_MSGHEAD stMsgHead;
    bme_version_t bme_version;
}msg_bme_version_t;



/* 模块接口返回值类型 */
typedef int  MSG_RET;

/* 回调消息处理函数指针 */
typedef void (*FUN_RECEIVE)(ST_MSG *pstMsg);


/* 模块注册, 注销接口 */
MSG_RET MSG_RegModule(unsigned short usMID, FUN_RECEIVE funReceive);
MSG_RET MSG_UnregModule(unsigned short usMID, FUN_RECEIVE funReceive);

void MSG_AllModStartOK(void);

/* 消息包构建，释放接口 */
ST_MSG *MSG_CreateMessage(unsigned long ulSize);
void MSG_ReleaseMessage(ST_MSG *pstMsg);


/* 消息发送，接收接口 */
MSG_RET MSG_SendMessage(ST_MSG *pstMsg);
MSG_RET MSG_ReceiveMessage(ST_MSG **ppstMsg, unsigned long ulSecond);


/* 消息调度 */
void MSG_Poll(void);



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /*__TBS_MSG_H__*/

