#ifndef __COMMON_H_
#define __COMMON_H_	1

typedef unsigned char		uchar;
typedef volatile unsigned long	vu_long;
typedef volatile unsigned short vu_short;
typedef volatile unsigned char	vu_char;

#ifndef __BOOT__
#define __BOOT__
#endif

#ifndef __TBS_AUTOCONF_H__
#define __TBS_AUTOCONF_H__
#include "autoconf.h"
#endif


#include <config.h>
#include <types.h>
#include <asm/ptrace.h>
#include <stdarg.h>

#ifdef CONFIG_ARM
#define asmlinkage	/* nothing */
#endif

#ifdef CONFIG_ARM
# include <asm/mach-types.h>
# include <asm/setup.h>
# include <asm/u-boot-arm.h>    /* ARM version to be fixed! */
#endif /* CONFIG_ARM */

#include <flash.h>

#include "flash_layout.h"          /*包含blp/common下的头文件*/
#include "sdram_layout.h"
#include "led.h"
#include "btn.h"

extern int serial_transfer ;	/*初始化为0,当需要用串口传送文件时设置为1*/
#define CMD_SAVEB
#define CMD_BOOTH
#define CMD_TCP
#define  sys_printf printf
#ifdef	CONFIG_BOOTLOADER_DEBUG //Global debug switch
#define debug(fmt,args...)	printf (fmt ,##args)
#define debugX(level,fmt,args...) if (DEBUG>=level) printf(fmt,##args);
#else
#define debug(fmt,args...)
#define debugX(level,fmt,args...)
#endif	/* DEBUG */

#ifdef CONFIG_BOOTLOADER_ETH_DEBUG  //For Ethernet debug
#define	etdebug(fmt, args...) printf(fmt, ##args)
#else
#define etdebug(fmt, args...)
#endif

#ifdef CONFIG_FLASH_DEBUG  //For Ethernet debug
#define	fldebug(fmt, args...) printf(fmt, ##args)
#else
#define fldebug(fmt, args...)
#endif
/* BOOT启动时，是否校验image完整性 */
//#define IMAGE_CHECKSUM
/* BOOT启动时，是否允许通过升级服务器进行自动升级 /// replaced by CONFIG_TBS_MASSIVE_UPGRADE at make menuconfig
#define IMAGE_AUTOUPGRADE            */

#define BUG() do { \
	printf("BUG: failure at %s:%d/%s()!\n", __FILE__, __LINE__, __FUNCTION__); \
	panic("BUG!"); \
} while (0)
#define BUG_ON(condition) do { if (unlikely((condition)!=0)) BUG(); } while(0)

#define XMK_STR(x)      #x
#define MK_STR(x)       XMK_STR(x)
#define ARRAY_SIZE(x)		(sizeof(x) / sizeof((x)[0]))

#include <asm/global_data.h>	/* global data used for startup functions */

/* for simulation codes */
#define REG8(reg) 	(*(volatile u8 *)(reg))
#define REG32(reg) (*((volatile u32 *)(reg)))
#define REGWRITE32(reg, value) *(u32 *)(reg) = (volatile u32)(value)

/*
 * General Purpose Utilities
 */
#define min(X, Y)				\
	({ typeof (X) __x = (X), __y = (Y);	\
		(__x < __y) ? __x : __y; })

#define max(X, Y)				\
	({ typeof (X) __x = (X), __y = (Y);	\
		(__x > __y) ? __x : __y; })

void	hang		(void) __attribute__ ((noreturn));

/* common/main.c */
void main_loop(void);
int	run_command	(const char *cmd, int flag);
int	readline	(const char *const prompt);
void	init_cmd_timeout(void);
void	reset_cmd_timeout(void);
void udelay(unsigned long usec);
void mdelay(unsigned long ms);
void delay_msec(unsigned long ms);
void flush_cache (ulong start_addr, ulong size);
char * strcpy(char * dest,const char *src);
char * strcat(char * dest, const char * src);
size_t strnlen(const char * s, size_t count);
size_t strlen(const char * s);
int strcmp(const char * cs,const char * ct);
char * strchr(const char * s, int c);
int strncmp(const char * cs,const char * ct,size_t count);
char * strchr(const char * s, int c);

