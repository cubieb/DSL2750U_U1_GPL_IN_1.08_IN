#include <common.h>
#include <command.h>
#include <flash.h>
#include <flash_layout_private.h>

void read_ip(char *argv, unsigned char iTempIP[])
{
	unsigned char i,k;	
	unsigned char ip_right=0;
	unsigned char ip_data[4][3];
	int j;

	for(i=0;i<4;i++)
	{
		for(k=0;k<3;k++)
			ip_data[i][k]=0;		
	}
	
	i=4;
	j=0;
	k=2;	

	argv[strlen(argv)]='.';
	argv[strlen(argv)+1]='\0';
	for(j=(strlen(argv)-1);j>=0;j--)
	{		
		if(argv[j]=='.')
		{				
			i--;
			k=2;
			continue;
		}
		ip_data[i][k]=argv[j];
		k--;		
	}
	for(i=0;i<15;i++)
		argv[i]=0;
	for(i=0;i<4;i++)
	{		
		for(k=0;k<3;k++)
		{
			if(ip_data[i][k]>'9')
			{
				printf("Illegal IP address\n");
				ip_right=1;
				break;
			}
			else if(ip_data[i][k]>=48 && ip_data[i][k]<=57)     
			ip_data[i][k]-=48;
		}
	}
	if(ip_right!=1)
	{
		for(i=0;i<4;i++)
		{		
			iTempIP[i]=ip_data[i][0]*100+ip_data[i][1]*10+ip_data[i][2];		
		}
	}
}

void read_mac(char *argv, unsigned char iTempMAC[])
{
	unsigned char j,k;
	unsigned char mac_ok = 1;

	for(j=0;j<17;j++)
		{
		if(argv[j]==':')
			continue;
		if(argv[j]>=65 && argv[j]<=70)       /*'A'<=argv[j]<='F'*/
			argv[j]-=55;
		else if(argv[j]>=97 && argv[j]<=102)     /*'a'<=argv[j]<='f'*/
			argv[j]-=87;	
		else if(argv[j]>=48 && argv[j]<=57)      /*'0'<=argv[j]<='9'*/
			argv[j]-=48;
		else
			{
			printf("Illegal MAC address\n");
			mac_ok = 0;
			break;
			}
		}
	if(0 != mac_ok)
	{
		for(j=0,k=0;j<17;)
		{
			iTempMAC[k]=argv[j]*16+argv[j+1];
			k++;
			j+=3;
		}
	}
}


int do_modify_sysc (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned char  i,j,temp;	
	unsigned char iTempIP[4];
	unsigned char iTempMAC[6];
	struct sys_cfg *syscfg = (struct sys_cfg *)(gd->syscfg_addr);

	union 
	{  
		unsigned int iIpVal;  
		char   ch[4];  
	}iptmp = {0}; 	
	temp=0;       //说明修改的内容与原有相同
	for(i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-mac") == 0)            /* Handle MAC address parameter */
		{
              	i++;    
              	if(i >= argc || *argv[i] == '-')            /* if the next parameter is wrong */
              	{
                  		 printf("The -mac parameter is wrong.\n");
                  		 return 0;
              	}
			read_mac(argv[i],  iTempMAC);
			for(j=0;j<6;j++)
			{
				if((1 == gd->syscfg_ver) && (syscfg->mac[j]!=iTempMAC[j]))
					{
					temp=1;
					}
			}
			if(temp==1)
			{
				for(j=0;j<6;j++)
				{
					syscfg->mac[j]=iTempMAC[j];
				}		
			}
		}

		else if(strcmp(argv[i],"-ip") == 0)            /* Handle IP address parameter */
         	{
              	i++;
              	if(i >= argc || *argv[i] == '-')            /* if the next parameter is wrong */
              	{
                   		printf("The -ip parameter is wrong.\n");
                   		return 0;
              	}
			read_ip(argv[i] ,  iTempIP);
			for(j=0;j<4;j++)
			{
				if(iptmp.ch[j]!=iTempIP[j])
				temp=1;
			}
			if(temp==1)
			{
				for(j=0;j<4;j++)
              		{				
					iptmp.ch[j] = iTempIP[j];
              		}
				syscfg->ip = iptmp.iIpVal;
			}               
            
		}
	}
	if(temp == 1)
		{
		sysdata_save(syscfg); 
		}
	
	return 0;
}

U_BOOT_CMD(
	modify_sysc,	5,	1,	do_modify_sysc,
 	"modify_sysc    - sysc modify \n",
	"-mac xx.xx.xx.xx.xx.xx -ip x.x.x.x\n"
);
