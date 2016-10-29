#ifndef _FLASH_LAYOUT_PRIVATE_H_
#define _FLASH_LAYOUT_PRIVATE_H_

void get_sys_ip(unsigned long *ip);
int sysdata_save(void *sys_data);
int item_map_and_check(void);
int item_recycle(void);
unsigned int prepare_kernel(void);
int do_bootm_linux(unsigned int src, char *arg);
int system_config_map(unsigned long addr);

#endif  /*_FLASH_LAYOUT_PRIVATE_H_*/


