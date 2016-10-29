#include "imgbuilder_ex.h"

/*************************************************************************
 *                                STRUCT                                      *
*************************************************************************/



/*************************************************************************
 *                               GLOBAL VAR                                   *
*************************************************************************/
crc_checksum_t	crc_checksum;


/*************************************************************************
 *                               FUNCTION                                     *
*************************************************************************/


/*=========================================================================
 Function:		static int ConvertEndian(int val,int endian)

 Description:		转换长度为4字节的字节序
 Calls:			无
 Data Accessed:
 Data Updated:
 Input:			val:		要转换的数据
 				endian:	大小端标识
 				
 Output:			i:		转换的结果
 Return:			i
			
 Others:			
=========================================================================*/

static int ConvertEndian(int val,int endian)
{
	int i;
	unsigned char *p;
	unsigned char *q;

	i = val;
	p = ( unsigned char * ) &val;
	q = ( unsigned char * ) &i;

	if(endian == 0x1)
	{
		*q = *(p+3);
		*(q+1) = *(p+2);
		*(q+2) = *(p+1);
		*(q+3) = *p;
	}

	return i;
}

/*=========================================================================
 Function:		static short ConvertEndian16(short val,int endian)

 Description:		转换长度为2字节的字节序
 Calls:			无
 Data Accessed:
 Data Updated:
 Input:			val:		要转换的数据
 				endian:	大小端标识
 				
 Output:			i:		转换的结果
 Return:			i
			
 Others:			
=========================================================================*/

static short ConvertEndian16(short val,int endian)
{
	short i;
	unsigned char *p;
	unsigned char *q;

	i = val;
	p = ( unsigned char * ) &val;
	q = ( unsigned char * ) &i;

	if(endian == 0x1)
	{
		*q = *(p+1);
		*(q+1) = *p;
	}    
	return i;
}

static int item_generate( void *data , char *item_name ,unsigned short data_len ,item_t *item , int *item_len , ST_IMAGE_INFO *pstImgInfo )
{
	if( ( data == NULL ) || ( item_name == NULL ) || ( item == NULL ) || ( item_len ==NULL ) || ( pstImgInfo == NULL ) )
	{
		return 0;
	}

	if( data_len >= pstImgInfo->iBlockSize )
	{
		return 0;
	}

	item->hdr.avail = ITEM_AVAIL;			/* 值条目有效位为11*/
	memcpy( item->data , item_name , strlen( item_name ) );
	memcpy( item->data + strlen( item_name ) , "\0" , 1 );	
	memcpy( item->data + strlen( item_name ) + 1 , data , data_len );	
	item->hdr.len = strlen( item_name ) + 1 + data_len;
	*item_len = ITEM_SIZE( item->hdr.len );

	crc16( item->data , item->hdr.len , &( item->hdr.crc ) );
	item->hdr.len = ConvertEndian16( item->hdr.len , pstImgInfo->cEndian);
	item->hdr.crc = ConvertEndian16( item->hdr.crc , pstImgInfo->cEndian);

	return 1;
}






/*=========================================================================
 Function:		void PrintHelp(void)

 Description:		输出打包工具使用方法
 Calls:			无
 Data Accessed:
 Data Updated:
 Input:			无
  				
 Output:			无
 Return:			无
			
 Others:			
=========================================================================*/

void PrintHelp(void)
{
	printf("This utility generates a BIN file and a IMG file \
	        \nThe generated BIN file is used to write in flash. \
	        \nThe generated IMG file is used to upgrade the system. \n\n"); 
	printf("Usage:  mkimage_ex [-h,--help] \n"); 
	printf("\t\t[-b filename] [-k1 filename] [-k2 filename] [-r1 filename] [-r2 filename] [-app_cfg item_name filename] [-obin filename] [-oimg filename] \n");
	printf("\t\t[-p product] [-v version] [-id board_id]\n");
	printf("\t\t[-eb,-el] [-mac address] [-ip address] [-blk flash_erasesize] [-boot boot_index] [-t repeat mode]\n\n");
	printf("   -h,--help\tPrint this infomation\n");
	printf("   -b\t\tThe name of bootloader file (required)\n"); 
	printf("   -k1\t\tThe name of first Linux kernel file (required)\n");
	printf("   -r1\t\tThe name of first root file system file (required)\n");
	printf("   -k2\t\tThe name of second Linux kernel file (required)\n");
	printf("   -r2\t\tThe name of second root file system file (required)\n");
	printf("   -app_cfg\tThe name of app default config item and file (required)\n");
	
	printf("   -obin\tThe name of output BIN file \n");
	printf("   -oimg\tThe name of output IMG file \n");
	printf("   -p\t\tThe name of product (required)\n");
	printf("   -v\t\tThe version of product (required)\n");
	printf("   -id\t\tThe board_id of product \n");
	printf("\n");
	printf("Configuration information:\n");
	printf("   -be,-le\tBig-endian or little-endian (required)\n");
	printf("   -blk\t\tThe system flash erasesize\n");
	printf("   -boot\t\tThe system boot index\n");
	printf("   -mac\t\tMAC address(required)\n");
	printf("   -ip\t\tIP address(required)\n");
	printf("   -t\t\tConfig repeat mode\n");

	printf("\n");
	return;
}





/*************************************************************************
 Function:      int AnalyseParameter(int argc, char *argv[], ST_IMAGE_INFO *pstImgInfo , 
 						struct  sys_cfg *pstSysConfig)

 Description:		analyse parameter,save result to pstImgInfo,pstSysConfig
 Calls:			无
 Data Accessed:
 Data Updated:
 Input:			argc: 输入参数个数
 				*argv[]:参数字符串指针
 Output:			
 Return:			1: 成功
				0: 失败
 Others:
 ************************************************************************/
