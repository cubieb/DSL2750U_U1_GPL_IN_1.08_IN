/**************************************************************
$Workfile:   bmetl.h  $
***************************************************************
**
** FIRMWARE, Copyright (C) 2000-2002
** Ikanos Communication, Fremont, CA
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**
$Revision: 1.1 $
$Date: 2008/07/18 11:08:36 $
**************************************************************/
#ifndef _BMETL_H_
#define _BMETL_H_

/* #includes */

/* #defines */
#define MAX_TL_CMDS			  90
#define MAX_CMD_LENGTH		20
#define MAX_NUM_OF_PARAMS	20
#define MAX_PARAM_LENGTH	32
#define MAX_TAG_LENGTH		2

/* parameter type */
#define _COMMAND_			0
#define _CTAG_				1
#define _BMENUM_			2
#define _PORTNUM_			3

/* global vars */
typedef struct tTlCmdString {
	U8 Cmd[MAX_CMD_LENGTH+1];
	U8 CTag[MAX_TAG_LENGTH+1];
	eCommands ApiCmd;
	} tTlCmdString;

typedef struct tCmdInfo {
	U8 Index;
	int Length;	
	} tCmdInfo;

typedef enum eTlCmdType {
	HELP_CMD,	
	CONFIG_CMD,
	CTRL_CMD,
	STATUS_REQUEST_CMD,
	PERFORMANCE_REPORT_CMD,
	THRESHOLD_REPORT_CMD,
	AUTONOMOUS_CMD,
	DEBUG_CMD,
	WRONG_CMD
	} eTlCmdType;

extern int ReceiveTLCommand (U8 *);
extern int ReceiveTLCommandACK(U8 *, send_ack_info_t *);
extern U8 remDldImgFlag;

#endif /* _BMETL_H_ */

/* END_OF_FILE */
