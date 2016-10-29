/*==============================================================
 #Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 #Function:解压bootloader的第二阶段
 #Designed by xuanguanglei 
 #Date:2008-07-11
 ==============================================================*/

#include <common.h>
#include <mkimage.h>
#include <zlib.h>

void uncompress_stage2(unsigned int addr)
{
	int ret;
	volatile stage2_hdr_t *head ;
	volatile int (*stage2)(void);

	head = (volatile stage2_hdr_t *)addr;
		
	ret = uncompressLZMA((Bytef *)(head->image_load), (Bytef *)(addr + sizeof(stage2_hdr_t)));
	if(0 == ret)
		{
		stage2 = (void *)(head->image_load);
		(*stage2)();
		}
	else
		{
		//debug("uncompress failed! ret=%d\n", ret);
		while(1);
		}	
}


