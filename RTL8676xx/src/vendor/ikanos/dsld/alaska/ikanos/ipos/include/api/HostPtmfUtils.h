/******************************************************************************/
/*                                                                            */
/*         Copyright(c) 2000-2002 by Ikanos Communications Inc.               */
/*                                                                            */
/*  All rights are reserved.  Reproduction in whole or in part is prohibited  */
/*  without the written consent of the copyright owner.                       */
/*                                                                            */
/*  The information and contents of this file are the proprietary information */
/*  of Ikanos Communication and may not be disclosed or used without the      */
/*  formal written approval of Ikanos Communication Inc.                      */
/*                                                                            */
/*  This Copyright notice may not be removed or modified without prior        */
/*  written consent of Ikanos Communications, Inc.                            */
/*                                                                            */
/******************************************************************************/
/*
**  Filename:    HostPtmfUtils.h
**
**  Project:     Ikanos INI ASIC (AMUR) Bring UP
**
**  Target:      MOTOROLA XPC855T based host board
**
**  Platform specific:
**    None.
**
**  Description:
**
**  $Revision: 1.1 $
**  $Date: 2008/07/18 11:09:32 $
**
**
**  **************************************************************************** 
**  Modification History:
**
**    Date         By     Description
**    -----------  ---    ------------------------------------------------------
**	  Jun-07-2002  SFA    Created
**    Jul-24-2002  SFA    Integration into new host code
*/
/******************************************************************************/

/* HEADER SENTINEL                                                            */
#ifndef _HostPtmfUtils_H_
#define _HostPtmfUtils_H_

/*------------------------------------------------------------------------------
  INCLUDE FILES
  ----------------------------------------------------------------------------*/
//#include "HostIniHwRegs.h"
#include "apitype.h"

/*------------------------------------------------------------------------------
  PREPROCESSOR CONSTANTS
  ----------------------------------------------------------------------------*/
#define DEBUG_INI

/*------------------------------------------------------------------------------
  PREPROCESSOR MACROS
  ----------------------------------------------------------------------------*/
#define EPLD_BASE_ADDRESS               0x07000000
#define EPLD_INTERFACE_SIZE             0x00010000     /* 64kb */

#define EPLD_INI_XMII_MODE              EPLD_BASE_ADDRESS
#define EPLD_INI_CHIP_ADDRESS           EPLD_BASE_ADDRESS + 1
#define EPLD_INI_PHY_MAC_DEV_ID         EPLD_BASE_ADDRESS + 2

/*------------------------------------------------------------------------------
  ENUMERATION DEFINITIONS
  ----------------------------------------------------------------------------*/
/* Error codes enumeration for the module Host INI FPGA Utils                 */
  typedef enum
  {
    HOST_INI_NO_ERROR = 0,
    HOST_INI_MDIO_READ_ERROR,
    HOST_INI_MDIO_WRITE_ERROR,
    HOST_INI_MAX_ENET_PORT_ERROR,
    HOST_INI_REG_READ_TIMEOUT,
	  HOST_INI_ILLEGAL_MDIO_REG,
    HOST_INI_INVALID_ADDRESS_ENTRY,
    HOST_INI_UNDEFINED_ERROR

  } eIniRetCode;

/*------------------------------------------------------------------------------
  TYPE DEFINITIONS
  ----------------------------------------------------------------------------*/
typedef struct _tsHostIniMdioAddrData
{
  U8   bIniBaseAddr;
  U16  uiIniPhyAddr;
  U16  uiIniRegAddr;
  U16  uiIniRdWrData;

} tsHostIniMdioAddrData, *ptsHostIniMdioAddrData;
typedef struct _tsHostIniMdioAddrData1
{
  U32  uiIniRegAddr;
  U32  uiIniRdWrData;
} tsHostIniMdioAddrData1, *ptsHostIniMdioAddrData1;

/*------------------------------------------------------------------------------
  EXTERNAL REFERENCES
  ----------------------------------------------------------------------------*/
extern U8                  gbDebugLevel;

/*------------------------------------------------------------------------------
  PUBLIC FUNCTION PROTOTYPE
  ----------------------------------------------------------------------------*/
void 
  iposPtmfMdioOff (void);

void
  iposPtmfMdioSetBaseAddr(U8 bMdioAddr);

void 
  iposPtmfSemCreate (void);

void
  iposPtmfModeSet(U8 bIniAsicMode);

eIniRetCode
  iposMdioRead (ptsHostIniMdioAddrData ptsIniReg);

eIniRetCode
  iposMdioWrite (ptsHostIniMdioAddrData ptsIniReg);

eIniRetCode
  iposPtmfMdioRead (ptsHostIniMdioAddrData ptsIniReg);

eIniRetCode
  iposPtmfMdioRead1 (ptsHostIniMdioAddrData1 ptsIniReg1);

eIniRetCode
  iposPtmfMdioWrite (ptsHostIniMdioAddrData ptsIniReg);

eIniRetCode
  iposPtmfMdioWrite1 (ptsHostIniMdioAddrData1 ptsIniReg1);
void 
  iposPtmfResetPort (U8 bPtmfBaseAddr, U16 bPortAddr);

#endif  // _HostPtmfUtils_H_
/*------------------------------------------------------------------------------
                                 END OF FILE
------------------------------------------------------------------------------*/
