/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef LIB_DSL_MDIO_H
#define LIB_DSL_MDIO_H

#ifdef __cplusplus
   extern "C" {
#endif


/** \file
   Common MDIO interface definitions
   for clause 22 and clause 45 of IEEE 802.3
*/

/**
   Address and register bits of the
   clause 22 Control Register
   IEEE 802.3 chapter 22.2.4.1
*/
#define MDIO_22_CONTROL             0x0000

#define MDIO_22_CONTROL_RESET       0x8000
#define MDIO_22_CONTROL_SPEED_LSB   0x2000
#define MDIO_22_CONTROL_AUTONEG     0x1000
#define MDIO_22_CONTROL_DUPLEX      0x0100
#define MDIO_22_CONTROL_SPEED_MSB   0x0040


/**
   Addresses of the
   clause 45 MDIO Menagable Devices (MMD)
   IEEE 802.3 chapter 45.2
*/
#define MDIO_45_PMA_PMD 1
#define MDIO_45_WIS 2
#define MDIO_45_PCS 3
#define MDIO_45_PHY_XS 4
#define MDIO_45_DTE_XS 5
#define MDIO_45_TC 6
#define MDIO_45_CL22_EXT 29
#define MDIO_45_VS_1 30
#define MDIO_45_VS_2 31

/**
   Defines the access type of an MDIO transmission.
*/
typedef enum
{
   /** Read access */
   DSL_VNX_MDIO_READ,
   /** Write access */
   DSL_VNX_MDIO_WRITE,
   /** delimiter */
   DSL_VNX_MDIO_ACCESS_LAST
} DSL_MdioDirection_t;

/**
   This structure holds information for one
   MDIO Clause45 register read or write access.
   See IEEE 802.3 Chapter 45
*/
typedef struct
{
   /**
      Address of the MDIO Menaged Device
      See IEEE 802.3 Chapter 45.2
   */
   DSL_uint8_t    nMmdAddress;
   /**
      Address of the Register
      See IEEE 802.3 Chapter 45.2.x
   */
   DSL_uint16_t   nRegisterAddress;
   /**
      Value of the Register
   */
   DSL_uint16_t   nValue;
} DSL_MDIO_Clause45_t;

#ifdef __cplusplus
}
#endif

#endif /* LIB_DSL_MDIO_H */
