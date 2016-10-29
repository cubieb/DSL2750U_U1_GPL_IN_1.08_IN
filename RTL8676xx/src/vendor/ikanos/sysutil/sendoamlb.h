/* Copyright (C) 2006 Ikanos Communications 
 * See IKANOS_PROP_LICENSE.txt for license information. */


/*****************************************************************************
 * File Name     : sendoamlb.h
 * Author        : 
 * Description   :
 *****************************************************************************/

#define VP   1
#define VC   2
#define NUM_PARAMS  4
#define MAX_OAMFLOW_NAME 3
#define MAX_LLID_STRING  32

typedef struct
{
    unsigned char vpi;       /* VPI number of connection
                              * Value Range: 0 - 255
                              * Important Note: * */
    unsigned short vci;      /* VCI number of connection
                              * Value Range: 16 - 65535
                              * Important Note: All reserved values would 
                                be added  */
    unsigned int ulOAMFlowType;   /* OAM flow type
                                   * Value Range: "PLT_F5_SEG" or "PLT_F5_ET E"
                                   * Important Note: */

    unsigned int ulLoopbackTime;  /* Output : time taken for loopback in m sec
                                   * Value Range: "PLT_F5_SEG" or "PLT_F5_ ETE"
                                   * Important Note: */
    unsigned char  ucLLID[16];    /* Loopback location ID
                                   * Value Range: 0x0000000000000000..0xFFFFFFFFFFFFFFFF
                                   * Default Value : 0xFFFFFFFFFFFFFFFF
                                   * Important Note: */
    unsigned char ucOAMFlowTypeName[MAX_OAMFLOW_NAME]; /* OAM flow type
                                                        * Value Range: "seg", "ete"
                                                        * Important Note: */
    unsigned char ucLLIDString[MAX_LLID_STRING+1];
                                  /* Input string with possible Loopback location ID info
                                   * Value Range: 0x0000000000000000..0xFFFFFFFFFFFFFFFF
                                   * Default Value : 0xFFFFFFFFFFFFFFFF
                                   * Important Note: */
} atmOamRequestLS;

unsigned char gcliaucBuff[80];

#define ATMSTATHELP "Displays ATM Statistics"
#define ATMSTATUSAGE "atmstats"

#define SHOWMIBHELP "Displays atm connections"
#define SHOWMIBUSAGE "showatmconn"

#define VCTRAFFICHELP "This command allows to add and configure VC level atm connection"

#define OAMF5LBHELP \
"Send an OAM F5 loopback cell.\n"

/* Added for OAM LLID -sairam */

#define OAMF5USAGE \
"USAGE: sendoamlb <vpi> <vci> <flow type> -o <LLID>\n \
         vpi - vpi value \n \
         vci - vci value \n \
   flow type - seg / ete \n \
        LLID - loopback location ID (32 hex digits)\n \
               (default (hex): FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)\n"

int getValue(unsigned char *ptr,int *val);
#define IS_NETPRO 1
#define ATM 1
/*added for OAM LLID -sairam */
#if IS_EXPLORER || (IS_NETPRO && ATM)

unsigned char gaucModemLLID[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};

unsigned char gaucModemLLIDAllFFs[16]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
                                  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

unsigned char gaucModemLLIDAllZeros[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

unsigned char gaucModemLLIDAll6As[16]={0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,
                                   0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a,0x6a};
#endif

