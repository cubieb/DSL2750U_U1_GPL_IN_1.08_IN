#ifndef __SOAPPROCESSOR_H__
#define __SOAPPROCESSOR_H__ 1
#include "ILibParsers.h"
#include "SOAPParser.h"
#include<time.h>
#include<syslog.h>
#include "cm_utils.h"

/*general information used by all modules */
#define C_TAGSTART_BRACES "<"
#define C_TAGSTARTCLOSINGTAG_BRACE "</"
#define C_TAGEND_BRACE ">"
#define C_TOPTREENODE "InternetGatewayDevice"
char *getMsg (char *msgval, int len);
int getArraySize (struct ILibXMLNode *currentEnv, char *arrayType);
char *construct_EmptySoapMessage (char *RPCMethodName, soapHdrStruct * soapRespHdrStr, int *responseBufferLen);

struct paramNameStructure
{
  char *paramName;
  struct paramNameStructure *Next;
};
typedef struct paramNameStructure paramNameStruct;

#define C_PARAMETERLIST "ParameterList"
#define C_ARRAYEND "]\""
#define C_ARRAYTYPETAG "arrayType"

#define C_PARAMETERLISTARR "ParameterList "C_SOAPENC":arrayType=\""C_CWMP":ParameterValueStruct["

#define C_PARAMETERVALUESTRUCT "ParameterValueStruct"
#define C_PARAMETERVALUESTRNAME "Name"
#define C_PARAMETERVALUE "Value"
#define C_PARAMETERATTRTYPE " xsi:type=\"xsd:"
#define C_ENDQUOTE "\""
struct parameterValueStructure
{
  char *name;
  char *value;
  char *paramAttrType;
  struct parameterValueStructure *Next;
};
typedef struct parameterValueStructure parameterValueStruct;
char *intToStr (int conversionValue);

/* general information end */
/* START Factory reset*/
#define C_FACTORYRESET "FactoryReset"


/* END Factory Reset */

/*START Schedule Inform*/
#define C_SCHEDULEINFORM "ScheduleInform"


/* END Schedule Inform*/

/* START GetRPCEMethod */
#define C_GETRPCMETHOD "GetRPCMethods"
#define C_XSDSTRING "xsd:string"
#define C_NONXSDSTRING "string"

//#define C_METHODLISTARR "MethodList soapenc:arrayType=\"xsd:string["
#define C_METHODLISTARR "MethodList "C_SOAPENC":arrayType=\""C_XSDSTRING"["

#define C_METHODLIST "MethodList"


char *construct_GetRPCMethodSoapMessage (char *RPCMethodName, soapHdrStruct * soapRespHdrStr, int *responseBufferLen, int *faultCode);
/* END GetRPCEMethod*/

/* START Transfer Complete*/
#define C_TRANSFERCOMPLETE "TransferComplete"
#define C_TCFAULTCODE "FaultCode"
#define C_TCFAULTSTRING "FaultString"
#define C_TCFAULTSTRUCT "FaultStruct"
#define C_TCCOMMANDKEY "CommandKey"
#define C_TCSTARTTIME "StartTime"
#define C_TCCOMPLETETIME "CompleteTime"
//#define C_TCFAULTSTRUCTURE "FaultStruct soapenc:arrayType=\"cwmp:FaultStruct["
#define C_TCFAULTSTRUCTURE "FaultStruct"
#define C_TCFAULTSTRUCTURETAG "FaultStruct"
struct transCompFaultStructure
{
  int faultCode;
  char *faultString;
  struct transCompFaultStructure *Next;
};
typedef struct transCompFaultStructure transCompFaultStruct;
struct transferCompleteStructure
{
  char *commandKey;
  char *startTime;
  char *completeTime;
  transCompFaultStruct *TCFaultStr;
};
typedef struct transferCompleteStructure transferCompleteStruct;
char *construct_TrasferCompleteSoapMessage (transferCompleteStruct *
					    transferCompleteStr, char *RPCMethodName, soapHdrStruct * soapRespHdrStr, int *responseBufferLen);
/* END  Transfer Complete*/

/* START get/set Parameter Attributes*/
#define C_SETPARAMETERATTRIBUTES "SetParameterAttributes"
#define C_GETPARAMETERATTRIBUTES "GetParameterAttributes"
#define C_SETATTRPARAMETERLIST "ParameterList"
#define C_GETPARAMATTRSTRUCT "ParameterAttributeStruct"
#define C_SETPARAMATTRSTRUCT "SetParameterAttributesStruct"
#define C_SETPARAMATTRNAME "Name"
#define C_SETPARAMATTRNOTCH "NotificationChange"
#define C_SETPARAMATTRNOT "Notification"
#define C_SETPARAMATTRACCLTCH "AccessListChange"

