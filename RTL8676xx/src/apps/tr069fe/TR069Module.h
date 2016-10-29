#ifndef __TR069MODULE_H__
#define __TR069MODULE_H__

/* -- INCLUDES -- */
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <ucontext.h>
#include <syslog.h>
#include <string.h>

#include "soapProcessor.h"
#include "TR069Errno.h"
#include "TR069StateMachine.h"
#include "http_client.h"
#include "cfg_interface.h"
#include "mp-qm_interface.h"


/* -- CONSTANTS -- */
#define C_CPEMAXENVELOPES 1
#define C_MAX_INFORMMSG_RETRIES 1
#define C_TRANSACTION_TIMEOUT 30
#define C_REQUEST_RESEND_TIME 5
#define C_FAULTCLIENT "Client"
#define C_FAULTSTR "CWMP Fault"
#define C_URLSEPERATOR ":"
#define C_DEFAULTTARGETURL "/"
#define COMM_TIMEOUT 20

#define RET_SUCCESS 0
#define RET_ERROR_1 1
#define RET_ERROR_2 2
#define RET_SEND_EMPTYMSG_6 6



/* -- FUNCTION PROTOTYPES -- */
void termination_handler (int signum, siginfo_t * siginfo, void *context);
void freeParamValueStr (parameterValueStruct * paramValStr);

#endif
