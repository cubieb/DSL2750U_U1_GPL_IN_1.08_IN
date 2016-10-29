/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 ÎÄ¼þÃû³Æ : pc_mon.h
 ÎÄ¼þÃèÊö : ½ø³Ì¼à¿Ø¹¦ÄÜÊµÏÖµÄÏà¹Ø¶¨ÒåºÍÉùÃ÷


 º¯ÊýÁÐ±í :


 ÐÞ¶©¼ÇÂ¼ :
          1 ´´½¨ : ³ÂÔ¾¶«
            ÈÕÆÚ : 2007-8-23
            ÃèÊö :

**********************************************************************/


#ifndef __PC_MON_H__
#define __PC_MON_H__


#include "pc_main.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif



/******************************************************************************
 *                                 MACRO                                      *
******************************************************************************/

/* ÅäÖÃÎÄ¼þÒ»ÐÐ×î´ó³¤¶È¶ */
#define PC_FILE_LINE_MAX_LEN   512

/* ×î´óÆô¶¯²ÎÊý¸öÊý */
#define PC_START_MAX_ARGC      20

/* ×é¼þ½ø³ÌÊä³ö×î´ó³¤¶È */
#define PC_PROC_MAX_OUT_LEN    1024

/* Æ¥Åäµ½µÄÐÅÏ¢µÄ×î´ó³¤¶È( ²»º¬ÏûÏ¢Í·) */
#define PC_MSG_MAX_OUT_LEN      5*1024




/******************************************************************************
 *                                 ENUM                                       *
******************************************************************************/



/******************************************************************************
 *                                STRUCT                                      *
******************************************************************************/



/******************************************************************************
 *                               GLOBAL VAR                                   *
******************************************************************************/




/******************************************************************************
 *                               FUNCTION                                     *
******************************************************************************/



/*************************************************************************
 Function:      PC_RET PC_MonInit(void)
 Description:   ¼à¿Ø¹ÜÀíÄ£¿é³õÊ¼»¯º¯Êý
 Calls:         PC_StartKeyProc
 Data Accessed:
 Data Updated:
 Input:         ÎÞ
 Output:        ÎÞ
 Return:        0,³É¹¦;
                ÆäËü, Ê§°Ü
 Others:
 *************************************************************************/
PC_RET PC_MonInit(const char *pcCfgFile);

/*************************************************************************
 Function:      PC_RET PC_StartKeyProc(char *pcCfgFile)
 Description:   Æô¶¯ÏµÍ³¹Ø¼ü½ø³Ì
 Calls:         PC_AllocPcb,
                PC_GetNextWord,
                PC_GetStartInfo
 Data Accessed:
 Data Updated:
 Input:         pcCfgFile, ÅäÖÃÎÄ¼þÃû, º¬Â·¾¶
 Output:        ÎÞ
 Return:        0,³É¹¦;
                ÆäËü, Ê§°Ü
 Others:
 *************************************************************************/
PC_RET PC_StartKeyProc(const char *pcCfgFile);


/*************************************************************************
 Function:      PC_RET PC_OutMonProc(void)
 Description:   Êä³öÐÅÏ¢¼à¿Ø´¦Àíº¯Êý
 Calls:         PC_GetNextPcb
                MSG_SendMessage
 Data Accessed:
 Data Updated:
 Input:         ÎÞ
 Output:        ÎÞ
 Return:        0,³É¹¦;
                ÆäËü, Ê§°Ü
 Others:
 *************************************************************************/
PC_RET PC_OutMonProc(void);


/*************************************************************************
 Function:      PC_RET PC_StateMonProc(void)
 Description:   ½ø³Ì×´Ì¬¼à¿Ø´¦Àíº¯Êý
 Calls:         PC_GetPcbByPid
                MSG_SendMessage
                forkpty
 Data Accessed:
 Data Updated:
 Input:         pcCfgFile, ÏµÍ³ÅäÖÃÎÄ¼þ
 Output:        ÎÞ
 Return:        0,³É¹¦;
                ÆäËü, Ê§°Ü
 Others:
 *************************************************************************/
PC_RET PC_StateMonProc(void);


PC_RET PC_MsgFunc(ST_PC_PROC_CTRL_INFO *pstPcMsg, unsigned short usComID,
                  unsigned char *pucProcID);


/******************************************************************************
 *                                 END                                        *
******************************************************************************/

#ifdef _PC_VER_R1

#include "pc_pcb.h"

PC_RET PC_StartProcOld(
    unsigned short           usComID,
    unsigned char            ucNeedSteady,
    char            *apcArgVals[],
    unsigned char            ucMonCount,
    char            *apcMonVals[],
    unsigned char           *pucProcID);


PC_RET PC_SendProcSignalOld(
    unsigned short           usComID,
    unsigned char            ucProcID,
    unsigned char            ucSignal,
    unsigned char            ucNeedRestart,
    unsigned char            ucNeedSteady,
    char                    *apcArgVals[],
    unsigned char            ucMonCount,
    char                    *apcMonVals[]);

PC_RET PC_StateMonProcOld(ST_PC_PCB *pstPcb, unsigned char ucProcID);


PC_RET PC_OutMonProcOld(const ST_PC_PCB *pstPcb, unsigned char ucProcID);

#endif



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /* __PC_MON_H__ */



