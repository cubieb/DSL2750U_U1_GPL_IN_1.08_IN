/*
 * Copyright (C) 2000 Lennert Buytenhek
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <asm/param.h>
#include "libbridge.h"
#include "brctl.h"

#ifdef CONFIG_VLAN_MODE
static int vlan_mode(int argc,char*const* argv,VLAN_MODE_T *br_vlan_t)
{
	int i,j,k;

	/* 参数解析，赋给结构体 */
	if(argv[2]==NULL)
		goto ERROR;

	/* transparent mode */
	if(!strcmp(argv[2],"transparent"))
	{	
		br_vlan_t->mode=VLAN_TRANSPARENT;
		if(argc>3)
			goto ERROR;
		return 0;
	}
	/* tag mode */
	if(argv[3]==NULL)
		goto ERROR;
	if(!strcmp(argv[2],"tag"))
	{
		br_vlan_t->mode=VLAN_TAG;
		br_vlan_t->pvlan = atoi(argv[4]);
		if(argc>5)
			goto ERROR;
		return 0;
	}
	/* trunk mode */
	if(!strcmp(argv[2],"trunk"))
	{

		br_vlan_t->mode=VLAN_TRUNK;
		br_vlan_t->pvlan = atoi(argv[4]);
		if(argc>(VLAN_MAX_TRUNK+5))
			goto ERROR;
		for(i=5;argv[i]!=NULL;i++)
			br_vlan_t->u.trunk[i-5]=atoi(argv[i]);
		br_vlan_t->num = i-5;
        printf("vlan set trunk: pvid = %d\n",br_vlan_t->pvlan);
		for(i=5;argv[i]!=NULL;i++)
            printf("vlan set trunk: vid%d = %d\n",i-5,br_vlan_t->u.trunk[i-5]);
		return 0;
	}
    /*hybird mode*/
    if(!strcmp(argv[2], "hybird"))
    {
		br_vlan_t->mode=VLAN_HYBIRD;
		br_vlan_t->pvlan = atoi(argv[4]);
		if(argc>(VLAN_MAX_TRUNK+VLAN_MAX_UNTAG+7))
			goto ERROR;
		for(i=5;argv[i]!=NULL;i++)
        {
            if(!strcmp(argv[i], "trans"))
            {
                j = 1;//1-trans,2-untag
                k = 0;
                continue;
            }
            if(!strcmp(argv[i], "untag"))
            {
                j = 2;//1-trans,2-untag
                k = 0;
                continue;
            }
            if(j == 1)//trans vlan id
            {
                br_vlan_t->u.hybird.trans_vlan[k] = atoi(argv[i]);
                br_vlan_t->u.hybird.trans_num = k+1;
                k++;
                if(k > VLAN_MAX_TRUNK)
                    goto ERROR;
            }
            else if(j == 2)//untag vlan id
            {
                br_vlan_t->u.hybird.untag_vlan[k] = atoi(argv[i]);
                br_vlan_t->u.hybird.untag_num = k+1;
                k++;
                if(k > VLAN_MAX_UNTAG)
                    goto ERROR;
            }
        }      
        return 0;
    }
	/* translate mode */
	if(argv[4]==NULL)
		goto ERROR;
	if(!strcmp(argv[2],"translate"))
	{
		br_vlan_t->mode=VLAN_TRANSLATE;
		br_vlan_t->pvlan = atoi(argv[3]);
		if(argc > (2*VLAN_MAX_TRUNK + 5)||(argc%2!=0))
			goto ERROR;
		int num = 0;	
		for(i = 4;argv[i]!=NULL&&argv[i+1]!=NULL;i=i+2)
		{
			
      printf("%d %d\n",atoi(argv[i]),atoi(argv[i+1]));
			br_vlan_t->u.translate[num].old_vlan= atoi(argv[i]);
			br_vlan_t->u.translate[num].new_vlan= atoi(argv[i+1]);
			num++;
		}
		
		br_vlan_t->num = num;
		
		for(i=0;i<br_vlan_t->num;i++)
		{
			printf("show translate node:\n");
			printf("%d %d\t\n",br_vlan_t->u.translate[i].old_vlan,br_vlan_t->u.translate[i].new_vlan);
		}
		return 0;
	}
	/* aggr mode */
	if(!strcmp(argv[2],"aggr"))
	{
		br_vlan_t->mode=VLAN_AGGR;
		br_vlan_t->pvlan = atoi(argv[3]);
		i=j=k=0;
		if(strcmp(argv[4],"n"))
			goto ERROR;
		for(i=4;argv[i]!=NULL;i++)
		{
			
			if(!strcmp(argv[i],"n"))
			{
				k++;
				j=0;
			}
			else
			{
				if(j==0)
					br_vlan_t->u.aggr[k-1].vlan_1=atoi(argv[i]);
				else
					br_vlan_t->u.aggr[k-1].vlan_n[j-1]=atoi(argv[i]);
				j++;
			}
			br_vlan_t->u.aggr[k-1].num=j-1;
		}	
		if(br_vlan_t->u.aggr[k].num>VLAN_MAX_AGGR_N)
			goto ERROR;
		br_vlan_t->num = k;
		if(br_vlan_t->num>VLAN_MAX_AGGR)
			goto ERROR;
		return 0;
	}
	goto ERROR;
	
	/* 参数错误 */
