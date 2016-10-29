/***************************************************************************
*     (c)2004-2009 Broadcom Corporation
*  
*  This program is the proprietary software of Broadcom Corporation and/or its licensors,
*  and may only be used, duplicated, modified or distributed pursuant to the terms and
*  conditions of a separate, written license agreement executed between you and Broadcom
*  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
*  no license (express or implied), right to use, or waiver of any kind with respect to the
*  Software, and Broadcom expressly reserves all rights in and to the Software and all
*  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
*  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
*  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.  
*   
*  Except as expressly set forth in the Authorized License,
*   
*  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
*  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
*  and to use this information only in connection with your use of Broadcom integrated circuit products.
*   
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
*  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
*  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO 
*  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES 
*  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, 
*  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION 
*  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF 
*  USE OR PERFORMANCE OF THE SOFTWARE.
*  
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS 
*  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR 
*  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR 
*  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF 
*  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT 
*  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE 
*  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF 
*  ANY LIMITED REMEDY.
 *
 * $brcm_Workfile: bdlna.c $
 * $brcm_Revision: 3 $
 * $brcm_Date: 9/30/09 4:36p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/core/src/bdlna.c $
 * 
 * 3   9/30/09 4:36p dliu
 * SW7405-2482: Move ConverTime function to here
 * 
 * 2   9/2/09 11:00a dliu
 * SW7405-2482: Add network interface to UPnP settings
* 
***************************************************************************/
#include "bdlna.h"

void BDlna_GetDefaultOpenSettings(BDlna_OpenSettings* pSettings)
{
    assert(pSettings);
    memset(pSettings, 0, sizeof(*pSettings));
    pSettings->port = 54321;
}

BDlna_Error BDlna_Initialize(const BDlna_OpenSettings* pSettings)
{
    BDlna_Error result;
    BUPnPOpenSettings upnpSettings;

    upnpSettings.ipAddress = NULL;
    upnpSettings.portNumber = pSettings->port;
    upnpSettings.networkInterface = pSettings->networkInterface;
    if ( (result = BUPnP_Initialize(&upnpSettings)) != UPNP_E_SUCCESS )
        return result;

    return BDLNA_E_SUCCESS;
}

void BDlna_Uninitialize(void)
{
    BUPnP_Close();
}

BDlna_Error BDlna_Start(void)
{
    return BUPnP_Start();
}

void BDlna_Stop(void)
{
    BUPnP_Stop();
}

int ConvertTime(const char* timeTarget)
{
    int hour, minute, second;

    if (timeTarget == 0)
        return 0;

    hour = minute = second = 0;

    hour = atoi(timeTarget);
    timeTarget = strchr(timeTarget, ':');
    if (timeTarget != NULL)
    {
        timeTarget++;
        minute = atoi(timeTarget);

        timeTarget = strchr(timeTarget, ':');
        if (timeTarget != NULL)
        {
            timeTarget++;
            second = atoi(timeTarget);
    if (second > 59)
        second = 59;
        }
    }

    return hour*3600 + minute * 60 + second;
}

