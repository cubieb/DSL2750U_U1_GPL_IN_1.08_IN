#ifndef __TR069STATEMACHINE_H__
#define __TR069STATEMACHINE_H__ 1
#include "soapProcessor.h"

/* -- CONSTANTS -- */
#define C_RPC "RPC"
#define C_ACSMAXENVELOPES "ACSMaxEnvelopes"

/* -- STRUCTURES --*/
struct RPCMethodResponseStatusStruct
{
  char *requestMethodName;
  char *sessionId;
  struct RPCMethodResponseStatusStruct *Next;
};
typedef struct RPCMethodResponseStatusStruct RPCMethodRespStateStr;

struct tr069StateMachineStruct
{
  int holdRequest;
  int noMoreRequest;
  int ACSMaxEnvelopes;
  RPCMethodRespStateStr *RPCMethodRespState;
};
typedef struct tr069StateMachineStruct tr069StateMachineStr;


/* -- FUNCTION PROTOTYPES -- */
int stateMachine_init ();
void stateMachine_close ();
int setState (char *requestMethodName, int varValue, char *cwmpSessionID, char *messageType);
int resetState (char *requestMethodName, char *messageType);
int getState (char *requestMethodName, char *messageType);
char *getSessionId (char *requestMethodName);
RPCMethodRespStateStr *getOutStandingRequests ();
char *getStateRPCName (char *sessionID);
#endif
