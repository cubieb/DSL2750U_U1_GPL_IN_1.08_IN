#include <stdio.h>
#include "../include/ebtables_u.h"


static void print_help(const char **hn)
{
	printf("Supported chain for the proute table:\n");
	printf("%s\n",hn[NF_BR_PROUTING]);
}

static struct
ebt_u_table table =
{
	.name		= "proute",
	.help		= print_help,
};

void _init(void)
{
	ebt_register_table(&table);	
}

