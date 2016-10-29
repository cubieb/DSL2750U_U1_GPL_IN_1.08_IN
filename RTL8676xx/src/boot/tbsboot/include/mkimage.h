#ifndef __MKIMAGE_H__
#define __MKIMAGE_H__

typedef struct 
{	
	volatile unsigned int image_load;				/*stage2入口地址*/
	volatile unsigned int image_len;				/*stage2压缩后的长度*/
	unsigned char tag[3];					        /*  'two'  */
}stage2_hdr_t;

#endif  /*endif __MKIMAGE_H__*/

