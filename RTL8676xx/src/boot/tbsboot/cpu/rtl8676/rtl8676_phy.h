/*
* ----------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* $Header: /usr/local/dslrepos/boot0412/tftpnaive/sw/include/rtl8650/phy.h,v 1.1.1.1 2011/06/10 08:06:31 yachang Exp $
*
* Abstract: PHY access header file.
*
* $Author: yachang $
*
* $Log: phy.h,v $
* Revision 1.1.1.1  2011/06/10 08:06:31  yachang
*
*
* Revision 1.3  2005/09/22 05:22:31  bo_zhao
* *** empty log message ***
*
* Revision 1.1.1.1  2005/09/05 12:38:24  alva
* initial import for add TFTP server
*
* Revision 1.2  2004/03/31 01:49:20  yjlou
* *: all text files are converted to UNIX format.
*
* Revision 1.1  2004/03/16 06:36:13  yjlou
* *** empty log message ***
*
* Revision 1.1.1.1  2003/09/25 08:16:56  tony
*  initial loader tree 
*
* Revision 1.1.1.1  2003/05/07 08:16:07  danwu
* no message
*
* ---------------------------------------------------------------
*/

#ifndef _PHY_H_
#define _PHY_H_


#define	CONFIG_RTL8196C 1
#define CONFIG_RTL8196D 1
#define CONFIG_RTL865XC 1
//#define	CHIP_RTL8367RB	1

#define PHY_CTRL_REG            0
#define PHY_STS_REG             1
#define PHY_ID1_REG             2
#define PHY_ID2_REG             3
#define PHY_ANADV_REG           4
#define PHY_ANLP_REG            5


u32 phy_readReg(u32 port, u32 regnum);
int phy_writeReg(u32 port, u32 regnum, u32 value);
#ifdef 	EXTERNAL_GIGAPHY
u32 rtk_switch_init(void);
#endif

#endif   /* _PHY_H_ */

