#############################################################################
#    (c)2009 Broadcom Corporation
# 
# This program is the proprietary software of Broadcom Corporation and/or its licensors,
# and may only be used, duplicated, modified or distributed pursuant to the terms and
# conditions of a separate, written license agreement executed between you and Broadcom
# (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
# no license (express or implied), right to use, or waiver of any kind with respect to the
# Software, and Broadcom expressly reserves all rights in and to the Software and all
# intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
# HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
# NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.  
#  
# Except as expressly set forth in the Authorized License,
#  
# 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
# secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
# and to use this information only in connection with your use of Broadcom integrated circuit products.
#  
# 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
# AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
# WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO 
# THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES 
# OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, 
# LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION 
# OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF 
# USE OR PERFORMANCE OF THE SOFTWARE.
# 
# 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS 
# LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR 
# EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR 
# USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF 
# THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT 
# ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE 
# LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF 
# ANY LIMITED REMEDY.
#
# $brcm_Workfile: bdms.inc $
# $brcm_Revision: 1 $
# $brcm_Date: 7/24/09 10:58a $
# 
#############################################################################

#Identify this library
#
B_LIBS += BDLNA_DMS

#
#Define source files in this library
#
BDLNA_DMS_SOURCES =   \
    	$(DMS_TOP)/Dependencies/LibDLNA/audio_aac.c \
        $(DMS_TOP)/Dependencies/LibDLNA/audio_ac3.c \
        $(DMS_TOP)/Dependencies/LibDLNA/audio_amr.c \
        $(DMS_TOP)/Dependencies/LibDLNA/audio_atrac3.c \
        $(DMS_TOP)/Dependencies/LibDLNA/audio_g726.c \
        $(DMS_TOP)/Dependencies/LibDLNA/audio_lpcm.c \
        $(DMS_TOP)/Dependencies/LibDLNA/audio_mp2.c \
        $(DMS_TOP)/Dependencies/LibDLNA/audio_mp3.c \
        $(DMS_TOP)/Dependencies/LibDLNA/audio_wma.c \
        $(DMS_TOP)/Dependencies/LibDLNA/av_mpeg1.c \
        $(DMS_TOP)/Dependencies/LibDLNA/av_mpeg2.c \
        $(DMS_TOP)/Dependencies/LibDLNA/av_mpeg4_part10.c \
        $(DMS_TOP)/Dependencies/LibDLNA/av_mpeg4_part2.c \
        $(DMS_TOP)/Dependencies/LibDLNA/av_wmv9.c \
        $(DMS_TOP)/Dependencies/LibDLNA/containers.c \
        $(DMS_TOP)/Dependencies/LibDLNA/image_jpeg.c \
        $(DMS_TOP)/Dependencies/LibDLNA/image_png.c \
        $(DMS_TOP)/Dependencies/LibDLNA/profiles.c \
        $(DMS_TOP)/buffer.c \
        $(DMS_TOP)/CDS.c \
        $(DMS_TOP)/CDS_Iface.c \
        $(DMS_TOP)/CMS.c \
        $(DMS_TOP)/CMS_Iface.c \
        $(DMS_TOP)/dirdbase.c \
        $(DMS_TOP)/Http_streamer.c \
        $(DMS_TOP)/supported_mimes.c \
        $(DMS_TOP)/trace.c \
        $(DMS_TOP)/DMS.c \
        $(DMS_TOP)/LinuxOSL/ScanDir.c

BDLNA_DMS_PUBLIC_INCLUDES += $(DMS_TOP)/ \
BDLNA_DMS_PUBLIC_INCLUDES += $(DLNA_TOP)/core/include \
BDLNA_DMS_PUBLIC_INCLUDES += $(DLNA_TOP)/core/src \
BDLNA_DMS_PUBLIC_INCLUDES += $(UPNP_TOP)/include \
BDLNA_DMS_PUBLIC_INCLUDES += $(UPNP_TOP)/src \
BDLNA_DMS_PUBLIC_INCLUDES += $(DMS_TOP)/Dependencies/LibDLNA \
BDLNA_DMS_PUBLIC_INCLUDES += $(DMS_TOP)/Dependencies/AVCodecHeaders/libavcodec \
BDLNA_DMS_PUBLIC_INCLUDES += $(DMS_TOP)/Dependencies/AVCodecHeaders/libavutil \
BDLNA_DMS_PUBLIC_INCLUDES += $(DMS_TOP)/Dependencies/AVCodecHeaders/libavformat \

#Define private include path(s) for this library
#
BDLNA_DMS_PRIVATE_INCLUDES += $(DMS_TOP)/include \

BDLNA_DMS_DEFINES += LINUX_BUILD _POSIX HAVE_DLNA

#ifeq ($(NETACCEL_SUPPORT), y)
#currently always assume NETACCEL is defined, so pcp header is aligned to 16 bytes.
#B_DTCP_IP_LIB_DEFINES += B_HAS_NETACCEL
#endif

#This is hack to get the C++ style comment in the lib to compile!
B_LIB_CFLAGS += -std=c99 
