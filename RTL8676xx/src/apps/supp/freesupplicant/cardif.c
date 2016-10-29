#include <net/if.h>
#include <errno.h>
#include <stdio.h> 
#include "cardif.h"
#include "statemachine.h"
#include "main.h"
extern struct config_data config_info;
struct cardif_data cardif;

unsigned long int crc32_table[256]; 
unsigned long int ulPolynomial = 0x04c11db7; 
unsigned long int reflect(unsigned long int ref, char ch)
{ 
	unsigned long int value=0;
	int i;
	// 交换bit0和bit7，bit1和bit6，类推 
	for(i = 1; i < (ch + 1); i++) { 
		if(ref & 1) 
			value |= 1 << (ch - i); 
		ref >>= 1; 
	} 
	return value; 
} 
void init_crc32_table() 
{ 
	unsigned long int crc,temp; 
	int i,j;
	unsigned long int t1,t2; 
	unsigned long int flag;
	// 256个值 
	for(i = 0; i <= 0xFF; i++) { 
		temp=reflect(i, 8); 
		crc32_table[i]= temp<< 24; 
		for (j = 0; j < 8; j++){ 
			 flag=crc32_table[i]&0x80000000; 
			t1=(crc32_table[i] << 1); 
			if(flag==0) 
				t2=0; 
			else 
				t2=ulPolynomial; 
			crc32_table[i] =t1^t2 ; 
		} 
		crc=crc32_table[i]; 
		crc32_table[i] = reflect(crc32_table[i], 32); 
	} 
}

unsigned long generateCRC32(char  * databuf,unsigned long len) 
{ 
	unsigned long oldcrc32; 
	unsigned long crc32; 
	unsigned long oldcrc; 
	unsigned int charcnt; 
	unsigned char c,t; 
	oldcrc32 = 0x00000000; //初值为0 
	charcnt=0;
	while (len--) { 
		t= (oldcrc32 >> 24) & 0xFF; //要移出的字节的值 
		oldcrc=crc32_table[t]; //根据移出的字节的值查表 
		c=databuf[charcnt]; //新移进来的字节值 
		oldcrc32= (oldcrc32 << 8) | c; //将新移进来的字节值添在寄存器末字节中 
		oldcrc32=oldcrc32^oldcrc; //将寄存器与查出的值进行xor运算 
		charcnt++; 
	} 
	crc32=oldcrc32; 
	return crc32; 
} 
void get_mac()
{
	struct ifreq ifr;
	int ret;
	int sock=socket(AF_INET,SOCK_DGRAM,0);
	if(sock<0){
		printf("bad socket\n");
	}
	strcpy(ifr.ifr_name,cardif.ifname);
	ret=ioctl(sock,SIOCGIFHWADDR,&ifr);
	if(ret<0){
		printf("bad ioctl\n");
		close(sock);
		exit(0);
	}
	memcpy(cardif.src_mac,ifr.ifr_hwaddr.sa_data,6);
	close(sock);
	return;
}
void init_cardif()
{
	int ret;
	struct ifreq ifr;
	memset(&cardif,0,sizeof(cardif));
	strncpy(cardif.ifname, config_info.cardif, strlen(config_info.cardif)+1);
	cardif.recv_frame=(char*)malloc(MAX_MTU_LEN);
	cardif.send_frame=(char*)malloc(MAX_MTU_LEN);
	if(cardif.recv_frame==NULL||cardif.send_frame==NULL){
		printf("bad\n");
		exit(0);
	}
	bzero(cardif.recv_frame,MAX_MTU_LEN);
	bzero(cardif.send_frame,MAX_MTU_LEN);
	while(1){
		cardif.ifindex=if_nametoindex(cardif.ifname);
		if(cardif.ifindex>0){
			printf("cardif.ifindex=%d\n",cardif.ifindex);
			break;
		}
		sleep(1);
	}
	cardif.sll.sll_family = PF_PACKET;
  	cardif.sll.sll_ifindex = cardif.ifindex;
  	cardif.sll.sll_protocol = htons(ETH_P_EAPOL);
	ret=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_EAPOL)); 
	if(ret<=0){
		printf("bad socket,errno=%d\n",errno);
		return;
	}
	printf("cardif.sock=%d\n",ret);
	cardif.sock=ret;
	ret=bind(cardif.sock,(const struct sockaddr *)&cardif.sll,sizeof(cardif.sll));
	if(ret<0){
		printf("bind bad\n");
		exit(0);
	}
	get_mac();
	memset(&ifr,0,sizeof(ifr));
	strncpy((char*)&ifr.ifr_name,cardif.ifname,strlen(cardif.ifname));
	ret=ioctl(cardif.sock,SIOCGIFFLAGS,&ifr);
	if(ret<0){
		printf("bad ioctl\n");
	}
	if(ifr.ifr_flags & IFF_ALLMULTI){
		//already support allmulti.
		return;
	}
	ifr.ifr_flags |= IFF_ALLMULTI;
	ret=ioctl(cardif.sock,SIOCSIFFLAGS,&ifr);
	if(ret<0){
		printf("bad ioctl\n");
	}	
}
int cardif_getframe(){
	char dot1x_default_dest[6] = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x03};
	int ret;
	int a1,a2,a3;
	memset(cardif.recv_frame,0,MAX_MTU_LEN);
	cardif.recv_size=0;
	ret=recvfrom(cardif.sock,cardif.recv_frame,MAX_MTU_LEN,0,0,0);
	if(ret<0){
		printf("recvfrom() bad\n");
		return ret;
	}else if(ret==0){
		return ret;
	}else{
		a1=memcmp(cardif.recv_frame,cardif.src_mac,6);
		a2=memcmp(cardif.recv_frame,dot1x_default_dest,6);
		a3=memcmp((char*)&cardif.recv_frame[6],cardif.src_mac,6);
		if((a1==0)||((a2==0)&&(a3!=0))){
			//the frame we need.
			cardif.recv_size=ret;
			printf("get a frame,len=%d\n",ret);
			return ret;
		}
		return 0;
	}
	
}
int cardif_sendframe(){
	int ret;
	int pad;
	int crc;
	char nomac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	char dot1x_default_dest[6] = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x03};
	printf("in cardif sendframe\n");
	memcpy(cardif.send_frame,cardif.dst_mac,6);
	memcpy((char*)&cardif.send_frame[6],cardif.src_mac,6);
	ret=memcmp(cardif.send_frame,nomac,6);
	if(ret==0){
		memcpy(cardif.send_frame,dot1x_default_dest,6);
	}
	if(cardif.send_size<60){
		pad=60-cardif.send_size;
		memset((char*)&cardif.send_frame[cardif.send_size],0,pad);
		cardif.send_size=60;
	}
	//set CRC.
	*((int *)&(cardif.send_frame[cardif.send_size]))=generateCRC32(cardif.send_frame, cardif.send_size);
	ret=sendto(cardif.sock,cardif.send_frame,cardif.send_size,0,NULL,0);
	if(ret<=0){
		printf("sendto bad\n");
		exit(0);
	}
	memset(cardif.send_frame,0,MAX_MTU_LEN);
	cardif.send_size=0;
	return ret;
	
}
void send_logstart_frame(){
	char *p=cardif.send_frame;
	struct eapol_header *h=(struct eapol_header*)&(p[OFFSET_PAST_MAC]);
	h->ether_type=htons(ETH_P_EAPOL);
	h->proto_ver=0x1;
	h->type=EAPOL_START;
	h->len=0x0;
	cardif.send_size=OFFSET_PAST_MAC+sizeof(struct eapol_header);
	cardif_sendframe();
}


