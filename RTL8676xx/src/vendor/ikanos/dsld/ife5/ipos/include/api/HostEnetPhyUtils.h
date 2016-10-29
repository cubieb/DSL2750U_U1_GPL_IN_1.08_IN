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
**  Filename:    HostEnetPhyUtils.h
**
**  Project:     BME-CPE 2.0 based modem.
**
**  Target:      MOTOROLA XPC855T based host board
**
**  Platform specific:
**    None.
**
**  Description:
**
**  $Revision: 1.1 $
**  $Date: 2008/07/18 11:09:31 $
**
**
**  **************************************************************************** 
**  Modification History:
**
**    Date         By     Description
**    -----------  ---    ------------------------------------------------------
**	  Jun-16-2002  SFA    Created
**    Jul-17-2002  SFA    Integration into new host code
**    Nov-19-2002  SFA    Ethernet PHY related function seperated from PTMF code
*/
/******************************************************************************/

/* HEADER SENTINEL                                                            */
#ifndef _HostEnetPhyUtils_H_
#define _HostEnetPhyUtils_H_

/*------------------------------------------------------------------------------
  INCLUDE FILES
  ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
  PREPROCESSOR CONSTANTS
  ----------------------------------------------------------------------------*/

// IEEE 802.3 STANDARD REGISTERS.
#define ENET_PHY_STATUS_REG       1
#define ENET_VALID_LINK           0x04
#define ENET_AUTONEGO_CAPABLE     0x08
#define ENET_AUTO_NEGOTIATE_OVER  0x20

#define ENET_PHY_LINK_ABILITY     5
#define ENET_10_HDX               0x0020
#define ENET_10_FDX               0x0040
#define ENET_100_HDX              0x0080
#define ENET_100_FDX              0x0100
#define ENET_T4                   0x0200
#define ENET_FCS                  0x0400

#define ENET_PHY_ID1_REG          2
#define ENET_PHY_ID2_REG          3

#if defined(P2CPE)

// Intel LXT9785 specific definitions (USED IN 8 Port reference)
#define ENET_PHY_SPEED_STATUS_REG   17
#define ENET_PHY_LINK_SPEED         0x4000

#define ENET_PHY_DUPLEX_STATUS_REG  17
#define ENET_PHY_LINK_DUPLEX        0x200

#define ENET_PHY_PAUSE_STATUS_REG   5
#define ENET_LINK_PAUSE_CAPABLE     0x400

#endif //#if defined(P2CPE)

// Intel LXT971A specific definitions (USED IN PHASE 2 CPE with INI FPGA)
#define ENET_PHY_INT_ENABLE_REG     0x12
#define ENET_PHY_INTEN              0x02
#define ENET_PHY_LINK_INT_MASK      0x10

#define ENET_PHY_INT_STATUS_REG     0x13
#define ENET_PHY_MII_INT            0x04


/*------------------------------------------------------------------------------
  PREPROCESSOR MACROS
  ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  ENUMERATION DEFINITIONS
  ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  TYPE DEFINITIONS
  ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  EXTERNAL REFERENCES
  ----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  PUBLIC FUNCTION PROTOTYPE
  ----------------------------------------------------------------------------*/

void 
  iposEnetPhyEnableInt(U8 bEnetPhyMdioAddr);

U16 
  iposEnetPhyReadIntStatus(U8 bEnetPhyMdioAddr);

U16 
  iposReadEnetPhyStatus(U8 bEnetPhyMdioAddr);

U16 
  iposReadEnetPhyGetDuplex(U8 bEnetPhyMdioAddr);

U16 
  iposReadEnetPhyGetSpeed(U8 bEnetPhyMdioAddr);

U16 
  iposReadEnetPhyLinkAbility(U8 bEnetPhyMdioAddr);

U16 
  iposReadEnetPhyLinkPauseCapable(U8 bEnetPhyMdioAddr);

U16 
  iposReadEnetPartnerAbility(U8 bEnetPhyMdioAddr);


#if defined(CPE_HOSTLESS)
void 
  iposEnetPhyHwReset(void);
#endif //#if defined(CPE_HOSTLESS)

#endif  // _HostEnetPhyUtils_H_
/*------------------------------------------------------------------------------
                                 END OF FILE
------------------------------------------------------------------------------*/