ERROR:
	printf("invalid argument\n");
	return -1;
}
static int br_cmd_vlan(int argc, char*const* argv)
{
	int err;
	char port[128]={0};
	VLAN_MODE_T br_vlan;
	/* vlan show */
	if (!strcmp(argv[1],"show"))
	{
		if(argv[2] != NULL)
		{
			err = br_vlan_show(argv[2]);
			return err;
				
		}	
		else
			printf("invalid argument\n");
	}
	/* vlan set */
	else
	{
		
		/* 结构体初始化 */
		br_vlan.mode = 0;
		br_vlan.num = 0;
		br_vlan.pvlan =0;
		memset(&br_vlan,0,sizeof(VLAN_MODE_T));
		/* 参数写入 */
		if(vlan_mode(argc,argv,&br_vlan))
			return -1;
		strcpy(port,argv[1]);
		switch(err=br_vlan_set(argv[1],&br_vlan))
		{
			case 0:
			{
				printf("vlan set success\n");
				return 0;

			}
			default:
				printf("something is wrong,vlan set fail\n");
		}
	}
}
#endif


static int strtotimeval(struct timeval *tv, const char *time)
{
	double secs;
	if (sscanf(time, "%lf", &secs) != 1) 
		return -1;
	tv->tv_sec = secs;
	tv->tv_usec = 1000000 * (secs - tv->tv_sec);
	return 0;
}

static int br_cmd_addbr(int argc, char*const* argv)
{
	int err;

	switch (err = br_add_bridge(argv[1])) {
	case 0:
		return 0;

	case EEXIST:
		fprintf(stderr,	"device %s already exists; can't create "
			"bridge with the same name\n", argv[1]);
		return 1;
	default:
		fprintf(stderr, "add bridge failed: %s\n",
			strerror(err));
		return 1;
	}
}

static int br_cmd_delbr(int argc, char*const* argv)
{
	int err;

	switch (err = br_del_bridge(argv[1])){
	case 0:
		return 0;

	case ENXIO:
		fprintf(stderr, "bridge %s doesn't exist; can't delete it\n",
			argv[1]);
		return 1;

	case EBUSY:
		fprintf(stderr, "bridge %s is still up; can't delete it\n",
			argv[1]);
		return 1;

	default:
		fprintf(stderr, "can't delete bridge %s: %s\n",
			argv[1], strerror(err));
		return 1;
	}
}

static int br_cmd_addif(int argc, char *const* argv)
{
	const char *brname;
	int err;

	argc -= 2;
	brname = *++argv;

	while (argc-- > 0) {
		const char *ifname = *++argv;
		err = br_add_interface(brname, ifname);

		switch(err) {
		case 0:
			continue;

		case ENODEV:
			if (if_nametoindex(ifname) == 0)
				fprintf(stderr, "interface %s does not exist!\n", ifname);
			else
				fprintf(stderr, "bridge %s does not exist!\n", brname);
			break;

		case EBUSY:
			fprintf(stderr,	"device %s is already a member of a bridge; "
				"can't enslave it to bridge %s.\n", ifname,
				brname);
			break;

		case ELOOP:
			fprintf(stderr, "device %s is a bridge device itself; "
				"can't enslave a bridge device to a bridge device.\n",
				ifname);
			break;

		default:
			fprintf(stderr, "can't add %s to bridge %s: %s\n",
				ifname, brname, strerror(err));
		}
		return 1;
	}
	return 0;
}