int AnalyseParameter(int argc, char *argv[], ST_IMAGE_INFO *pstImgInfo , struct  sys_cfg *pstSysConfig)
{
	char			szCurrentDir[256];
	char			szTempPath[256];
	char			szSlash[2] = "/";
	unsigned int	uTempIP[4];
	unsigned int	uTempMAC[6];
	int			i;
	int			j;

	union 
	{  
		unsigned int uIpVal;  
		char   ch[4];  
	}unIpTemp = {0}; 


	// 设置默认输出的Image文件名称
    strcpy(pstImgInfo->szOutputBinName, DEFAULT_BINFILE);
	strcpy(pstImgInfo->szOutputImgName, DEFAULT_IMGFILE);
	// 默认从小Image启动
	pstImgInfo->iArgRequited = 0;
	pstImgInfo->iBootImgFlag = 0;	
	pstImgInfo->cEndian = 0xf;
	pstImgInfo->iBootImgFlag = 0;
	strcpy((char *)pstImgInfo->byTailVersion, IMAGE_TAIL_VERSION);		
	strcpy((char *)pstImgInfo->bySysVerion, SYSTEM_CONFIG_VERSION);
	pstImgInfo->iBlockSize = 65536;      /* default flash block size is 64k */

	memcpy(pstSysConfig->tag, "sysc", 4);  /* fill tag */
	pstSysConfig->endian = 0xf;          /* endian is not seted */	
	pstSysConfig->mac[0] = 0xff;         /* mac is not seted */
	pstSysConfig->ip = 0x0;              /* ip is not seted */	
	pstSysConfig->boot_img_flag = 0;	// set first image is default system	


    for(i=1;i<argc;i++)
    {
         if(strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"--help") == 0)
         {
              PrintHelp();
              return 2;
         }
         else if(strcmp(argv[i],"-b") == 0)            /* deal with bootload name parameter */
         {
              i++;
              if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
              {
                   printf("The -b parameter is wrong.\n");
                   return 0;
              }

              pstImgInfo->iArgRequited ++;              /* a requited parameter have got */
              strcpy(pstImgInfo->szBootloaderName, argv[i]);
         }
         else if(strcmp(argv[i],"-k1") == 0)            /* deal with kernel name parameter */
         {
              i++;
              if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
              {
                   printf("The -k parameter is wrong.\n");
                   return 0;
              }
              pstImgInfo->iArgRequited ++;              /* a requited parameter have got */
              strcpy(pstImgInfo->szFirstKernelName, argv[i]);
         }
		 else if(strcmp(argv[i],"-k2") == 0)            /* deal with kernel name parameter */
         {
              i++;
              if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
              {
                   printf("The -k parameter is wrong.\n");
                   return 0;
              }
              pstImgInfo->iArgRequited ++;              /* a requited parameter have got */

              strcpy(pstImgInfo->szSecondKernelName, argv[i]);
         }
         else if(strcmp(argv[i],"-r1") == 0)            /* deal with rootfs name parameter */
         {
              i++;
              if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
              {
                   printf("The -r parameter is wrong.\n");
                   return 0;
              }
              pstImgInfo->iArgRequited ++;              /* a requited parameter have got */
              strcpy(pstImgInfo->szFirstRootFileSystemName, argv[i]);
         }
		 else if(strcmp(argv[i],"-r2") == 0)            /* deal with rootfs name parameter */
         {
              i++;
              if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
              {
                   printf("The -r parameter is wrong.\n");
                   return 0;
              }
              pstImgInfo->iArgRequited ++;              /* a requited parameter have got */
              strcpy(pstImgInfo->szSecondRootFileSystemName, argv[i]);
         }
         else if(strcmp(argv[i],"-blk") == 0)            /* deal with block size parameter */
         {
              i++;                   
              if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
              {
                   printf("The -blk parameter is wrong.\n");
                   return 0;
              }         
              if(sscanf(argv[i],"%d",&(pstImgInfo->iBlockSize)) != 1)
              {
                   printf("The -blk parameter is wrong.example \"-blk 64\",it means 64k.\n");
                   return 0;
              }
              if(pstImgInfo->iBlockSize < 64)
              {
                   printf("The -blk parameter must >= 64.\n");
                   return 0;
              } 
              pstImgInfo->iBlockSize *= 1024;
     
         }  
         else if(strcmp(argv[i],"-img_type") == 0)            /* deal with image type parameter */
         {
           i++;
         
             if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
             {
                  printf("The -imgmark parameter is wrong.\n");
                  return 0;
             }
           
            if( strlen( argv[i] ) > ( IMAGE_TYPES_NAME_LEN - 1 ) )
            {
               printf("imgmark  is too long ,please use less than %d characters !\n" , ( IMAGE_TYPES_NAME_LEN - 1 ) );
               return 0;
            }
            if(strcmp(argv[i], "imgs") == 0)
            {
                pstImgInfo->uImageType = TYPE_SINGLE_BIN;
            }
            else if(strcmp(argv[i], "imgd") == 0)
            {
                pstImgInfo->uImageType = TYPE_DUAL_BIN;
            }
            else if(strcmp(argv[i], "imgb") == 0)
            {
                pstImgInfo->uImageType = TYPE_BACKUP_BIN;
            }
            else
            {
                printf("The -imgmark parameter is wrong.\n");
                return 0;
            }
         } 
		 else if(strcmp(argv[i], "-boot") == 0)
		 {
		      i++;
			  if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
              {
                   printf("The -boot parameter is wrong.\n");
                   return 0;
              }         
              if(sscanf(argv[i],"%d",&(pstImgInfo->iBootImgFlag)) != 1)
              {
                   printf("The -boot parameter is wrong.example \"-boot 0\",it means boot from first image.\n");
                   return 0;
              }
              if( (pstImgInfo->iBootImgFlag != 0) && (pstImgInfo->iBootImgFlag != 1) )
              {
                   printf("The -blk parameter must >= 64.\n");
                   return 0;
              } 
		 }
         else if(strcmp(argv[i],"-obin") == 0)            /* deal with output bin file name parameter */
         {
              i++;
              if(i >= argc || *argv[i] == '-')            /* if the next parameter is wrong */
              {
                   printf("The -obin parameter is wrong.\n");
                   return 0;
              }
              strcpy(pstImgInfo->szOutputBinName, argv[i]);
         }
         else if(strcmp(argv[i],"-oimg") == 0)            /* deal with output img file name parameter */
         {
              i++;
              if(i >= argc || *argv[i] == '-')            /* if the next parameter is wrong */
              {
                   printf("The -oimg parameter is wrong.\n");
                   return 0;
              }              
              strcpy(pstImgInfo->szOutputImgName, argv[i]);
         }
         else if(strcmp(argv[i],"-mac") == 0)            /* deal with MAC address parameter */
         {
              i++;    
              if(i >= argc || *argv[i] == '-')            /* if the next parameter is wrong */
              {
                   printf("The -mac parameter is wrong.\n");
                   return 0;
              }
              if(sscanf(argv[i],"%x:%x:%x:%x:%x:%x", &uTempMAC[0], &uTempMAC[1], &uTempMAC[2], &uTempMAC[3], &uTempMAC[4], &uTempMAC[5]) != 6)
              {
                   printf("The -mac parameter is wrong.\n");
                   return 0;
              }

              for(j=0;j<6;j++)
              {
                   if(uTempMAC[j] < 256)
                   {
                        pstSysConfig->mac[j] = uTempMAC[j];
                   }
                   else
                   {
                        printf("The -mac parameter is wrong.\n");
                        return 0;
                   }
              }

         }
         else if(strcmp(argv[i],"-ip") == 0)            /* deal with IP address parameter */
         {
              i++;
              if(i >= argc || *argv[i] == '-')            /* if the next parameter is wrong */
              {
                   printf("The -ip parameter is wrong.\n");
                   return 0;
              }
              if(sscanf(argv[i],"%d.%d.%d.%d", &uTempIP[0], &uTempIP[1], &uTempIP[2], &uTempIP[3]) != 4)   
              {
                   printf("The -ip parameter is wrong.\n");
                   return 0;
              }

              for(j=0;j<4;j++)
              {
                   if(uTempIP[j] < 255)
                   {
                        unIpTemp.ch[j] = uTempIP[j];
                   }
                   else
                   {
                        printf("The -ip parameter is wrong.\n");
                        return 0;
                   }
              }               
              pstSysConfig->ip = unIpTemp.uIpVal;
		}
	  	else if(strcmp(argv[i],"-p") == 0)            /* deal with product name parameter */
        {
			i++;
            if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
            {
            	printf("The -p parameter is wrong.\n");
                return 0;
            }
			if( strlen( argv[i] ) > ( PRODUCT_NAME_LEN - 1 ) )
			{
				printf("product name is too long ,please use less than %d characters !\n",( PRODUCT_NAME_LEN - 1 ));
				return 0;
			}
			strcpy(pstSysConfig->product , argv[i]  );
		}	  
	  	else if(strcmp(argv[i],"-v") == 0)            /* deal with versionparameter */
        {
			i++;
            if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
            {
            	printf("The -v parameter is wrong.\n");
                return 0;
            }		
			if( strlen( argv[i] ) > ( TBS_VERSION_NAME_LEN - 1 ) )
			{
				printf("version name is too long ,please use less than %d characters !\n", ( TBS_VERSION_NAME_LEN - 1 ) );
				return 0;
			}
			strcpy(pstSysConfig->version, argv[i]  );
	  	}		
	  	else if(strcmp(argv[i],"-id") == 0)            /* deal with system id name parameter */
        {
			i++;
            if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
            {
            	printf("The -id parameter is wrong.\n");
                return 0;
            }		
			if( strlen( argv[i] ) > ( BOARD_ID_NAME_LEN - 1 ) )
			{
				printf("board_id  is too long ,please use less than %d characters !\n" , ( BOARD_ID_NAME_LEN - 1 ) );
				return 0;
			}
			strcpy(pstSysConfig->board_id, argv[i]  );
	  	}         
		else if(strcmp(argv[i],"-t") == 0)            /* deal with cofig repeat mode parameter */
        {
			i++;
            if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
            {
            	printf("The -id parameter is wrong.\n");
                return 0;
            }		
			if( (strcmp(argv[i],"flash") == 0) )
			{
				pstImgInfo->cTransfer_mode = 0;
			}
			else if( (strcmp(argv[i],"sdram") == 0) )
			{
				pstImgInfo->cTransfer_mode = 1;
			}
	  	}         
		else if(strcmp(argv[i],"-be") == 0)            /* deal with big-endian parameter */
        {                         
        	pstSysConfig->endian = 1;
			pstImgInfo->cEndian =1;
        }
        else if(strcmp(argv[i],"-le") == 0)            /* deal with little-endian parameter */
        {                             
        	pstSysConfig->endian = 0;
			pstImgInfo->cEndian =0;
        }
	  	else if(strcmp(argv[i],"-app_cfg") == 0)            /* deal with app default config parameter */
        {
			i++;
            if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
            {
            	printf("The -app_cfg parameter is wrong.\n");
                return 0;
            }
			strcpy(pstImgInfo->szAppDefaultCfgItemName, argv[i]  );

			i++;		
			if(i >= argc || *argv[i] == '-')         /* if the next parameter is wrong */
            {
            	printf("The -app_cfg parameter is wrong.\n");
                return 0;
            }		
			strcpy( pstImgInfo->szAppDefaultCfgFileName , argv[i]  );
	  	}
	}  /* end of for(i=1;i<argc;i++) */

    

    /* Generate absolute path */
    getcwd(szCurrentDir,256);           /* Get current dir name */
 
    if(*(pstImgInfo->szBootloaderName) != '/')
    {
         strcpy(szTempPath,szCurrentDir);  /* Check bootloader file */
         strcat(szTempPath,szSlash);
    }
    else
    {
         szTempPath[0] = '\0';
    }
    strcat(szTempPath, pstImgInfo->szBootloaderName);
    strcpy(pstImgInfo->szBootloaderName, szTempPath);


    if(access(pstImgInfo->szBootloaderName,F_OK))
    {
         printf("The bootloader file is not exist.\n");
         return 0;
    }

    if(*(pstImgInfo->szFirstKernelName) != '/')
    {
         strcpy(szTempPath, szCurrentDir);  /* Check kernel file */
         strcat(szTempPath, szSlash);
    }
    else
    {
         szTempPath[0] = '\0';
    }

    strcat(szTempPath, pstImgInfo->szFirstKernelName);
    strcpy(pstImgInfo->szFirstKernelName, szTempPath);

    if(access(pstImgInfo->szFirstKernelName, F_OK))
    {
         printf("The First KernelName file is not exist.\n");
         return 0;
    }


	if(*(pstImgInfo->szSecondKernelName) != '/')
    {
         strcpy(szTempPath, szCurrentDir);  /* Check kernel file */
         strcat(szTempPath, szSlash);
    }
    else
    {
         szTempPath[0] = '\0';
    }

    strcat(szTempPath, pstImgInfo->szSecondKernelName);
    strcpy(pstImgInfo->szSecondKernelName, szTempPath);

    if(access(pstImgInfo->szSecondKernelName, F_OK))
    {
         printf("The Second KernelName file is not exist.\n");
         return 0;
    }

	

    if(*(pstImgInfo->szFirstRootFileSystemName) != '/')
    {
         strcpy(szTempPath,szCurrentDir);  /* Check rootfs file */
         strcat(szTempPath,szSlash);
    }
    else
    {
         szTempPath[0] = '\0';
    }

    strcat(szTempPath, pstImgInfo->szFirstRootFileSystemName);
    strcpy(pstImgInfo->szFirstRootFileSystemName, szTempPath);


    if(access(pstImgInfo->szFirstRootFileSystemName, F_OK))
    {
         printf("The first rootfs file is not exist.\n");
         return 0;
    }


	if(*(pstImgInfo->szSecondRootFileSystemName) != '/')
    {
         strcpy(szTempPath,szCurrentDir);  /* Check rootfs file */
         strcat(szTempPath,szSlash);
    }
    else
    {
         szTempPath[0] = '\0';
    }

    strcat(szTempPath, pstImgInfo->szSecondRootFileSystemName);
    strcpy(pstImgInfo->szSecondRootFileSystemName, szTempPath);


    if(access(pstImgInfo->szSecondRootFileSystemName, F_OK))
    {
         printf("The second rootfs file is not exist.\n");
         return 0;
    }	

    if(*(pstImgInfo->szAppDefaultCfgFileName) != '/')
    {
         strcpy(szTempPath,szCurrentDir);  /* Check app default config file */
         strcat(szTempPath,szSlash);
    }
    else
    {
         szTempPath[0] = '\0';
    }

    strcat(szTempPath, pstImgInfo->szAppDefaultCfgFileName);
    strcpy(pstImgInfo->szAppDefaultCfgFileName, szTempPath);

    if(access(pstImgInfo->szAppDefaultCfgFileName, F_OK))
    {
         printf("The app default config file is not exist.\n");
         return 0;
    }
    
    if(*(pstImgInfo->szOutputBinName) != '/')
    {
         strcpy(szTempPath, szCurrentDir);  /*  BIN file */
         strcat(szTempPath, szSlash);
    }
    else
    {
         szTempPath[0] = '\0';
    }

    strcat(szTempPath, pstImgInfo->szOutputBinName);
    strcpy(pstImgInfo->szOutputBinName, szTempPath);


    if(*(pstImgInfo->szOutputImgName) != '/')
    {              
         strcpy(szTempPath, szCurrentDir);  /*  IMG file */
         strcat(szTempPath, szSlash);
    }
    else
    {
         szTempPath[0] = '\0';
    }

    strcat(szTempPath, pstImgInfo->szOutputImgName);
    strcpy(pstImgInfo->szOutputImgName, szTempPath);


    /* Check MAC,IP and endian configuration */

    if(pstSysConfig->mac[0] == 0xff)
    {
         printf("Please input MAC address configuration.\n");
         return 0;
         
    }

    if(pstSysConfig->ip == 0)
    {
         printf("Please input IP address configuration.\n");
         return 0;
         
    }


    if(pstSysConfig->endian == 0xf)
    {
         printf("Please input endian configuration.\n");
         return 0;
         
    }

    if( pstSysConfig->product[0] == 0x0 )
    {
         printf("Please input product name.\n");
         return 0;
    }
	
    if(  pstSysConfig->version[0] == 0x0 )
    {
         printf("Please input version.\n");
         return 0;
    }  	

    if( pstImgInfo->szAppDefaultCfgItemName[0] == 0x0 )
    {
         printf("Please input app default config item name.\n");
         return 0;
    }
    return 1;     /* succeed */
}

