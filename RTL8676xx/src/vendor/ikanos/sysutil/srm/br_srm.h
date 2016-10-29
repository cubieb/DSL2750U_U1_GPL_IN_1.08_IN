/* Copyright (C) 2006 Ikanos Communications
 * See IKANOS_PROP_LICENSE.txt for license information. */

# include "../../../../buildroot/build_mips_nofpu/bridge-utils-1.0.6/libbridge/libbridge.h"
# include "../../../../buildroot/build_mips_nofpu/bridge-utils-1.0.6/libbridge/libbridge_private.h"
# include "../../acc_utillib/interface.h"

struct port
{
	char pname[25];
	struct port_info info;
};

struct bridge
{
	char ifname[25];	  // bridge name
	int status;		  // up/down status
	struct bridge_info info;
	struct interface iface;   // interface information
	int portCnt;		  // number of ports
	struct port pinfo[5];
};

