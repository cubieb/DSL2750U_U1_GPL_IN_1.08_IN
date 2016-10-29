/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : imgbuildermain.c
 文件描述 : 打包工具主程序入口


 函数列表 :
				main

 修订记录 :
          1 创建 : 张喻
            日期 : 200-7-18
            描述 :

          2 修订: 轩光磊
             日期: 2008-2-21
             描述: 为image增加CRC校验和

**********************************************************************/

#include "imgbuilder.h"

int main(int argc, char *argv[])
{
	int i;
	int ret;
	ImageInfo	g_stImgInfo;
	sys_config_t	g_stSysConfig;
	update_hdr_t	g_update_hdr;


	g_stImgInfo.tagArgRequited = 0;
	g_stImgInfo.tagOutputBin = 0;
	g_stImgInfo.tagOutputImg = 0;
	g_stImgInfo.iBlockSize = 65536;      /* default flash block size is 64k */

	memset(&g_stSysConfig,0,sizeof(sys_config_t));
	memset(&g_update_hdr,0,sizeof(update_hdr_t));
	memcpy(g_stSysConfig.tag,"sysc",4);  /* fill tag */
	g_stSysConfig.endian = 0xf;          /* endian is not seted */
	g_stSysConfig.mac[0] = 0xff;         /* mac is not seted */
	g_stSysConfig.ip = 0x0;              /* ip is not seted */



	for(i=1;i<argc;i++)
		debug("%s\n",argv[i]);

	ret = AnalyseParameter(argc, argv, &g_stImgInfo,  &g_stSysConfig, &g_update_hdr);
	if( ret == 1 )    /* if analyse parameter fail,exit  */
	{    	
		if(CreateBinFile(&g_stImgInfo, &g_stSysConfig))
		{
			if(CreateImgFile(&g_stImgInfo, &g_stSysConfig, &g_update_hdr))
			{
				printf( "Generate BIN file and IMG file successfully!\n" );
				debug( "BIN file checksum: 0x%x\n" , crc_checksum.bin_file_checksum);
				debug( "BIN image checksum: 0x%x\n" ,  crc_checksum.bin_image_checksum );
				debug( "IMG file checksum: 0x%x\n" ,  crc_checksum.img_file_checksum );
				debug( "IMG image checksum: 0x%x\n" ,  crc_checksum.img_image_checksum );

			}
		}
		image_info();
	}
	else if( ret == 0 )
	{
		printf("Generate BIN file and IMG file failed!\n");
	}
	
	return 0 ;
}
