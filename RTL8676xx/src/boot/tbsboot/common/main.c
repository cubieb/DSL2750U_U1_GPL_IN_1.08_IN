#include <common.h>
#include <command.h>
#include <net.h>
#include <crc.h>
#include <tbs_common.h>
#include <flash_layout_private.h>
#ifdef CMD_TCP
#include <tcp.h>
#endif
#include "led.h"

#define MAX_DELAY_STOP_STR 32

extern int button_pressed(void);
char console_buffer[CFG_CBSIZE];		/* console I/O buffer	*/
static char erase_seq[] = "\b \b";		/* erase sequence	*/
static char tab_seq[] = "        ";		/* used to expand TABs	*/
static unsigned int time_stamp = 0;
static int boot_failed = 0;

#define	endtick(seconds) (get_ticks() + (uint64_t)(seconds) * get_tbclk())

static inline int abortboot(void)
{
	int abort = 0;
	int	j = 0;
	int i;
	#ifdef CONFIG_BOOTDELAY
	int bootdelay = CONFIG_BOOTDELAY;
	#else
	int bootdelay = 30;
	#endif

	if(button_pressed())
		{
		printf("\nButton: pressed down!\n");
		set_all_led(LED_BOOT_ON);
		bootdelay = 3;
		}
	printf("Hit Space or Enter key to stop autoboot: %2d ", bootdelay);
	for(j = 0; (bootdelay > 0) && (!abort); bootdelay--)
		{/* delay 1s */
		for(i = 0; !abort && i < 100; ++i)
			{
			printf("\b\b\b%2d ", bootdelay);
			if((j >= 280) || pause())
				{/* we got Space or Enter key press	*/
				set_all_led(LED_BOOT_ON);
				abort = 1;	/* don't auto boot	*/
				bootdelay = 1;	/* no more delay	*/
				getc();  /* consume input	*/
				break;
				}
			if(button_pressed())
				{
				if((j % 30) == 1)
					{
					set_all_led(LED_BOOT_ON);
					}
				j++;
				}
			else
				{
				if(j)
					{
					//set_all_led(LED_BOOT_OFF);
					j = 1;
					}
				else
					{
					//set_all_led(-1);
					}
				}
			if((j % 30) == 15)
				{
				set_all_led(LED_BOOT_OFF);
				}
			mdelay(5);
			}
		}
	printf("\b\b\b%2d \n", bootdelay);

	return abort;
}

/****************************************************************************/
#ifndef CONFIG_CMDLINE_EDITING
static char *delete_char (char *buffer, char *p, int *colp, int *np, int plen)
{
	char *s;

	if(0 == *np)
		{
		return (p);
		}
	if(*(--p) == '\t')
		{/* will retype the whole line */
		while(*colp > plen)
			{
			puts(erase_seq);
			(*colp)--;
			}
		for(s = buffer; s < p; ++s)
			{
			if(*s == '\t')
				{
				puts (tab_seq + ((*colp) & 07));
				*colp += 8 - ((*colp) & 07);
				}
			else
				{
				++(*colp);
				putc (*s);
				}
			}
		}
	else
		{
		puts(erase_seq);
		(*colp)--;
		}
	(*np)--;

	return (p);
}
#endif /* CONFIG_CMDLINE_EDITING */

/****************************************************************************/
/* Prompt for input and read a line.
 * If  CONFIG_BOOT_RETRY_TIME is defined and retry_time >= 0,
 * time out when time goes past endtime (timebase time in ticks).
 * Return:	number of read characters
 *		-1 if break		-2 if timed out  */
