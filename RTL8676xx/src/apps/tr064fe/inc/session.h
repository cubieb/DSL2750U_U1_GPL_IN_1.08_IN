#ifndef SESSION_H
#define SESSION_H

/* session timeout should be in the range of 30-45 seconds */
#define TR64_SESSION_TIMEOUT      45  
#define TR64_MAX_WAIT_EVENT       10
#define TR64_SSID_LEN             16

typedef enum {
   WAIT_EVENT_RESOURCE_ERROR,
   WAIT_EVENT_OK
} sessionWaitEventStatus;


typedef enum {
   SESSION_ACTIVE,
   SESSION_EXPIRED
} sessionState;

typedef enum {
   CHANGE_APPLIED,
   REBOOT_REQUIRED,
} sessionConfigStatus;

typedef struct waitEvent
{
   struct http_connection *c;
   struct waitEvent *next;
} waitEvent, *pWaitEvent;

typedef struct waitEventList
{
   int count;
   pWaitEvent head;
   pWaitEvent tail;
} waitEventList, *pWaitEventList;

typedef struct sessionInfo
{
#ifdef UUID_TYPE
   uuid_t sessionId;              /* session ID in uuid_t format */
#else
   char sessionId[TR64_SSID_LEN]; /* session ID 16 hex octets format */
#endif
   sessionState state;    /* session state; active or expired  */
   time_t timer;          /* timer handle */
   time_t expires;        /* keep track of when session expires */
   sessionAuth auth;      /* digest authentication details */
   sessionConfigStatus configStatus;  /* status of session: changeApplied, rebootRequired */
   struct waitEventList eventList; /* event to be processed after session unlocks. */
} sessionInfo, *pSessionInfo;

int sidStrToUuid(char *sidStr, char *sid);
sessionWaitEventStatus sessionEnqueueWaitEvent(struct http_connection *c);
void createSession(char *sessionId);

#endif   // SESSION_H
