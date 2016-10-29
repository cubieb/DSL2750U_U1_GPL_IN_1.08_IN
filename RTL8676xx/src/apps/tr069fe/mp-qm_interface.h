/*	Author:        	MultiTech Software Systems
**  Start Date:    	28-07-05
**  File Name: 		mp-qm_interface.h
**  Warning:       	-
**  Notes:          -
**  References:  	Technical Report from DSL Forum.
**  Modification History:
**/

//#include "tr069common.h"
//#include "cm_message.h"
//#include "msgparse.h"
#include "soapProcessor.h"
#include "TR069Errno.h"


#define DEVICE_SUMMARY              "InternetGatewayDevice.DeviceSummary"
#define	SPEC_VERSION				"InternetGatewayDevice.DeviceInfo.SpecVersion"
#define	HARDWARE_VERSION			"InternetGatewayDevice.DeviceInfo.HardwareVersion"
#define	SOFTWARE_VERSION			"InternetGatewayDevice.DeviceInfo.SoftwareVersion"
#define	PROV_CODE					"InternetGatewayDevice.DeviceInfo.ProvisioningCode"
#define	CONN_REQUEST_URL			"InternetGatewayDevice.ManagementServer.ConnectionRequestURL"
#define	PARAMETER_KEY				"InternetGatewayDevice.ManagementServer.ParameterKey"
//#define	DEFAULT_CONN_SERV			"InternetGatewayDevice.ManagementServer.X_TWSZ-COM_ManageWANConnectionPath"
#define	DEFAULT_CONN_SERV			"InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW"
#define	INFORM_SPEC_PARAM			"notificationChange"
#define	LAN_ETHERNET_MACADDRESS     "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddress"

#define RECV_SINGLE_RESP            0
#define RECV_MULTIPLE_RESP          1
#define	TRANSFER_PARAMETERS			4
#define RESPONSE_OK    				0
#define RESPONSE_TIMEOUT  			-1
#define RESPONSE_ERROR    			-2

#define	COUNT_ZERO					0
#define	COUNT_ONE					1
#define	COUNT_TWO					2
#define	COUNT_THREE					3
#define	COUNT_FOUR					4
#define	COUNT_FIVE					5
#define	COUNT_SIX					6
#define	COUNT_SEVEN					7
#define	COUNT_EIGHT					8


/* QUERIES START */
#define STAT_ACS_USERNAME "InternetGatewayDevice.ManagementServer.Username"
#define STAT_ACS_PASSWORD "InternetGatewayDevice.ManagementServer.Password"
#define STAT_CAFILENAME "InternetGatewayDevice.ManagementServer.X_TWSZ-COM_CAFilePath"
/* QUERIES END */

int SendXMLSetParameterValue (setParamValStr * reqSetParamStr, int *faultCode,
                   int *status, soapSetParameterValueFaultStr **ppstFaultList, BOOL *bReboot);

int SendXMLGetParameterValue (char **attrParaattrArrSiz, int attrArrSiz,
       int *detFaultCode, int iSpecMethod, parameterValueStruct **ppstList);

int SendXMLGetParameterNames (char *paramName, int nextLevel,
               int *detFaultCode, paramInfoStruct **ppstList);

int SendXMLInformEvent(int *faultCode, eventStruct **ppstList);

parameterValueStruct *SendXMLInformParameterList (int *detFaultValue);

int SendXMLAddDelObject (addDelObjStr * addObj, char *rpcMethodType, int *instanceNum, int *status, BOOL *bReboot);

int SendXMLFactoryReset ();
int SendXMLReboot (char *commandKey);
int SendXMLDownload (downloadStr * inputStruct, char *pszStartTime, char *pszCompleteTime, int *status);
int SendXMLUpload (uploadStr * inputStruct, char *pszStartTime, char *pszCompleteTime, int *status);
int SendXMLTransferComplete (transferCompleteStruct * transCompleteStruct);


int SendXMLSetParameterAttributes (paramAttrNameStruct * setParamValStr);

int SendXMLGetParameterAttributes (char **attrParaattrArrSiz, int attrArrSiz,
                      int *detFaultCode, getParamAttrNameStruct **ppstList);

int SendClearEvents_Cmd (unsigned short ulDstMID, unsigned short usMsgType);

int SendXMLScheduleDelay(char *DelaySeconds, char *CommandKey);

#ifdef CONFIG_TR069_CONNECTION_SET_HOLD
int SendTr069SessionEnd();
#endif

