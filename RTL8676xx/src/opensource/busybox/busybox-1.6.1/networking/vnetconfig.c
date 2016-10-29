/* vi: set sw=4 ts=4: */
/*
 * vnetconfig implementation for busybox
 *
 * Copyright (C) 2001  
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

/* BB_AUDIT SUSv3 N/A */

#include "libbb.h"
#include <net/if.h>

enum vnet_ioctl_cmds
{
    ADD_VNET_CMD,
    DEL_VNET_CMD,

};

#define VNET_MODE_ROUTER   1
#define VNET_MODE_BRIDGE   2


struct vnet_ioctl_args {
	int cmd; /* Should be one of the vlan_ioctl_cmds enum above. */
	char device1[24];
	char device2[24];
      int mode;
};


#define SIOCSIFVNET	0x89c0		/* Set 802.1Q VNET options */


int vnetconfig_main(int argc, char **argv);
int vnetconfig_main(int argc, char **argv)
{
	struct vnet_ioctl_args ifr;
	int fd;


	if (argc < 3) {
		bb_show_usage();
	}


	memset(&ifr, 0, sizeof(struct vnet_ioctl_args));

	++argv;

	if(strcmp("add",argv[0])==0)
	{
	    ifr.cmd = ADD_VNET_CMD;
	    if(argc != 4)
	    {
               bb_show_usage();
	    }
      }
      else if(strcmp("del",argv[0])==0)
      {
          ifr.cmd = DEL_VNET_CMD;
      }
      else
      {
           bb_show_usage();
      }


      if (strlen(argv[1]) >= IF_NAMESIZE) {
			bb_error_msg_and_die("if_name >= %d chars", IF_NAMESIZE);
	}

	strcpy(ifr.device1, argv[1]);

	
      

	if (ifr.cmd == ADD_VNET_CMD) { 
            if (strlen(argv[2]) >= IF_NAMESIZE) {
			bb_error_msg_and_die("if_name >= %d chars", IF_NAMESIZE);
	      }

	      strcpy(ifr.device2, argv[2]);

	}
		
	

	fd = xsocket(AF_INET, SOCK_STREAM, 0);
	if (ioctl(fd, SIOCSIFVNET, &ifr) < 0) {
		bb_perror_msg_and_die("ioctl error for %s", *argv);
	}

	return 0;
}
