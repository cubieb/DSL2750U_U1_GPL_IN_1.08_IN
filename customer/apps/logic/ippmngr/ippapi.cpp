#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "psiapi.h"
#include "ippapi.h"
#include "syscall.h"

static PSI_HANDLE IPP_Info = NULL;
static int ippRefresh = FALSE;
static pIppConfig pIppInfo = NULL;
IPP_STATUS BcmIpp_refresh(void);

IPP_STATUS BcmIpp_init(void){
  UINT16 retLen = 0;

  if (IPP_Info == NULL) {
    if ( (IPP_Info = BcmPsi_appOpen( IPP_APP_ID ) ) == NULL ) {
      return IPP_INIT_FAILED;
    }
  }

  if (pIppInfo == NULL)
    pIppInfo = (pIppConfig)malloc(sizeof(ippConfig));

  memset( pIppInfo, 0, sizeof( ippConfig ) ); // Clear the NTP info field

  if (BcmPsi_objRetrieve( IPP_Info, IPP_CONFIG, (void*)pIppInfo, sizeof(ippConfig), &retLen ) == PSI_STS_OK) {
    if( pIppInfo->enabled )
       BcmIpp_serverRestart();
  }

  return IPP_OK;
}

int BcmIpp_isInitialized(void) {
  return (IPP_Info != NULL );
}



IPP_STATUS BcmIpp_Store( void ) {
  if( IPP_Info ) {
    BcmPsi_objRemove( IPP_Info, IPP_CONFIG );
    BcmPsi_objStore( IPP_Info, IPP_CONFIG, pIppInfo, sizeof( ippConfig ) );
  }

  return IPP_OK;
}

IPP_STATUS BcmIpp_SetVar( char *var, char *value ) {
  if( strcmp( var, "ipp_enabled" ) == 0 ) {
    if( strcmp( value, "1" ) == 0 ) {
      // if ipp state is changed from disabled to enabled
      // then need to refresh ipp hmtl page
      if ( pIppInfo->enabled == 0 )
         ippRefresh = TRUE;
      pIppInfo->enabled = 1;
    } else
      pIppInfo->enabled = 0;
  } else if ( strcmp(var, "ippRefresh") == 0 ) {
    ippRefresh = atoi(value);
  } else if ( strcmp( var, "ippMake") == 0 ) {
    strcpy( pIppInfo->make, value );
  } else if ( strcmp( var, "ippDev") == 0 ) {
    strcpy( pIppInfo->dev, value );
  } else if ( strcmp( var, "ippName") == 0) {
    strcpy( pIppInfo->name, value );
  } else
    strcpy( value, "" );
  return IPP_OK;
}

#define	SUPPORT_PRINT_INFO
#ifdef	SUPPORT_PRINT_INFO
#include "lptool.h" //printer
// printer. 2007.5.28
#define	PRINTER_FLAG	"printer."
#define	PRINTER_DEV	"/dev/printer0"	//"/dev/usb/lp0"
static void BcmIpp_Printer_GetVar(char *var, char *value)
{
	char *pMember;
	char *pDevId = NULL;
	char *pDevIdSeg = NULL;
	int segLen = 0;

	pMember = var + strlen(PRINTER_FLAG);
	pDevId = lpDevId_Pickup(PRINTER_DEV);
	if (NULL != pDevId) {
		if ( strcmp(pMember, "manufacturer") == 0 )
			pDevIdSeg = lpDevId_Seg_Offset(pDevId, DIS_MFG, &segLen);
		else if ( strcmp(pMember, "model") == 0)
			pDevIdSeg = lpDevId_Seg_Offset(pDevId, DIS_MDL, &segLen);
		else if ( strcmp(pMember, "cmd") == 0)
			pDevIdSeg = lpDevId_Seg_Offset(pDevId, DIS_CMD, &segLen);
		else if ( strcmp(pMember, "firmware") == 0)
			pDevIdSeg = lpDevId_Seg_Offset(pDevId, DIS_FWVER, &segLen);

		if (pDevIdSeg){
//			printf("%s[%d], devidSeg:%s\n", __FILE__, __LINE__, pDevIdSeg);
			strncpy(value, pDevIdSeg, segLen);
			value[segLen] = '\0';
		}else
			strcpy(value, "Unknown");

		lpDevId_Release(pDevId); /* release dev_id buffer */
		return;
	}

	strcpy(value, "");
	return;
}
#endif

