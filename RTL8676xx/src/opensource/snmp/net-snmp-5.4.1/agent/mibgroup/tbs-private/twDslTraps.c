/*
 * Note: this file be created by Yu Jinshi
 * Email: yujinshi@twsz.com
 * date: 2010-12-12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "tbs-private/twDslTraps.h"
#include "tbs-private/adslNodes.h"
#include "tbs-private/remoteNodes.h"
#include "common/cmmif.h" 
#include "common/snmpProcess.h"

#define CPU_TICK_KIND  7
#if (CPU_TICK_KIND == 4)
#define CPU_TICK_FMT  "cpu %d %d %d %d"
#define CPU_TICK_ARG(ticks) &ticks[0], &ticks[1], &ticks[2], &ticks[3]
#elif (CPU_TICK_KIND == 7)
#define CPU_TICK_FMT  "cpu %d %d %d %d %d %d %d"
#define CPU_TICK_ARG(ticks) &ticks[0], &ticks[1], &ticks[2], &ticks[3], &ticks[4], &ticks[5], &ticks[6]
#endif

#define NOTIFICATION_POLL_INTERVAL           40  /* every X second (s) */

/* CPU利用率的过限阈值(%) 95*/
extern  int nDEV_CPU_THRESHOLD ;
/* 内存利用率的过限阈值(%)  90 */
extern  int nDEV_MEM_THRESHOLD ;                   

#define DEV_CPU_OCCUP_TIME                   60 /* 计算CPU利用率的单位时间(s) */
#define DEV_MEM_USAGE_TIME                   60 /* 计算MEM利用率的单位时间(s) */
#define TRAPBASIC_STR_SIZE                   32 /* 最常用的缓存大小 */

#define TRAP_CONFIG_CHANGE_FILE              "/var/tmp/cfgchg.smp"
#define TRAP_AUTH_FAILED                     "/var/auth.smp"
#define TRAP_WLAN_FILE "/var/run/snmpd_wlan"

#define LINE_BUF_SIZE                    128
#define BUF_MAX_SIZE                     80

#define TOOLS_SEEK_SET                   1
#define TOOLS_SEEK_NO                    0

#ifdef DEBUG_SNMP_MIB
#define dprints( fmt, args... )    printf( "[%s](%d)==>" fmt, __FUNCTION__, __LINE__, ## args)
#else
#define dprints( fmt, args... ) 
#endif

extern int isChangeConfig ;
typedef struct __dev_occupation_t
{
	//u_long threshold;               /* 阀值 */
	int notified;                   /* 是否已发告警，已发为1；否则为0 */
	u_long since_time;              /* 已经经过的时间(s) */
	u_long old_time;                /* 上一次的时间值 */
	int collectin_second;           /* 采集的次数 */
	u_long devOccupationTotal;      /* 利用率的采集总数 */
	u_long old_idle;                /* 上次的空闲值*/
	u_long old_total;               /* 上次的总的大小值 */
}dev_occupation_t;

typedef struct tag_ST_IfNet
{
    int ifIndex;
    char ifName[ TRAPBASIC_STR_SIZE ];
    short ifFlag;
    int iNewFlag;
	char ifAddr[TRAPBASIC_STR_SIZE];
    struct tag_ST_IfNet *pNext;
}ST_IfNet;

typedef void threshold_notification_function( u_long val );

int g_nLinkUp = 0 ;
static dev_occupation_t cpu_occupation;
static dev_occupation_t mem_occupation;
static ST_IfNet *ifLink_head;

static oid IfIndex_oid[] = { 1, 3, 6, 1, 2, 1, 2, 2, 1, 1 };
static oid IfAdminStatus_oid[] = { 1, 3, 6, 1, 2, 1, 2, 2, 1, 7 };
static oid ifOperStatus_oid[] = { 1, 3, 6, 1, 2, 1, 2, 2, 1, 8 };
static oid wanIpAddr_oid[] =    {1, 3, 6, 1, 4, 1, 33003, 2, 1, 1, 3 };
static oid wanIpAddrType_oid[] ={1, 3, 6, 1, 4, 1, 33003, 2, 1, 1, 2 };
static oid wlanSSID_oid[] ={1, 3, 6, 1, 4, 1, 33003, 4, 1, 1, 1 };

static int g_isColdStartTrapSent = 0; /*标识冷启动至今，cold start trap是不是已经发送过*/
static int g_isWarmStartTrapSent = 0; /*标识热启动至今，warm start trap是不是已经发送过*/

