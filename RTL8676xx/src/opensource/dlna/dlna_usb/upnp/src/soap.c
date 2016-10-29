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
 * $brcm_Workfile: soap.c $
 * $brcm_Revision: 6 $
 * $brcm_Date: 9/30/09 3:33p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/soap.c $
 * 
 * 6   9/30/09 3:33p dliu
 * SW7405-2482: Back out persistent connection
 * 
 * 5   9/30/09 1:41p dliu
 * SW7405-2482: Check in persistent connection
* 
***************************************************************************/
#include "upnp_priv.h"
#include "bsocket.h"
#include "service.h"
#include "soap.h"
#include "nanoxml.h"
#include "debug.h"
#include "device.h"
#include "controlpoint.h"
 
static BUPnPError Soap_SendResult(HttpContextHandle hContext, const char* serviceType, BUPnPActionHandle hAction);
static BUPnPError Soap_SendFault(HttpContextHandle hContext, BUPnPError errorCode);
static const char* Soap_ParseActionName( HttpContextHandle hContext );
static BUPnPError Soap_ParseEnvelope(const char* soapEnvelope, BUPnPActionHandle hAction, int* argStartIndex);
static void Soap_AddManHeader(char* headers);

/* move to structure with action */
typedef struct SoapParserArg
{
    size_t argumentIndex;
    bool actionFound;
    bool bSoapClient;
    size_t numberInArguments;
    BUPnPActionHandle hAction;
} SoapParserArg;

BUPnPError Soap_ProcessPost( HttpContextHandle hContext )
{
    BUPnPError error;
    BUPnPServiceHandle hService;
    BUPnPActionHandle hAction;
    const char *uri, *actionName, *soapEnvelope;

    uri = Http_GetRequestUri(hContext);

    hService = BUPnPService_GetFromUrl(uri);
    if ( !hService )
        return Soap_SendFault(hContext, UPNP_E_INVALID_CONTROL_URL);

    actionName = Soap_ParseActionName(hContext);
    if ( !actionName )
        return Soap_SendFault(hContext, UPNP_E_INVALID_ACTION);
    
    hAction = BUPnPService_GetAction(hService, actionName);
    if ( !hAction )
        return Soap_SendFault(hContext, UPNP_E_INVALID_ACTION);
    
    soapEnvelope = Http_GetBody(hContext);

    /* IN parameters should always start at 0 */
    error = Soap_ParseEnvelope(soapEnvelope, hAction, NULL ); 
    if ( error != UPNP_E_SUCCESS )
        return Soap_SendFault(hContext, error);

    error = BUPnPAction_Invoke(hAction);
    if ( error != UPNP_E_SUCCESS )
        error = Soap_SendFault(hContext, error);
    else
        error = Soap_SendResult(hContext, hService->serviceInfo->serviceType, hAction);

    return error;
}

static const char* Soap_ParseActionName( HttpContextHandle hContext )
{
    const char *actionName, *actionHeader;
    char *actionStart, *actionEnd;

    actionHeader = Http_GetHeaderValue(hContext, "SOAPACTION");
    if ( !actionHeader )
        return NULL;

    actionName = NULL;

    /* Search for a soapaction header, which looks like this, */
    /* SOAPACTION: urn:schemas-upnp-org:service:serviceType:version#actionName */
    actionStart = (char*)strchr(actionHeader, '#');
    if ( actionStart != NULL )
    {
        actionEnd = (char*)strchr(++actionStart, '"');
        if ( actionEnd != NULL )
        {
            *actionEnd = '\0';
            actionName = actionStart;
        }
    }
    
    return actionName;
}

static void BUPnPSoap_TagEnd(nxml_t handle, const char *tag_name, unsigned len)
{
    BUPnPActionHandle hAction;
    SoapParserArg* pParserArg;
    char tempName[256];
    char *start;
    UNUSED_ARGUMENT(len);

    pParserArg = (SoapParserArg*)handle->context;
    if (pParserArg->actionFound == false) 
        return;

    hAction = pParserArg->hAction;
    sprintf(tempName, "%s", hAction->name);

    /* action suppose to have a namespace, since this is tagEnd, we can get anything here */
    if ((start = strpbrk(tag_name, ":>")) != NULL)
    {
        start++;
        if (MATCH_PREFIX(start, tempName))
            pParserArg->actionFound = false;
    }
}