#define C_SETPARAMATTRACCLT "AccessList "C_SOAPENC":arrayType=\""C_XSDSTRING"["

#define C_SETPARAMATTRACCLTTAG "AccessList"
#define C_SETPARAMATTRACCLTNAME "string"

#define C_PARAMETERATTRSTRUCT "ParameterList "C_SOAPENC":arrayType=\""C_CWMP":ParameterAttributeStruct["

struct setParamAttrNamesStructure
{
  char *paramName;
  int notificationChange;
  int notification;
  int accessListChange;
  int accesListArraySize;
  char **accessList;
  struct setParamAttrNamesStructure *Next;
};
typedef struct setParamAttrNamesStructure paramAttrNameStruct;

struct getParamAttrNameStructure
{
  char *paramName;
  int notification;
  int accesListArraySize;
  char **accessList;
  struct getParamAttrNameStructure *Next;
};
typedef struct getParamAttrNameStructure getParamAttrNameStruct;

paramAttrNameStruct *getSetParameterAttr (struct ILibXMLNode *currentEnv, char *RPCMethodName, int messageType);
void freeParamAttrStruct (paramAttrNameStruct * paramAttrNode);
char **getParamAttrNames (struct ILibXMLNode *currentEnv, char *RPCMethodName, int messageType, int *retArraySize);
char *construct_GetParamNamesAttrRespose (getParamAttrNameStruct * paramAttrStr, soapHdrStruct * soapRespHdrStr, int *responseBufferLen);

/* END get/set Parameter Attributes*/

/* START Reboot */
#define C_REBOOT "Reboot"
#define C_COMMANDKEY "CommandKey"
char *getRebootParameters (struct ILibXMLNode *currentEnv, char *RPCMethodName, int messageType);

/* END Reboot */

/* START get Parameter Names*/
#define C_GETPARAMETERNAMES "GetParameterNames"
#define C_GETPARAMETERPATH "ParameterPath"
#define C_NEXTLEVEL "NextLevel"
#define C_WRITABLE "Writable"
#define C_NAMEGETPARAMNAME "Name"

#define C_PARAMETERINFOLISTARR "ParameterList "C_SOAPENC":arrayType=\""C_CWMP":ParameterInfoStruct["

#define C_PARAMETERINFOSTRUCT "ParameterInfoStruct"
#define C_PARAMETERINFONAME "Name"
#define C_PARAMETERNAMEWRITABLE "Writable"

struct paramInfoStructure
{
  char *paramName;
  int writable;
  struct paramInfoStructure *Next;
};
typedef struct paramInfoStructure paramInfoStruct;
char *processGetParameterNames (struct ILibXMLNode *currentEnv, char *RPCMethodName, int messageType, int *nextLevel);
char *construct_GetParamNamesValueRespose (paramInfoStruct * paramInfoStr, soapHdrStruct * soapRespHdrStr, int *responseBufferLen);

/* END get Parameter Names */

/*START setParameterValue Structure and function prototypes*/
#define C_SETPARAMETERVALUE "SetParameterValues"
//#define C_PARAMLISTARRTYPE "cwmp:ParameterValueStruct"
#define C_PARAMLISTARRTYPE ":ParameterValueStruct"
struct setParameterValueStructure
{
  char *parameterKey;
  parameterValueStruct *paramValStr;
};
typedef struct setParameterValueStructure setParamValStr;

setParamValStr *getSetParamValAttr (struct ILibXMLNode *currentEnv, char *RPCMethodName, int messageType);
void freeSetParamValStr (setParamValStr * retSetParamValStr);
char *construct_SetParamValueRespose (int status, soapHdrStruct * soapRespHdrStr, int *responseBufferLen);

/*END setParameterValue Structure and function prototypes*/

/* Fault Parameters, Structure and function prototypes Start */
#define C_CWMPFAULT "cwmp:Fault"

struct setParameterValueFaultStr
{
  int faultCode;		// numeric fault code
  char *faultStr;		//human readable fault description
  char *parameterName;		//Name of the parameter in error
  struct setParameterValueFaultStr *Next;
};
typedef struct setParameterValueFaultStr soapSetParameterValueFaultStr;

