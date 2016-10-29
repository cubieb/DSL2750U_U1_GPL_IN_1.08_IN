/******************************************************************************
 *    (c)2009 Broadcom Corporation
 * 
 * This program is the proprietary software of Broadcom Corporation and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.  
 *  
 * Except as expressly set forth in the Authorized License,
 *  
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *  
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO 
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES 
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, 
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION 
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF 
 * USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS 
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR 
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR 
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT 
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE 
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF 
 * ANY LIMITED REMEDY.
 *
 * $brcm_Workfile: supported_mimes.c $
 * $brcm_Revision: 5 $
 * $brcm_Date: 9/25/09 3:39p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/supported_mimes.c $
 * 
 * 5   9/25/09 3:39p ismailk
 * SW7405-3080: DMS IP Streaming (Live channel) Feature.
 * 
 * 4   7/30/09 4:53p ajitabhp
 * PR55165: FLV types support
 * 
 * 3   7/30/09 4:31p ajitabhp
 * PR55165: Support For Unsupported Files Added.
 * 
 * 2   6/30/09 7:46p ajitabhp
 * PR55165: MP4 was not parsed correctly.
 * 
 *****************************************************************************/
#include "bcm_types.h"
#include "mimes.h"

const struct mime_type_t MIME_Type_List[]= {
    /* Video files */
    { "asf",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/x-ms-asf:"},
    { "avc",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/avi:"},
    { "avi",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/avi:"},
    { "dv",    ITEM_CLS_UPNP_VIDEO, "http-get:*:video/x-dv:"},
    { "divx",  ITEM_CLS_UPNP_VIDEO, "http-get:*:video/avi:"},
    { "wmv",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/x-ms-wmv:"},
    { "mjpg",  ITEM_CLS_UPNP_VIDEO, "http-get:*:video/x-motion-jpeg:"},
    { "mjpeg", ITEM_CLS_UPNP_VIDEO, "http-get:*:video/x-motion-jpeg:"},
    { "mpeg",  ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg:"},
    { "mpg",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg:"},
    { "mpe",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg:"},
    { "mp2p",  ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mp2p:"},
    { "vob",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mp2p:"},
    { "mp2t",  ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mp2t:"},
    { "m2ts",  ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mp2t:"},
    { "m1v",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg:"},
    { "m2v",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg2:"},
    { "mpg2",  ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg2:"},
    { "mpeg2", ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg2:"},
    { "m4v",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mp4:"},
    { "m4p",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mp4:"},
    { "mp4",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mp4:"},
    { "mp4ps", ITEM_CLS_UPNP_VIDEO, "http-get:*:video/x-nerodigital-ps:"},
    { "ts",    ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg2:"},
    { "ogm",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg:"},
    { "mkv",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg:"},
    { "rmvb",  ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg:"},
    { "mov",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/quicktime:"},
    { "hdmov", ITEM_CLS_UPNP_VIDEO, "http-get:*:video/quicktime:"},
    { "qt",    ITEM_CLS_UPNP_VIDEO, "http-get:*:video/quicktime:"},
    { "bin",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg2:"},
    { "iso",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/mpeg2:"},
    { "flv",   ITEM_CLS_UPNP_VIDEO, "http-get:*:video/x-flv:"},

    /* Audio files */
    { "3gp",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/3gpp:"},
    { "aac",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/x-aac:"},
    { "ac3",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/x-ac3:"},
    { "aif",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/aiff:"},
    { "aiff", ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/aiff:"},
    { "at3p", ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/x-atrac3:"},
    { "au",   ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/basic:"},
    { "snd",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/basic:"},
    { "dts",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/x-dts:"},
    { "rmi",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/midi:"},
    { "mid",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/midi:"},
    { "mp1",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/mp1:"},
    { "mp2",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/mp2:"},
    { "mp3",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/mpeg:"},
    { "mp4",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/mp4:"},
    { "m4a",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/mp4:"},
    { "ogg",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/x-ogg:"},
    { "wav",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/wav:"},
    { "pcm",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/l16:"},
    { "lpcm", ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/l16:"},
    { "l16",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/l16:"},
    { "wma",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/x-ms-wma:"},
    { "mka",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/mpeg:"},
    { "ra",   ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/x-pn-realaudio:"},
    { "rm",   ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/x-pn-realaudio:"},
    { "ram",  ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/x-pn-realaudio:"},
    { "flac", ITEM_CLS_UPNP_AUDIO, "http-get:*:audio/x-flac:"},

    /* Images files */
    { "bmp",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/bmp:"},
    { "ico",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/x-icon:"},
    { "gif",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/gif:"},
    { "jpeg", ITEM_CLS_UPNP_PHOTO, "http-get:*:image/jpeg:"},
    { "jpg",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/jpeg:"},
    { "jpe",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/jpeg:"},
    { "pcd",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/x-ms-bmp:"},
    { "png",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/png:"},
    { "pnm",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/x-portable-anymap:"},
    { "ppm",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/x-portable-pixmap:"},
    { "qti",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/x-quicktime:"},
    { "qtf",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/x-quicktime:"},
    { "qtif", ITEM_CLS_UPNP_PHOTO, "http-get:*:image/x-quicktime:"},
    { "tif",  ITEM_CLS_UPNP_PHOTO, "http-get:*:image/tiff:"},
    { "tiff", ITEM_CLS_UPNP_PHOTO, "http-get:*:image/tiff:"},

    /* Playlist files */
    { "pls", ITEM_CLS_UPNP_PLAYLIST, "http-get:*:audio/x-scpls:"},
    { "m3u", ITEM_CLS_UPNP_PLAYLIST, "http-get:*:audio/mpegurl:"},
    { "asx", ITEM_CLS_UPNP_PLAYLIST, "http-get:*:video/x-ms-asf:"},

    /* Subtitle Text files */
    { "srt", ITEM_CLS_UPNP_TEXT, "http-get:*:text/srt:"}, /* SubRip */
    { "ssa", ITEM_CLS_UPNP_TEXT, "http-get:*:text/ssa:"}, /* SubStation Alpha */
    { "stl", ITEM_CLS_UPNP_TEXT, "http-get:*:text/srt:"}, /* Spruce */
    { "psb", ITEM_CLS_UPNP_TEXT, "http-get:*:text/psb:"}, /* PowerDivX */
    { "pjs", ITEM_CLS_UPNP_TEXT, "http-get:*:text/pjs:"}, /* Phoenix Japanim */
    { "sub", ITEM_CLS_UPNP_TEXT, "http-get:*:text/sub:"}, /* MicroDVD */
    { "idx", ITEM_CLS_UPNP_TEXT, "http-get:*:text/idx:"}, /* VOBsub */
    { "dks", ITEM_CLS_UPNP_TEXT, "http-get:*:text/dks:"}, /* DKS */
    { "scr", ITEM_CLS_UPNP_TEXT, "http-get:*:text/scr:"}, /* MACsub */
    { "tts", ITEM_CLS_UPNP_TEXT, "http-get:*:text/tts:"}, /* TurboTitler */
    { "vsf", ITEM_CLS_UPNP_TEXT, "http-get:*:text/vsf:"}, /* ViPlay */
    { "zeg", ITEM_CLS_UPNP_TEXT, "http-get:*:text/zeg:"}, /* ZeroG */
    { "mpl", ITEM_CLS_UPNP_TEXT, "http-get:*:text/mpl:"}, /* MPL */

    /* Miscellaneous text files */
    { "bup", ITEM_CLS_UPNP_TEXT, "http-get:*:text/bup:"}, /* DVD backup */
    { "ifo", ITEM_CLS_UPNP_TEXT, "http-get:*:text/ifo:"}, /* DVD information */

    /*Broadcast items*/
    { "mpeg", ITEM_CLS_UPNP_VIDEO_BROADCAST, "http-get:*:video/vnd.dlna.mpeg-tts:"},

    { NULL, NULL, NULL}
};