static void BUPnPSoap_TagBegin(nxml_t handle, const char *tag_name, unsigned len)
{
    BUPnPActionHandle hAction;
    SoapParserArg* pParserArg;
    char tempName[256];
    char* start;
    BUPnPArgumentHandle hArg;
    cstring_t* pstr;

    pParserArg = (SoapParserArg*)handle->context;
    hAction = pParserArg->hAction;
    sprintf(tempName, "%s", hAction->name);

    if ((start = strpbrk(tag_name, ":>")) == NULL)
        return;
    else
        start++;

    if (pParserArg->actionFound == false)
    {
        /* look until we get to the action part */
        if (MATCH_PREFIX(start, tempName))
        {
            pParserArg->actionFound = true;
        }
    }
    else
    {
        /* Once we get all the arguements we have to discard any unknown ones (DLNA 7.2.23) */
        if ((!pParserArg->bSoapClient) && (pParserArg->argumentIndex == pParserArg->numberInArguments))
        {
            pParserArg->actionFound = false;
            return;
        }

        cmsUtl_strncpy(tempName, tag_name, len);
        tempName[len] = '\0';
        hArg = BUPnPAction_GetArgumentByIndex(hAction, pParserArg->argumentIndex++);

        if (hArg != NULL)
        {
            if (!MATCH(tempName, hArg->name))
            {
                handle->error = UPNP_E_INVALID_ARG;
                return;
            }
        }
        else
        {
            handle->error = UPNP_E_INVALID_ARG;
            return;
        }

        /* clear it */
        pstr = &hArg->value;
        CString_SetText(pstr, "");
    }
}

static void BUPnPSoap_Data(nxml_t handle, const char *tag_name, unsigned len)
{
    BUPnPActionHandle hAction;
    SoapParserArg* pParserArg;
    BUPnPArgumentHandle hArg;
    cstring_t* pstr;
    char argumentName[256];

    /* ignore any data till we get get to the functions */
    pParserArg = (SoapParserArg*)handle->context;
    if (pParserArg->actionFound == false)
        return;

    /* Once we get all the arguments we have to discard any unknown ones (DLNA 7.2.23) */
    if ((!pParserArg->bSoapClient) && (pParserArg->argumentIndex-1 == pParserArg->numberInArguments)) /* -1 because we increment in the begin_tag */
        return;

    hAction = pParserArg->hAction;
    cmsUtl_strncpy(argumentName, handle->lastStartTag, handle->lastStartTagLen);
    argumentName[handle->lastStartTagLen] = '\0';
    hArg = BUPnPAction_GetArgumentByName(hAction, argumentName);

    if ((hArg == NULL))
    {
        handle->error = UPNP_E_INVALID_ARG;
        return;
    }

    pstr = &hArg->value;
    /* TODO: Check type before during DecodeHex */
    /*    DecodeHex((char*)value, (char*)tag_name); */
    CString_SetTextN(pstr, len, tag_name);
}

static BUPnPError Soap_ParseEnvelope(const char* soapEnvelope, BUPnPActionHandle hAction, int* argStartIndex)
{
    int retCode, clientId = 0;
    nxml_t handle;
    nxml_settings settings;
    SoapParserArg* pParserArg;

    memset(&settings, 0, sizeof(settings));
    pParserArg = (SoapParserArg*)BUPnPCalloc(1, sizeof(SoapParserArg));
    pParserArg->hAction = hAction;

    if (argStartIndex != NULL)
    {
        /* This tells us we are acting as a soap client */
        pParserArg->argumentIndex = *argStartIndex;
        pParserArg->bSoapClient = true;
    }
    pParserArg->numberInArguments = hAction->inArgumentCount;
    pParserArg->actionFound = false;

    settings.tag_end = BUPnPSoap_TagEnd;
    settings.tag_begin = BUPnPSoap_TagBegin;
    settings.data = BUPnPSoap_Data;

    nxml_open(clientId, &handle,&settings);
    handle->context = (void*)pParserArg;

    nxml_write(handle, soapEnvelope, cmsUtl_strlen(soapEnvelope));

    retCode = handle->error;
    if (retCode == UPNP_E_SUCCESS)
    {
        /* check we got all the in arguments */
        if ((!pParserArg->bSoapClient) && (pParserArg->argumentIndex != hAction->inArgumentCount))
            retCode = UPNP_E_INVALID_ARG;
    }
    BUPnPFree(pParserArg);
    nxml_close(handle);
    return retCode;
}

