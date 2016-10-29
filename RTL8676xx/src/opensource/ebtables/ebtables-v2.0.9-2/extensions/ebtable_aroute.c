#include <stdio.h>
#include "../include/ebtables_u.h"


static void print_help(char **hn)
{
	printf("Supported chain for the aroute table:\n");
	printf("%s\n",hn[NF_BR_AROUTING]);
}

static struct
ebt_u_table table =
{
	.name		= "aroute",
	.help		= print_help,
};

void _init(void)
{
	ebt_register_table(&table);
}
