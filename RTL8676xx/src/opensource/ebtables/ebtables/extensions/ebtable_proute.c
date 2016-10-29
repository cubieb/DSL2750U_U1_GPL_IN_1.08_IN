#include <stdio.h>
#include "../include/ebtables_u.h"


static void print_help(char **hn)
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

static void _init(void) __attribute__ ((constructor));
static void _init(void)
{
	register_table(&table);
}

