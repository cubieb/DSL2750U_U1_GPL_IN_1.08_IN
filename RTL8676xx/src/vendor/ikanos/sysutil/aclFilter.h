/****************************************************************************************
 *											*
 * Copyright (C) 2007 Ikanos Communications Inc. All rights reserved			*
 *											*
 * The information and source code contained herein is the exclusive property		*
 * of Ikanos Communications and may not be disclosed, examined or reproduced in whole	*
 * or in part without the explicit written authorization from Ikanos Communications.	*
 *											*
 ****************************************************************************************/

/****************************************************************************************
 *                                                                                      *
 * File name    : aclFilter.h								*
 * Author       : Ikanos								*
 * Version      : 									*
 * Date		: 27th of Aug 2007							*	
 * Description  : Declaration/Defination for ACL filter rules. 				*
 *											*
 ****************************************************************************************/
#include "aclapidef.h"


/********************************************************************************
 * Function Name        :checkInterfaceName                                     *
 * Return value         :0 -> success -1 -> Error                               *
 * Input Parameters     :Interface Name                                         *
 * Output Parameters    :Interface Number                                       *
 * Result               :                                                       *
 * Description          :Check the interface name and fills the iFaceNum with   *
 *                       with appropriate interface number.                     *
 ********************************************************************************/

static int checkInterfaceName(char * const iFaceName,unsigned long *iFaceNum)
{
        if(strstr(iFaceName,"eth0") != NULL)
        {
                *iFaceNum = 0;
                return 0;
        }
        if(strstr(iFaceName,"eth1") != NULL)
        {
                *iFaceNum = 1;
                return 0;
        }
        if(strstr(iFaceName,"wlan") != NULL)
        {
                *iFaceNum = 3;
                return 0;
        }

        return -1;
}

/********************************************************************************
 * Function Name        :createAclDeviceFile                                    *
 * Return value         :                                                       *
 * Input Parameters     :Device name, device major number                       *
 * Output Parameters    :None                                                   *
 * Result               :                                                       *
 * Description          :This function create specified device file.            *
 *                                                                              *
 ********************************************************************************/
static void createAclDeviceFile(char *name, int majorNum)
{
        char cmd[50];
        sprintf(cmd, "mknod /dev/%s c %d 0",name, majorNum);
        system(cmd);
}


/********************************************************************************
 * Function Name        :aclIOCTL                                               *
 * Return value         :0 -> success -1 -> Error                               *
 * Input Parameters     :command type and stucture data                         *
 * Output Parameters    :                                                       *
 * Result               :                                                       *
 * Description          :It opens acl_ap's device file and execute specified    *
 *                       ioctl.                                                 *
 ********************************************************************************/
static int aclIOCTL(int cmd_type,aclRule_t *data)
{
        int fd = -1, ret = -1;

        fd = open(ACL_DRIVER_NAME, 0);
        if(fd < 0)
        {
                printf("\nacll2: Unable to open %s device file...\n",ACL_DRIVER_NAME);
                exit(0);
        }

        ret = ioctl(fd, cmd_type, data);
        if(ret == -1)
                printf("\nacll2: Error while executing IOCTL...\n");
        close(fd);

        return ret;
}