int CreateTempPartImage(ST_IMAGE_INFO *pstImgInfo, int iImageIndex, ST_PART_IMAGE_INFO *pstPartImgInfo)
{
	char				buf[4096];
	FILE				*pfin;
	FILE				*pfout;
	int					ret;
	char				*buff;

	int				iFileLength = 0;      /* count how many bytes have been write to file */
	int				iReadCount;
	int				iWriteCount;
	int				tmp;
	int				i;
	
	unsigned long checksum_result = 0;
	char *pszKernelFileName = NULL;
	char *pszRootFSFileName = NULL;

	if(iImageIndex == 0)
	{
		sprintf(pstPartImgInfo->szFileName, "%s.part1", pstImgInfo->szOutputImgName);
		pszKernelFileName = pstImgInfo->szFirstKernelName;
		pszRootFSFileName = pstImgInfo->szFirstRootFileSystemName;
	}
	else
	{
		sprintf(pstPartImgInfo->szFileName, "%s.part2", pstImgInfo->szOutputImgName);
		pszKernelFileName = pstImgInfo->szSecondKernelName;
		pszRootFSFileName = pstImgInfo->szSecondRootFileSystemName;
	}

	debug("Open output file: %s\n", pstPartImgInfo->szFileName);
	pfout = fopen(pstPartImgInfo->szFileName, "wb+");
	if(pfout == NULL)
	{
		printf("Can't open output file: %s\n", pstPartImgInfo->szFileName);	
		return 0;
	}
	

/******************************************************************
				Copy kernel file              
******************************************************************/
	debug("Open kernel file: %s,start address=0x%x\n", pszKernelFileName, iFileLength);
	pstPartImgInfo->iKernelOffset = iFileLength;
 
	pfin = fopen(pszKernelFileName, "r");  
	if(pfin == NULL)
	{
		printf("Can't open first kernel file: %s\n", pszKernelFileName);
		fclose(pfout);
		remove(pstPartImgInfo->szFileName);
		return 0;
	}

	while(1)
	{
		iReadCount = fread(buf,1,4096,pfin);
		iWriteCount = fwrite(buf,1,iReadCount,pfout);
		if(iReadCount != iWriteCount && iReadCount > 0)  /* fail to write ? */
		{
			printf("Fail to write kernel.\n");
			fclose(pfin);
			fclose(pfout);
			remove(pstPartImgInfo->szFileName);
			return 0;
		}

		iFileLength += iWriteCount ;
		pstPartImgInfo->iKernelLength += iWriteCount;

		if(iReadCount < 4096)
		{
			if(!feof(pfin))        /* fail to read ? */
			{
				printf("Fail to read first kernel file.\n");
				fclose(pfin);
				fclose(pfout);
				remove(pstPartImgInfo->szFileName);
				return 0;
			}
			fclose(pfin);
			break;
		}
	}

    
/******************************************************************
				Align rootfs image start address to 0x100             
******************************************************************/

	tmp = iFileLength % 0x100;
	tmp = 0x100 - tmp;

	if(tmp)
	{
		for(i=0; i<tmp; i++)
		{
			fputc(0xffff,pfout);
			iFileLength++;
		}
	}

/******************************************************************
				Copy rootfs file              
******************************************************************/

	debug("Open first rootfs file: %s,start address=0x%x\n", pszRootFSFileName, iFileLength);
	pstPartImgInfo->iRootFileOffset = iFileLength;
	pfin = fopen(pszRootFSFileName, "r");
	if(pfin == NULL)
	{
		printf("Can't open rootfs file: %s\n", pszRootFSFileName);
		fclose(pfout);
		remove(pstPartImgInfo->szFileName);
		return 0;
	}

	while(1)
	{
		iReadCount = fread(buf,1,4096,pfin);
		iWriteCount = fwrite(buf,1,iReadCount,pfout);
		if(iReadCount != iWriteCount && iReadCount > 0)  /* fail to write ? */
		{
			printf("Fail to write rootfs.\n");
			fclose(pfin);
			fclose(pfout);
			remove(pstPartImgInfo->szFileName);
			return 0;
		}

		iFileLength += iWriteCount ;
		pstPartImgInfo->iRootFileLength += iWriteCount;
		if(iReadCount < 4096)
		{
			if(!feof(pfin))        /* fail to read ? */
			{
				printf("Fail to read rootfs file.\n");
				fclose(pfin);
				fclose(pfout);
				remove(pstPartImgInfo->szFileName);
				return 0;
			}
			fclose(pfin);
			break;
		}
	}

	if( tbs_crc_file( pfout , 0 , &checksum_result ) == 0 )
	{
		printf("Fail to calculate crc for file.\n");
		fclose(pfout);
		remove(pstPartImgInfo->szFileName);
		return 0;
	}
	debug("calc %d image crc value=%08x\n", iImageIndex, checksum_result);
	pstPartImgInfo->ulCRC = checksum_result;
	fclose(pfout);
	return 1;
}