struct faultDetailStr
{
  int faultCode;		// numeric fault code
  char *faultStr;		//human readable fault description
  soapSetParameterValueFaultStr *setParameterValueFaultStr;
  //this is filled only if error is due to setParameterValue RPC method
};
typedef struct faultDetailStr soapFaultDetailStr;

struct faultStructure
{
  char *faultCode;		// source of the  fault either client or server
  char *faultStr;		// must contain the string CWMP Fault
  soapFaultDetailStr *faultDetDesc;	//pointer to the fault details structure
};
typedef struct faultStructure soapFaultStructure;

void freeSetParamFaultStructure (soapSetParameterValueFaultStr * retFaultStr);
void freeFaultStructure (soapFaultStructure * retFaultStr);
soapFaultStructure *getFaultAttributes (struct ILibXMLNode *currentEnv, int messageType);
char *construct_FaultResponse (soapFaultStructure * faultStr, soapHdrStruct * soapRespHdrStr, int *responseBufferLen);
/* Fault Structure End */

/*Start Add Object structure, constants and function prototypes*/

struct addDelObjStructure
{
  char *objectName;
  char *parameterKey;
};
typedef struct addDelObjStructure addDelObjStr;

#define C_ADDOBJECT "AddObject"
#define C_DELETEOBJECT "DeleteObject"
#define C_OBJECTNAME "ObjectName"
#define C_PARAMETERKEY "ParameterKey"
#define C_INSTANCENUMBER "InstanceNumber"
#define C_STATUS "Status"

void freeAddDelObjStr (addDelObjStr * retAddDelObjectStr);
addDelObjStr *getAddDelObjectAttr (struct ILibXMLNode *currentEnv, char *RPCMethodName, int messageType);
char *construct_AddDelObjectRespose (char *RPCMethodName, unsigned int instanceNumber, int status, soapHdrStruct * soapRespHdrStr, int *responseBufferLen);

/*End Add Object structure*/

/*Start Download, constants and function prototypes*/

struct downloadStructure
{
  char *commandKey;
  char *fileType;
  char *URL;
  char *username;
  char *password;
  char *fileSize;
  char *targetFileName;
  char *delaySecs;
  char *successURL;
  char *failureURL;
};
typedef struct downloadStructure downloadStr;

#define C_UPLOAD "Upload" //wei added
#define C_DOWNLOAD "Download"
#define C_STARTTIME "StartTime"
#define C_COMPLETETIME "CompleteTime"
#define C_COMMANDKEY "CommandKey"
#define C_FILETYPE "FileType"
#define C_URL "URL"
#define C_USERNAME "Username"
#define C_PASSWORD "Password"
#define C_FILESIZE "FileSize"
#define C_TARGETFILENAME "TargetFileName"
#define C_DELAYSECOUNDS "DelaySeconds"
#define C_SUCCESSURL "SuccessURL"
#define C_FAILUREURL "FailureURL"

void freeDownloadStr (downloadStr * downloadStruct);
downloadStr *getDownloadAttrs (struct ILibXMLNode *currentEnv, char *RPCMethodName, int messageType);
char *construct_DownloadRespose (int status, char *startTime, char *completeTime, soapHdrStruct * soapRespHdrStr, int *responseBufferLen);

/*End Download structure*/

/*added by wei:Start Upload, constants and function prototypes*/
struct uploadStructure
{
  char *commandKey;
  char *fileType;
  char *URL;
  char *username;
  char *password;
  char *delaySecs;
};
typedef struct uploadStructure uploadStr;

void freeUploadStr (uploadStr * uploadStruct);
uploadStr *getUploadAttrs (struct ILibXMLNode *currentEnv, char *RPCMethodName, int messageType);
char *construct_UploadRespose (int status, char *startTime, char *completeTime, soapHdrStruct * soapRespHdrStr, int *responseBufferLen);

/*end <added by wei> End Upload structure*/

/*soap GetParameterVALUES start*/

#define C_GETPARAMETERVALUES "GetParameterValues"
#define C_PARAMETERNAMES "ParameterNames"
#define C_PARAMETERNAME "ParameterName"

//#define C_ARRAYTYPE C_XSDSTRING
#define C_ARRAYTYPE ":string"

#define C_ACCESSLISTELEM "string"
#define C_GETPARAMETERNAME "string"
char **getParameterValues (struct ILibXMLNode *currentEnv, char *RPCMethodName, int messageType, int *arraySize);
void freeArray (char **arrayFree, int arraySize);
char *construct_GetParamValueRespose (parameterValueStruct * a_ParameterValueStr, soapHdrStruct * soapRespHdrStr, int *responseBufferLen, int arrSiz);