static traps_info ipChangeTrapMsg       ={"IP Change",                        21202, { 1,3,6,1,4,1,33003,99,1,2},10};
static traps_info willRebootTrapMsg     ={"Will Reboot",                      21203, { 1,3,6,1,4,1,33003,99,1,3},10};
static traps_info cfgChangeTrapMsg      ={"Config Change",                    21204, { 1,3,6,1,4,1,33003,99,1,4 },10};
static traps_info cpuThreshTrapMsg      ={"CPU Usage Threshold Exceed",       21205, { 1,3,6,1,4,1,33003,99,1,5},10};
static traps_info cpuThreshClearTrapMsg ={"Cpu Usage Threshold Exceed Clear", 21206, { 1,3,6,1,4,1,33003,99,1,6},10};
static traps_info ramThreshTrapMsg      ={"RAM Usage Threshold Exceed",       21207, { 1,3,6,1,4,1,33003,99,1,7},10};
static traps_info ramThreshClearTrapMsg ={"RAM Usage Threshold Exceed Clear", 21208, { 1,3,6,1,4,1,33003,99,1,8},10};
static traps_info wlanEnableTrapMsg     ={"wlan start",                       21209, { 1,3,6,1,4,1,33003,99,1,9},10};
static traps_info wlanDisableTrapMsg    ={"wlan stop", 						  21210, { 1,3,6,1,4,1,33003,99,1,10},10};

static traps_info coldStartTrapMsg      ={"Cold Start",   21301,   {1,3,6,1,6,3,1,1,5,1},10};
static traps_info warnStartTrapMsg      ={"Warm start",   21302,   {1,3,6,1,6,3,1,1,5,2},10};
static traps_info linkDownTrapMsg       ={"Link Down",    21303,   {1,3,6,1,6,3,1,1,5,3},10};
static traps_info linkUpTrapMsg         ={"Link Up",      21304,   {1,3,6,1,6,3,1,1,5,4},10};
//authentication failed  21306

void check_CpuThreshold( dev_occupation_t *notification_ctx,
    threshold_notification_function *send_alarm,
    threshold_notification_function *send_alarm_clear );

void check_MemThreshold( dev_occupation_t *notification_ctx,
    threshold_notification_function *send_alarm,
    threshold_notification_function *send_alarm_clear );

void send_devCpuOccupationTooHighAlarm( u_long cpu_occup );
void send_devCpuOccupationTooHighAlarmClear( u_long cpu_occup );
void send_devMemOccupationTooHighAlarm( u_long mem_occup );
void send_devMemOccupationTooHighAlarmClear( u_long mem_occup );
void send_configureChangeAlarm();
void send_startTrap();

int check_LinkUpAndLinkDown( void );
void check_CpuOccupation( void );
void check_MemOccupation( void );
int check_ConfigChange( void );
void check_wlanStart();

int dsl_get_cur_ticks( int *ticks );
int dsl_get_cpu_occup( dev_occupation_t *cpu_occup );
int dsl_get_mem_occupation( dev_occupation_t *mem_occup );

void send_LinkUpAlarm( ST_IfNet *pInIf );
void send_LinkDownAlarm( ST_IfNet *pInIf );
void send_wlanDeviceFailureAlarm( u_long fail_id );
void send_willRebootAlarm( int idata );

int RenewIfList( ST_IfNet **pInLink,int *pSokFd);
int GetItemForFile( FILE *fp, char * const pItmOut, const int iItmOutLen, const char *pKey, const int iSeekSig );
int GetItemFromBuf( char *pBufIn, char * const pItmOut, const int iItmOutLen, const char *pKey );
void send_wanManageIpChangeAlarm(unsigned char* new_ip);

/*-------------------------------------------------------------------------
 * init
 */
void init_twDslTraps( void )
{
    FILE *fp_pid = NULL;
    int iFd;
    ST_IfNet *pIfCur;
    struct ifreq ifrq;

    snmp_alarm_register( NOTIFICATION_POLL_INTERVAL,  /* seconds */
                         SA_REPEAT,      /* repeat */
                         check_twDslNotifications,     /* our callback */
                         NULL    /* no callback data needed */);
	
    if ( ( fp_pid = fopen("/var/run/snmpd_pid", "w" ) ) != NULL )
    {
        fprintf( fp_pid, "%d\n", getpid() );
        fclose( fp_pid );
    }
	
	//busybox init.c 中发送了 kill SIGUSR2  SIGUSR2 SendSingleToSnmpd
    signal( SIGUSR2, send_willRebootAlarm );
	
	memset( &cpu_occupation, 0, sizeof( cpu_occupation ));
	memset( &mem_occupation, 0, sizeof( mem_occupation ));

    dsl_get_cpu_occup( &cpu_occupation );
	cpu_occupation.old_time = time( NULL );
	cpu_occupation.notified = 0;

	dsl_get_mem_occupation( &mem_occupation );
	mem_occupation.old_time = time( NULL );
	mem_occupation.notified = 0;
	
    ifLink_head = NULL;
    
    iFd = socket(AF_INET, SOCK_DGRAM, 0 );
	if( iFd < 0 ) 
	{
	    printf( "<net-snmp> open socket error.\n" );
	    return;
	}

	RenewIfList( &ifLink_head, &iFd );

    close( iFd );
}