/****************************************************************************/
int readline(const char *const prompt)
{
	char *p = console_buffer;
	int	n = 0;				/* buffer index		*/
	int	plen = 0;			/* prompt length	*/
	int	col;				/* output column cnt	*/
	char c;
	unsigned int temp;

	/* print prompt */
	if(0 != prompt)
		{
		plen = strlen(prompt);
		puts(prompt);
		}
	col = plen;
	for(;;)
		{
		c = getc();
		if(0 == c)
			{
			eth_rx();
			#ifdef CMD_TCP
			TcpTimeoutCheck();
			#endif
			if(boot_failed < 0)
				{
				SetLed(led_power_green, LED_BOOT_OFF);
				temp = get_timer(0);
				if((temp - time_stamp) > CFG_HZ / 4)
					{
					tbs_led_system_fault_blinking(led_power_red);
					time_stamp = temp;
					}
				}
			continue;
			}
		switch (c)
			{/* Special character handling  */
			case '\r': 				/* Enter		*/
			case '\n':
				*p = '\0';
				puts("\r\n");
				return (p - console_buffer);

			case '\0': 				/* nul			*/
				continue;

			case 0x03: 				/* ^C - break		*/
				console_buffer[0] = '\0';	/* discard input */
				return ( -1);

			case 0x15: 				/* ^U - erase line	*/
				while(col > plen)
					{
					puts (erase_seq);
					--col;
					}
				p = console_buffer;
				n = 0;
				continue;

			case 0x17: 				/* ^W - erase word 	*/
				p = delete_char(console_buffer, p, &col, &n, plen);
				while((n > 0) && (*p != ' '))
					{
					p = delete_char(console_buffer, p, &col, &n, plen);
					}
				continue;

			case 0x1B:
				if(0x5B == getc())
					{
					if(0x41 == getc())
						{
						puts("Got the one!");
//						printf("%s", console_buffer);
						}
					}
				continue;
				
			case 0x08: 				/* ^H  - backspace	*/
			case 0x7F: 				/* DEL - backspace	*/
				p = delete_char(console_buffer, p, &col, &n, plen);
				continue;

			default:
				/* Must be a normal character then */
            if(n < CFG_CBSIZE - 2)
				{
                if(c == '\t')
					{	/* expand TABs		*/
					puts (tab_seq + (col&07));
					col += 8 - (col & 07);
					}
				else
					{
					++col;		/* echo input		*/
					putc (c);
					}
				*p++ = c;
				++n;
				}
			else
				{			/* Buffer full		*/
				putc ('\a');
				}
			}
		}
}

/****************************************************************************/
int parse_line (char *line, char *argv[])
{
	int nargs = 0;

	#ifdef DEBUG_PARSER
	printf ("parse_line: \"%s\"\n", line);
	#endif
	while(nargs < CFG_MAXARGS)
		{
		while((*line == ' ') || (*line == '\t'))
			{/* skip any white space */
			++line;
			}
		if(*line == '\0')
			{/* end of line, no more args */
			argv[nargs] = NULL;
			#ifdef DEBUG_PARSER
			printf("parse_line: nargs=%d\n", nargs);
			#endif
			return (nargs);
			}
		argv[nargs++] = line;	/* begin of argument string	*/
		while(*line && (*line != ' ') && (*line != '\t'))
			{/* find end of string */
			++line;
			}
		if(*line == '\0')
			{/* end of line, no more args */
			argv[nargs] = NULL;
			#ifdef DEBUG_PARSER
			printf("parse_line: nargs=%d\n", nargs);
			#endif
			return (nargs);
			}
		*line++ = '\0';	/* terminate current arg */
		}
	printf("** Too many args (max. %d) **\n", CFG_MAXARGS);
	#ifdef DEBUG_PARSER
	printf("parse_line: nargs=%d\n", nargs);
	#endif


	return (nargs);
}