static BUPnPError Soap_SendFault(HttpContextHandle hContext, BUPnPError errorCode)
{
    cstring_t body;
    char header[512], date[64];

    CString_Create(&body);

    CString_Append(&body,
        "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n"
           "<s:Body>\r\n"
              "<s:Fault>\r\n"
                 "<faultcode>s:Client</faultcode>\r\n"
                 "<faultstring>BUPnPError</faultstring>\r\n"
                 "<detail>\r\n"
                    "<UPnPError xmlns=\"urn:schemas-upnp-org:control-1-0\">\r\n"
                       "<errorCode>%ld</errorCode>\r\n"
                       "<errorDescription>%s</errorDescription>\r\n"
                    "</UPnPError>\r\n"
                 "</detail>\r\n"
              "</s:Fault>\r\n"
           "</s:Body>\r\n"
        "</s:Envelope>\r\n", errorCode, BUPnPAction_GetErrorText(errorCode));

    snprintf(header, sizeof(header)-1, 
              "CONTENT-LENGTH: %d\r\n"
              "CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n"
              "DATE: %s\r\n"
              "EXT:\r\n"
              "SERVER: %s\r\n\r\n",
                  body.length, 
                  BUPnP_GetRfc1123Date(date, sizeof(date)), 
                  BUPnP_GetDescription());

    /* 3.2.2 UPnP specifies 500 as error code for Action response */
    Http_SendResponse(hContext, HTTP_INTERNAL_SERVER_ERROR, header, body.bytes);

    CString_Destroy(&body);

    return UPNP_E_SUCCESS;
}

static BUPnPError Soap_SendResult(HttpContextHandle hContext, const char* serviceType, BUPnPActionHandle hAction)
{
    size_t i;
    const char* pValue;
    char header[512], date[64];
    BUPnPArgumentInfo* pArg;
    cstring_t *pstr;
    cstring_t body;

    CString_Create(&body);

    CString_Append(&body, 
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
           "<s:Body>"
           "<u:%sResponse xmlns:u=\"%s\">", hAction->name, serviceType);

    for (i=0; i<hAction->argumentCount; i++)
    {
        pArg = hAction->arguments[i].argumentInfo;

        if (UPNP_GET_ATTRIBUTE(pArg, BUPnPAttribute_Out))
        {
            pstr = (cstring_t*)&hAction->arguments[i].value;
            pValue = ( pstr->length == 0 ) ? "" : CString_GetText(pstr);
            CString_Append(&body, "<%s>%s</%s>", pArg->name, pValue, pArg->name);
        }
    }

    CString_Append(&body, 
           "</u:%sResponse>"
           "</s:Body>"
        "</s:Envelope>\r\n", hAction->name);

    snprintf(header, sizeof(header)-1, 
              "CONTENT-LENGTH: %d\r\n"
              "CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n"
              "DATE: %s\r\n"
              "EXT:\r\n"
              "SERVER: %s\r\n\r\n", body.length, 
                                    BUPnP_GetRfc1123Date(date, sizeof(date)), 
                                    BUPnP_GetDescription());

    Http_SendResponse(hContext, HTTP_OK, header, body.bytes);

    CString_Destroy(&body);

    return UPNP_E_SUCCESS;
}

