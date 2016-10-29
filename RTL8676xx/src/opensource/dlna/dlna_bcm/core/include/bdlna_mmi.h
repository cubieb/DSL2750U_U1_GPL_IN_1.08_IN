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
 * $brcm_Workfile: bdlna_mmi.h $
 * $brcm_Revision: 4 $
 * $brcm_Date: 9/15/09 3:31p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/core/include/bdlna_mmi.h $
 * 
 * 4   9/15/09 3:31p dliu
 * SW7405-2482: Add function definition
 * 
 * 3   7/28/09 4:56p dliu
 * PR55165: Add trickmode support
 * 
 * 2   7/7/09 5:21p dliu
 * PR55165: Add seek forward support
* 
***************************************************************************/
#ifndef BDLNA_MMI_H_
#define BDLNA_MMI_H_

#ifdef __cplusplus
extern "C" {
#endif

BDlna_MmiHandle BDlna_Mmi_Create(BDlna_MmiOpenSettings* pMmiSettings);

BDlna_Error BDlna_Mmi_PlaybackStart(BDlna_MmiHandle hMmi);

BDlna_Error BDlna_Mmi_SetUri(BDlna_MmiHandle hMmi,const char* uri, const char* protocolInfo);

BDlna_Error BDlna_Mmi_PlaybackPause(BDlna_MmiHandle hMmi);

BDlna_Error BDlna_Mmi_PlaybackStop(BDlna_MmiHandle hMmi);

void BDlna_Mmi_RegisterUriChangedCallback(BDlna_MmiHandle hMmi, BDlna_Mmi_UriChanged uriChangedCallback);

void BDlna_Mmi_RegisterStateChangedCallback(BDlna_MmiHandle hMmi, BDlna_Mmi_StateChanged stateChangedCallback);

void BDlna_Mmi_UnregisterStateChangedCallback(BDlna_MmiHandle hMmi, BDlna_Mmi_StateChanged stateChangedCallback);

void BDlna_Mmi_UnregisterUriChangedCallback(BDlna_MmiHandle hMmi, BDlna_Mmi_UriChanged uriChangedCallback);

BDlna_Error BDlna_Mmi_GetPlaybackStatus(BDlna_MmiHandle hMmi, uint32_t* elapsedTime, int* count);

const char* BDlna_Mmi_GetUri(BDlna_MmiHandle hMmi);

BDlna_MediaState BDlna_Mmi_GetState(BDlna_MmiHandle hMmi);

void BDlna_Mmi_SetMute(BDlna_MmiHandle hMmi, bool bMute);

void BDlna_Mmi_GetMute(BDlna_MmiHandle hMmi, bool* bMute);

void BDlna_Mmi_GetDefaultSettings(BDlna_MmiOpenSettings* pMmiSettings);

const char* BDlna_Mmi_GetProtocolInfo(BDlna_MmiHandle hMmi);

void BDlna_Mmi_Destroy(BDlna_MmiHandle hMmi);

BDlna_Error BDlna_Mmi_Seek(BDlna_MmiHandle hMmi, int seconds);

void BDlna_Mmi_SeekForward(BDlna_MmiHandle hMmi, int seconds);

void BDlna_Mmi_SeekBackward(BDlna_MmiHandle hMmi, int seconds);

bool BDlna_Mmi_SupportHttpStalling(BDlna_MmiHandle hMmi);

bool BDlna_Mmi_SupportByteSeek(BDlna_MmiHandle hMmi);


#ifdef __cplusplus
}
#endif

#endif /* BDLNA_DMP_H_ */

