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
# $brcm_Workfile: bdlna.inc $
# $brcm_Revision: 3 $
# $brcm_Date: 7/28/09 4:53p $
# 
#############################################################################

#Identify this library
#
B_LIBS += BDLNA

#
#Define source files in this library
#
BDLNA_SOURCES =  $(UPNP_TOP)/src/action.c \
                 $(UPNP_TOP)/src/bsocket.c \
                 $(UPNP_TOP)/src/linux/osl.c \
                 $(UPNP_TOP)/src/cstring.c \
                 $(UPNP_TOP)/src/upnp.c \
                 $(UPNP_TOP)/src/device.c \
                 $(UPNP_TOP)/src/controlpoint.c \
                 $(UPNP_TOP)/src/nanoxml.c \
                 $(UPNP_TOP)/src/heap.c \
                 $(UPNP_TOP)/src/ssdp.c \
                 $(UPNP_TOP)/src/http.c \
                 $(UPNP_TOP)/src/service.c \
                 $(UPNP_TOP)/src/subscription.c \
                 $(UPNP_TOP)/src/soap.c \
                 $(UPNP_TOP)/src/gena.c \
                 $(UPNP_TOP)/src/debug.c \
                 $(UPNP_TOP)/src/variable.c \
                 $(UPNP_TOP)/src/url.c \
                 $(UPNP_TOP)/src/timer.c \
                 $(DLNA_TOP)/core/src/bdlna.c \
                 $(DLNA_TOP)/core/src/bdlna_didl.c \
                 $(DLNA_TOP)/core/src/bdlna_debug.c 

BDLNA_PUBLIC_INCLUDES += $(DLNA_TOP)/core/include \
BDLNA_PUBLIC_INCLUDES += $(DLNA_TOP)/core/src \
BDLNA_PUBLIC_INCLUDES += $(UPNP_TOP)/include \
BDLNA_PUBLIC_INCLUDES += $(UPNP_TOP)/src \

#Define private include path(s) for this library
#

#
#Defines extra C definations, These are from original DTCP package.
#NOTE: the DTCP_IP_HARDWARE_DECRYPTION need to be defined, if you wan to use
#Broadcom's M2M hardware to decrypt DTCP content stream.
#
BDLNA_LIB_DEFINES += LINUX_BUILD _POSIX

#This is hack to get the C++ style comment in the lib to compile!
B_LIB_CFLAGS += -std=c99
