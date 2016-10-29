

#ifndef _PINMNGR_H_
#define _PINMNGR_H_

#include "common.h"

#include "new_msg.h"
#include "common_func.h"
#include "common_msg_handler.h"
#include "autoconf.h"

#if defined(__cplusplus)
extern "C" {
#endif

//#define PIN_DEBUG
#ifdef PIN_DEBUG
#define pinmngrdbg(format, args...) printf("[%d:%s()]==>"format, __LINE__, __FUNCTION__, ##args)
#else
#define pinmngrdbg(format, args...)
#endif

#define USB_DT_DEVICE_SIZE		18
#define PROC_BUS_USB "/proc/bus/usb"
#define DEV_REFRESH_TERM 240

#define ATTEMPT_PIN_TIME 1
#define ATTEMPT_PUK_TIME 2
#define ATTEMPT_LOCK_PIN_TIME 3
#define IFC_LARGE_LEN 266
#define IFC_SMALL_LEN 32
#define IFC_GIANT_LEN 1024
#define IFC_SMALL_LEN 32

#define PIN_CARD_INDEX 11

#define AT_KEYWORD_CPIN "CPIN"
#define AT_KEYWORD_CLCK "CLCK:"
#define AT_KEYWORD_PINTIME "CPIN"
#define AT_KEYWORD_PINTIME_DWM152 "QPVRF:"
#define AT_KEYWORD_PINTIME_DWM152V3 "ZPINPUK:"

#define AT_KEYWORD_PIN_OK "OK"

enum PIN_STATE
{
    UNKNOW_STATE = 0,
    CPIN_READY,
    CPIN_PIN,
    CPIN_PUK
};
#define FILE_PIN_PROTECT_UNLOCK "/var/pppd3g/pin_protect_unlock"

typedef struct st_pin_times_{
    int checktimes;
    int puktimes;
    int pintimes;
    int puk2times;
    int pin2times;
}st_pin_times;

enum{
	DATA_CARD_AT_GET_CPIN = 0,
	DATA_CARD_AT_GET_CLCK,
	DATA_CARD_AT_GET_PINTIME,
	DATA_CARD_AT_GET_PINTIME_DWM152,
	DATA_CARD_AT_GET_PINTIME_DWM152v3,
	DATA_CARD_AT_EN_PIN,
	DATA_CARD_AT_DIS_PIN,
	DATA_CARD_AT_CHANGE_PIN,
	DATA_CARD_AT_UNLCK_SIM_WITH_PIN,
	DATA_CARD_AT_UNLCK_WITH_PUK,
};

int pinmngr_get_CPIN_state(void);
int pinmngr_get_clck_stat(void);
int pinmngr_get_pin_att_time(int type);
int pinmngr_set_pin_enable(char *pin);
int pinmngr_set_pin_disable(char *pin);
int pinmngr_set_new_pin(char *old_pin, char *pin);
int pinmngr_unlock_sim_with_pin(char *pin);
int pinmngr_unlock_pin_with_puk(char *puk, char *pin);
int pinmngr_get_pin_attall_time(struct st_pin_times_  *pin_info);

#if defined(__cplusplus)
}
#endif
#endif
