#ifndef __SOAPPARSER_H__
#define __SOAPPARSER_H__ 1
#define PRINT 0
/* -- INCLUDES -- */
#include <stdio.h>
//#include "tr069common.h"
#include "ILibParsers.h"
#include "cm_utils.h"

/* -- CONSTANTS -- */
//SOAP constants
#define REQUEST_CODE 1
#define RESPONSE_CODE 2
#define FAULT_CODE 3

#define C_XMLNS "xmlns"
#define C_XSI "xsi"
#define C_XSD "xsd"

#define C_SOAPENV "SOAP-ENV"
#define C_SOAPENC "SOAP-ENC"

#define C_CWMP "cwmp"

#define C_ENCODINGSTYLE C_SOAPENC

#define C_SOAP_VALUE "http://schemas.xmlsoap.org/soap/envelope/"
#define C_ENCODINGSTYLE_VALUE "http://schemas.xmlsoap.org/soap/encoding/"
#define C_CWMP_VALUE "urn:dslforum-org:cwmp-1-0"
#define C_XSD_VALUE "http://www.w3.org/2001/XMLSchema"
#define C_XSI_VALUE "http://www.w3.org/2001/XMLSchema-instance"
#define C_ENVELOPE "Envelope"
#define C_HEADER "Header"
#define C_HEADERID "ID"
#define C_HOLDREQUESTS "HoldRequests"
#define C_NOMOREREQUESTS "NoMoreRequests"
#define C_BODY "Body"
#define C_RESPONSE "Response"
#define C_MUSTUNDERSTAND "mustUnderstand"

//Fault constant
#define C_FAULT "Fault"
#define C_DETAIL "detail"
#define C_FAULTCODE "faultcode"
#define C_FAULTSTRING "faultstring"
#define C_DETAILFAULTCODE "FaultCode"
#define C_DETAILFAULTSTRING "FaultString"
#define C_SETPARAMETERVALUESFAULT "SetParameterValuesFault"
#define C_PARAMETERNAME "ParameterName"

/* -- STRUCTURES & STRUCTURE VARIABLES-- */
struct soapHdrStructure
{
  char *hdrTagName;
  char *hdrTagValue;
  int mustUnderstand;
  struct soapHdrStructure *next;
};

typedef struct soapHdrStructure soapHdrStruct;

/* -- FUNCTION PROTOTYPES -- */
char *soapParser (char *buffer, int bufferLen);
struct ILibXMLNode *parseSOAPMessage (char *soapMsg, int soapMsgLen);
int validateSOAPMessage (struct ILibXMLNode *xmlNode, int maxEnvelope);
int getNoOfEnvelopes (struct ILibXMLNode *xmlNode);
struct ILibXMLNode *getEnvelope (struct ILibXMLNode *xmlNode, int envelopeNum);
soapHdrStruct *processSoapHeader (struct ILibXMLNode *currentEnv);
void freeSoapHdr (soapHdrStruct * soapHdrPtr);
int isHeader (struct ILibXMLNode *currentEnv);
char *getRPCMethodName (struct ILibXMLNode *currentEnv, int *RPCMethodNameLen, int *messageType);
struct ILibXMLNode *getSoapNode (struct ILibXMLNode *currentEnv, char *tagName);
char *getTagValue (struct ILibXMLNode *soapNode, int *valLen, int isEstimated);
void printToSyslog (char *errorLevel, char *fileName, char *methodName, char *errorStr);

#endif
