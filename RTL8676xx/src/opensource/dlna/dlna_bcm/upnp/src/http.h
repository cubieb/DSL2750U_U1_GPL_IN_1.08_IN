/***************************************************************************
*     (c)2008-2009 Broadcom Corporation
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
 * $brcm_Workfile: http.h $
 * $brcm_Revision: 2 $
 * $brcm_Date: 6/26/09 6:25p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/http.h $
 * 
 * 2   6/26/09 6:25p ajitabhp
 * PR55165 : Additional Status Codes and Include files for OSL layer
* 
***************************************************************************/
#ifndef Http_h__
#define Http_h__

#ifdef __cplusplus
extern "C" {
#endif

/* HTTP status codes organized by class */

/* 1xx: Informational - Request received, continuing process */
#define HTTP_CONTINUE 100

/* 2xx: Success - The action was successfully received, understood and accepted */
#define HTTP_OK    200 
#define HTTP_PARTIAL_XFER    206
/* 3xx: Redirection - Further action must be taken in order to complete the request */

/* 4xx: Client Error - The request contains bad syntax or cannot be fulfilled */
#define HTTP_BAD_REQUEST         400
#define HTTP_NOT_FOUND             404
#define HTTP_METHOD_NOT_ALLOWED     405
#define HTTP_NOT_ACCEPTABLE    406
#define HTTP_PRECONDITION_FAILED 412
#define HTTP_REQUEST_RANGE_INVALID    416

/* 5xx: Server Error - The server failed to fulfill an apparently valid request */
#define HTTP_INTERNAL_SERVER_ERROR    500 
#define HTTP_NOT_IMPLEMENTED        501
#define HTTP_SERVICE_UNAVAILABLE    503

typedef uint32_t HttpStatusCode;

/* Supported HTTP versions.  Only 1.0 and 1.1 */
typedef enum HttpVersion
{
    HttpVersion_10,
    HttpVersion_11
}HttpVersion;

/* Supported HTTP 1.1 Transfer Codings as described in section 3.6 of spec */
typedef enum HttpTransferCodings
{
    HttpTransferCoding_Default,
    HttpTransferCoding_Chunked
}HttpTransferCodings;

/* General position of the parser within an HTTP message.  StartLine is lumped with Header */
typedef enum HttpParserPosition
{
    HttpParserPosition_Header,
    HttpParserPosition_Body
}HttpParserPosition;

typedef struct HttpParser
{
    HttpParserPosition position; /* The position of the parser within the http entity */

    uint32_t bytesRemaining; /* Number of bytes remaining to be parsed */

}HttpParser;

/* Defines the offset and length of a request/response http entity within the http stream */
typedef struct HttpEntity
{
    uint32_t offset;
    size_t length;
    BLST_ENTRY(HttpEntity) link;
}HttpEntity;

/* Defines a generic HTTP body */
typedef struct HttpBody
{
    HttpTransferCodings encoding; /* Only default and "chunked" transfer encodings are supported */

    HttpParser parser;

    BLST_HEAD(HttpBodyList, HttpEntity) entities;    /* A body consists of 1 or more content entities */

    /* buffer containing a flattened content made up of the sum of all entities */
    /* we could get rid of this but we need to modify soap and gena to support receiving entity lists */
    char *flatBody;

}HttpBody;

/* Defines a standard request/response message as described in the HTTP specification */
typedef struct HttpMessage
{
    HttpEntity startLine;
    
    HttpEntity headerFields;

    HttpBody body;

}HttpMessage;

/* HTTP buffered stream */
typedef struct HttpStream
{
    size_t    length;
    size_t    capacity;
    size_t    position;
    char*    buffer;
}HttpStream;

typedef struct HttpContext
{
    SOCKET s;

    HttpStream stream;

    /* The HTTP request method.  Used by HTTP clients to help determine if content is expected in the response */
    char method[8];

    HttpVersion version;

    HttpParser parser;

    HttpMessage message;

    /* used by server to determine if the context is not persistent so the connection can be closed */
    bool isPersistent;
    
} HttpContext, *HttpContextHandle;

/* used by the parser to invoke a callback each time a complete http message has been created */
typedef int32_t (*HttpParserCallback)(HttpContextHandle);

HttpVersion Http_GetVersion(HttpContextHandle hContext);

int32_t Http_GetStatusCode(HttpContextHandle hContext);

int32_t Http_GetStatusGroupCode(HttpContextHandle hContext);

const char* Http_GetRequestMethod(HttpContextHandle hContext);

const char* Http_GetRequestUri(HttpContextHandle hContext);

const char* Http_GetHeaderValue(HttpContextHandle hContext, const char* name);

const char* Http_GetBody(HttpContextHandle hContext);

size_t Http_GetBodyLength(HttpContextHandle hContext);

bool Http_IsPersistent(HttpContextHandle hContext);

int32_t Http_SendRequest(HttpContextHandle hContext, const char* method, const char* uri, const char *headers, const char *body);

int32_t Http_SendResponse(HttpContextHandle hContext, HttpStatusCode statusCode, const char *headers, const char *content);

int32_t Http_GetResponse(HttpContextHandle hContext);

int32_t Http_Invoke(HttpContextHandle hContext, const char* method, const char* uri, const char *headers, const char *body);

int32_t Http_ParseHeader(HttpContextHandle hContext);

int32_t Http_ParseRequest(HttpContextHandle hContext, HttpParserCallback callback);

int32_t Http_CreateContext(HttpContextHandle hContext);

void Http_ClearContext(HttpContextHandle hContext);

void Http_DestroyContext(HttpContextHandle hContext);

int32_t Http_Receive(HttpContextHandle hContext);

#ifdef __cplusplus
}
#endif


#endif
