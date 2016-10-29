/* Copyright (C) 2006 Ikanos Communications
 * See IKANOS_PROP_LICENSE.txt for license information. */


/****************************************************************************
 * File Name     : srm.h
 * Author        : 
 * Description   : This file consists of structure and macro definitions for
 *                 saving the configuration to flash and restoring the 
 *                 configuration from flash.
 ****************************************************************************/

#ifndef SRM_H
#define SRM_H

# include "../../adiftpd/appdefines.h"
# include "../../adiftpd/apptypes.h"
# include "gsrm.h" 

# define DEBUG        0

#if DEBUG
#define SRMPRINTF(format, args...) printf("SRM: " format, ##args)
#else
#define SRMPRINTF(format, args...)
#endif


/* Size of RAM or Flash file name */

#define FILE_NAME_SIZE   35

/* Type of module */

#define MODULE_APPLICATION 1
#define MODULE_KERNEL      2


/* SRM Module registration structure */

typedef struct srmRegister_t
{
    INT16  iType;                   /*
                                     * Indicates whether the module is 
                                     * application or kernel module
                                     * Value = MODULE_APPLICATION for 
                                     * application modules
                                     * Value = MODULE_KERNEL for Kernel modules 
                                     * Value = MODULE_USER for 
                                     * ifconfig, routeconfig modules
                                     */

    INT32  iModuleId;			/* Unique Id for identifying each module */
    BOOL   bUpDownStatus;      		/* Demon is to be executed or not */
    UCHAR  aucRamFileName[FILE_NAME_SIZE];     
	/* Specifies the path and name of the file stored in RAM */
    UCHAR  aucFlashFileName[FILE_NAME_SIZE];   
	/* Specifies the path and name of the file stored in flash*/
    BOOL   bStatus; /* Indicates whether the module has been saved or not */
    UCHAR  buffer[5000];
    int (*saveFn)(srmDataBuff_t *);
    int (*restoreFn)(srmDataBuff_t *);

    char processName[10];

    INT16  iBinaryFileLen;
     
    struct srmRegister_t *next; 	/* Pointer to the next registration structure */
} srmRegister_t;

/* Structure used for passing the registration info to the message queue */

typedef struct
{
    UCHAR         ucCommand;  /* Can take values 1 - 8 */
    srmRegister_t params;
} srmCommand_t;

/* The type of commands */

#define COMMAND_SAVE       1
#define COMMAND_ERASE      2
#define COMMAND_REG        3
#define COMMAND_RESTORE    4
#define SAVE_SUCCESS       5
#define SAVE_FAILURE       6
#define ERASE_SUCCESS      7
#define ERASE_FAILURE      8

#define SRM_NOT_STORED	   0
#define SRM_STORED	   1

#define DEVICE_NET	   1
#define DEVICE_CHAR        2

INT16 srmUpdateConfigFile(srmRegister_t *pSrmRegInfoPtr);
INT16 srmReadConfigFile(srmRegister_t **pSrmRegInfoPtr);
INT16 srmSendIoctl(srmRegister_t *, INT16 iCommand,srmDataBuff_t *pSrmData);
INT16 srmReadKernelInfoFromFile(srmRegister_t *pSrmInfo, srmDataBuff_t *pSrmData);
INT16 srmReadAppInfoFromFile(srmRegister_t *pSrmInfo);
INT16 srmUpdateKernelInfoToFile(srmRegister_t *pSrmInfo, srmDataBuff_t *pSrmData
);
INT16 srmUpdateAppInfoToFile(srmRegister_t *pSrmInfo);
INT16 srmAddNode(srmRegister_t *pSrmInfo, srmRegister_t **pSrmRegInfoPtr);

extern int br_save(srmDataBuff_t *srmbuf);
#endif //SRM_H
