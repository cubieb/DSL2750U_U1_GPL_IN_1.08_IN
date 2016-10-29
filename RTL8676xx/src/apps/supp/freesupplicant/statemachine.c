#include "statemachine.h"
#include "cardif.h"
#include "main.h"
#include "eap.h"
#include "MD5.h"

struct state_info state;
extern struct cardif_data cardif;
extern struct config_data config_info;
void init_statemachine(){
	memset((void*)&state,0,sizeof(state));
	state.supp_state=INITIALIZE;
	state.max_start=4;
	state.md5_v=(struct md5_values*)malloc(sizeof(struct md5_values));
	if(state.md5_v==NULL){
		return;
	}
}
//len,only the length of data(except the first byte of data)
void build_eap_header(char code,char type,int len){
	char *p=cardif.send_frame;
	struct eapol_header *h=(struct eapol_header*)&(p[OFFSET_PAST_MAC]);
	struct eap_header *a=(struct eap_header*)&(p[OFFSET_TO_EAP]);
	char *b=&p[OFFSET_TO_DATA];
	int frame_len=sizeof(struct eap_header)+len;
	h->ether_type=htons(ETH_P_EAPOL);
	h->proto_ver=state.eapol_ver;
	h->type=EAPOL_PACKET;
	h->len=htons(frame_len);
	a->code=code;
	a->identifier=state.receive_id;
	a->len=htons(frame_len);
	a->type=type;
	return;
}
void send_res_id(){
	char *p=cardif.send_frame;
	char *b=&p[OFFSET_TO_DATA];
	int name_len=strlen(config_info.name);
	build_eap_header(EAP_RES, EAP_REQUEST_ID, name_len);
	strncpy(b,config_info.name,name_len);
	cardif.send_size=OFFSET_TO_DATA+name_len;
	cardif_sendframe();
}
void send_res_md5_challenge(){
	unsigned char *tohash;
	unsigned char md5_result[16];
	int tohashlen;
	int datalen,i;
	struct md5_values *m=state.md5_v;
	char *p=cardif.send_frame;
	char *b=&p[OFFSET_TO_DATA];
	tohashlen=1+m->length+strlen(config_info.passwd);
	tohash=(char*)malloc(tohashlen);
	if(tohash==NULL){
		return;
	}
	tohash[0]=state.receive_id;
	memcpy((void*)&tohash[1],config_info.passwd,strlen(config_info.passwd));
	memcpy((void*)&tohash[1+strlen(config_info.passwd)],m->randval,m->length);
	for(i=0;i<0x14;i++){
		printf("%02X ",tohash[i]);
	}
	MD5(tohash,tohashlen,md5_result);
	for(i=0;i<0x10;i++){
		printf("%02X ",md5_result[i]);
	}
	memset(m,0,sizeof(struct md5_values));

	b[0]=0x10;
	memcpy(&b[1],md5_result,0x10);
	memcpy(&b[0x11],config_info.name,strlen(config_info.name));
	datalen=0x11+strlen(config_info.name);
	build_eap_header(EAP_RES, EAP_TYPE_MD5_CHALLENGE, datalen);
	cardif.send_size=OFFSET_TO_DATA+datalen;
	cardif_sendframe();
	state.send_id=state.receive_id;
}
void sm_handle_init(){
	struct eapol_header *l;
	struct eap_header *p;
	char *a;
	a=cardif.recv_frame;
	l=(struct eapol_header*)&(a[OFFSET_PAST_MAC]);
	p=(struct eap_header*)&(a[OFFSET_TO_EAP]);
	state.eapol_ver=l->proto_ver;
	if(l->type!=EAPOL_PACKET){
		return;
	}

}
void handle_type_identity(){
	struct eap_header *p;
	char *a;
	a=cardif.recv_frame;
	p=(struct eap_header*)&(a[OFFSET_TO_EAP]);
	if(state.eap_req==FALSE){
		state.receive_id=p->identifier;
		state.eap_req=TRUE;
		send_res_id();
		state.eap_res=TRUE;
		state.send_id=state.receive_id;
		return;
	}
	if(state.eap_req==TRUE){
		if(p->identifier!=state.receive_id){
			//think the request should be handle by other supplicant.
			return;
		}
		if(p->identifier==state.receive_id){
			//ask us to retry response.
			send_res_id();
		}
		return;
	}
	
}
void handle_type_md5_challenge(){
	struct eap_header *p;
	char *a,*d;
	struct md5_values *v;
	a=cardif.recv_frame;
	p=(struct eap_header*)&(a[OFFSET_TO_EAP]);	
	d=(char*)&a[OFFSET_TO_DATA];
	if(state.eap_req==FALSE||state.eap_res==FALSE){
		//before request,get md5 challenge.
		return;
	}
	state.receive_id=p->identifier;
	v=state.md5_v;
	v->length=d[0];
	memcpy((void*)v->randval,&d[1],v->length);
	send_res_md5_challenge();
	printf("have send response of md5 challenge\n");
	state.supp_state=AUTHENTICATING;
}
void handle_eap_suc(){
	struct eap_header *p;
	char *a;
	a=cardif.recv_frame;
	p=(struct eap_header*)&(a[OFFSET_TO_EAP]);	
    printf("Success!\n");
	if(state.eap_req==FALSE||state.eap_res==FALSE){
		//before request,get suc frame from multicast.
		return;
	}
	if(p->identifier==state.send_id){
		state.supp_state=INITIALIZE;
	}
}
void handle_eap_fail(){
	struct eap_header *p;
	char *a;
	a=cardif.recv_frame;
	p=(struct eap_header*)&(a[OFFSET_TO_EAP]);	
    printf("802.1x Failed\n");
	if(state.eap_req==FALSE||state.eap_res==FALSE){
		//before request,get suc frame from multicast.
		return;
	}
	if(p->identifier==state.send_id){
		state.supp_state=INITIALIZE;
	}
}
void sm_handle_packet(){
	struct eap_header *p;
	char *a;
	a=cardif.recv_frame;
	p=(struct eap_header*)&(a[OFFSET_TO_EAP]);	
	switch(p->code){
		case EAP_REQ:
			switch(p->type){
				case EAP_TYPE_IDENTITY:
					handle_type_identity();
					break;
				case EAP_TYPE_MD5_CHALLENGE:
					handle_type_md5_challenge();
					break;
				default:
					break;
			}
			return;
		case EAP_SUC:
			handle_eap_suc();
			break;
		case EAP_FAIL:
			handle_eap_fail();
			break;
	}
}
void run_statemachine(){
	struct eapol_header *l;
	char *a;
	a=cardif.recv_frame;
	l=(struct eapol_header*)&(a[OFFSET_PAST_MAC]);
	state.eapol_ver=l->proto_ver;
	switch(l->type){
		case EAPOL_PACKET:
			sm_handle_packet();
			break;
		case EAPOL_ASF:
		case EAPOL_LOGOFF:
		case EAPOL_KEY:
			return;
	}
	return;
}