static int br_cmd_delif(int argc, char *const* argv)
{
	const char *brname;
	int err;

	argc -= 2;
	brname = *++argv;

	while (argc-- > 0) {
		const char *ifname = *++argv;
		err = br_del_interface(brname, ifname);
		switch (err) {
		case 0:
			continue;

		case ENODEV:
			if (if_nametoindex(ifname) == 0)
				fprintf(stderr, "interface %s does not exist!\n", ifname);
			else
				fprintf(stderr, "bridge %s does not exist!\n", brname);
			break;

		case EINVAL:
			fprintf(stderr, "device %s is not a slave of %s\n",
				ifname, brname);
			break;

		default:
			fprintf(stderr, "can't delete %s from %s: %s\n",
				ifname, brname, strerror(err));
		}
		return 1;
	}
	return 0;
}

static int br_cmd_setageing(int argc, char *const* argv)
{
	int err;
	struct timeval tv;
	
	if (strtotimeval(&tv, argv[2])) {
		fprintf(stderr, "bad ageing time value\n");
		return 1;
	}

	err = br_set_ageing_time(argv[1], &tv);
	if (err)
		fprintf(stderr, "set ageing time failed: %s\n",
			strerror(err));

	return err != 0;
}

static int br_cmd_setbridgeprio(int argc, char *const* argv)
{
	int prio;
	int err;

	if (sscanf(argv[2], "%i", &prio) != 1) {
		fprintf(stderr,"bad priority\n");
		return 1;
	}

	err = br_set_bridge_priority(argv[1], prio);
	if (err)
		fprintf(stderr, "set bridge priority failed: %s\n",
			strerror(err));
	return err != 0;
}

static int br_cmd_setfd(int argc, char *const* argv)
{
	struct timeval tv;
	int err;

	if (strtotimeval(&tv, argv[2])) {
		fprintf(stderr, "bad forward delay value\n");
		return 1;
	}

	err = br_set_bridge_forward_delay(argv[1], &tv);
	if (err)
		fprintf(stderr, "set forward delay failed: %s\n",
			strerror(err));

	return err != 0;
}

static int br_cmd_sethello(int argc, char *const* argv)
{
	struct timeval tv;
	int err;

	if (strtotimeval(&tv, argv[2])) {
		fprintf(stderr, "bad hello timer value\n");
		return 1;
	}

	err = br_set_bridge_hello_time(argv[1], &tv);
	if (err)
		fprintf(stderr, "set hello timer failed: %s\n",
			strerror(err));

	return err != 0;
}

static int br_cmd_setmaxage(int argc, char *const* argv)
{
	struct timeval tv;
	int err;

	if (strtotimeval(&tv, argv[2])) {
		fprintf(stderr, "bad max age value\n");
		return 1;
	}
	err = br_set_bridge_max_age(argv[1], &tv);
	if (err)
		fprintf(stderr, "set max age failed: %s\n",
			strerror(err));

	return err != 0;
}

static int br_cmd_setpathcost(int argc, char *const* argv)
{
	int cost, err;

	if (sscanf(argv[3], "%i", &cost) != 1) {
		fprintf(stderr, "bad path cost value\n");
		return 1;
	}

	err = br_set_path_cost(argv[1], argv[2], cost);
	if (err)
		fprintf(stderr, "set path cost failed: %s\n",
			strerror(err));
	return err != 0;
}

static int br_cmd_setportprio(int argc, char *const* argv)
{
	int cost, err;

	if (sscanf(argv[3], "%i", &cost) != 1) {
		fprintf(stderr, "bad path priority value\n");
		return 1;
	}

	err = br_set_port_priority(argv[1], argv[2], cost);
	if (err)
		fprintf(stderr, "set port priority failed: %s\n",
			strerror(errno));

	return err != 0;
}

static int br_cmd_mld_setportsnooping(int argc, char*const* argv)
{
	int err;

	err = br_mld_set_port_snooping(argv[1], argv[2], argv[3]);
	if (err)
		fprintf(stderr, "set mld port snooping failed: %s\n",
			strerror(errno));

	return err != 0;
}

static int br_cmd_mld_clearportsnooping(int argc, char*const* argv)
{
	int err;

	err = br_mld_clear_port_snooping(argv[1], argv[2], argv[3]);
	if (err)
		fprintf(stderr, "clear mld port snooping failed: %s\n",
			strerror(errno));

	return err != 0;
}

static int br_cmd_mld_enableportsnooping(int argc, char*const* argv)
{
	int err;
	int enable;

	if (sscanf(argv[1], "%i", &enable) != 1) {
		fprintf(stderr, "bad value\n");
		return 1;
	}

	err = br_mld_enable_port_snooping(enable);
	if (err)
		fprintf(stderr, "enable mld port snooping failed: %s\n",
			strerror(errno));

	return err != 0;
}

