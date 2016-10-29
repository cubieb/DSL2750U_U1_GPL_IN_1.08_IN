#include "usbctl.h"

//#include "../boa/src/LINUX/options.h"
#include <linux/usbdevice_fs.h>
#include <dirent.h>

#define USBCTL "usbctl: "
#define USBPATH "/proc/bus/usb"
#define USBHUBPATH1 "/proc/bus/usb/001/001"
#define USBHUBPATH2 "/proc/bus/usb/002/001"


struct usbdevfs_usbphy {
	unsigned char is_read;	//read or write register
	unsigned char port;	//usb phy port 0 or port 1
	unsigned char reg;
	unsigned char value;
};

struct usbdevfs_mem {
	unsigned char is_read;	//read or write register
	unsigned int addr;	//usb phy port 0 or port 1
	int len;
	unsigned int value;
};

#define USBDEVFS_USBPHY				_IOR('U', 124, struct usbdevfs_usbphy)	//100421 cathy, support phy register access
#define USBDEVFS_DBG				_IOR('U', 125, unsigned int)	//100421 cathy, print message if error
#define USBDEVFS_HCTESTPKT			_IOR('U', 126, unsigned char)		//100518 cathy, host sends test packet
#define USBDEVFS_MEM				_IOR('U', 127, struct usbdevfs_mem)


unsigned int s2i(char  *str_P)
{
    unsigned int  val;
    
    if ( (str_P[0] == '0') && (str_P[1] == 'x') )
    {
        str_P += 2;
        for (val = 0; *str_P; str_P++)
        {
            val *= 16;
            if ( '0' <= *str_P && *str_P <= '9' )
                val += *str_P - '0';
            else if ( 'a' <= *str_P && *str_P <= 'f' )
                val += *str_P - 'a' + 10;
            else if ( 'A' <= *str_P && *str_P <= 'F' )
                val += *str_P - 'A' + 10;
            else
                break;
        }
    }
    else
    {
        for (val = 0; *str_P; str_P++)
        {
            val *= 10;
            if ( '0' <= *str_P && *str_P <= '9' )
                val += *str_P - '0';
            else
                break;
        }
    }
    
    return val;
}



static void usage(void)
{
	char usage_desc_now[] = 
"Usage:\n\
  get_phy <port> <hex-reg>\n\
  set_phy <port> <hex-reg> <hex-value>\n\
  dbg <on/off>\n\
  test_pkt <port>\n\
  r <hex-addr> <len>\n\
  w <hex-addr> <hex-value>\n\
";

	printf(usage_desc_now);

	return;
}

static int check_usb_vfs(const char *dirname)
{
  DIR *dir;
  struct dirent *entry;
  int found = 0;

  dir = opendir(dirname);
  if (!dir)
    return 0;

  while ((entry = readdir(dir)) != NULL) {
    /* Skip anything starting with a . */
    if (entry->d_name[0] == '.')
      continue;

    /* We assume if we find any files that it must be the right place */
    found = 1;
    break;
  }

  closedir(dir);

  return found;
}

static int usb_phy_access(char **spp, unsigned char is_read)
{
	int skfd;
	struct usbdevfs_usbphy phy;

	phy.port = s2i(spp[1]);
	if((phy.port!=0) && (phy.port!=1)) {
		printf(USBCTL "usb phy port number should be 0 or 1!\n");
		return -1;
	}
	
	phy.reg = s2i(spp[2]);
	if((phy.reg < 0xE0) || (phy.reg > 0xF6) || ((phy.reg>0xE7)&&(phy.reg<0xF0))) {
		printf(USBCTL "bad register address: 0x%02x\n", phy.reg);
		return -1;
	}

	phy.is_read = is_read;
	
	if(!is_read) {
		phy.value = s2i(spp[3]);
	}

	skfd = open(USBHUBPATH1, O_WRONLY);

	if(skfd < 0) {
		printf("Error! USB HUB cannot be opened!\n");
		return -1;
    }

	if(ioctl(skfd, USBDEVFS_USBPHY, &phy)< 0) {
		printf("Error usb ioctl\n");
		return -1;
    }

	if(skfd!=(int)NULL)
		close(skfd);

	return 0;
}

static int usb_dbg(char **spp)
{
	int skfd;
	unsigned int dbg; 
	
	if(!strcmp(*(spp+1), "on")) {
		dbg = 1;
	}
	else if(!strcmp(*(spp+1), "off")) {
		dbg = 0;
	}
	else {
		printf(USBCTL "usb debug error on or off?\n");
		return -1;
	}

	skfd = open(USBHUBPATH1, O_WRONLY);

	if(skfd < 0) {
		printf("Error! USB HUB cannot be opened!\n");
		return -1;
    }

	if(ioctl(skfd, USBDEVFS_DBG, &dbg)< 0) {
		printf("Error usb ioctl\n");
		return -1;
    }
	
	return 0;
}


