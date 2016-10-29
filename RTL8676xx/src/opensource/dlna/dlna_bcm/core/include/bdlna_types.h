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
 * $brcm_Workfile: bdlna_types.h $
 * $brcm_Revision: 6 $
 * $brcm_Date: 9/21/09 5:29p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/core/include/bdlna_types.h $
 * 
 * 6   9/21/09 5:29p dliu
 * SW7405-2482: Add support for LPCM files
 * 
 * 5   9/15/09 4:32p dliu
 * SW7405-2482: Add SeekPause function defintion
 * 
 * 4   7/28/09 4:54p dliu
 * PR55165: Add debug support to DLNA
 * 
 * 3   7/8/09 12:15p dliu
 * PR55165: Add BMedia_Close function
 * 
 * 2   7/7/09 5:22p dliu
 * PR55165: Add DTCP-IP support
* 
***************************************************************************/
#ifndef BDLNA_TYPES_H_
#define BDLNA_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum BDlna_MediaFormat
{
    BDlna_MediaFormat_eProbe,
    BDlna_MediaFormat_eMP3,
    BDlna_MediaFormat_eLPCM
} BDlna_MediaFormat;

typedef enum BDlna_MediaState
{
    BDlna_MediaState_eNoMediaPresent,
    BDlna_MediaState_eStopped,
    BDlna_MediaState_ePlaying,
    BDlna_MediaState_ePausedPlay,
    BDlna_MediaState_eTransitioning,
    BDlna_MediaState_eRecording,
    BDlna_MediaState_ePausedRecording
} BDlna_MediaState;

typedef enum BDlna_MediaLinkProtectionMode
{
    BDlna_MediaLinkProtectionMode_eNone,
    BDlna_MediaLinkProtectionMode_eDtcpIp
} BDlna_MediaLinkProtectionMode;

typedef struct BDlna_MediaStatus 
{
    int position;
    int first;
    int last;

} BDlna_MediaStatus;

/**
Summary:
Function prototype used by BDlna_CallbackDesc.
**/
typedef void (*BDlna_Callback)(void *context, int param);  

typedef struct BDlna_CallbackDesc {
    BDlna_Callback callback; /* Function pointer */
    void *context;           /* First parameter to callback function. */
    int param;               /* Second parameter to callback function. */
} BDlna_CallbackDesc;

typedef struct BMedia* BMediaHandle;

typedef struct BMedia_Settings {
    void* displayHandle;
    BDlna_CallbackDesc endOfStreamCallback;
    unsigned int preChargeTime;  /* how many seconds to precharge the buffers before display begings */
    int initialSeekTime; /* how many seconds to seek into the media before starting the initial playback */
    bool disableRangeHeader; /* If server specifies it doesn't support Range header, than the http client must not send it */

    BDlna_MediaFormat format;
    unsigned int sampleRate; /* used to pass LPCM info */
    unsigned char channels; /* used to pass LPCM info */

    /* security related */
    BDlna_MediaLinkProtectionMode lpMode; 

} BMedia_Settings;

typedef struct BMedia_AudioSettings
{
    int32_t leftVolume;
    int32_t rightVolume;
    bool muted;
} BMedia_AudioSettings;

BMediaHandle BMedia_Open(BMedia_Settings* pSettings);

int BMedia_Play(BMediaHandle bMedia);

void BMedia_Stop(BMediaHandle bMedia);

void BMedia_Pause(BMediaHandle bMedia);

void BMedia_SeekPause(BMediaHandle bMedia);

void BMedia_GetStatus(
    BMediaHandle bMedia,
    BDlna_MediaStatus* status
    );

void BMedia_SetUri(BMediaHandle bMedia, const char* uri);

void BMedia_GetAudioSettings(BMediaHandle hMedia, BMedia_AudioSettings* pSettings);

void BMedia_SetAudioSettings(BMediaHandle hMedia, BMedia_AudioSettings* pSettings);

void BMedia_GetSettings(BMediaHandle hMedia, BMedia_Settings *pSettings);

void BMedia_SetSettings(BMediaHandle hMedia, BMedia_Settings *pSettings);

int BMedia_PerformDtcpAke(BMediaHandle hNexus, const char* dtcpHost, int dtcpPort);

void BMedia_Seek(BMediaHandle hNexus, int seekTimeSeconds);

void BMedia_SeekBackward(BMediaHandle hNexus, int seekTimeSeconds);

void BMedia_SeekForward(BMediaHandle hNexus, int seekTimeSeconds);

void BMedia_Close(BMediaHandle hNexus);

#ifdef __cplusplus
}
#endif

#endif /* BDLNA_TYPES_H_ */

