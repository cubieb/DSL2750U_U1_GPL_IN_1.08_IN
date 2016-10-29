/*=========================================================================
 Copyright (c), 1991-2012, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : cmd_reg.c
 文件描述 : 寄存器相关操作

 修订记录 :
          1 修订: 夏超仁
            日期: 2012-02-15
            描述: 创建文件,实现寄存器读与写功能             

=========================================================================*/

#include <common.h>
#include <command.h>

#if (CONFIG_COMMANDS & CFG_CMD_REGINFO)
/*=========================================================================
 Function Description:	寄存器读写功能函数
 Data Accessed: register
 Data Updated:
 Input:			-r/w:读或者写
 				addr:寄存器操作地址
 				length:仅限读取模式
 Output:		寄存器内容
 Return:		0
 Others:
=========================================================================*/
int do_reg(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned long addr;
	unsigned long value;
	int i;
	int ret_val = -1;
	
	if(4 == argc)
		{
		addr = simple_strtoul(argv[2], NULL, 16);
		if(addr % sizeof(unsigned long))
			{
			goto error;
			}
		if(0 == strcmp(argv[1], "-r"))
			{
			value = simple_strtoul(argv[3], NULL, 0);
			for(i = 0; i < value; i += sizeof(unsigned long))
				{
				if(!(i % 16))
					{
					printf("\nAddr:%#x\n", addr);
					}					
				printf("  %08X", REG32(addr));
				addr += i;
				}
			printf("\n");
			ret_val = 0;
			}
		else if(0 == strcmp(argv[1], "-w"))
			{
			value = simple_strtoul(argv[3], NULL, 16);
			REGWRITE32(addr, value);
			printf("Addr:%#x  Value:%08X\n", addr, REG32(addr));
			ret_val = 0;
			}
		else
			{
			goto error;
			}
		}
	
error:
	if(ret_val < 0)
		{
		printf("Usage:\n%s\n", cmdtp->usage);
		}
	
	return ret_val;
}


/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	reg, CFG_MAXARGS, CMD_FLAG_REPEAT,	do_reg,
	"reg     - read[write] register at address 'addr'\n",
	" -r[-w] register_addr length[value]\n"
);
#endif