/*soap GetParameterNames END*/

/* SOAP Inform message,Tags and structure defination */
#define C_INFORM "Inform"
#define C_INFORMRESPONSE "InformResponse"

#define C_MANUFACTURER "Manufacturer"
#define C_OUI "OUI"
#define C_PRODUCTCLASS "ProductClass"
#define C_SERIALNUMBER "SerialNumber"
struct deviceIdStructure
{
  char *manufacturer;
  char *OUI;
  char *productClass;
  char *serialNumber;
};
typedef struct deviceIdStructure deviceIdStruct;

#define C_TRANSFERCOMPLETERESPONSE "TransferCompleteResponse"
#define C_TRANSCOMPEVENT "7 TRANSFER COMPLETE"
#define C_EVENTSTRUCT "EventStruct"
#define C_EVENT "Event"

#define C_EVENTARR "Event "C_SOAPENC":arrayType=\""C_CWMP":EventStruct["

#define C_EVENTCODE "EventCode"
#define C_COMMANDKEY "CommandKey"
struct eventStructure
{
  char *eventCode;
  char *commandKey;
  struct eventStructure *Next;
};
typedef struct eventStructure eventStruct;

#define C_DEVICEID "DeviceId"
#define C_MAXENVELOPES "MaxEnvelopes"
#define C_CURRENTTIME "CurrentTime"
#define C_RETRYCOUNT "RetryCount"
struct informStructure
{
  deviceIdStruct *deviceIdStr;
  eventStruct *eventStr;
  unsigned int maxEnvelopes;
  time_t currentTime;
  unsigned int retrycount;
  parameterValueStruct *parameterValueStr;
};
typedef struct informStructure informStruct;

char *construct_InformMessage (informStruct * informStr, soapHdrStruct * soapRespHdrStr, int *responseBufferLen);
int getInformRespAttr (struct ILibXMLNode *currentEnv, char *RPCMethodName, int messageType);
/*End inform */

/* SOAP Tags and header defination */
#define C_SOAP_CWMP_VALUE "urn:dslforum-org:cwmp-1-0"

#define C_SOAP_ENVELOPE C_SOAPENV":Envelope"
#define C_SOAP_ENVELOPE_2 " "C_XMLNS":"C_SOAPENV"=\"http://schemas.xmlsoap.org/soap/envelope/\""
#define C_SOAP_ENVELOPE_3 " "C_XMLNS":"C_SOAPENC"=\"http://schemas.xmlsoap.org/soap/encoding/\""

#define C_SOAP_ENVELOPE_4 " "C_XMLNS":"C_CWMP"=\"urn:dslforum-org:cwmp-1-0\""
#define C_SOAP_ENVELOPE_5 " "C_XMLNS":"C_XSD"=\"http://www.w3.org/2001/XMLSchema\""
#define C_SOAP_ENVELOPE_6 " "C_XMLNS":"C_XSI"=\"http://www.w3.org/2001/XMLSchema-instance\""
#define C_STATUSTAG_0 "<Status>0</Status>"
#define C_STATUSTAG_1 "<Status>1</Status>"
#define C_NAMESPACETAG_SOAP C_SOAPENV":"

#define C_NAMESPACETAG_CWMP C_CWMP":"
#define C_NAMESPACETAG_XMLNS C_XMLNS":"
#define C_MUSTUNDERSTAND_SET "mustUnderstand=\"1\""

/* end SOAP Tags and header defination */

/* -- CONSTANTS -- */
#define C_BRACELEN strlen(C_TAGSTART_BRACES) + (2 * strlen(C_TAGEND_BRACE)) + strlen(C_TAGSTARTCLOSINGTAG_BRACE)
#define C_STARTTAGBRACELEN strlen(C_TAGSTART_BRACES) +  strlen(C_TAGEND_BRACE)
#define C_ENDTAGBRACELEN strlen(C_TAGSTARTCLOSINGTAG_BRACE) +  strlen(C_TAGEND_BRACE)
#define C_NAMESPACETAG_SOAPLEN strlen(C_NAMESPACETAG_SOAP)
#define C_NAMESPACETAG_CWMPLEN strlen(C_NAMESPACETAG_CWMP)
#define C_NOOFRPCMETHOD 13


#endif