static int br_cmd_mld_enableproxymode(int argc, char*const* argv)
{
	int err;
	int enable;

        sscanf(argv[2], "%i", &enable);

	if((enable != 0) && (enable != 1)) {
		fprintf(stderr, "bad value\n");
		return 1;
	}

	err = br_mld_enable_proxy_mode(argv[1], enable);
	if (err)
		fprintf(stderr, "enable mld proxy mode failed: %s\n",
			strerror(errno));

	return err != 0;
}

static int br_cmd_stp(int argc, char *const* argv)
{
	int stp, err;

	if (!strcmp(argv[2], "on") || !strcmp(argv[2], "yes") 
	    || !strcmp(argv[2], "1"))
		stp = 1;
	else if (!strcmp(argv[2], "off") || !strcmp(argv[2], "no") 
		 || !strcmp(argv[2], "0"))
		stp = 0;
	else {
		fprintf(stderr, "expect on/off for argument\n");
		return 1;
	}

	err = br_set_stp_state(argv[1], stp);
	if (err)
		fprintf(stderr, "set stp status failed: %s\n", 
			strerror(errno));
	return err != 0;
}

static int br_cmd_showstp(int argc, char *const* argv)
{
	struct bridge_info info;

	if (br_get_bridge_info(argv[1], &info)) {
		fprintf(stderr, "%s: can't get info %s\n", argv[1],
			strerror(errno));
		return 1;
	}

	br_dump_info(argv[1], &info);
	return 0;
}

static int br_cmd_enableportsnooping(int argc, char *const* argv)
{
	int err;
	int enable;
/*
	if (sscanf(argv[1], "%i", &enable) != 1) {
		fprintf(stderr, "bad value\n");
		return 1;
	}
*/
	if (!strcmp(argv[1], "on") || !strcmp(argv[1], "yes") 
	    || !strcmp(argv[1], "1"))
		enable = 1;
	else if (!strcmp(argv[1], "off") || !strcmp(argv[1], "no") 
		 || !strcmp(argv[1], "0"))
		enable = 0;
	else {
		fprintf(stderr, "expect on/off for argument\n");
		return 1;
	}

	err = br_enable_port_snooping(enable);
	if (err)
		fprintf(stderr, "enable port snooping failed: %s\n",
			strerror(errno));

	return err != 0;
}

static int show_bridge(const char *name, void *arg)
{
	struct bridge_info info;

	printf("%s\t\t", name);
	fflush(stdout);

	if (br_get_bridge_info(name, &info)) {
		fprintf(stderr, "can't get info %s\n",
				strerror(errno));
		return 1;
	}

	br_dump_bridge_id((unsigned char *)&info.bridge_id);
	printf("\t%s\t\t", info.stp_enabled?"yes":"no");

	br_dump_interface_list(name);
	return 0;
}

static int br_cmd_show(int argc, char *const* argv)
{
	printf("bridge name\tbridge id\t\tSTP enabled\tinterfaces\n");
	br_foreach_bridge(show_bridge, NULL);
	return 0;
}

static int compare_fdbs(const void *_f0, const void *_f1)
{
	const struct fdb_entry *f0 = _f0;
	const struct fdb_entry *f1 = _f1;

	return memcmp(f0->mac_addr, f1->mac_addr, 6);
}

static int br_cmd_showmacs(int argc, char *const* argv)
{
	const char *brname = argv[1];
#define CHUNK 128
	int i, n;
	struct fdb_entry *fdb = NULL;
	int offset = 0;

	for(;;) {
		fdb = realloc(fdb, (offset + CHUNK) * sizeof(struct fdb_entry));
		if (!fdb) {
			fprintf(stderr, "Out of memory\n");
			return 1;
		}
			
		n = br_read_fdb(brname, fdb+offset, offset, CHUNK);
		if (n == 0)
			break;

		if (n < 0) {
			fprintf(stderr, "read of forward table failed: %s\n",
				strerror(errno));
			return 1;
		}

		offset += n;
	}

	qsort(fdb, offset, sizeof(struct fdb_entry), compare_fdbs);

	printf("port no\tmac addr\t\tis local?\tageing timer\n");
	for (i = 0; i < offset; i++) {
		const struct fdb_entry *f = fdb + i;
		printf("%3i\t", f->port_no);
		printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\t",
		       f->mac_addr[0], f->mac_addr[1], f->mac_addr[2],
		       f->mac_addr[3], f->mac_addr[4], f->mac_addr[5]);
		printf("%s\t\t", f->is_local?"yes":"no");
		br_show_timer(&f->ageing_timer_value);
		printf("\n");
	}
	return 0;
}

