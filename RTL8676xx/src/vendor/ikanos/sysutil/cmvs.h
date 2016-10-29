/* Copyright (C) 2006 Ikanos Communications 
 * See IKANOS_PROP_LICENSE.txt for license information. */



/*****************************************************************************
 * File Name     : cmvs.h
 * Author        : 
 * Description   :
 *****************************************************************************/
typedef enum 
{
	RET_SUCCESS,
	RET_ERROR,
	RET_ERROR_UNKNOWN_CMV,
	ERT_ERROR_SOCKET_CREATE,
	RET_ERROR_SOCK_REQ,
	RET_ERROR_ME_CMVFAILED,
	RET_ERROR_TIMEOUT,
	RET_ERROR_NOTSUPPORTED,
	RET_ERROR_PREV_HDLCMSG_PENDING,
	RET_ERROR_INVALID_HDLC_CMD_ISSUED,
	RET_ERROR_HDLCTIMEOUT_FROM_FW,

} ADSL_ERRORS;

/* new cmv data structure */
typedef struct {
int				numberOfCMV;
union {
	struct {
			unsigned char	cmv[4];
			}eagle;
	struct {
			unsigned short	group;
			unsigned short	address;
			}eagleplus;
	}u_cmv;
union {
		unsigned short	offset;	 /* eagle */
		unsigned short	index;	 /* eagleplus */
}u;
unsigned short	pad;
unsigned int	data;
}cmvHeader;

typedef struct {
union {
	struct {
			unsigned char	cmv[4];
			}eagle;
	struct {
			unsigned short	group;
			unsigned short	address;
			}eagleplus;
	}u_cmv;
union {
		unsigned short	offset;	 /* eagle */
		unsigned short	index;	 /* eagleplus */
}u;
unsigned short	pad;
unsigned int	data;
}cmvCommand;