void shutdown_twDslTraps(void)
{
    Free_WriteMethodStruct();
}

static void send_wlanSSidAlarm(traps_info* pWlanTrapinfo ,char* pcSSID )
{
    netsnmp_variable_list *notification_vars = NULL;
	tw_AddTrapMsg(&notification_vars,pWlanTrapinfo);

    snmp_varlist_add_variable(&notification_vars, wlanSSID_oid, OID_LENGTH(wlanSSID_oid),ASN_OCTET_STR, 
   	(u_char *)pcSSID, strlen(pcSSID));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

/*-------------------------------------------------------------------------
 * send a alarm when authentication failed
 */
void check_wlanStart()
{    
    if( access( TRAP_WLAN_FILE, F_OK ) == 0 )
    {   
        char sLine[64];
		FILE* fp_dev;
        fp_dev = fopen( TRAP_WLAN_FILE, "r" );
        if( fp_dev != NULL)
        {
            while( fgets( sLine, 64, fp_dev ) != NULL)
            {
                char* pTMP = strstr(sLine,"##");
				sLine[pTMP-sLine] = '\0';
			    int nFlag = atoi(pTMP + 2);

				if(nFlag == 1)
				{
				   send_wlanSSidAlarm(&wlanEnableTrapMsg,sLine );
				}
				else if(nFlag == 0)
				{
				  send_wlanSSidAlarm(&wlanDisableTrapMsg,sLine );
				}
            }
			
            fclose( fp_dev );
        }
        unlink( TRAP_WLAN_FILE );
    }  
}

/*-------------------------------------------------------------------------
 * send a alarm when manage's IP change 
 */
void send_wanManageIpChangeAlarm(unsigned char* new_ip)
{
    netsnmp_variable_list *notification_vars = NULL;
    int iIpType = 1;//1代表IPV4或2代表IPV6

    char sDevSN[ 100 ]= {0};
	char szNodeName[70]={0};
    
	snprintf(szNodeName ,sizeof(szNodeName)-1 ,"%s.SerialNumber",TR069_DeviceInfo);
	if(CMM_GetStr(TR069_DeviceInfo "SerialNumber", sDevSN,100, NULL, 0)!= CMM_SUCCESS)
	{
	  	printf("wanIpChangeAlarm::get Serial Number failed...\n");
		return ;	
	}

    tw_AddTrapMsg(&notification_vars,&ipChangeTrapMsg);
    
    snmp_varlist_add_variable(&notification_vars,
        wanIpAddrType_oid, 
        OID_LENGTH( wanIpAddrType_oid ),
        ASN_INTEGER,
        ( u_char* )&( iIpType ),
        sizeof( iIpType ) );
	
    snmp_varlist_add_variable(&notification_vars,
        wanIpAddr_oid, 
        OID_LENGTH( wanIpAddr_oid ),
        ASN_IPADDRESS,
        CHARS_TO_intIPADDR(new_ip),
        sizeof( u_long ) );
	
    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

/*
 * Get a key's string from buffer.
 */
int GetItemFromBuf( char *pBufIn, char * const pItmOut, const int iItmOutLen, 
	const char *pKey )
{
	int iret = -1;
	char *pBufTmp, *pTmp;
	
	if( ( pBufIn == NULL ) 
		|| ( pKey == NULL) 
		|| ( strlen( pBufIn ) < strlen( pKey ) ) )
	{
		return -1;
	}

	pBufTmp = pBufIn;
	pBufTmp = strstr( pBufTmp, pKey );
	if( pBufTmp != NULL )
	{
		pBufTmp = pBufTmp + strlen( pKey );
		while( ( pBufTmp != NULL) && ( *pBufTmp!= '\0' ) ) 
		{
			if( (*pBufTmp != 0x20) && (*pBufTmp != 0x09) )	/*not SPACE:' ' and TAB:'\t'*/
			{
				break;
			}
			++pBufTmp;
		}
		pTmp = pBufTmp;
		while( ( pTmp != NULL ) && ( *pTmp != '\0' ) )
		{
			if( ( *pTmp == 0x0A ) || ( *pTmp == 0x0D ) )
			{
				*pTmp = '\0';
				break;
			}
			++pTmp;
		}
		strncpy( pItmOut, pBufTmp, iItmOutLen );
		pItmOut[ iItmOutLen - 1 ] = '\0';
		
		iret = 0;
	}

	return iret;
}

/*
 * Get a key's string from file
 */
int GetItemForFile( FILE *fp, char * const pItmOut, const int iItmOutLen, 
	const char *pKey, const int iSeekSig )
{
	char sLine[ LINE_BUF_SIZE ];
	char sVal[ BUF_MAX_SIZE ];
	int iRet = -1;

	if( iSeekSig == TOOLS_SEEK_SET )
	{
		fseek( fp, 0L, SEEK_SET );
	}
	
	while( fgets( sLine, LINE_BUF_SIZE, fp ) != NULL )
	{
		if( GetItemFromBuf( sLine, sVal, BUF_MAX_SIZE, pKey ) == 0 )
		{
			if( iItmOutLen > BUF_MAX_SIZE )
			{
				strncpy( pItmOut, sVal, BUF_MAX_SIZE );
				pItmOut[ BUF_MAX_SIZE - 1 ] = '\0';
			}
			else
			{
				strncpy( pItmOut, sVal, iItmOutLen );
				pItmOut[ iItmOutLen - 1 ] = '\0';
			}
			iRet = 0;
			break;
		}
	}

	return iRet;
}

/*-------------------------------------------------------------------------
 * check device occupation and send alarm
 */
void check_CpuThreshold( dev_occupation_t *notification_ctx,
    threshold_notification_function *send_alarm,
    threshold_notification_function *send_alarm_clear )
{
	int ioccup;

	ioccup = notification_ctx->devOccupationTotal/notification_ctx->collectin_second;
    dprints( "uloccup = %u\n", ioccup );

	//printf( "cpu ioccup = %d , nDEV_CPU_THRESHOLD = %d \n"  , ioccup , nDEV_CPU_THRESHOLD );
	
    if( ioccup > nDEV_CPU_THRESHOLD)
    {
		if( notification_ctx->notified == 0 )
		{
			notification_ctx->notified = 1;
			send_alarm( ioccup );
		}
    }
	else
	{
		if( notification_ctx->notified == 1 )
		{
			notification_ctx->notified = 0;
			send_alarm_clear( ioccup );
		}
	}
}


/*-------------------------------------------------------------------------
 * check configaure change
 */
int check_ConfigChange( void )
{
    FILE *fp_dev;
    int ivar = 0;

    if( access( TRAP_CONFIG_CHANGE_FILE, F_OK ) == 0 )
    {
        fp_dev = fopen( TRAP_CONFIG_CHANGE_FILE, "r" );
        if( fp_dev != NULL )
        {
            fscanf( fp_dev, "%d", &ivar );
            if( ivar == 1 )
            {
                send_configureChangeAlarm( ivar );
            }
            fclose( fp_dev );
        }
        unlink( TRAP_CONFIG_CHANGE_FILE );
    }

    return 0;
}

/*-------------------------------------------------------------------------
 * check device occupation and send alarm
 */
void check_MemThreshold( dev_occupation_t *notification_ctx,
    threshold_notification_function *send_alarm,
    threshold_notification_function *send_alarm_clear )
{
	int ioccup;

	ioccup = notification_ctx->devOccupationTotal/notification_ctx->collectin_second;
    dprints( "uloccup = %u\n", ioccup );
	//printf( "MEM ioccup = %d , nDEV_Mem_THRESHOLD = %d \n"  , ioccup , nDEV_MEM_THRESHOLD );
    if( ioccup > nDEV_MEM_THRESHOLD)
    {
		if( notification_ctx->notified == 0 )
		{
			notification_ctx->notified = 1;
			send_alarm( ioccup );
		}
    }
	else
	{
		if( notification_ctx->notified == 1 )
		{
			notification_ctx->notified = 0;
			send_alarm_clear( ioccup );
		}
	}
}

/*-------------------------------------------------------------------------
 * send alarm when CPU occupation is too high
 */
void send_devCpuOccupationTooHighAlarm( u_long cpu_occup )
{
    netsnmp_variable_list *notification_vars = NULL;

	tw_AddTrapMsg(&notification_vars, &cpuThreshTrapMsg);
    
    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

/*-------------------------------------------------------------------------
 * send alarm to clear alarm
 */
void send_devCpuOccupationTooHighAlarmClear( u_long cpu_occup )
{
    netsnmp_variable_list *notification_vars = NULL;
	tw_AddTrapMsg(&notification_vars, &cpuThreshClearTrapMsg);
   
    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

/*-------------------------------------------------------------------------
 * send alarm when memery occupation is too high
 */
void send_devMemOccupationTooHighAlarm( u_long mem_occup )
{
    netsnmp_variable_list *notification_vars = NULL;
	tw_AddTrapMsg(&notification_vars, &ramThreshTrapMsg);

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

/*-------------------------------------------------------------------------
 * send alarm to clear alarm
 */
void send_devMemOccupationTooHighAlarmClear( u_long mem_occup )
{
    netsnmp_variable_list *notification_vars = NULL;
	char sDevSN[ 100 ]= {0};
	char szNodeName[70]={0};
    dprints( "send cpu occupation trap.\n" );
    
	snprintf(szNodeName ,sizeof(szNodeName)-1 ,"%s.SerialNumber",TR069_DeviceInfo);
	if(CMM_GetStr(TR069_DeviceInfo "SerialNumber", sDevSN,100, NULL, 0)!= CMM_SUCCESS)
	{
	  	dprints("MemAlarmClear::get Serial Number failed...");
		return ;	
	}

    tw_AddTrapMsg(&notification_vars, &ramThreshClearTrapMsg);

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

/*-------------------------------------------------------------------------
 * check CPU occupation
 */
void check_CpuOccupation( void )
{
	time_t tnow;
	
	if( cpu_occupation.since_time < DEV_CPU_OCCUP_TIME )
	{
		dsl_get_cpu_occup( &cpu_occupation );
		tnow = time( NULL );
		cpu_occupation.since_time = cpu_occupation.since_time + 
			( tnow - cpu_occupation.old_time );
		cpu_occupation.old_time = tnow;
	}
	else
	{
		cpu_occupation.since_time = 0;
		cpu_occupation.old_time = time( NULL );
	    check_CpuThreshold( &cpu_occupation,
	        send_devCpuOccupationTooHighAlarm,
	        send_devCpuOccupationTooHighAlarmClear );
		cpu_occupation.devOccupationTotal = 0;
		cpu_occupation.collectin_second = 0;
	}
}

/*-------------------------------------------------------------------------
 * get memery occupation
 */
int dsl_get_mem_occupation( dev_occupation_t *mem_occup )
{
    int iret = -1;
    u_long mem_total = 0;
    u_long mem_free = 0;
	char s_mem[ BUF_MAX_SIZE ];
	FILE *fp_mem;
	
	fp_mem = fopen( "/proc/meminfo", "r" );
	if( fp_mem != NULL )
	{
		s_mem[ 0 ] = '\0';
		iret = GetItemForFile( fp_mem, s_mem, BUF_MAX_SIZE, 
			"MemTotal:", TOOLS_SEEK_NO );
		if( iret == 0 )
		{
			mem_total = strtoul( s_mem, NULL, 0 );
			
			s_mem[ 0 ] = '\0';
			iret = GetItemForFile( fp_mem, s_mem, BUF_MAX_SIZE, 
				"MemFree:", TOOLS_SEEK_NO );
			if( iret == 0 )
			{
				mem_free = strtoul( s_mem, NULL, 0 );
				
				mem_occup->devOccupationTotal = 
					mem_occup->devOccupationTotal 
					+ ( mem_total - mem_free ) * 100 / mem_total;

				++mem_occup->collectin_second;
				iret = 0;
			}
			else
				dprints( "get 'MemFree' value error.\n" );
		}
		else
			dprints( "get 'MemTotal' value error.\n" );
		fclose( fp_mem );
	}
	else
		dprints( "open meminfo error.\n" );

    return iret;
}

/*-------------------------------------------------------------------------
 * check memery occupation
 */
void check_MemOccupation( void )
{
	time_t tnow;

	if( mem_occupation.since_time < DEV_MEM_USAGE_TIME )
	{
		dsl_get_mem_occupation( &mem_occupation );
		tnow = time( NULL );
		mem_occupation.since_time = mem_occupation.since_time + 
			( tnow - mem_occupation.old_time );
		mem_occupation.old_time = tnow;
	}
	else
	{
		mem_occupation.since_time = 0;
		mem_occupation.old_time = time( NULL );
	    check_MemThreshold( &mem_occupation,
	        send_devMemOccupationTooHighAlarm,
	        send_devMemOccupationTooHighAlarmClear );
		mem_occupation.devOccupationTotal = 0;
		mem_occupation.collectin_second = 0;
	}
}

void check_twDslNotifications( unsigned int clientreg, void *clientarg )
{
    send_startTrap();
    check_CpuOccupation();
    check_MemOccupation();
    check_LinkUpAndLinkDown();
	check_ConfigChange();
	check_wlanStart();
}

/*-------------------------------------------------------------------------
 * get cpu occupation ticks
 */
int dsl_get_cur_ticks( int *ticks )
{
    FILE *pfstat = NULL;
    char *stat_file = "/proc/stat";
    char file_buf[ 100 ] = { 0 };
    char *pbuf = NULL;

    pfstat = fopen( stat_file, "r" );
    if ( NULL == pfstat )
    {
        return SNMP_ERR_GENERR;
    }

    pbuf = fgets( file_buf, sizeof( file_buf ), pfstat );
    fclose( pfstat );
    if ( NULL == pbuf )
    {
        return SNMP_ERR_GENERR;
    }

    sscanf( file_buf, CPU_TICK_FMT, CPU_TICK_ARG( ticks ) );

    return SNMP_ERR_NOERROR;
}

/*-------------------------------------------------------------------------
 * get cpu occupation
 */
int dsl_get_cpu_occup( dev_occupation_t *cpu_occup )
{
    int i, ret = 0;
    int ticks[ CPU_TICK_KIND ];
    u_long total_tmp = 0, total;
    u_long idel = 0;

    ret = mib_get_cur_ticks( ticks );
    if ( 0 != ret )
    {
	  dprints( "get cpu ticks fail.\n" );
        return ret;
    }

	idel = ticks[ 3 ] - cpu_occup->old_idle;
	cpu_occup->old_idle = ticks[ 3 ];
	for( i = 0; i < CPU_TICK_KIND; ++i )
	{
		total_tmp = total_tmp + ticks[ i ];
	}
	total = total_tmp - cpu_occup->old_total;
	cpu_occup->old_total = total_tmp;

    cpu_occup->devOccupationTotal = cpu_occup->devOccupationTotal +
		( total - idel ) * 100 / total;
	++cpu_occup->collectin_second;
	
    return SNMP_ERR_NOERROR;
}

/*-------------------------------------------------------------------------
 * send a alarm when reboot system 
 */
void send_willRebootAlarm( int idata )
{
    SNMP_SaveEventsToFlash("1");

    //send will send trap
    netsnmp_variable_list *notification_vars = NULL;
	tw_AddTrapMsg(&notification_vars, &willRebootTrapMsg);
   
    send_v2trap(notification_vars);

    snmp_free_varbind(notification_vars);
}
void send_startTrap( void )
{
   // 0表示冷启动  1表示热启动   2已经发送trap
    if(g_nLinkUp == 1 && ( g_isColdStartTrapSent < 1||g_isWarmStartTrapSent< 1))
    {
        char szReadBuf[65] = {0};
		//得到之后自动清空。
        SNMP_ReadEventFromFlash(szReadBuf);
        int nWarnStart = 0 ;
		if( strcmp( szReadBuf,"")!=0)
		{
		   nWarnStart = atoi(szReadBuf);
		}
	
        g_isWarmStartTrapSent ++;
        g_isColdStartTrapSent ++;
        if( nWarnStart == 1 )
        {   
            SNMPA_Send_Comm_Trap(&warnStartTrapMsg);
			SNMPA_TRACE("send warn Start Trap Msg ....\n");
        }else
        {
	        SNMPA_Send_Comm_Trap(&coldStartTrapMsg);
		    SNMPA_TRACE("Get send cold Start Trap Msg ....\n");
        }
    }
}


/*-------------------------------------------------------------------------
 * send a alarm when configure change 
 */
void send_configureChangeAlarm()
{   
    netsnmp_variable_list *notification_vars = NULL;
	tw_AddTrapMsg(&notification_vars,&cfgChangeTrapMsg);
    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

int check_LinkUpAndLinkDown( void )
{
    int iret = -1;
    int iSokFd;
    ST_IfNet *pLinkCur, *pLinkStore, *pLinkTmp;
    
    iSokFd = socket(AF_INET, SOCK_DGRAM, 0 );
	if ( iSokFd < 0 ) 
	{
	    printf( "<net-snmp> open socket error.\n" );
	    return iret;
	}

    //dprints( "check interface is link up or link down.\n" );
    iret = RenewIfList( &ifLink_head ,&iSokFd );
    if( iret < 0 )
    {
        dprints( "renew interface link fail.\n" );
        return iret;
    }

	struct ifreq ifrq;
	ifrq.ifr_addr.sa_family = AF_INET;

    pLinkTmp = NULL;
    pLinkStore = NULL;
    pLinkCur = ifLink_head;
    while( pLinkCur != NULL )
    {
        if( pLinkCur->iNewFlag == 0 )
        {
            if( pLinkStore == NULL )
            {
                dprints( "Delete a node: %d %s\n", pLinkCur->ifIndex, pLinkCur->ifName );
                free( pLinkCur );
                pLinkCur = ifLink_head;
            }
            else
            {
                dprints( "Delete a node: %d %s\n", pLinkCur->ifIndex, pLinkCur->ifName );
                pLinkStore->pNext = pLinkCur->pNext;
                free( pLinkCur );
                pLinkCur = pLinkStore->pNext;
            }
            
            pLinkTmp = pLinkCur;
            while( pLinkCur != NULL )
            {
                pLinkCur->ifIndex--;
                pLinkCur = pLinkCur->pNext;
            }
            pLinkCur = pLinkTmp;
            continue;
        }
        strncpy( ifrq.ifr_name, pLinkCur->ifName, sizeof( ifrq.ifr_name ) );
        ifrq.ifr_name[ sizeof( ifrq.ifr_name ) - 1 ] = 0;
        if( ioctl( iSokFd, SIOCGIFFLAGS, &ifrq ) >= 0 )
        {   
            if( ( pLinkCur->ifFlag & IFF_RUNNING ) 
                != ( ifrq.ifr_ifru.ifru_flags & IFF_RUNNING ) )
            {
                pLinkCur->ifFlag = ifrq.ifr_ifru.ifru_flags;
                if( ( pLinkCur->ifFlag & IFF_RUNNING ) == 0 )
                    send_LinkDownAlarm( pLinkCur );
                else
                {   
                    dprints("ifrq.ifr_name = %s \n" , ifrq.ifr_name);
                    send_LinkUpAlarm( pLinkCur );
                } 
            }
        }
        //dprints( "--%d %s %d\n", pLinkCur->ifIndex, pLinkCur->ifName, pLinkCur->ifFlag );
        pLinkCur->iNewFlag = 0;
        pLinkStore = pLinkCur;
        pLinkCur = pLinkCur->pNext;
    }
    close( iSokFd );
    
    return 0;
}

/*-------------------------------------------------------------------------
 * send link up alarm
 */
void send_LinkUpAlarm( ST_IfNet *pInIf )
{
    netsnmp_variable_list *notification_vars = NULL;
    int iflag;
    
    dprints( "send link up trap.\n" );

    tw_AddTrapMsg(&notification_vars, &linkUpTrapMsg);
    
	snmp_varlist_add_variable(&notification_vars,
        IfIndex_oid, 
        OID_LENGTH( IfIndex_oid ),
        ASN_INTEGER,
        ( u_char* )&( pInIf->ifIndex ),
        sizeof( int ) );
    
    iflag = pInIf->ifFlag & IFF_UP ? 1 : 2;
    snmp_varlist_add_variable(&notification_vars,
        IfAdminStatus_oid, 
        OID_LENGTH( IfAdminStatus_oid ),
        ASN_INTEGER,
        ( u_char* )&iflag,
        sizeof( int ) );
    
    iflag = pInIf->ifFlag & IFF_RUNNING ? 1 : 2;
    snmp_varlist_add_variable(&notification_vars,
        ifOperStatus_oid, 
        OID_LENGTH( ifOperStatus_oid ),
        ASN_INTEGER,
        ( u_char* )&iflag,
        sizeof( int ) );

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

/*-------------------------------------------------------------------------
 * send link down alarm
 */
void send_LinkDownAlarm(ST_IfNet *pInIf )
{
    netsnmp_variable_list *notification_vars = NULL;
    int iflag;
    
    dprints( "send link down trap.\n" );

	tw_AddTrapMsg(&notification_vars, &linkDownTrapMsg);
    
	snmp_varlist_add_variable(&notification_vars,
        IfIndex_oid, 
        OID_LENGTH( IfIndex_oid ),
        ASN_INTEGER,
        ( u_char* )&( pInIf->ifIndex ),
        sizeof( int ) );
    
    iflag = pInIf->ifFlag & IFF_UP ? 1 : 2;
    snmp_varlist_add_variable(&notification_vars,
        IfAdminStatus_oid, 
        OID_LENGTH( IfAdminStatus_oid ),
        ASN_INTEGER,
        ( u_char* )&iflag,
        sizeof( int ) );
    
    iflag = pInIf->ifFlag & IFF_RUNNING ? 1 : 2;
    snmp_varlist_add_variable(&notification_vars,
        ifOperStatus_oid, 
        OID_LENGTH( ifOperStatus_oid ),
        ASN_INTEGER,
        ( u_char* )&iflag,
        sizeof( int ) );

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

/*-------------------------------------------------------------------------
 * get interface name list
 */
int RenewIfList( ST_IfNet **pInLink ,int *pSokFd)
{
    FILE *fp_if;
    int i, iret = -1;
    char sLine[ 256 ];
    char sIfName[ TRAPBASIC_STR_SIZE ];
    char *pTmp;
    unsigned long ultmp;
    ST_IfNet **ppLink, *pLinkTmp;

	struct ifreq ifrq;
	ifrq.ifr_addr.sa_family = AF_INET;

    ppLink = pInLink;
    fp_if = fopen( "/proc/net/dev", "r" );
    if( fp_if != NULL )
    {
        i = 1;
        while( fgets( sLine, 256, fp_if ) != NULL )
        {
            pTmp = strchr( sLine, ':' );
            if( pTmp != NULL )
            {
                *pTmp = '\0';

                if( sscanf( ( pTmp + 1 ), "%lu %lu %lu", &ultmp, &ultmp, &ultmp ) != 3 )
                    continue;
                
                pTmp = sLine;
                while( ( ( *pTmp == 0x20 ) || ( *pTmp == 0x09 ) ) && ( *pTmp != 0 ) )
                    ++pTmp;
                strncpy( sIfName, pTmp, TRAPBASIC_STR_SIZE );
                if(strstr(sIfName , "ppp" ) == NULL && strstr(sIfName ,"nas") == NULL)
                {
                   continue;
                }

                char* pcAddr = "" ;
				strncpy(ifrq.ifr_name, sIfName, sizeof(ifrq.ifr_name));
		        ifrq.ifr_name[ sizeof(ifrq.ifr_name)-1 ] = 0;
				if( ioctl( *pSokFd, SIOCGIFADDR, &ifrq ) >= 0 )
				{   
					struct in_addr ina;
					memcpy(&ina, &ifrq.ifr_addr.sa_data[2], sizeof(struct in_addr));
				    pcAddr = inet_ntoa(ina);
					if(strcmp(pcAddr,"") ==0 )
				    {  
				        printf( "pcAdd is NULL , sIfName = %s \n" ,sIfName );
				        continue ; 
				    }
				}

                int nIfFlag = -1 ;
				if( g_nLinkUp == 0 )
                {
                    nIfFlag = ioctl(*pSokFd, SIOCGIFFLAGS, &ifrq ) < 0 ? 0 : ifrq.ifr_ifru.ifru_flags;
                    if( IFF_RUNNING & nIfFlag == IFF_RUNNING )
                    {   
					    g_nLinkUp = 1 ;
						//link up 之前发送冷热启动
                        send_startTrap();
                    }
                }

                if( *ppLink == NULL )
                {
                    pLinkTmp = ( ST_IfNet * )malloc( sizeof( ST_IfNet ) );
                    pLinkTmp->ifIndex = i;
                    strncpy( pLinkTmp->ifName, sIfName, TRAPBASIC_STR_SIZE );
                    pLinkTmp->ifName[ TRAPBASIC_STR_SIZE - 1 ] = '\0';
					
					strncpy(pLinkTmp->ifAddr,pcAddr ,TRAPBASIC_STR_SIZE);
					pLinkTmp->ifAddr[TRAPBASIC_STR_SIZE -1] = '\0';

					if(nIfFlag == -1 )
					{
					   pLinkTmp->ifFlag = ioctl(*pSokFd, SIOCGIFFLAGS, &ifrq ) < 0 ? 0 : ifrq.ifr_ifru.ifru_flags;
					}else
					{
					   pLinkTmp->ifFlag = nIfFlag;
					}
					
                    if(strstr( pcAddr ,"." ) != NULL && ((pLinkTmp->ifFlag & IFF_RUNNING ) == IFF_RUNNING))
					{
					    g_nLinkUp = 1 ;
						//link up 之前发送冷热启动
                        send_startTrap();
					}

                    pLinkTmp->iNewFlag = 1;
                    pLinkTmp->pNext = NULL;
                    
                    ++i;
                    iret = i;
                    *ppLink = pLinkTmp;
                    ppLink = &pLinkTmp->pNext;
                }
                else
                {
                    pLinkTmp = *ppLink;
                    while( pLinkTmp != NULL )
                    {
                        if( strcmp( pLinkTmp->ifName, sIfName ) == 0 )
                        {
                            pLinkTmp->iNewFlag = 1;
                            if(strcmp(pLinkTmp->ifAddr,pcAddr) != 0)
                            {
								strncpy(pLinkTmp->ifAddr,pcAddr ,TRAPBASIC_STR_SIZE);
					            pLinkTmp->ifAddr[TRAPBASIC_STR_SIZE -1] = '\0';
                                //send  ip change alarm
                                send_wanManageIpChangeAlarm((unsigned char*)pcAddr);
                            } 
                            break;
                        }
                        pLinkTmp = pLinkTmp->pNext;
                    }

                    if( pLinkTmp == NULL )
                    {
                        pLinkTmp = *ppLink;
                        while( pLinkTmp->pNext != NULL )
                        {
                            pLinkTmp = pLinkTmp->pNext;
                        }
                        
                        pLinkTmp->pNext = ( ST_IfNet * )malloc( sizeof( ST_IfNet ) );
                        pLinkTmp->pNext->ifIndex = pLinkTmp->ifIndex + 1;
                        pLinkTmp = pLinkTmp->pNext;
                        strncpy( pLinkTmp->ifName, sIfName, TRAPBASIC_STR_SIZE );
                        pLinkTmp->ifName[ TRAPBASIC_STR_SIZE - 1 ] = '\0';

						strncpy(pLinkTmp->ifAddr,pcAddr ,TRAPBASIC_STR_SIZE);
					    pLinkTmp->ifAddr[TRAPBASIC_STR_SIZE -1] = '\0';

						if( nIfFlag == -1 )
						{
						    pLinkTmp->ifFlag = ioctl(*pSokFd, SIOCGIFFLAGS, &ifrq ) < 0 ? 0 : ifrq.ifr_ifru.ifru_flags;
						}else
						{
						    pLinkTmp->ifFlag = nIfFlag;
						}
					
                        if(strstr( pcAddr ,"." ) != NULL && ((pLinkTmp->ifFlag & IFF_RUNNING ) == IFF_RUNNING))
						{
						    g_nLinkUp = 1 ;
							//link up 之前发送冷热启动
                            send_startTrap();
						}

						pLinkTmp->iNewFlag = 1;
                        pLinkTmp->pNext = NULL;
                    }

                    iret = 0;
                    ppLink = pInLink;
                }
            }
        }
        fclose( fp_if );
    }
    
    return iret;
}