static int usb_test_packet(char **spp)
{
	int skfd;
	unsigned char port;

	port = s2i(spp[1]);
	if((port!=0) && (port!=1)) {
		printf(USBCTL "usb phy port number should be 0 or 1!\n");
		return -1;
	}

	skfd = open(USBHUBPATH1, O_WRONLY);

	if(skfd < 0) {
		printf("Error! USB HUB cannot be opened!\n");
		return -1;
    }

	if(ioctl(skfd, USBDEVFS_HCTESTPKT, &port)< 0) {
		printf("Error usb ioctl\n");
		return -1;
    }
}

static int usb_mem_access(char **spp, unsigned char is_read)
{
	int skfd;
	struct usbdevfs_mem mem;
	
	mem.addr = s2i(spp[1]);
	if ((((mem.addr & 0xF0000000) != 0xA0000000)
		&& ((mem.addr & 0xF0000000) != 0x80000000)
		&& ((mem.addr & 0xF0000000) != 0xB0000000)
	)) {
		printf("USBCTL Wrong address: 0x%08x\n", mem.addr);
		return -1;
	}

	mem.is_read = is_read;
	
	if(!is_read) {
		mem.value = s2i(spp[2]);
	}
	else {
		mem.len = s2i(spp[2]);
	}

	skfd = open(USBHUBPATH1, O_WRONLY);

	if(skfd < 0) {
		printf("Error! USB HUB cannot be opened!\n");
		return -1;
    }

	if(ioctl(skfd, USBDEVFS_MEM, &mem)< 0) {
		printf("Error usb ioctl\n");
		return -1;
    }

	if(skfd!=(int)NULL)
		close(skfd);

	return 0;

}

int main(int argc, char *argv[])
{
	
	int skfd,eocsd,i,j;
	struct ifreq				ifr;
	char	**spp;
	unsigned char mount_retry = 5;
	unsigned char mountOK = 0;

	argc--;
	argv++;

	if (argc == 0) goto USAGE_END;

	while(mount_retry) {
		if(!check_usb_vfs(USBPATH)) {
			system("mount -t usbfs none /proc/bus/usb");
			usleep(10000);
		}
		else {
			mountOK = 1;
			break;
		}
		mount_retry--;
	}

	if(!mountOK) {
		printf("mount usbfs fail!\n");
		goto ERROR_END;
	}
//	strncpy(ifr.ifr_name, dev_name, IFNAMSIZ);

//	ifr.ifr_data = (void *)&cfg;
	spp = argv;
//	mysio.number = 0;

	if(!strcmp(*spp, "get_phy")) {
		if(argc != 3) {
			printf(USBCTL "wrong input!\n");
			goto ERROR_END;
		}
		if(usb_phy_access(spp, 1) < 0)
			goto ERROR_END;
			
	}
	else if(!strcmp(*spp, "set_phy")) {
		if(argc != 4) {
			printf(USBCTL "wrong input!\n");
			goto ERROR_END;
		}
		if(usb_phy_access(spp, 0) < 0)
			goto ERROR_END;
	}
	else if(!strcmp(*spp, "dbg")) {
		if(argc != 2) {
			printf(USBCTL "wrong input!\n");
			goto ERROR_END;
		}
		if(usb_dbg(spp) < 0)
			goto ERROR_END;
	}
	else if(!strcmp(*spp, "test_pkt")) {
		if(argc != 2) {
			printf(USBCTL "wrong input!\n");
			goto ERROR_END;
		}
		if(usb_test_packet(spp) < 0)
			goto ERROR_END;
	}
	else if(!strcmp(*spp, "r")) {
		if(argc != 3) {
			printf(USBCTL "wrong input!\n");
			goto ERROR_END;
		}
		if(usb_mem_access(spp, 1) < 0)
			goto ERROR_END;
	}
	else if(!strcmp(*spp, "w")) {
		if(argc != 3) {
			printf(USBCTL "wrong input!\n");
			goto ERROR_END;
		}
		if(usb_mem_access(spp, 0) < 0)
			goto ERROR_END;
	}
	else
		goto USAGE_END;

	goto END;	
	

USAGE_END:
	usage();

END:
	if(skfd!=(int)NULL)
		close(skfd);	
	exit(0);

ERROR_END:
	//printf("ioctl error\n"); 
	exit(1);


}