/****************************************************************************/
static void process_macros(const char *input, char *output)
{
	char c, prev;
	const char *varname_start = NULL;
	int inputcnt = strlen (input);
	int outputcnt = CFG_CBSIZE;
	int state = 0;		/* 0 = waiting for '$' */
	int i;
	char envname[CFG_CBSIZE];
	char *envval;
	int envcnt;
	
    /* 1 = waiting for '(' or '{' */ /* 2 = waiting for ')' or '}' */ /* 3 = waiting for '''  */
	prev = '\0';		/* previous character */
	while(inputcnt && outputcnt)
		{
		c = *input++;
		inputcnt--;
		if(state != 3)
			{/* remove one level of escape characters */
			if((c == '\\') && (prev != '\\'))
				{
				if (inputcnt-- == 0)
					{
					break;
					}
				prev = c;
				c = *input++;
				}
			}
		switch(state)
			{
			case 0: 	/* Waiting for (unescaped) $ */
				if((c == '\'') && (prev != '\\'))
					{
					state = 3;
					break;
					}
				if((c == '$') && (prev != '\\'))
					{
					state++;
					}
				else
					{
					*(output++) = c;
					outputcnt--;
					}
				break;

			case 1: 	/* Waiting for (        */
				if(c == '(' || c == '{')
					{
					state++;
					varname_start = input;
					}
				else
					{
					state = 0;
					*(output++) = '$';
					outputcnt--;
					if(outputcnt)
						{
						*(output++) = c;
						outputcnt--;
						}
					}
				break;

			case 2: 	/* Waiting for )        */
				if(c == ')' || c == '}')
					{
					envcnt = input - varname_start - 1;	/* Varname # of chars */
					/* Get the varname */
					for(i = 0; i < envcnt; i++)
						{
						envname[i] = varname_start[i];
						}
					envname[i] = 0;
					/* Get its value */
					envval = ""; // getenv (envname);
					/* Copy into the line if it exists */
					if(envval != NULL)
						{
						while ((*envval) && outputcnt)
							{
							*(output++) = *(envval++);
							outputcnt--;
							}
						}
					/* Look for another '$' */
					state = 0;
					}
				break;
				
            case 3: 	/* Waiting for '        */
				if((c == '\'') && (prev != '\\'))
					{
					state = 0;
					}
				else
					{
					*(output++) = c;
					outputcnt--;
					}
				break;
			}
		prev = c;
		}
	if(outputcnt)
		{
		*output = 0;
		}
}

/****************************************************************************
 * returns:
 *	1  - command executed, repeatable
 *	0  - command executed but not repeatable, interrupted commands are
 *	     always considered not repeatable
 *	-1 - not executed (unrecognized, bootd recursion or too many args)
 *           (If cmd is NULL or "" or longer than CFG_CBSIZE-1 it is
 *           considered unrecognized)
 *
 * WARNING:
 *
 * We must create a temporary copy of the command since the command we get
 * may be the result from getenv(), which returns a pointer directly to
 * the environment data, which may change magicly when the command we run
 * creates or modifies environment variables (like "bootp" does).
 */

int run_command(const char *cmd, int flag)
{
	cmd_tbl_t *cmdtp;
	char cmdbuf[CFG_CBSIZE];	/* working copy of cmd		*/
	char *token;			/* start of token in cmdbuf	*/
	char *sep;			/* end of token (separator) in cmdbuf */
	char finaltoken[CFG_CBSIZE];
	char *str = cmdbuf;
	char *argv[CFG_MAXARGS + 1];	/* NULL terminated	*/
	int argc, inquotes;
	int repeatable = 1;
	int rc = 0;

	clear_ctrlc();		/* forget any previous Control C */
	if(!cmd || !*cmd)
		{
		return -1;	/* empty command */
		}
	if(strlen(cmd) >= CFG_CBSIZE)
		{
		puts ("## Command too long!\n");
		return -1;
		}
	strcpy(cmdbuf, cmd);
	#if 0
	int i;
	for(i=0; i< strlen(cmdbuf); i++)
		{
		printf("%02x ", cmdbuf[i]);
		}
	printf("\n");
	#endif
	/* Process separators and check for invalid repeatable commands */
	while(*str)
		{
		/* Find separator, or string end Allow simple escape of ';' by writing "\;" */
		for(inquotes = 0, sep = str; *sep; sep++)
			{
			if((*sep == '\'') && (*(sep - 1) != '\\'))
				{
				inquotes = !inquotes;
				}
			if(!inquotes && (*sep == ';') && (sep != str) && (*(sep - 1) != '\\'))
				{/* separator *//* past string start	*//* and NOT escaped */
				break;
				}
			}
		/* Limit the token to data between separators */
		token = str;
		if(*sep)
			{
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
			}
		else
			{
			str = sep;	/* no more commands for next pass */
			}
		/* find macros in this token and replace them */
		process_macros (token, finaltoken);
		/* Extract arguments */
		if((argc = parse_line (finaltoken, argv)) == 0)
			{
			rc = -1;	/* no command at all */
			continue;
			}
		/* Look up command in command table */
		if((cmdtp = find_cmd(argv[0])) == NULL)
			{
			printf ("Unknown command '%s' - try 'help'\n", argv[0]);
			rc = -1;	/* give up after bad command */
			continue;
			}
		/* found - check max args */
		if(argc > cmdtp->maxargs)
			{
			printf ("Usage:\n%s\n", cmdtp->usage);
			rc = -1;
			continue;
			}
		/* OK - call function to do the command */
		if((cmdtp->cmd) (cmdtp, flag, argc, argv) != 0)
			{
			rc = -1;
			}
		repeatable &= cmdtp->repeatable;
		/* Did the user stop this? */
		if(had_ctrlc())
			{
			return 0;	/* if stopped then not repeatable */
			}
		}
	
    return rc ? rc : repeatable;
}


