#ifndef IMGBUILDER_EX_H
#define IMGBUILDER_EX_H

#include <inttypes.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "flash_layout.h"

//#define DEBUG 1    
#define bool int
#define true 1
#define false 0

#ifdef	DEBUG
#define debug(fmt,args...)	printf (fmt ,##args)
#else
#define debug(fmt,args...)
#endif	

#define DEFAULT_BINFILE 		"image.bin"
#define DEFAULT_IMGFILE 		"image.img"

typedef struct tag_ST_IMAGE_INFO
{
	int				iArgRequited;          					/* record how many requited parameters have got */
	char			szBootloaderName[256];
	char			szFirstKernelName[256];					// min Image kernel
	char			szFirstRootFileSystemName[256];			// min Image rootfs
	char			szSecondKernelName[256];				// big Image kernel
	char			szSecondRootFileSystemName[256];		// big Image rootfs
	char			szOutputBinName[256];
	char			szOutputImgName[256];
	char			szAppDefaultCfgItemName[256];
	char			szAppDefaultCfgFileName[256];
	char			cTransfer_mode;							/* sysconfig transfer mode. 0: use flash, 1: use sdram */
	char			cEndian;
	int				iBootImgFlag;							/*0: 系统引导image1；1: 系统引导image 2*/
	int				iBlockSize;             						/* Block size of the flash,kernel code will start with this address. */
	unsigned int	uImageType; 								/*文件类型标识符*/
	unsigned char	byTailVersion[VERSION_LEN];	/*尾部信息版本*/
	unsigned char	bySysVerion[VERSION_LEN];
}ST_IMAGE_INFO;

typedef struct tag_ST_PART_IMAGE_INFO
{
	unsigned long	ulCRC;		// 包含kernel和rootfs的校验和数据
	int	iKernelOffset;
	int iKernelLength;
	int iRootFileOffset;
	int iRootFileLength;
	char szFileName[512];
}ST_PART_IMAGE_INFO;

typedef struct
{
        unsigned long bin_image_checksum ;
        unsigned long bin_file_checksum ;
        unsigned long img_image_checksum ;
        unsigned long img_file_checksum ;
}crc_checksum_t;

extern crc_checksum_t   crc_checksum;

void PrintHelp(void);

int AnalyseParameter(int argc, char *argv[], ST_IMAGE_INFO *pstImgInfo , struct  sys_cfg *pstSysConfig);

int CreateBackupImage(ST_IMAGE_INFO *pstImgInfo,  struct  sys_cfg *pstSysConfig);

#endif