BUPnPError Soap_Invoke(BUPnPDeviceHandle hDevice, struct BUPnPService* pService, BUPnPActionHandle hAction)
{
    size_t i;
    HttpContext context;
    HttpStatusCode statusCode;
    char headers[1024];
    cstring_t soapEnvelope;
    BUPnPError result;
    BUPnPArgumentHandle hArg;
    int outParamStartIndex = 0;

    result = Http_CreateContext(&context);
    if ( result != UPNP_E_SUCCESS )
        return result;

    context.s = BSocket_CreateTcpClient(hDevice->url.host, hDevice->url.port);
    if ( context.s <= 0 )
    {
        Http_DestroyContext(&context);
        return UPNP_E_SOCKET;
    }

    CString_Create(&soapEnvelope);

    CString_Append(&soapEnvelope, "<?xml version=\"1.0\"?>"
            "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
            "<s:Body>"
            "<u:%s xmlns:u=\"%s%s:%d\">", hAction->name, UPNP_HEADER, pService->serviceInfo->serviceId, pService->version);

    for ( i=0; i<hAction->argumentCount; i++ )
    {
        hArg = &hAction->arguments[i];
        if (hArg->argumentInfo->attributes & BUPnPAttribute_In)
            CString_Append(&soapEnvelope, "<%s>%s</%s>", hArg->name, (hArg->value.bytes ? hArg->value.bytes : ""), hArg->name);
        else /* we make assumption that all in params are before out params */
        {
            outParamStartIndex = i;
            break;
        }
    }
    CString_Append(&soapEnvelope, "</u:%s>", hAction->name);
    CString_Append(&soapEnvelope, "</s:Body></s:Envelope>");

    snprintf(headers, sizeof(headers)-1,
        "HOST: %s:%d\r\n"
        "CONTENT-LENGTH: %d\r\n"
        "CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n"
        "USER-AGENT: %s\r\n" 
        "SOAPACTION: \"%s%s:%d#%s\"\r\n"
        "CONNECTION: close\r\n"
        "\r\n",
            hDevice->url.host, hDevice->url.port,  
            soapEnvelope.length, BUPnPControlPoint_GetUserAgent(), UPNP_HEADER, pService->serviceInfo->serviceId, 
            pService->version, hAction->name);

    UPNP_DEBUG_TRACE(BUPnPDebugCategory_Control, ("Soap Invoke on %s:\n %s, %s\n", 
        pService->serviceInfo->controlUrl, headers, soapEnvelope.bytes));
    result = Http_Invoke(&context, "POST", pService->serviceInfo->controlUrl, headers, soapEnvelope.bytes);

    if ( result == UPNP_E_SUCCESS )
    {
        /* Error checking */
        statusCode = Http_GetStatusCode(&context);

        /* UPnP-DA Section 3.2.1: If request is rejected with error code 405, then a second request must be sent
           using the MAN header and M-Prefix */
        if (statusCode == HTTP_METHOD_NOT_ALLOWED)
        {
            Soap_AddManHeader(headers);
            result = Http_Invoke(&context, "M-POST", pService->serviceInfo->controlUrl, headers, soapEnvelope.bytes);

            if (result == UPNP_E_SUCCESS) /* grab new status code */
                statusCode = Http_GetStatusCode(&context);
        }
        
        if (statusCode != HTTP_OK)
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Control, ("Soap action failed with HTTP response: %d", statusCode));
            result = UPNP_E_ACTION_FAILED;
        }
        else
        {
            UPNP_DEBUG_TRACE(BUPnPDebugCategory_Control, ("Soap Invoke result\n %s", Http_GetBody(&context)));
            result = Soap_ParseEnvelope(Http_GetBody(&context), hAction, &outParamStartIndex);
        }
    }

    CString_Destroy(&soapEnvelope);
    Http_DestroyContext(&context);
    closesocket(context.s);

    return result;
}

void Soap_AddManHeader(char* headers)
{
    char temp[1024];
    const char* soapActionHeader;
    char *manHeader = "MAN: \"http://schemas.xmlsoap.org/soap/envelope/\"; "
        "ns=01\r\n01-";

    soapActionHeader = cmsUtl_strstr( headers, "SOAPACTION:" );
    assert( soapActionHeader != NULL ); /* This should never fail */

    /* Insert the MAN header just before SOAPACTION */
    cmsUtl_strncpy(temp, soapActionHeader, sizeof(temp)-1); 
    headers[soapActionHeader-headers] = '\0';
    strncat(headers, manHeader, cmsUtl_strlen(manHeader));
    strncat(headers, temp, cmsUtl_strlen(temp)); 
}

