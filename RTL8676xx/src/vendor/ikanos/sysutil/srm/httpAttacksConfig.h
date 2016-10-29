#ifndef __HTTPATTACKSCONFIG_H__
#define __HTTPATTACKSCONFIG_H__
#define HTTP_EXEC
//#include "apptypes.h"
typedef enum
{
    /* 0   */ ATTACK_BEGIN =0,
    /* 1   */ TCP_SYNC_ATTACK,
    /* 2   */ TCP_PORT_SCAN,
    /* 3   */ IP_SPOOF,
    /* 4   */ IP_ADDRESS_SWEEP_ATTACK,
    /* 5   */ DOS_ATTACK,
    /* 6   */ PING_OF_DEATH,
    /* 7   */ LAND_ATTACK,
    /* 8   */ ATTACK_NONE
}
E_ATTACKS;

typedef enum
{
   ATTACK_DISABLE =0 ,
   ATTACK_ENABLE
}E_STATUS;

VOID operAttack(E_ATTACKS, E_STATUS);  
E_STATUS checkStatus(E_ATTACKS);
#endif // __HTTPATTACKSCONFIG_H__