/*************************************************************************
 Function:      int CreateBackupImage(ST_IMAGE_INFO *pstImgInfo,  struct  sys_cfg *pstSysConfig)

 Description:		build backup type bin/img file
 Calls:			无
 Data Accessed:
 Data Updated:
 Input:			pstImgInfo: local param
 				pstSysConfig: sysconfig param
 Output:			
 Return:			1: 成功
				0: 失败
 Others:
 ************************************************************************/

int CreateBackupImage(ST_IMAGE_INFO *pstImgInfo,  struct  sys_cfg *pstSysConfig)
{
	char				buf[4096];
	FILE				*pfin;
	FILE				*pfout;
	FILE				*pAppconfigfile;
	int					ret;
	char				*buff;

	int				iFileLength = 0;      /* count how many bytes have been write to file */
	int				iReadCount;
	int				iWriteCount;
	int				tmp;
	int				i;
	unsigned long		checksum_result;

	item_t			*item;
	int				item_len;
	int				item_len_sum = 0;

	char	*pszOutputFileName = NULL;
	bool	bBuildBin = false;


	ST_PART_IMAGE_INFO m_stPartImg1Info, m_stPartImg2Info;	
	struct  image_tail m_stImgTail;
	memset(&m_stImgTail, 0, sizeof(m_stImgTail));
	memset(&m_stPartImg1Info, 0, sizeof(m_stPartImg1Info));
	memset(&m_stPartImg2Info, 0, sizeof(m_stPartImg2Info));	

	if( (pstImgInfo->uImageType != TYPE_BACKUP_BIN) &&
		(pstImgInfo->uImageType != TYPE_BACKUP_IMG) )
	{
		printf("Param error, image type isn't backup type, type is %d.\n", pstImgInfo->uImageType);
		return 0;
	}

	if (pstImgInfo->uImageType == TYPE_BACKUP_BIN)
	{
		pszOutputFileName = pstImgInfo->szOutputBinName;
		bBuildBin = true;
	}
	else
		pszOutputFileName = pstImgInfo->szOutputImgName;


	if(CreateTempPartImage(pstImgInfo, 0, &m_stPartImg1Info) == 0)
	{
		printf("create temp part image 1 fail.\n");
		return 0;
	}
	
	if(CreateTempPartImage(pstImgInfo, 1, &m_stPartImg2Info) == 0)
	{
		printf("create temp part image 2 fail.\n");
		return 0;
	}
	
	debug("Open output file: %s\n", pszOutputFileName);
	pfout = fopen(pszOutputFileName, "wb+");
	if(pfout == NULL)
	{
		printf("Can't open output file: %s\n", pszOutputFileName);	
		return 0;
	}
	


/******************************************************************
				initialize  tail
******************************************************************/
	m_stImgTail.img_type = pstImgInfo->uImageType;	
	memcpy( m_stImgTail.tail_version, pstImgInfo->byTailVersion, sizeof(m_stImgTail.tail_version));
	m_stImgTail.boot_img_flag = pstImgInfo->iBootImgFlag;
	
	memcpy( m_stImgTail.board_id, pstSysConfig->board_id, sizeof(m_stImgTail.board_id));
	memcpy( m_stImgTail.version, pstSysConfig->version, sizeof(m_stImgTail.version));
	memcpy( m_stImgTail.product , pstSysConfig->product, sizeof(m_stImgTail.product));

/******************************************************************
				Copy bootloader file              
******************************************************************/

	if(bBuildBin)
	{
		debug("Open bootloader file: %s\n", pstImgInfo->szBootloaderName);	
		pfin = fopen(pstImgInfo->szBootloaderName, "r");
		if(pfin == NULL)
		{
			printf("Can't open bootloader file: %s\n", pstImgInfo->szBootloaderName);
			return 0;
		}		 
	    
		while(1)
		{
			iReadCount = fread(buf,1,4096,pfin); 
			iWriteCount = fwrite(buf,1,iReadCount,pfout);
			if(iReadCount != iWriteCount && iReadCount > 0)  /* fail to write ? */
			{
				printf("Fail to write code to BIN file.\n");
				fclose(pfin);
				fclose(pfout);
				remove(pszOutputFileName);
				return 0;
			}
			
			iFileLength += iWriteCount ;

			if(iReadCount < 4096)
			{
				if(!feof(pfin))        /* fail to read ? */
				{
					printf("Fail to read bootloader file.\n");
					fclose(pfin);
					fclose(pfout);
					remove(pszOutputFileName);
					return 0;
				}

				fclose(pfin);
				break;
			}
		}

		m_stImgTail.bootloader_len = iFileLength;
		
		if( iFileLength > BOOTCODE_LENGTH)     /* Fix bootloader length to 64K */
		{
			printf("The bootloader file is too big( bigger then 64K ).\n");
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		} 
		else
		{
			for(; iFileLength < pstImgInfo->iBlockSize; iFileLength++)
	            fputc(0xffff,pfout);
		}

/******************************************************************
				Fix config space length to flash sector size             
******************************************************************/

		for( i = 0; i < pstImgInfo->iBlockSize; i++ , iFileLength++ )
		{
			fputc(0xffff,pfout);
		}	

/******************************************************************
				Fix transfer space length to flash sector size              
******************************************************************/

		if( pstImgInfo->cTransfer_mode == 0 )
		{
			for( i = 0; i < pstImgInfo->iBlockSize; i++ , iFileLength++ )
			{
				fputc(0xffff,pfout);
			}
		}
	}

/******************************************************************
				Copy mini  image , part1 image file             
******************************************************************/
	

	debug("Open part2 image file: %s,start address=0x%x\n", m_stPartImg1Info.szFileName, iFileLength); 
	m_stImgTail.first_kernel_offset = iFileLength + m_stPartImg1Info.iKernelOffset;
	m_stImgTail.first_kernel_len = m_stPartImg1Info.iKernelLength;
	m_stImgTail.first_rootfs_offset = iFileLength + m_stPartImg1Info.iRootFileOffset;
	m_stImgTail.first_rootfs_len = m_stPartImg1Info.iRootFileLength;		
	m_stImgTail.first_image_checksum = m_stPartImg1Info.ulCRC;

	pfin = fopen(m_stPartImg1Info.szFileName, "r");  
	if(pfin == NULL)
	{
		printf("Can't open first part image file: %s\n", m_stPartImg1Info.szFileName);
		fclose(pfout);
		remove(pszOutputFileName);
		return 0;
	}

	while(1)
	{
		iReadCount = fread(buf,1,4096,pfin);
		iWriteCount = fwrite(buf,1,iReadCount,pfout);
		if(iReadCount != iWriteCount && iReadCount > 0)  /* fail to write ? */
		{
			printf("Fail to write part image1 file.\n");
			fclose(pfin);
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		}

		iFileLength += iWriteCount ;
		if(iReadCount < 4096)
		{
			if(!feof(pfin))        /* fail to read ? */
			{
				printf("Fail to read part image1 file.\n");
				fclose(pfin);
				fclose(pfout);
				remove(pszOutputFileName);
				return 0;
			}
			fclose(pfin);
			break;
		}
	}

	
	
/******************************************************************
				Align  image start address to blocksize             
******************************************************************/
	if(bBuildBin)
	{
		tmp = iFileLength % pstImgInfo->iBlockSize;
		tmp = pstImgInfo->iBlockSize - tmp;

		if(tmp)
		{
			for(i=0; i<tmp; i++)
			{
				fputc(0xffff,pfout);
				iFileLength++;
			}
		}
	}

/******************************************************************
				Copy Second temp part image file              
******************************************************************/
	debug("Open second part image file: %s,start address=0x%x\n", m_stPartImg2Info.szFileName, iFileLength); 
	m_stImgTail.second_kernel_offset = iFileLength + m_stPartImg2Info.iKernelOffset;
	m_stImgTail.second_kernel_len = m_stPartImg2Info.iKernelLength;
	m_stImgTail.second_rootfs_offset = iFileLength + m_stPartImg2Info.iRootFileOffset;
	m_stImgTail.second_rootfs_len = m_stPartImg2Info.iRootFileLength;		
	m_stImgTail.second_image_checksum = m_stPartImg2Info.ulCRC;

	pfin = fopen(m_stPartImg2Info.szFileName, "r");  
	if(pfin == NULL)
	{
		printf("Can't open second part image file: %s\n", m_stPartImg2Info.szFileName);
		fclose(pfout);
		remove(pszOutputFileName);
		return 0;
	}

	while(1)
	{
		iReadCount = fread(buf,1,4096,pfin);
		iWriteCount = fwrite(buf,1,iReadCount,pfout);
		if(iReadCount != iWriteCount && iReadCount > 0)  /* fail to write ? */
		{
			printf("Fail to write part image2 file.\n");
			fclose(pfin);
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		}

		iFileLength += iWriteCount ;
		if(iReadCount < 4096)
		{
			if(!feof(pfin))        /* fail to read ? */
			{
				printf("Fail to read part image2 file.\n");
				fclose(pfin);
				fclose(pfout);
				remove(pszOutputFileName);
				return 0;
			}
			fclose(pfin);
			break;
		}
	}	

/******************************************************************
				Deal with sys_cfg          
******************************************************************/
	if(bBuildBin)
	{
		pstSysConfig->endian = pstImgInfo->cEndian;
		pstSysConfig->boot_img_flag = pstImgInfo->iBootImgFlag;
		memcpy(pstSysConfig->cfg_version, pstImgInfo->bySysVerion, sizeof(pstSysConfig->cfg_version));
		pstSysConfig->first_kernel_offset = ConvertEndian(m_stImgTail.first_kernel_offset , pstSysConfig->endian);
		pstSysConfig->first_kernel_len = ConvertEndian(m_stImgTail.first_kernel_len , pstSysConfig->endian);
		pstSysConfig->first_rootfs_offset = ConvertEndian(m_stImgTail.first_rootfs_offset , pstSysConfig->endian);	
		pstSysConfig->first_rootfs_len = ConvertEndian(m_stImgTail.first_rootfs_len , pstSysConfig->endian);	
		pstSysConfig->first_image_checksum = ConvertEndian(m_stImgTail.first_image_checksum , pstSysConfig->endian);	
		pstSysConfig->second_kernel_offset = ConvertEndian(m_stImgTail.second_kernel_offset , pstSysConfig->endian);	
		pstSysConfig->second_kernel_len = ConvertEndian(m_stImgTail.second_kernel_len , pstSysConfig->endian);	
		pstSysConfig->second_rootfs_offset = ConvertEndian(m_stImgTail.second_rootfs_offset , pstSysConfig->endian);	
		pstSysConfig->second_rootfs_len = ConvertEndian(m_stImgTail.second_rootfs_len , pstSysConfig->endian);	
		pstSysConfig->second_image_checksum = ConvertEndian(m_stImgTail.second_image_checksum , pstSysConfig->endian);	
	
/******************************************************************
				Deal with system config          
******************************************************************/

		if( fseek( pfout , pstImgInfo->iBlockSize , SEEK_SET ) == -1 )
		{
			printf("Fail to point config addr.\n");
	      	fclose(pfout);
	      	remove(pszOutputFileName);
		    return 0;                         /* fail to lseek */
		}

		iWriteCount = fwrite( CONFIG_MARK , CONFIG_MARK_LEN , 1 , pfout );
		if(iWriteCount != 1)  /* fail to write ? */
		{
		     printf("Fail to write config mark to BIN file.\n");
	    	 fclose(pfout);
		     remove(pszOutputFileName);		 
		     return 0;
		}

		item_len_sum += CONFIG_MARK_LEN;
	
		item = malloc( pstImgInfo->iBlockSize );
		if( item == NULL )
		{
			printf("Can't malloc space for item.\n");
			fclose(pfout);
			remove(pszOutputFileName);		
			return 0;
		}
		memset(item, 0, pstImgInfo->iBlockSize);

		/* Deal with llconfig item */
	
		ret = item_generate( pstSysConfig , LLCONFIG_NAME , sizeof(struct  sys_cfg) , item , &item_len , pstImgInfo );
		if( ret == 0 )
		{
			printf("Generate %s item fail!\n",LLCONFIG_NAME);
			free( item );
			fclose(pfout);
			remove(pszOutputFileName);

			return 0;
		}

		item_len_sum += item_len;
		if( item_len_sum > pstImgInfo->iBlockSize )
		{
			printf("System config space is full!\n");
			free( item );
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		}

		iWriteCount = fwrite( item , item_len , 1 , pfout );
		if(iWriteCount != 1)  /* fail to write ? */
		{
		     printf("Fail to write %s item to BIN file.\n",LLCONFIG_NAME);
	    	 free( item );
		     fclose(pfout);
		     remove(pszOutputFileName);		 
		     return 0;
		}

		/* Deal with app default config item */

		pAppconfigfile= fopen(pstImgInfo->szAppDefaultCfgFileName, "r");                          
		if(pAppconfigfile == NULL)
		{
			printf("Can't open app default config file: %s\n",pstImgInfo->szAppDefaultCfgFileName);
			free( item );
			fclose(pfout);
	    	remove(pszOutputFileName);		 
			return 0;
		}

		fseek(pAppconfigfile, 0 , SEEK_SET);
		fseek(pAppconfigfile, 0 , SEEK_END);
		tmp = ftell(pAppconfigfile);

		if( tmp > ( pstImgInfo->iBlockSize - item_len_sum ) )
		{
		     printf("App default config is too long !\n");
	    	 free( item );
	         fclose(pAppconfigfile);
		     fclose(pfout);
	    	 remove(pszOutputFileName);		 
		     return 0;
		}

		buff = malloc( pstImgInfo->iBlockSize );
		if(buff == NULL)
		{
		     printf("buff malloc fail !\n");
	    	 free( item );
	         fclose(pAppconfigfile);
		     fclose(pfout);
	    	 remove(pszOutputFileName);		 
		     return 0;
		}

		fseek(pAppconfigfile, 0 , SEEK_SET);
		if(fread(buff,1,tmp,pAppconfigfile) != tmp)
		{
	     	printf("read default config file %s fail !\n", pstImgInfo->szAppDefaultCfgFileName);
	     	free( item );
         	fclose(pAppconfigfile);
	     	fclose(pfout);
	     	remove(pszOutputFileName);		 
	     	return 0;
		}
	
		memset(item, 0, pstImgInfo->iBlockSize);
		ret = item_generate( buff , TBS_APP_CFG ,tmp , item , &item_len , pstImgInfo );
		if( ret == 0 )
		{
			printf("Generate %s item fail!\n",TBS_APP_CFG);
			free( item );
			free(buff);
			fclose(pAppconfigfile);
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		}

		item_len_sum += item_len;
		if( item_len_sum > pstImgInfo->iBlockSize )
		{
			printf("System config space is full!\n");
			free( item );
			free( buff );
			fclose(pAppconfigfile);
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		}

		iWriteCount = fwrite( item , item_len , 1 , pfout );
		if(iWriteCount != 1)  /* fail to write ? */
		{
			printf("Fail to write %s item to BIN file.\n", TBS_APP_CFG);
			free( item );
			free( buff );
			fclose(pAppconfigfile);
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		}
	
		fclose(pAppconfigfile);
		free( buff );
		free( item );

		m_stImgTail.config_block_offset = pstImgInfo->iBlockSize;
		m_stImgTail.config_block_len = item_len_sum;
	}


/******************************************************************
				Add BIN file tail             
******************************************************************/
	if( fseek( pfout , 0 , SEEK_SET ) == -1 )
	{
	      printf("Fail point to BIN file start.\n");
	      fclose(pfout);
	      remove(pstImgInfo->szOutputBinName);
	      return 0;                        
	}
	
	if( fseek( pfout , 0 , SEEK_END ) == -1 )
	{
	      printf("Fail point to BIN file end.\n");
	      fclose(pfout);
	      remove(pstImgInfo->szOutputBinName);
	      return 0;                        
	}

	m_stImgTail.image_len = ftell(pfout);
	
	if(!bBuildBin)
	{
		// 由于该函数是反复调用的，入参存在上次的数据，下列数据在制作IMG时需要重置
		m_stImgTail.bootloader_len = 0;
		m_stImgTail.config_block_offset = 0;
		m_stImgTail.config_block_len = 0;
	}

	// Deal with  image_tail
	m_stImgTail.img_type = ConvertEndian(m_stImgTail.img_type, pstSysConfig->endian);
	m_stImgTail.boot_img_flag = ConvertEndian(m_stImgTail.boot_img_flag, pstSysConfig->endian);	
	m_stImgTail.bootloader_len = ConvertEndian(m_stImgTail.bootloader_len, pstSysConfig->endian);	
	m_stImgTail.first_kernel_offset = ConvertEndian(m_stImgTail.first_kernel_offset, pstSysConfig->endian);	
	m_stImgTail.first_kernel_len = ConvertEndian(m_stImgTail.first_kernel_len, pstSysConfig->endian);	
	m_stImgTail.first_rootfs_offset = ConvertEndian(m_stImgTail.first_rootfs_offset, pstSysConfig->endian);	
	m_stImgTail.first_rootfs_len = ConvertEndian(m_stImgTail.first_rootfs_len, pstSysConfig->endian);	
	m_stImgTail.first_image_checksum = ConvertEndian(m_stImgTail.first_image_checksum, pstSysConfig->endian);	
	m_stImgTail.second_kernel_offset = ConvertEndian(m_stImgTail.second_kernel_offset, pstSysConfig->endian);	
	m_stImgTail.second_kernel_len = ConvertEndian(m_stImgTail.second_kernel_len, pstSysConfig->endian);	
	m_stImgTail.second_rootfs_offset = ConvertEndian(m_stImgTail.second_rootfs_offset, pstSysConfig->endian);	
	m_stImgTail.second_rootfs_len = ConvertEndian(m_stImgTail.second_rootfs_len, pstSysConfig->endian);	
	m_stImgTail.second_image_checksum = ConvertEndian(m_stImgTail.second_image_checksum, pstSysConfig->endian);	
	m_stImgTail.config_block_offset = ConvertEndian(m_stImgTail.config_block_offset, pstSysConfig->endian);	
	m_stImgTail.config_block_len = ConvertEndian(m_stImgTail.config_block_len, pstSysConfig->endian);	
	m_stImgTail.image_len = ConvertEndian(m_stImgTail.image_len, pstSysConfig->endian);
	m_stImgTail.image_checksum = 0;	
	
	iWriteCount = fwrite( &m_stImgTail, sizeof(struct  image_tail) - sizeof(m_stImgTail.image_checksum) , 1 , pfout );
	if(iWriteCount != 1)  
	{
	     printf("Fail to write image_bin_tail to BIN file.\n");
	     fclose(pfout);
	     remove(pszOutputFileName);		 
	     return 0;
	}	

	if( tbs_crc_file( pfout , 0 , &checksum_result ) == 0 )
	{
		printf("Fail to calculate crc for file.\n");
		fclose(pfout);
		remove(pszOutputFileName);
		return 0;
	}

	if(bBuildBin)
		crc_checksum.bin_file_checksum = checksum_result;
	else
		crc_checksum.img_file_checksum = checksum_result;
	checksum_result = ConvertEndian( checksum_result , pstSysConfig->endian);
	if( tbs_set_sum( pfout , checksum_result ) == 0 )
	{
		printf("Fail write crc to BIN file.\n");
		fclose(pfout);
		remove(pszOutputFileName);
		return 0;                
	}

	iFileLength += sizeof(struct  image_tail);

	fclose(pfout);
	return 1;
}


