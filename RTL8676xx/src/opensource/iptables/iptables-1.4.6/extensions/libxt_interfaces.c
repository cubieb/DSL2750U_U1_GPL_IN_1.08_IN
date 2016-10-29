/* Shared library add-on to iptables to add NFMARK matching support. */
#include <stdbool.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <xtables.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_interfaces.h>



static const struct option interfaces_opts[] = {
	{"in", 1, NULL, 'I'},
	{"out", 1, NULL, 'O'},
	{"both", 1, NULL, 'B'},
	{NULL},
};


static void interfaces_help(void)
{
	printf(
"interfaces match options:\n"
"in|out|both inteface_name\n");
}

static void interfaces_print(const void *ip,
                             const struct xt_entry_match *match, int numeric)
{
	const struct xt_interfaces_info *info = (const void *)match->data;

	printf("Interfaces match ");

	if (info->direction ==IN_DIR)
	    printf("in ");
  else if(info->direction ==OUT_DIR)
      printf("out ");
  else 
      printf("both ");

  if (info->invert)
	    printf("!");
	
	if(info->interfaces == LAN_INTERFACES)
      printf("LAN ");
  else if(info->interfaces == WAN_INTERFACES)
      printf("WAN ");
  else if(info->interfaces == WAN_LAN)
  	  printf("WAN/LAN ");
  else if(info->interfaces == SPECIFIC_LAN)
  	  printf("%s ", info->name);	            
}

static void interfaces_save(const void *ip, const struct xt_entry_match *match)
{
	const struct xt_interfaces_info *info = (const void *)match->data;

	printf("Interfaces match ");

	if (info->direction ==IN_DIR)
	    printf("in ");
  else if(info->direction ==OUT_DIR)
      printf("out ");
  else 
      printf("both ");

  if (info->invert)
	    printf("!");
	
	if(info->interfaces == LAN_INTERFACES)
      printf("LAN ");
  else if(info->interfaces == WAN_INTERFACES)
      printf("WAN ");
  else if(info->interfaces == WAN_LAN)
  	  printf("WAN/LAN ");
  else if(info->interfaces == SPECIFIC_LAN)
  	  printf("%s ", info->name);	              
}


static int
interfaces_parse(int c, char **argv, int invert, unsigned int *flags,
           const void *entry, struct xt_entry_match **match)
{
    char *err;
	  int i;
    struct xt_interfaces_info *info = (struct xt_interfaces_info *)(*match)->data;
    xtables_check_inverse(optarg, &invert, &optind, 0,argv);
    if(strcmp(optarg,"WAN")==0)
        info->interfaces = WAN_INTERFACES;
    else if(strcmp(optarg,"LAN")==0)
        info->interfaces = LAN_INTERFACES; 
    else if(strcmp(optarg,"WAN/LAN")==0)
        info->interfaces = WAN_LAN;
    else
    {
        info->interfaces = SPECIFIC_LAN;
        memcpy(info->name, optarg, strlen(optarg));
    }    	    
    
    info->invert = invert; 
    
	switch (c) 
	{
	case 'I':
		  if (*flags & 0x1)
			{
			    printf("--in may be given only once\n");
                          exit(PARAMETER_PROBLEM);
      }
		  *flags |= 0x1;
      info->direction = IN_DIR;
		  break;
	case 'O':
		  if (*flags & 0x2)
      {      
			    printf("--out may be given only once\n");
                        exit(PARAMETER_PROBLEM);
      }
		  *flags |= 0x2;
      info->direction = OUT_DIR;
		  break;
  case 'B':
      if (*flags & 0x4)
	    {
	        printf( "--both may be given only once\n");
                        exit(PARAMETER_PROBLEM);
      }
		  *flags |= 0x4;
      info->direction = BOTH_DIR;
      break;
	default:
		return 0;
  }
  //printf("%d***%d*** %d** line:%d*\n",info->interfaces,info->direction,info->invert,__LINE__);
    return 1;
}


static void interfaces_check(unsigned int flags)
{
	if (!flags)
		xtables_error(PARAMETER_PROBLEM,
			   "mark match: The --interfaces option is required");
}


static struct xtables_match interfaces_match = {
	.family		= AF_INET,
	.name 		= "interfaces",
	.version 	= XTABLES_VERSION,
	.size 		= XT_ALIGN(sizeof(struct xt_interfaces_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct xt_interfaces_info)),
	.help		= interfaces_help,
	.parse		= interfaces_parse,
	.print		= interfaces_print,
	.save     = interfaces_save,
	.final_check   = interfaces_check,	
	.extra_opts	= interfaces_opts,
};

static struct xtables_match interfaces_match6 = {
	.family		= AF_INET6,
	.name 		= "interfaces",
	.version 	= XTABLES_VERSION,
	.size 		= XT_ALIGN(sizeof(struct xt_interfaces_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct xt_interfaces_info)),
	.help		= interfaces_help,
	.parse		= interfaces_parse,
	.final_check	= interfaces_check,
	.print		= interfaces_print,
	.save     = interfaces_save,
	.extra_opts	= interfaces_opts,
};

void _init(void)
{
	xtables_register_match(&interfaces_match);
	xtables_register_match(&interfaces_match6);
}


