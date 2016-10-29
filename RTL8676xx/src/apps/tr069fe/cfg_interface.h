/*	Author:        	MultiTech Software Systems
**  Start Date:    	28-07-05
**  File Name: 		cfg_interface.h
**  Warning:       	-
**  Notes:          -
**  References:  	Technical Report from DSL Forum.
**  Modification History:
**/
#ifndef __CFG_INTERFACE_H__
#define __CFG_INTERFACE_H__ 1
#include "soapProcessor.h"

char *getACSURL ();
char *getManufacturer ();
char *getOUI ();
char *getProductClass ();
char *getSerialNumber ();
parameterValueStruct *getParameterList ();
char *getNewSessionId (void);
int addObject_Request (addDelObjStr * addObj, int *instanceNum);
soapSetParameterValueFaultStr *setParamValue_Request (setParamValStr * reqSetParamStr, int *faultCode, int *status, BOOL *bReboot);
parameterValueStruct *getParamValue_Request (char **attrParamValue, int attrArrSiz, int *detFaultCode);
paramInfoStruct *getParamNames_Request (char *paramName, int nextLevel, int *detFaultCode);
int setParamAttr_Request (paramAttrNameStruct * paramattrStr);
getParamAttrNameStruct *getParamAttr_Request (char **attrparam, int arraSize, int *detFaultCode);
int getDownloadType (downloadStr * downloadStruct, char *pszStartTime, char *pszCompleteTime);
int getUploadType (uploadStr * uploadStruct, char *pszStartTime, char *pszCompleteTime);
transferCompleteStruct *getTCDS ();
int sendRebootSignal (char *commandKey);
int SendScheduleDelay(char *DelaySeconds, char *CommandKey);
int factoryResetSignal ();
int addDelObject_Request (addDelObjStr * addObj, int *instanceNum, char *req, int *status, BOOL *bReboot);
char *sendNGetUsername (int *faultCode);
char *sendNGetPassword (int *faultCode);


#endif