/* lib_$(ARCH)/board.c */
void board_init_r(unsigned long);
int	checkboard    (void);
int	checkflash    (void);
int	checkdram     (void);
char *	strmhz(char *buf, long hz);
int	last_stage_init(void);
extern ulong monitor_flash_len;

extern ulong load_addr;		/* Default Load Address */

void prepare_kernel_commandline(char *);

/* $(BOARD)/$(BOARD).c */
int board_early_init_f (void);
int board_late_init (void);
int board_postclk_init (void); /* after clocks/timebase, before env/serial */
int board_early_init_r (void);
void board_poweroff (void);

/* $(CPU)/cpu.c */
int	checkcpu      (void);
int	checkicache   (void);
int	checkdcache   (void);
void	upmconfig     (unsigned int, unsigned int *, unsigned int);
ulong	get_tbclk     (void);
void	reset_cpu     (ulong addr);

/* $(CPU)/serial.c */
int	serial_init   (void);
void	serial_addr   (unsigned int);
void	serial_setbrg (void);
void	serial_putc   (const char);
void	serial_putc_raw(const char);
void	serial_puts   (const char *);
int	serial_getc   (void);
int	serial_tstc   (void);

void	_serial_setbrg (const int);
void	_serial_putc   (const char, const int);
void	_serial_putc_raw(const char, const int);
void	_serial_puts   (const char *, const int);
int	_serial_getc   (const int);
int	_serial_tstc   (const int);

/* lib_generic/vsprintf.c */
ulong	simple_strtoul(const char *cp,char **endp,unsigned int base);
#ifdef CFG_64BIT_VSPRINTF
unsigned long long	simple_strtoull(const char *cp,char **endp,unsigned int base);
#endif
long simple_strtol(const char *cp,char **endp,unsigned int base);
void panic(const char *fmt, ...);
int	sprintf(char * buf, const char *fmt, ...);
int	vsprintf(char *buf, const char *fmt, va_list args);

/* lib_generic/crc32.c */
ulong crc32 (ulong, const unsigned char *, uint);
ulong crc32_no_comp (ulong, const unsigned char *, uint);
/* common/console.c */
int	console_init_f(void);	/* Before relocation; uses the serial  stuff	*/
int	console_init_r(void);	/* After  relocation; uses the console stuff	*/
int	console_assign (int file, char *devname);	/* Assign the console	*/
int	ctrlc (void);
int pause(void);
int	had_ctrlc (void);	/* have we had a Control-C since last clear? */
void clear_ctrlc (void);	/* clear the Control-C condition */
int	disable_ctrlc (int);	/* 1 to disable, 0 to enable Control-C detect */

/* serial stuff */
void	serial_printf (const char *fmt, ...);

/* stdin */
int	getc(void);
int	tstc(void);

/* stdout */
void	putc(const char c);
void	puts(const char *s);
void	printf(const char *fmt, ...);
void	vprintf(const char *fmt, va_list args);

/* display_options.c */
void print_size (ulong size, const char *s);
int display_options (void);

/* stderr */
#define eputc(c)		fputc(stderr, c)
#define eputs(s)		fputs(stderr, s)
#define eprintf(fmt,args...)	fprintf(stderr,fmt ,##args)

/* FILE based functions (can only be used AFTER relocation!) */

#define stdin		0
#define stdout		1
#define stderr		2
#define MAX_FILES	3

int	pcmcia_init (void);

extern flash_info_t * info;

/* gpio */
void SetLed(led_name led , char state);
int	init_led(void);

#endif	/* __COMMON_H_ */

