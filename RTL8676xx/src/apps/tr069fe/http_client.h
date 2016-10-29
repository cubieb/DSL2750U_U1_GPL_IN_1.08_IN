
#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif

struct HTTPDiagnostics{

     unsigned int uiTotalBytesSent;	 
     unsigned int uiTestBytesReceived;
     unsigned int uiTotalBytesReceived;
     time_t ROMTime;
     suseconds_t ROMTimeMicro;
     time_t BOMTime;
     suseconds_t BOMTimeMicro;     
     time_t EOMTime;
     suseconds_t EOMTimeMicro;     
     time_t TCPOpenRequestTime;
     suseconds_t TCPOpenRequestTimeMicro;     
     time_t TCPOpenResponseTime;
     suseconds_t TCPOpenResponseTimeMicro;     
};

enum {
	
     TCP_REQUEST_TIME,
     TCP_RESPONE_TIME,
     HTTP_REQUEST_TIME,
     HTTP_BEGIN_TRANSMISSION_TIME,
     HTTP_END_TRANSMISSION_TIME,
     
     TOTAL_BYTES_SENT,
     
     TEST_BYTES_RECEIVED,
     TOTAL_BYTES_RECEIVED, 
     
};

int Record_DiagnosticsData(int iDataType, unsigned int iNum);
int TR143_HTTPDiagnostics_main(int argc, char **argv);

int http_init (const char *url, char *usrname, char *passwd, char *cafile, char *capath);

char *SendSoapMessage (char *buf, int len, int *status);

void CloseHttpSession ();

int comLine_main (int argc, char ** argv);

#ifdef TR069_FE_DEBUG
#define ERR_TRACE()  printf("error: %s %s %d\n", __FILE__, __func__, __LINE__)
#define FUNC_TRACE(fmt, arg...) \
    printf("%s %s %d " fmt "\n", __FILE__, __func__, __LINE__, ##arg)
#else
#define ERR_TRACE()     do{;} while(0)
#define FUNC_TRACE(fmt, arg...)    do{;} while(0)
#endif




#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif



#endif  /* __HTTP_CLIENT_H__ */