/****************************************************************************/

void main_loop(void)
{
    char lastcommand[CFG_CBSIZE] = {0, };
    unsigned int len;
	unsigned int src;
    int rc = -1;
    int flag = -1;
	char bootcmd[20] = {'\0'};
    char cmd[] = "bootm";
	int i;
	#if (CONFIG_COMMANDS & CFG_CMD_NET)
	struct eth_device *dev = NULL;
	#endif
	
	if((!gd->bi_flashsize) || (!gd->bi_memsize) || (!gd->have_console) || (gd->syscfg_ver < 0))
		{
		boot_failed = -1;
		}
	#if (CONFIG_COMMANDS & CFG_CMD_NET)
	boot_failed = network_init();
	dev = eth_get_dev();
	#endif

	debug("FUNC:%s,LINE:%d\n", __func__, __LINE__);
	/* Boot Command Processing */
    if(1 != abortboot())
    	{
		//set_all_led(-1);
		#if (CONFIG_COMMANDS & CFG_CMD_NET)
		printf("Checking ethernet link state... ");
		for(i = 0; i< 2000; i++)
			{/* Waiting for network to be ready in 2 seconds */
			if(eth_get_link_state())
				{
				break;
				}
			delay_msec(1);
			}
		printf("%s!\n", (i < 2000) ? "UP" : "DOWN");
		#ifdef CONFIG_TBS_MASSIVE_UPGRADE
		rc = check_udp_massive_upgrade();
		#endif
		eth_halt();
		#endif
		src = prepare_kernel();
		if(!src)
			{/* No bootable image availiable on flash */
			boot_failed = -1;
			}
		if(rc < 0)
			{/* No massive upgrade reply, Booting from flash */
			sprintf(bootcmd, "%s %#x", cmd, src);
			run_command(bootcmd, 1);
			}
		#if (CONFIG_COMMANDS & CFG_CMD_NET)
		eth_init();
		#endif
		}
    /*
     * Main Loop for Monitor Command Processing
     */
    if(boot_failed >= 0)
		{
		set_all_led(LED_BOOT_ON);
		}
#if (CONFIG_COMMANDS & CFG_CMD_NET)
#ifdef CMD_TCP
    tbs_tcp();
#endif
#endif
    for(;;)
		{
        len = readline(CFG_PROMPT);
        flag = 0;	/* assume no special flags for now */
        if (len > 0)
        	{
            strcpy (lastcommand, console_buffer);
        	}
        else if (len == 0)
        	{
            flag |= CMD_FLAG_REPEAT;
        	}
        if (len == -1)
            puts("<INTERRUPT>\n");
        else
            rc = run_command(lastcommand, flag);

        if(rc <= 0)
			{/* invalid command or not repeatable, forget it */
			lastcommand[0] = 0;
			}
		}
}


