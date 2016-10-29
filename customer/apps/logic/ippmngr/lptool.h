/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称:lptool.h
 文件描述:Usb Printer Tool header file
 修订记录:
         1 创建:雷达
           日期:2009.04.28
           描述:

**********************************************************************/
#ifndef	__LPTOOL_H__
#define	__LPTOOL_H__

#define	CMD_UNKNOWN		(0)
#define	LPCTL_OK		(0)
#define	LPCTL_FAIL		(-1)
#define	LPCTL_EMALLOC		(-2)
#define	LPCTL_EIOCTL		(-3)

typedef enum{
	LPST_UNKNOWN, 
	LPST_OK, 
	LPST_OutOfPage, 
	LPST_OffLine, 
	LPST_OnFire,
}TlpStatus;

typedef enum{
	DIS_MFG = 0, 
	DIS_MDL, 
	DIS_CMD, 
	DIS_CLS, 
	DIS_DES,
	DIS_FWVER,
	DIS_MAXSEG,
}TDevIdSeg;

//#define	PRINTER_DEV	"/dev/usb/lp0"

#if defined(__cplusplus)
extern "C" {
#endif

/* 
!Notice: if the return value is "(char *)!= NULL", the device-id string is store in allocated memory, 
		it must be free when no need to use by the function's caller.
*/
/* device id sample:
sample1: <2 bytes>MFG:Hewlett-Packard;MDL:HP LaserJet 1020;CMD:ACL;CLS:PRINTER;DES:HP LaserJet 1020;FWVER:20051028; 
sample2: <2 bytes ?>MFG:EPSON;CMD:ESCPAGES-01;MDL:EPL-5900L;CLS:PRINTER;DES:EPSON EPL-5900L; 
*/
/* return NULL for some_Error, or else for "Device_ID string in mem which is new malloc" */
extern char *lpDevId_Pickup(char *devfile);
#define	lpDevId_Release(pDevId)	do{ if(pDevId) free(pDevId); }while(0)
char *lpDevId_Seg_Offset(char *szDevId, int segId, int *segLen);

extern void lp_Reset(char *devfile);
extern int lpGet_VidPid(char *devfile, int *vid, int *pid);
#if defined(__cplusplus)
}
#endif

#endif
