/* Copyright (C) 2006 Ikanos Communications
 * See IKANOS_PROP_LICENSE.txt for license information. */

# include <linux/if.h>
# include "../../acc_utillib/interface.h"

# define MAX_IPOA_SRM 10

struct atmarp_srm
{
	char ifname[5];		// name of interface
	char serverip[25];	// this is in char format
	char qos[100];		// -q parameters
	char vpivalues[20];	// stores vci,vpi values
        struct interface iface; // interface information
};

struct ipoa_srm
{
	char atmarpd[50];	// stores atmaprd& command
	int ifaceno;		// number of interfaces with name atm[N]	
	struct atmarp_srm arpinfo[MAX_IPOA_SRM];	
};

