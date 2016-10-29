#include "soapProcessor.h"
#include "warnlog.h"

#define PAIR_NUM 16
#define PAIR_KEY_LEN 264
#define PAIR_VALUE_LEN 264

//#define ONE_MSG_MAX_LEN 512 ==> for more room
#define ONE_MSG_MAX_LEN 700

#ifndef TRUE
#define	TRUE	1
#endif

#ifndef FALSE
#define	FALSE	0
#endif

typedef char CA_Array[PAIR_VALUE_LEN];

//an array is just for saving set_parameter_values RPC treename
char **p_ppArrSaveSetParaValues;

//an int value just saving the number of treenames in p_ppArrSaveSetParaValues
int p_iSavedSetParaValues;


//a pointer to the list header of SetParameterAttributes
paramAttrNameStruct *p_ptSetParamAttrib;

// an unsigned long value for backup the MSGid when it is sent to CMM or TR069BE
//No response from destionation module, i have to use the msg id created when SendXML... function
unsigned long p_ulMsgID_backup;


unsigned char *TW_ConstructMessageHeader(unsigned char **pcDst, unsigned short usDstMID, 
                						                          unsigned long  ulMsgID, unsigned short usMsgType,
                						                          unsigned long  ulBodyLength);
#ifdef TR069_FE_DEBUG
//#define TR069FE_TRACE(fmt, args...) TBS_TRACE(MID_TR069FE, "[%s %s %d] "fmt, __FILE__, __func__, __LINE__, ##args)
//#define TR069FE_TRACE(fmt, args...) syslog(LOG_INFO, "[%s %s %d] "fmt, __FILE__, __func__, __LINE__, ##args);
//#define TR069FE_TRACE(fmt, args...) syslog(LOG_INFO, fmt, ##args)
#define TR069FE_TRACE(fmt, args...) printf("%s %s %d"fmt, __FILE__, __func__, __LINE__, ##args);
#else
#define TR069FE_TRACE(fmt, args...) do{;} while(0);
#endif