/*************************************************************************
 Function:      int CreateImage(ST_IMAGE_INFO *pstImgInfo,  struct  sys_cfg *pstSysConfig)

 Description:		build backup type bin/img file
 Calls:			无
 Data Accessed:
 Data Updated:
 Input:			pstImgInfo: local param
 				pstSysConfig: sysconfig param
 Output:			
 Return:			1: 成功
				0: 失败
 Others:
 ************************************************************************/

int CreateImage(ST_IMAGE_INFO *pstImgInfo,  struct  sys_cfg *pstSysConfig)
{
	char				buf[4096];
	FILE				*pfin;
	FILE				*pfout;
	FILE				*pAppconfigfile;
	int					ret;
	char				*buff;

	int				iFileLength = 0;      /* count how many bytes have been write to file */
	int				iReadCount;
	int				iWriteCount;
	int				tmp;
	int				i;
	unsigned long		checksum_result;

	item_t			*item;
	int				item_len;
	int				item_len_sum = 0;

	char	*pszOutputFileName = NULL;
	bool	bBuildBin = false;


	ST_PART_IMAGE_INFO m_stPartImg1Info, m_stPartImg2Info;	
	struct  image_tail m_stImgTail;
	memset(&m_stImgTail, 0, sizeof(m_stImgTail));
	memset(&m_stPartImg1Info, 0, sizeof(m_stPartImg1Info));

	if ((pstImgInfo->uImageType == TYPE_SINGLE_BIN) || (pstImgInfo->uImageType == TYPE_DUAL_BIN))
	{
		pszOutputFileName = pstImgInfo->szOutputBinName;
		bBuildBin = true;
	}
	else
		pszOutputFileName = pstImgInfo->szOutputImgName;


	if(CreateTempPartImage(pstImgInfo, 0, &m_stPartImg1Info) == 0)
	{
		printf("create temp part image 1 fail.\n");
		return 0;
	}
	
	
	debug("Open output file: %s\n", pszOutputFileName);
	pfout = fopen(pszOutputFileName, "wb+");
	if(pfout == NULL)
	{
		printf("Can't open output file: %s\n", pszOutputFileName);	
		return 0;
	}
	


/******************************************************************
				initialize  tail
******************************************************************/
	m_stImgTail.img_type = pstImgInfo->uImageType;	
	memcpy( m_stImgTail.tail_version, pstImgInfo->byTailVersion, sizeof(m_stImgTail.tail_version));
	m_stImgTail.boot_img_flag = pstImgInfo->iBootImgFlag;
	
	memcpy( m_stImgTail.board_id, pstSysConfig->board_id, sizeof(m_stImgTail.board_id));
	memcpy( m_stImgTail.version, pstSysConfig->version, sizeof(m_stImgTail.version));
	memcpy( m_stImgTail.product , pstSysConfig->product, sizeof(m_stImgTail.product));

/******************************************************************
				Copy bootloader file              
******************************************************************/

	if(bBuildBin)
	{
		debug("Open bootloader file: %s\n", pstImgInfo->szBootloaderName);	
		pfin = fopen(pstImgInfo->szBootloaderName, "r");
		if(pfin == NULL)
		{
			printf("Can't open bootloader file: %s\n", pstImgInfo->szBootloaderName);
			return 0;
		}		 
	    
		while(1)
		{
			iReadCount = fread(buf,1,4096,pfin); 
			iWriteCount = fwrite(buf,1,iReadCount,pfout);
			if(iReadCount != iWriteCount && iReadCount > 0)  /* fail to write ? */
			{
				printf("Fail to write code to BIN file.\n");
				fclose(pfin);
				fclose(pfout);
				remove(pszOutputFileName);
				return 0;
			}
			
			iFileLength += iWriteCount ;

			if(iReadCount < 4096)
			{
				if(!feof(pfin))        /* fail to read ? */
				{
					printf("Fail to read bootloader file.\n");
					fclose(pfin);
					fclose(pfout);
					remove(pszOutputFileName);
					return 0;
				}

				fclose(pfin);
				break;
			}
		}

		m_stImgTail.bootloader_len = iFileLength;
		
		if( iFileLength > BOOTCODE_LENGTH)     /* Fix bootloader length to 64K */
		{
			printf("The bootloader file is too big( bigger then 64K ).\n");
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		} 
		else
		{
			for(; iFileLength < pstImgInfo->iBlockSize; iFileLength++)
	            fputc(0xffff,pfout);
		}

/******************************************************************
				Fix config space length to flash sector size             
******************************************************************/

		for( i = 0; i < pstImgInfo->iBlockSize; i++ , iFileLength++ )
		{
			fputc(0xffff,pfout);
		}	

/******************************************************************
				Fix transfer space length to flash sector size              
******************************************************************/

		if( pstImgInfo->cTransfer_mode == 0 )
		{
			for( i = 0; i < pstImgInfo->iBlockSize; i++ , iFileLength++ )
			{
				fputc(0xffff,pfout);
			}
		}
	}

/******************************************************************
				Copy mini  image , part1 image file             
******************************************************************/
	

	debug("Open part2 image file: %s,start address=0x%x\n", m_stPartImg1Info.szFileName, iFileLength); 
	m_stImgTail.first_kernel_offset = iFileLength + m_stPartImg1Info.iKernelOffset;
	m_stImgTail.first_kernel_len = m_stPartImg1Info.iKernelLength;
	m_stImgTail.first_rootfs_offset = iFileLength + m_stPartImg1Info.iRootFileOffset;
	m_stImgTail.first_rootfs_len = m_stPartImg1Info.iRootFileLength;		
	m_stImgTail.first_image_checksum = m_stPartImg1Info.ulCRC;

	pfin = fopen(m_stPartImg1Info.szFileName, "r");  
	if(pfin == NULL)
	{
		printf("Can't open first part image file: %s\n", m_stPartImg1Info.szFileName);
		fclose(pfout);
		remove(pszOutputFileName);
		return 0;
	}

	while(1)
	{
		iReadCount = fread(buf,1,4096,pfin);
		iWriteCount = fwrite(buf,1,iReadCount,pfout);
		if(iReadCount != iWriteCount && iReadCount > 0)  /* fail to write ? */
		{
			printf("Fail to write part image1 file.\n");
			fclose(pfin);
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		}

		iFileLength += iWriteCount ;
		if(iReadCount < 4096)
		{
			if(!feof(pfin))        /* fail to read ? */
			{
				printf("Fail to read part image1 file.\n");
				fclose(pfin);
				fclose(pfout);
				remove(pszOutputFileName);
				return 0;
			}
			fclose(pfin);
			break;
		}
	}

	
	
/******************************************************************
				Align  image start address to blocksize             
******************************************************************/
	if(bBuildBin)
	{
		tmp = iFileLength % pstImgInfo->iBlockSize;
		tmp = pstImgInfo->iBlockSize - tmp;

		if(tmp)
		{
			for(i=0; i<tmp; i++)
			{
				fputc(0xffff,pfout);
				iFileLength++;
			}
		}
	}


/******************************************************************
				Deal with sys_cfg          
******************************************************************/
	if(bBuildBin)
	{
		pstSysConfig->endian = pstImgInfo->cEndian;
		pstSysConfig->boot_img_flag = pstImgInfo->iBootImgFlag;
		memcpy(pstSysConfig->cfg_version, pstImgInfo->bySysVerion, sizeof(pstSysConfig->cfg_version));
		pstSysConfig->first_kernel_offset = ConvertEndian(m_stImgTail.first_kernel_offset , pstSysConfig->endian);
		pstSysConfig->first_kernel_len = ConvertEndian(m_stImgTail.first_kernel_len , pstSysConfig->endian);
		pstSysConfig->first_rootfs_offset = ConvertEndian(m_stImgTail.first_rootfs_offset , pstSysConfig->endian);	
		pstSysConfig->first_rootfs_len = ConvertEndian(m_stImgTail.first_rootfs_len , pstSysConfig->endian);	
		pstSysConfig->first_image_checksum = ConvertEndian(m_stImgTail.first_image_checksum , pstSysConfig->endian);	
		pstSysConfig->second_kernel_offset = ConvertEndian(m_stImgTail.second_kernel_offset , pstSysConfig->endian);	
		pstSysConfig->second_kernel_len = ConvertEndian(m_stImgTail.second_kernel_len , pstSysConfig->endian);	
		pstSysConfig->second_rootfs_offset = ConvertEndian(m_stImgTail.second_rootfs_offset , pstSysConfig->endian);	
		pstSysConfig->second_rootfs_len = ConvertEndian(m_stImgTail.second_rootfs_len , pstSysConfig->endian);	
		pstSysConfig->second_image_checksum = ConvertEndian(m_stImgTail.second_image_checksum , pstSysConfig->endian);	
	
/******************************************************************
				Deal with system config          
******************************************************************/

		if( fseek( pfout , pstImgInfo->iBlockSize , SEEK_SET ) == -1 )
		{
			printf("Fail to point config addr.\n");
	      	fclose(pfout);
	      	remove(pszOutputFileName);
		    return 0;                         /* fail to lseek */
		}

		iWriteCount = fwrite( CONFIG_MARK , CONFIG_MARK_LEN , 1 , pfout );
		if(iWriteCount != 1)  /* fail to write ? */
		{
		     printf("Fail to write config mark to BIN file.\n");
	    	 fclose(pfout);
		     remove(pszOutputFileName);		 
		     return 0;
		}

		item_len_sum += CONFIG_MARK_LEN;
	
		item = malloc( pstImgInfo->iBlockSize );
		if( item == NULL )
		{
			printf("Can't malloc space for item.\n");
			fclose(pfout);
			remove(pszOutputFileName);		
			return 0;
		}
		memset(item, 0, pstImgInfo->iBlockSize);

		/* Deal with llconfig item */
	
		ret = item_generate( pstSysConfig , LLCONFIG_NAME , sizeof(struct  sys_cfg) , item , &item_len , pstImgInfo );
		if( ret == 0 )
		{
			printf("Generate %s item fail!\n",LLCONFIG_NAME);
			free( item );
			fclose(pfout);
			remove(pszOutputFileName);

			return 0;
		}

		item_len_sum += item_len;
		if( item_len_sum > pstImgInfo->iBlockSize )
		{
			printf("System config space is full!\n");
			free( item );
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		}

		iWriteCount = fwrite( item , item_len , 1 , pfout );
		if(iWriteCount != 1)  /* fail to write ? */
		{
		     printf("Fail to write %s item to BIN file.\n",LLCONFIG_NAME);
	    	 free( item );
		     fclose(pfout);
		     remove(pszOutputFileName);		 
		     return 0;
		}

		/* Deal with app default config item */

		pAppconfigfile= fopen(pstImgInfo->szAppDefaultCfgFileName, "r");                          
		if(pAppconfigfile == NULL)
		{
			printf("Can't open app default config file: %s\n",pstImgInfo->szAppDefaultCfgFileName);
			free( item );
			fclose(pfout);
	    	remove(pszOutputFileName);		 
			return 0;
		}

		fseek(pAppconfigfile, 0 , SEEK_SET);
		fseek(pAppconfigfile, 0 , SEEK_END);
		tmp = ftell(pAppconfigfile);

		if( tmp > ( pstImgInfo->iBlockSize - item_len_sum ) )
		{
		     printf("App default config is too long !\n");
	    	 free( item );
	         fclose(pAppconfigfile);
		     fclose(pfout);
	    	 remove(pszOutputFileName);		 
		     return 0;
		}

		buff = malloc( pstImgInfo->iBlockSize );
		if(buff == NULL)
		{
		     printf("buff malloc fail !\n");
	    	 free( item );
	         fclose(pAppconfigfile);
		     fclose(pfout);
	    	 remove(pszOutputFileName);		 
		     return 0;
		}

		fseek(pAppconfigfile, 0 , SEEK_SET);
		if(fread(buff,1,tmp,pAppconfigfile) != tmp)
		{
	     	printf("read default config file %s fail !\n", pstImgInfo->szAppDefaultCfgFileName);
	     	free( item );
         	fclose(pAppconfigfile);
	     	fclose(pfout);
	     	remove(pszOutputFileName);		 
	     	return 0;
		}
	
		memset(item, 0, pstImgInfo->iBlockSize);
		ret = item_generate( buff , TBS_APP_CFG ,tmp , item , &item_len , pstImgInfo );
		if( ret == 0 )
		{
			printf("Generate %s item fail!\n",TBS_APP_CFG);
			free( item );
			free(buff);
			fclose(pAppconfigfile);
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		}

		item_len_sum += item_len;
		if( item_len_sum > pstImgInfo->iBlockSize )
		{
			printf("System config space is full!\n");
			free( item );
			free( buff );
			fclose(pAppconfigfile);
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		}

		iWriteCount = fwrite( item , item_len , 1 , pfout );
		if(iWriteCount != 1)  /* fail to write ? */
		{
			printf("Fail to write %s item to BIN file.\n", TBS_APP_CFG);
			free( item );
			free( buff );
			fclose(pAppconfigfile);
			fclose(pfout);
			remove(pszOutputFileName);
			return 0;
		}
	
		fclose(pAppconfigfile);
		free( buff );
		free( item );

		m_stImgTail.config_block_offset = pstImgInfo->iBlockSize;
		m_stImgTail.config_block_len = item_len_sum;
	}


/******************************************************************
				Add BIN file tail             
******************************************************************/
	if( fseek( pfout , 0 , SEEK_SET ) == -1 )
	{
	      printf("Fail point to BIN file start.\n");
	      fclose(pfout);
	      remove(pstImgInfo->szOutputBinName);
	      return 0;                        
	}
	
	if( fseek( pfout , 0 , SEEK_END ) == -1 )
	{
	      printf("Fail point to BIN file end.\n");
	      fclose(pfout);
	      remove(pstImgInfo->szOutputBinName);
	      return 0;                        
	}

	m_stImgTail.image_len = ftell(pfout);
	
	if(!bBuildBin)
	{
		// 由于该函数是反复调用的，入参存在上次的数据，下列数据在制作IMG时需要重置
		m_stImgTail.bootloader_len = 0;
		m_stImgTail.config_block_offset = 0;
		m_stImgTail.config_block_len = 0;
	}

	// Deal with  image_tail
	m_stImgTail.img_type = ConvertEndian(m_stImgTail.img_type, pstSysConfig->endian);
	m_stImgTail.boot_img_flag = ConvertEndian(m_stImgTail.boot_img_flag, pstSysConfig->endian);	
	m_stImgTail.bootloader_len = ConvertEndian(m_stImgTail.bootloader_len, pstSysConfig->endian);	
	m_stImgTail.first_kernel_offset = ConvertEndian(m_stImgTail.first_kernel_offset, pstSysConfig->endian);	
	m_stImgTail.first_kernel_len = ConvertEndian(m_stImgTail.first_kernel_len, pstSysConfig->endian);	
	m_stImgTail.first_rootfs_offset = ConvertEndian(m_stImgTail.first_rootfs_offset, pstSysConfig->endian);	
	m_stImgTail.first_rootfs_len = ConvertEndian(m_stImgTail.first_rootfs_len, pstSysConfig->endian);	
	m_stImgTail.first_image_checksum = ConvertEndian(m_stImgTail.first_image_checksum, pstSysConfig->endian);	
	m_stImgTail.second_kernel_offset = ConvertEndian(m_stImgTail.second_kernel_offset, pstSysConfig->endian);	
	m_stImgTail.second_kernel_len = ConvertEndian(m_stImgTail.second_kernel_len, pstSysConfig->endian);	
	m_stImgTail.second_rootfs_offset = ConvertEndian(m_stImgTail.second_rootfs_offset, pstSysConfig->endian);	
	m_stImgTail.second_rootfs_len = ConvertEndian(m_stImgTail.second_rootfs_len, pstSysConfig->endian);	
	m_stImgTail.second_image_checksum = ConvertEndian(m_stImgTail.second_image_checksum, pstSysConfig->endian);	
	m_stImgTail.config_block_offset = ConvertEndian(m_stImgTail.config_block_offset, pstSysConfig->endian);	
	m_stImgTail.config_block_len = ConvertEndian(m_stImgTail.config_block_len, pstSysConfig->endian);	
	m_stImgTail.image_len = ConvertEndian(m_stImgTail.image_len, pstSysConfig->endian);
	m_stImgTail.image_checksum = 0;	
	
	iWriteCount = fwrite( &m_stImgTail, sizeof(struct  image_tail) - sizeof(m_stImgTail.image_checksum) , 1 , pfout );
	if(iWriteCount != 1)  
	{
	     printf("Fail to write image_bin_tail to BIN file.\n");
	     fclose(pfout);
	     remove(pszOutputFileName);		 
	     return 0;
	}	

	if( tbs_crc_file( pfout , 0 , &checksum_result ) == 0 )
	{
		printf("Fail to calculate crc for file.\n");
		fclose(pfout);
		remove(pszOutputFileName);
		return 0;
	}

	if(bBuildBin)
		crc_checksum.bin_file_checksum = checksum_result;
	else
		crc_checksum.img_file_checksum = checksum_result;
	checksum_result = ConvertEndian( checksum_result , pstSysConfig->endian);
	if( tbs_set_sum( pfout , checksum_result ) == 0 )
	{
		printf("Fail write crc to BIN file.\n");
		fclose(pfout);
		remove(pszOutputFileName);
		return 0;                
	}

	iFileLength += sizeof(struct  image_tail);

	fclose(pfout);
	return 1;
}



