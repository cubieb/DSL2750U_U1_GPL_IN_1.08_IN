/******************************************************************************
*
* Module: flash.h
*
* Description: Flash Module Definitions
*
* Project:     HSI VisionWare -- vWare
*
* Functions:
*
* Notes:
*
* History:		See Visual SourceSafe History
*
* Disclaimer: Copyright 1999-2000 Wind River Systems, Inc. All Rights Reserved
*
*******************************************************************************/
typedef struct
{
	int (*WriteVector)(volatile char *Dest, char *Source, int Length);
	int (*EraseVector)(volatile char *SectorAddress);
	int SectorSize;
	char DeviceName[64];
} FLASHOBJECT;


enum
{
    FLASH_ERROR_INVALID_DEV_CODE = 11,
    FLASH_ERROR_ILLEGAL_IMAGE,
    FLASH_ERROR_FILE_CHECKSUM,
    FLASH_FILE_DOES_NOT_EXIST
};

#define INVOLVE_COMPRESSION

#ifdef CPE_ATM

#define BOOT_PARA_BASE  	    0x500a0000
#define BOOT_CODE_BASE  	    0x50000000

#define VXWORKS_IMAGE1_ADRS	((char *) (0x50100000))
#define VXWORKS_IMAGE2_ADRS	((char *) (0x50180000))

#define IMAGE_LEN 0x700000

#define START_BOOT_LINE_ADRS	((char *) (BOOT_PARA_BASE + 0xf002))
#define START_MAC_IP_ADRS	((char *) (BOOT_PARA_BASE))
#define BOOT_PARA_LENGTH	262144
#define BOOTLINE_OFFSET	    BOOT_PARA_LENGTH-0x100+2

#define BOOTLOADER_INFO_ADRS	((char *) (BOOT_CODE_BASE + 0x10c))
#define CDX_BOOTLOADER_INFO_ADRS	BOOTLOADER_INFO_ADRS	  /* CDX-compatible boot loader*/
#define BDX_BOOTLOADER_INFO_ADRS	((char *) (0xfff02fcf))   /* BDX-compatible boot loader*/

#define DOWNLOAD_1_OFFSET	    0x10   /*first image flag*/
#define DOWNLOAD_2_OFFSET	    0x11   /*second image flag*/
#define DOWNLOAD_I_OFFSET	    0x12   /* index flag */

#define DOWNLOAD_1_ADRS	        ((char *) (BOOT_PARA_BASE + DOWNLOAD_1_OFFSET))   /*first image */
#define DOWNLOAD_2_ADRS	        ((char *) (BOOT_PARA_BASE + DOWNLOAD_2_OFFSET))   /*second image */
#define DOWNLOAD_I_ADRS	        ((char *) (BOOT_PARA_BASE + DOWNLOAD_I_OFFSET))   /* index flag */

#define IMAGE_1_OFFSET	        0x20   /* offset for image 1 length*/
#define IMAGE_2_OFFSET	        0x30   /* offset for image 2 length*/

#define IMAGE_1_LEN	            ((int *) (BOOT_PARA_BASE + IMAGE_1_OFFSET))   /* Address for image 1 length*/
#define IMAGE_2_LEN	            ((int *) (BOOT_PARA_BASE + IMAGE_2_OFFSET))   /* Address for image 2 length */

#define	SYSINFO_OFFSET			0x40	/* offset for system information */
#define	SYSINFO_LEN				0x20	/* 32 bytes for system information */
#define	CONFIG_INFO_OFFSET	    0x70	/* offset for system information */

#define STRING_1_OFFSET	        0x100   /* Address for string 1 */
#define STRING_2_OFFSET	        0x200   /* Address for string 1 */

#define STRING_1_ADRS	        ((char *) (BOOT_PARA_BASE + STRING_1_OFFSET))   /* Address for string 1 */
#define STRING_2_ADRS	        ((char *) (BOOT_PARA_BASE + STRING_2_OFFSET))   /* Address for string 1 */


#define FLASH_SECT_SIZE	0x10000
#define IMAGE_STR_SIZE	0x100

#else  /* CPE_ATM */

#define VXWORKS_IMAGE1_ADRS	((char *) (0xffC00000))
#define VXWORKS_IMAGE2_ADRS	((char *) (0xffD80000))

#define IMAGE_LEN 0x180000


#define START_BOOT_LINE_ADRS	((char *) (0xfffff002))
#define START_MAC_IP_ADRS	((char *) (0xfffc0000))
#define BOOT_PARA_LENGTH	262144
#define BOOTLINE_OFFSET	    BOOT_PARA_LENGTH-0x100+2

#define BOOTLOADER_INFO_ADRS	((char *) (0xfff0010c))
#define CDX_BOOTLOADER_INFO_ADRS	BOOTLOADER_INFO_ADRS	  /* CDX-compatible boot loader*/
#define BDX_BOOTLOADER_INFO_ADRS	((char *) (0xfff02fcf))   /* BDX-compatible boot loader*/

#define DOWNLOAD_1_ADRS	        ((char *) (0xfffC0010))   /*first image */
#define DOWNLOAD_2_ADRS	        ((char *) (0xfffc0011))   /*second image */
#define DOWNLOAD_I_ADRS	        ((char *) (0xfffc0012))   /* index flag */

#define DOWNLOAD_1_OFFSET	    0x10   /*first image flag*/
#define DOWNLOAD_2_OFFSET	    0x11   /*second image flag*/
#define DOWNLOAD_I_OFFSET	    0x12   /* index flag */

#define IMAGE_1_LEN	            ((int *) (0xfffc0020))   /* Address for image 1 length*/
#define IMAGE_2_LEN	            ((int *) (0xfffc0030))   /* Address for image 2 length */


#define IMAGE_1_OFFSET	        0x20   /* offset for image 1 length*/
#define IMAGE_2_OFFSET	        0x30   /* offset for image 2 length*/

#define	SYSINFO_OFFSET			0x40	/* offset for system information */
#define	SYSINFO_LEN				0x20	/* 32 bytes for system information */
#define	CONFIG_INFO_OFFSET	    0x70	/* offset for system information */


#define STRING_1_ADRS	        ((char *) (0xfffc0100))   /* Address for string 1 */
#define STRING_2_ADRS	        ((char *) (0xfffc0200))   /* Address for string 1 */



#define STRING_1_OFFSET	        0x100   /* Address for string 1 */
#define STRING_2_OFFSET	        0x200   /* Address for string 1 */


#define FLASH_SECT_SIZE	0x40000
#define IMAGE_STR_SIZE	0x100

#endif /* CPE_ATM */
