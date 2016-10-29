#ifndef FSK_DET_H_
#define FSK_DET_H_



extern long auto_cid_det[];
extern long cid_type[];

enum _AUTO_CID_DET_
{
    AUTO_CID_DET_OFF = 0,
    AUTO_CID_DET_ON_NTT,
    AUTO_CID_DET_ON_NO_NTT,
    AUTO_CID_DET_MAX
};

enum _CID_TYPE_
{
    CID_TYPE_FSK_BELLCORE = 0,
    CID_TYPE_FSK_ETSI,
    CID_TYPE_FSK_BT,
    CID_TYPE_FSK_NTT,
    CID_TYPE_DTMF,
    CID_TYPE_MAX
};


typedef struct
{
	char number[25]; //caller id - number	/* if caller id is bellcore SDMF number may eq 'P' or 'O' represent private or out-of-area */
	char date[9]; //caller id - date
	char cid_name[51];
	char called_number[26];//for japan DID usage
	char visual_indicator_deactivation;	/* =1: indicator absence of waiting messages */
	char visual_indicator_activation;	/* !=0: indicator presence of waiting messages */
	char number_absence;			/* !=0: indicator private or whatever absence number */
	char name_absence;			/* !=0: indicator private or whatever absence name */
	unsigned char byte_buf[256];//the fsk caller id orignal data. user can re verify the data. or re decode
					//this byte_buf for new country caller id spec:
	char cid_valid;		/* cid_valid =1 only when decode number or date */
	char cid_receive;		/* cid_rec =1 when other type message is received */
}
TstVoipciddet;

extern TstVoipciddet stVoipciddet[];

void init_cid_det_si3500(unsigned char chid);
void cid_det_si3050(unsigned short* page_addr, unsigned char chid);


/*
   setting the NTT short ring OFF level  unit = 10 ms,
   EX: set_NTT_sRing_off_lvl(25, 60) <- this is default value, but ENGINEER CAN MODIFY TO BEST Compatibility
   600ms >  valid sRing off time >250ms
 */
void set_NTT_sRing_off_lvl(int low_lvl, int hi_lvl);


/*
   setting the NTT short ring ON level  unit = 10 ms,
   EX: set_NTT_sRing_on_lvl(25, 60) <- this is default value, but ENGINEER CAN MODIFY TO BEST Compatibility
   600ms >  valid sRing on time >250ms
 */
void set_NTT_sRing_on_lvl(int low_lvl, int hi_lvl);


/*
   setting the NTT short ring ON-OFF times:
   EX: set_NTT_onoff_times( 2, 1) <- this is default value, but ENGINEER CAN MODIFY TO BEST Compatibility
   
   sRing_on -> sRing_off -> sRing_on ; in the second ring_on 
   if enable NTT caller id det. DAA going to will OFF-HOOK, to recv the japan caller id.
 */
void set_NTT_onoff_times(int ring_on_cnt, int ring_off_cnt );

void NTT_sRing_det(int chid, int ring_stat);


#endif /* FSK_DET_H_ */