IPP_STATUS BcmIpp_GetVar( char *var, char *value ) {
  BcmIpp_refresh();
  if( strcmp( var, "ipp_enabled" ) == 0 ) {
    if( pIppInfo->enabled ) {
       strcpy( value, "1" );
       ippRefresh = TRUE;
    }
    else {
      strcpy( value, "0" );
      ippRefresh = FALSE;
    }
  } else if ( strcmp(var, "ippRefresh") == 0 ) {
    sprintf( value, "%d", ippRefresh );
  } else if ( strcmp( var, "ippMake") == 0 ) {
    strcpy( value, pIppInfo->make );
  } else if ( strcmp( var, "ippDev") == 0 ) {
    strcpy( value, pIppInfo->dev );
  } else if ( strcmp( var, "ippName") == 0) {
    strcpy( value, pIppInfo->name );

#ifdef	SUPPORT_PRINT_INFO
  } else if ( strncmp( var, PRINTER_FLAG, strlen(PRINTER_FLAG) ) == 0) {
    BcmIpp_Printer_GetVar(var, value);
#endif

  } else
    strcpy( value, "" );
  return IPP_OK;
}

void BcmCfm_IPPserverRestart(pIppConfig pIPPCfg)
{
  FILE *fp;

  if (NULL == pIPPCfg)
      return;

  int pid = bcmGetPid("ippd");

  if( pid > 0 ) {
    kill( pid, SIGTERM );
  }

  if( pIPPCfg->enabled ) {
    // Write new conf file
    if( ( fp = fopen( "/var/printers.ini", "w") ) == NULL )
      return;

    if (pIPPCfg->dev[0] == '\0')
      strcpy(pIPPCfg->dev,IPP_PRINTER_DEV_DEFAULT);

    fprintf( fp, "[%s]\nmake=%s\ndevice=%s\n", pIPPCfg->name, pIPPCfg->make, pIPPCfg->dev );
    fclose( fp );
    bcmSystem("ippd /var/printers.ini &");
  }
}


void BcmIpp_serverRestart(void) {
  FILE *fp;
  int pid = bcmGetPid("ippd");

  if( pid > 0 ) {
    kill( pid, SIGTERM );
  }

  if( pIppInfo->enabled ) {
    // Write new conf file
    if( ( fp = fopen( "/var/printers.ini", "w") ) == NULL )
      return;

    if (pIppInfo->dev[0] == '\0')
      strcpy(pIppInfo->dev,IPP_PRINTER_DEV_DEFAULT);

    fprintf( fp, "[%s]\nmake=%s\ndevice=%s\n", pIppInfo->name, pIppInfo->make, pIppInfo->dev );
    fclose( fp );
    bcmSystem("ippd /var/printers.ini &");
  }
}

void BcmIpp_close( void ) {

  if (IPP_Info) {
    BcmPsi_appClose( IPP_Info );
  }
  if (pIppInfo)
    free(pIppInfo);

  return;
}

IPP_STATUS BcmIpp_refresh(void)
{
  UINT16 retLen = 0;
  BcmIpp_close();
  IPP_Info = NULL;
  pIppInfo = NULL;

  if (IPP_Info == NULL){
    if ( (IPP_Info = BcmPsi_appOpen( IPP_APP_ID ) ) == NULL ) {
      return IPP_INIT_FAILED;
    }
  }

  if (pIppInfo == NULL)
    pIppInfo = (pIppConfig)malloc(sizeof(ippConfig));

  if (pIppInfo == NULL)
      return IPP_OK;

  memset( pIppInfo, 0, sizeof( ippConfig ) ); // Clear the NTP info field

  if (BcmPsi_objRetrieve( IPP_Info, IPP_CONFIG, (void*)pIppInfo, sizeof(ippConfig), &retLen ) == PSI_STS_OK)
     return IPP_OK;

  return IPP_INIT_FAILED;
}