static int parser_mac_address(unsigned char *smac,unsigned char *bmac)
{
	int ret=0;

	ret = sscanf(smac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
	             &bmac[0], &bmac[1], &bmac[2], &bmac[3], &bmac[4], &bmac[5]);
    if (ret != 6)
        return 1;

    return 0;
}

extern int br_set_static_mac(int enable,const char* brname,const char* staticmac);
static int br_cmd_setstaticmac(int argc, char *const* argv)
{
	const char *brname;
	const char *staticmac;
	unsigned char mac[6];
	
	int err;

	brname = *++argv;
	staticmac = *++argv;
	
	if(!strcmp("off",staticmac)||!strcmp("OFF",staticmac))
	{
		err=br_set_static_mac(0,brname,NULL);
	}	
	else
	{
		err=parser_mac_address(staticmac,mac);
		if(err)
		{	
			fprintf(stderr, "parameter format is error,off | xx:xx:xx:xx:xx:xx\n");
			return err != 0; 
		}
		err=br_set_static_mac(1,brname,mac);
	}
	
	if (err)
		fprintf(stderr, "set static mac address failed: %s\n",strerror(errno));

	return err != 0;
}
static const struct command commands[] = {
	{ 1, "addbr", br_cmd_addbr, "<bridge>\t\tadd bridge" },
	{ 1, "delbr", br_cmd_delbr, "<bridge>\t\tdelete bridge" },
	{ 2, "addif", br_cmd_addif, 
	  "<bridge> <device>\tadd interface to bridge" },
	{ 2, "delif", br_cmd_delif,
	  "<bridge> <device>\tdelete interface from bridge" },
	{ 2, "setageing", br_cmd_setageing,
	  "<bridge> <time>\t\tset ageing time" },
	{ 2, "setbridgeprio", br_cmd_setbridgeprio,
	  "<bridge> <prio>\t\tset bridge priority" },
	{ 2, "setfd", br_cmd_setfd,
	  "<bridge> <time>\t\tset bridge forward delay" },
	{ 2, "sethello", br_cmd_sethello,
	  "<bridge> <time>\t\tset hello time" },
	{ 2, "setmaxage", br_cmd_setmaxage,
	  "<bridge> <time>\t\tset max message age" },
	{ 3, "setpathcost", br_cmd_setpathcost, 
	  "<bridge> <port> <cost>\tset path cost" },
	{ 3, "setportprio", br_cmd_setportprio,
	  "<bridge> <port> <prio>\tset port priority" },
	{ 0, "show", br_cmd_show, "\t\t\tshow a list of bridges" },
	{ 1, "showmacs", br_cmd_showmacs, 
	  "<bridge>\t\tshow a list of mac addrs"},
	{ 1, "showstp", br_cmd_showstp, 
	  "<bridge>\t\tshow bridge stp info"},
	{ 2, "stp", br_cmd_stp,
	  "<bridge> {on|off}\tturn stp on/off" },
/*  for igmp snooping   */
  	{ 1, "igmpsnooping", br_cmd_enableportsnooping,
	  "{on|off}\t\tenable igmp snooping" },
/*  end  igmp snooping  */
/*  for static mac   */
  	{ 2, "setstaticmac", br_cmd_setstaticmac,
	  "<bridge> <off|mac>\t\tset static mac address" },
/*  end  static mac  */
/*add vlan support*/
#ifdef CONFIG_VLAN_MODE
	{1,"vlan",br_cmd_vlan,"show <port>\t\tshow a list of vlan\n\t\t\t<port> <module> <pattern>\tset vlan pattern"},
#endif

	{ 3, "mldsetportsnooping", br_cmd_mld_setportsnooping,
	  "<bridge> <port> <addr>\tset mld/ipv6 port snooping" },
	{ 3, "mldclearportsnooping", br_cmd_mld_clearportsnooping,
	  "<bridge> <port> <addr>\tclear mld/ipv6 port snooping" },
	{ 1, "mldenableportsnooping", br_cmd_mld_enableportsnooping,
	  "<enable>\tenable mld port snooping" },
	{ 2, "mldenableproxymode", br_cmd_mld_enableproxymode,
	  "<bridge> <value> \tTo enable 1 or disable 0" },
};

const struct command *command_lookup(const char *cmd)
{
	int i;

	for (i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
		if (!strcmp(cmd, commands[i].name))
			return &commands[i];
	}

	return NULL;
}

void command_helpall(void)
{
	int i;

	for (i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
		printf("\t%-10s\t%s\n", commands[i].name, commands[i].help);
	}
}
