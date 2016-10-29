#ifndef __TBS_MESSAGE_H__
#define __TBS_MESSAGE_H__

typedef struct tag_ST_MSGHEAD
{
    unsigned short usSrcMID;
    unsigned short usDstMID;
    unsigned long  ulMsgID;
    unsigned short usMsgType;
    unsigned short usReserve;
    unsigned long  ulBodyLength;
}ST_MSGHEAD;

typedef struct tag_ST_MSG
{
    ST_MSGHEAD  stMsgHead;
    char        szMsgBody[0];
}ST_MSG;

enum
{

    MSG_BLP_BASE        = 0x0500,

    MSG_BLP_DEVCTL_ADD,
    MSG_BLP_DEVCTL_DEL,
    MSG_BLP_DEVCTL_QUERY,
    MSG_BLP_DEVCTL_CONFIG
} ;


#define TBS_MSG_ACK_TYPE(type)  (type | 0x8000)

#define MSG_BLP_DEVCTL_ADD_ACK              TBS_MSG_ACK_TYPE(MSG_BLP_DEVCTL_ADD)
#define MSG_BLP_DEVCTL_DEL_ACK              TBS_MSG_ACK_TYPE(MSG_BLP_DEVCTL_DEL)
#define MSG_BLP_DEVCTL_QUERY_ACK            TBS_MSG_ACK_TYPE(MSG_BLP_DEVCTL_QUERY)
#define MSG_BLP_DEVCTL_CONFIG_ACK           TBS_MSG_ACK_TYPE(MSG_BLP_DEVCTL_CONFIG)

#endif
