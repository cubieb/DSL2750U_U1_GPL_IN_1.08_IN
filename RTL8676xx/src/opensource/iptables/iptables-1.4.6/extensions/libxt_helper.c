/* Shared library add-on to iptables to add related packet matching support. */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <xtables.h>
#include <linux/netfilter/xt_helper.h>

static void helper_help(void)
{
	printf(
"helper match options:\n"
"[!] --helper string        Match helper identified by string\n");
}

static const struct option helper_opts[] = {
	{ "helper", 1, NULL, '1' },
	{ .name = NULL }
};

static int
helper_parse(int c, char **argv, int invert, unsigned int *flags,
             const void *entry, struct xt_entry_match **match)
{
	struct xt_helper_info *info = (struct xt_helper_info *)(*match)->data;
	char *help_list = NULL;
	char *pNext = NULL;     
	char *pCur = NULL;   

	switch (c) {
	case '1':
		if (*flags)
			xtables_error(PARAMETER_PROBLEM,
					"helper match: Only use --helper ONCE!");
		xtables_check_inverse(optarg, &invert, &optind, 0, argv);

		/*
		 * 分配临时内在
		 */
		help_list = malloc(strlen (optarg) +1);  
		if(NULL == help_list)                      
			xtables_error(PARAMETER_PROBLEM,
					"helper match: memery too little!");

		/*
		 * 将helper list 列表中的helper逐个取出
		 * 然后放到name二维数组中
		 */
		strcpy(help_list, optarg);              
		pNext = pCur = help_list;            
		info->num = 0;
		while(pCur)                           
		{                                        
			pNext = strchr(pNext, ',');          
			if(pNext)                            
				*pNext = 0;                      
			/*                                   
			 * 检查单个helper的长度是否超过15字节
			 */                                  
			if(16 > strlen(pCur))
				strncpy(info->name[info->num], pCur, 15);
			else
			{
				free(help_list);
				xtables_error(PARAMETER_PROBLEM,
						"helper match: help too long, it should be little than 16");
			}
			info->name[info->num][15] = '\0';
			info->num += 1;
			/*
			 * 最多存放8个helper
			 */
			if((info->num == 8 )|| !pNext)
				break;
			pCur = ++pNext;                 
		}                                        
		/*
		 * 释放临时内在
		 */
		free(help_list);                        

		if (invert)
			info->invert = 1;
		*flags = 1;
		break;

	default:
		return 0;
	}
	return 1;
}

static void helper_check(unsigned int flags)
{
	if (!flags)
		xtables_error(PARAMETER_PROBLEM,
			   "helper match: You must specify `--helper'");
}

static void
helper_print(const void *ip, const struct xt_entry_match *match, int numeric)
{
	const struct xt_helper_info *info = (const void *)match->data;
	int n = 1;

	printf("helper match %s\"", info->invert ? "! " : "");
	if(info->num)
		printf("%s", info->name[0]);
	
	while(n < info->num)
	{
		printf(",%s", info->name[n]);
		++n;
	}
	printf("\" ");
}

static void helper_save(const void *ip, const struct xt_entry_match *match)
{
	const struct xt_helper_info *info = (const void *)match->data;
	int n = 0;

	printf("%s--helper ",info->invert ? "! " : "");
	while(n < info->num)
		xtables_save_string(info->name[n ++]);
}

static struct xtables_match helper_match = {
	.family		= NFPROTO_UNSPEC,
	.name		= "helper",
	.version	= XTABLES_VERSION,
	.size		= XT_ALIGN(sizeof(struct xt_helper_info)),
	.help		= helper_help,
	.parse		= helper_parse,
	.final_check	= helper_check,
	.print		= helper_print,
	.save		= helper_save,
	.extra_opts	= helper_opts,
};

void _init(void)
{
	xtables_register_match(&helper_match);
}
