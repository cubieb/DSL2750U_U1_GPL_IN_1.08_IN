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
 * $brcm_Workfile: http.c $
 * $brcm_Revision: 7 $
 * $brcm_Date: 9/30/09 4:20p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/http.c $
 * 
 * 7   9/30/09 4:20p dliu
 * SW7405-2482: Fix bug in Http_GetBody where length exceeds stream size
 * 
 * 6   9/21/09 5:50p dliu
 * SW7405-2482: Fix issue with persistent connection
 * 
 * 5   8/24/09 5:52p dliu
 * SW7405-2482: Change select time out to 10s
 * 
 * 4   6/26/09 6:25p ajitabhp
 * PR55165 : Additional Status Codes and Include files for OSL layer
* 
***************************************************************************/
#include "upnp_priv.h"
#include "device.h"
#include "http.h"
#include "gena.h"
#include "soap.h"
#include "debug.h"

#define HTTP_BUF_LOWATER        256
#define HTTP_BUF_INCREMENT        2048
#define HTTP_BUF_MAX_CAPACITY    262144
#define HTTP_MAX_BODY_SIZE        4096
#define HTTP_RESPONSE_TIMEOUT    30000

#define HTTP_SKIP_WHITESPACE(p)    while ( p != NULL && *p == ' ' )p++;

typedef struct HttpStatus
{
    HttpStatusCode statusCode;
    const char* text;
}HttpStatus;

static HttpStatus g_httpStatus[] =
{
    { HTTP_OK,                        "OK" },
    { HTTP_PARTIAL_XFER,            "Partial Content"},
    { HTTP_INTERNAL_SERVER_ERROR,    "Internal Server Error" },
    { HTTP_BAD_REQUEST,                "Bad Request" },
    { HTTP_REQUEST_RANGE_INVALID,    "Request Range Not Satisfiable"},
    { HTTP_NOT_ACCEPTABLE,            "Not Acceptable"},
    { HTTP_NOT_FOUND,                "Not Found" },
    { HTTP_NOT_IMPLEMENTED,            "Not Implemented" },
    { HTTP_PRECONDITION_FAILED,        "Precondition Failed" },
    { 0, NULL }
};

static const char* Http_GetStatusText( uint32_t statusCode );
static void Http_FreeMessage(HttpContextHandle hContext);
static char* Http_TokenizeHeader(char *buf, int32_t length);
static int32_t Http_ParseBody(HttpContextHandle hContext);
static int32_t Http_Send(HttpContextHandle hContext, const char *header, const char *body);
static HttpEntity* Http_AddBodyEntity(HttpContextHandle hContext, int32_t position, size_t length);

/**************************************************************************************************
 HTTP Message Entity Accessor Methods
 *************************************************************************************************/

HttpVersion Http_GetVersion(HttpContextHandle hContext)
{
    return hContext->version;
}

const char* Http_GetRequestMethod(HttpContextHandle hContext)
{
    char *startLine;
    startLine = hContext->stream.buffer + hContext->message.startLine.offset;
    assert(startLine);
    return startLine;
}

const char* Http_GetRequestUri(HttpContextHandle hContext)
{
    char *startLine;
    startLine = hContext->stream.buffer + hContext->message.startLine.offset;
    assert(startLine);
    return startLine + cmsUtl_strlen(startLine) + 1;
}

int32_t Http_GetStatusCode(HttpContextHandle hContext)
{
    const char *pszCode;
    char *startLine;

    startLine = hContext->stream.buffer + hContext->message.startLine.offset;
    assert(startLine);
    pszCode = startLine + cmsUtl_strlen(startLine) + 1;
    return atol(pszCode);/* startLine is delimited by NULL's so cmsUtl_strlen will point to the uri */
}

int32_t Http_GetStatusGroupCode(HttpContextHandle hContext)
{
    const char *pszCode;
    char *startLine;

    startLine = hContext->stream.buffer + hContext->message.startLine.offset;
    assert(startLine);
    pszCode = startLine + cmsUtl_strlen(startLine) + 1;
    HTTP_SKIP_WHITESPACE(pszCode);
    assert((int)pszCode[0] > 48 && (int)pszCode[0] < 58); /* Must be a number from 1 to 9 ascii */
    return ((int)pszCode[0]-48) * 100;
}

static const char* Http_GetStatusText(uint32_t statusCode)
{
    struct HttpStatus* pStatus = NULL;
    for (pStatus = g_httpStatus; pStatus->text; pStatus++)
    {
        if (pStatus->statusCode == statusCode)
            return pStatus->text;
    }
    return "Unspecified Error";
}

const char* Http_GetHeaderValue(HttpContextHandle hContext, const char* name)
{
    const char *pos, *end;

    pos = hContext->stream.buffer + hContext->message.headerFields.offset;
    end = pos + hContext->message.headerFields.length;

    while ( pos != NULL && pos < end )
    {
        if (IMATCH(pos, name))
        {
            while (*pos != '\0')
                pos++;
            pos++;
            HTTP_SKIP_WHITESPACE(pos);
            return pos;
        }

        while (*pos != '\0') /* move past header name */
            pos++;
        ++pos;
        while (*pos != '\0') /* move past header value */
            pos++;
        ++pos;

        /* scan to the begining of the next line */
        while ( pos != NULL && (*pos == '\r' || *pos == '\n') )
            pos++;
    };

    return NULL;
}

size_t Http_GetBodyLength(HttpContextHandle hContext)
{
    HttpEntity *body;
    size_t length=0;

    body = BLST_FIRST(&hContext->message.body.entities);
    for (;body;body=BLST_NEXT(body, link))
        length += (body->length + 1);
    return length;
}

const char* Http_GetBody(HttpContextHandle hContext)
{
    HttpEntity *body, *next;
    size_t length=0;
    size_t localOffset=0;

    if ( hContext->message.body.flatBody )
        return hContext->message.body.flatBody;
    else
    {
        body = BLST_FIRST(&hContext->message.body.entities);
        for (;body;body=BLST_NEXT(body, link))
            length += (body->length + 1);

        if ( length > 0 )
        {
            hContext->message.body.flatBody = (char*)BUPnPCalloc(length+1, 1);
            if (hContext->message.body.flatBody == NULL)
                return NULL;

            body = BLST_FIRST(&hContext->message.body.entities);
            while(body)
            {
                next = BLST_NEXT(body, link);

                /* in the case where we didn't get a content length from the server, this entity will have the wrong
                   body length, so we need to correct it here */
                if (body->length > hContext->stream.length - body->offset)
                {
                    if (next == NULL)
                    {
                        body->length = hContext->stream.length - body->offset;
                    }
                    else
                    {
                        body->length = next->offset - body->offset;
                    }
                }
                memcpy(hContext->message.body.flatBody+localOffset, hContext->stream.buffer + body->offset, body->length);
                localOffset += body->length;
                body = next;
            }

            return hContext->message.body.flatBody;
        }
    }
    return NULL;
}


bool Http_IsPersistent(HttpContextHandle hContext)
{
    return hContext->isPersistent;
}

/**************************************************************************************************
 HTTP Request/Response Methods
 *************************************************************************************************/

int32_t Http_SendRequest(HttpContextHandle hContext, const char* method, const char* uri, const char *headers, const char *body)
{
    char buf[2048];
    memset(buf, 0, sizeof(buf));

    cmsUtl_strcpy(hContext->method, method);

    if (headers != NULL)
        snprintf(buf, sizeof(buf)-1, "%s %s HTTP/1.1\r\n%s", method, uri, headers);
    else
        snprintf(buf, sizeof(buf)-1, "%s %s HTTP/1.1\r\n\r\n", method, uri);

    assert(cmsUtl_strlen(buf) < sizeof(buf)-1);
    if ( cmsUtl_strstr(buf, "\r\n\r\n") == NULL )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Protocol, ("Http_SendRequest - Header does not contain empty line"));
        return UPNP_E_BAD_REQUEST;
    }

    return Http_Send(hContext, buf, body);
}

int32_t Http_SendResponse(HttpContextHandle hContext, HttpStatusCode statusCode, const char *headers, const char *body)
{
    char buf[2048];
    memset(buf, 0, sizeof(buf));

    if (headers != NULL)
        snprintf(buf, sizeof(buf)-1, "HTTP/1.1 %d %s\r\n%s", statusCode, Http_GetStatusText(statusCode), headers);
    else
        snprintf(buf, sizeof(buf)-1, "HTTP/1.1 %d %s\r\n\r\n", statusCode, Http_GetStatusText(statusCode));

    assert(cmsUtl_strlen(buf) < sizeof(buf)-1);
    if ( cmsUtl_strstr(buf, "\r\n\r\n") == NULL )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Protocol, ("Http_SendResponse - Header does not contain empty line"));
        return UPNP_E_BAD_REQUEST;
    }

    return Http_Send(hContext, buf, body);
}

int32_t Http_GetResponse(HttpContextHandle hContext)
{
    int n, elapsedTime, startTime, readCount;
    fd_set fds;
    int32_t result, statusCode;
    struct timeval timeout;

    FD_ZERO (&fds);
    FD_SET (hContext->s, &fds);

    timeout.tv_sec = 10; /* do 10 second timeouts */
    timeout.tv_usec = 0;

    elapsedTime = 0;
    readCount = 0;
    hContext->parser.position = HttpParserPosition_Header;

    startTime = GetTickCount();

    do
    {
        n = select (hContext->s+1, &fds, NULL, NULL, &timeout);

        if ( n > 0 )
        {
            result = Http_Receive(hContext);

            if ( result == UPNP_E_SUCCESS )
            {
                readCount++;
                do
                {
                    if ( hContext->parser.position == HttpParserPosition_Header )
                    {
                        result = Http_ParseHeader(hContext);
                        if ( result == UPNP_E_INCOMPLETE )
                            break; /* if no complete header go back to receive */

                        /* check for a 100 CONTINUE - Http 1.1 only */
                        statusCode = Http_GetStatusGroupCode(hContext);
                        if ( statusCode == 100 )
                        {
                            hContext->parser.position = HttpParserPosition_Header;
                            result = UPNP_E_INCOMPLETE;
                            continue;
                        }
                    }

                    /* Check the request method to help determine if content is expected */
                    if ( hContext->parser.position != HttpParserPosition_Body )
                    {
                        if (IMATCH(hContext->method, "GET") || IMATCH(hContext->method, "POST"))
                            hContext->parser.position = HttpParserPosition_Body;
                    }

                    if ( hContext->parser.position == HttpParserPosition_Body )
                        result = Http_ParseBody(hContext);

                } while (hContext->stream.position < hContext->stream.length );
            }
            else if ( result == UPNP_E_SOCKET_CLOSED )
            {
                if ((!hContext->isPersistent ) && (readCount != 0))
                    result = UPNP_E_SUCCESS;
            }
        }
        else if (n < 0)
        {
            perror("poll error\n");
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Protocol, ("selected return error"));
            result = UPNP_E_SELECT_FAILED;
        }
        else /* poll time out, so just continue */
            result = UPNP_E_INCOMPLETE;

        elapsedTime = GetTickCount();
        elapsedTime = elapsedTime-startTime;
        if (elapsedTime > HTTP_RESPONSE_TIMEOUT)
            result = UPNP_E_TIMEOUT;

    } while (result != UPNP_E_TIMEOUT && result == UPNP_E_INCOMPLETE);

    return result;
}

int32_t Http_Invoke(HttpContextHandle hContext, const char* method, const char* uri, const char *headers, const char *body)
{
    int32_t result;

    if ((result = Http_SendRequest(hContext, method, uri, headers, body)) != UPNP_E_SUCCESS)
        return result;

    if ((result = Http_GetResponse(hContext)) != UPNP_E_SUCCESS)
        return result;

    return UPNP_E_SUCCESS;
}

static int32_t Http_Send(HttpContextHandle hContext, const char *header, const char *body)
{
    if ( send(hContext->s, header, cmsUtl_strlen(header), 0) < 0 )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Protocol, ("Http_Send - Error %d sending header", errno));
        return UPNP_E_SOCKET;
    }

    if ( body && send(hContext->s, body, cmsUtl_strlen(body), 0) < 0 )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Protocol, ("Http_Send - Error %d sending body", errno));
        return UPNP_E_SOCKET;
    }

    return UPNP_E_SUCCESS;
}

int32_t Http_Receive(HttpContextHandle hContext)
{
    char *p;
    int nbytes;
    HttpStream *pStream;

    pStream = &hContext->stream;

    if ((pStream->capacity - pStream->length) < HTTP_BUF_LOWATER)
    {
        if ( pStream->capacity + HTTP_BUF_INCREMENT > HTTP_BUF_MAX_CAPACITY )
        {
            UPNP_DEBUG_TRACE(BUPnPDebugCategory_Protocol, ("Http_Receive - Max allowed buffer size (%d) exceeded", HTTP_BUF_MAX_CAPACITY));
            return UPNP_E_OUT_OF_MEMORY;
        }

        p = (char*)BUPnPRealloc(pStream->buffer, pStream->capacity + HTTP_BUF_INCREMENT);
        if ( p == NULL )
            return UPNP_E_OUT_OF_MEMORY;

        pStream->buffer = p;
        pStream->capacity += HTTP_BUF_INCREMENT;

        memset( p+pStream->length, 0, pStream->capacity-pStream->length );
    }

    nbytes = recv(hContext->s, pStream->buffer + pStream->length, pStream->capacity - pStream->length - 1, 0);
    if (nbytes == 0)
    {
        UPNP_DEBUG_TRACE(BUPnPDebugCategory_Protocol, ("Http_Receive - Socket Closed By Client"));
        return UPNP_E_SOCKET_CLOSED;
    }
    else if (nbytes < 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Protocol, ("Http_Receive - Error %d reading socket", errno));
        return UPNP_E_SOCKET;
    }

    pStream->length += nbytes;
    pStream->buffer[pStream->length] = 0;

    return UPNP_E_SUCCESS;
}

/**************************************************************************************************
 HTTP Context Methods
 *************************************************************************************************/

int32_t Http_CreateContext(HttpContextHandle hContext)
{
    memset(hContext, 0, sizeof(HttpContext));
    return UPNP_E_SUCCESS;
}

void Http_ClearContext(HttpContextHandle hContext)
{
    SOCKET s;
    bool isPersistent;
    s = hContext->s;
    isPersistent = hContext->isPersistent;

    Http_FreeMessage(hContext);

    if (hContext->stream.buffer)
        BUPnPFree(hContext->stream.buffer);

    memset(hContext, 0, sizeof(HttpContext));
    hContext->s = s;
    hContext->isPersistent = isPersistent;
}

void Http_DestroyContext(HttpContextHandle hContext)
{
    Http_ClearContext(hContext);
}

static void Http_FreeMessage(HttpContextHandle hContext)
{
    HttpEntity *bodyEntity;
    HttpEntity *bodyEntityPrev;

    while( (bodyEntity = BLST_FIRST(&hContext->message.body.entities)) != NULL )
    {
        bodyEntityPrev = bodyEntity;
        bodyEntity = BLST_NEXT(bodyEntity, link);
        BLST_REMOVE_HEAD(&hContext->message.body.entities, link);
        BUPnPFree(bodyEntityPrev);
    }

    if ( hContext->message.body.flatBody )
        BUPnPFree(hContext->message.body.flatBody);

    memset(&hContext->message, 0, sizeof(HttpMessage));
}

static HttpEntity* Http_AddBodyEntity(HttpContextHandle hContext, int32_t position, size_t length)
{
    HttpEntity *entity;

    entity = (HttpEntity*)BUPnPCalloc(1, sizeof(HttpEntity));
    if ( entity == NULL )
        return NULL;

    BLST_INSERT_TAIL(&hContext->message.body.entities, entity, link);

    entity->offset = position;
    entity->length = length;

    return entity;
}

/**************************************************************************************************
 HTTP Parsing Methods 
 *************************************************************************************************/

int32_t Http_ParseRequest(HttpContextHandle hContext, HttpParserCallback callback)
{
    int32_t result = UPNP_E_SUCCESS;
    const char *method;
    bool done = false;

    /* loop to process any pipelined requests */
    do
    {
        if ( hContext->parser.position == HttpParserPosition_Header )
        {
            result = Http_ParseHeader(hContext);

            if (( result == UPNP_E_INCOMPLETE ) || ( result == UPNP_E_BAD_REQUEST))
                break; /* if no complete header go back to receive */

            /* For POST and NOTIFY we expect some content. */  
            method = Http_GetRequestMethod(hContext);
            if (IMATCH(method, "POST") || IMATCH(method, "NOTIFY"))
                hContext->parser.position = HttpParserPosition_Body;
        }

        if ( hContext->parser.position == HttpParserPosition_Body )
            result = Http_ParseBody(hContext);

        if ( result == UPNP_E_SUCCESS )
        {
            (*(callback))(hContext);
            Http_FreeMessage(hContext);
        }

        if ( hContext->stream.position >= hContext->stream.length )
            done = true;

    } while (!done);
    
    if ( result != UPNP_E_INCOMPLETE )
    {
        if ( result != UPNP_E_SUCCESS )
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Protocol, ("Http_RequestHandler - Error %d: Unable to process the request", result));
        Http_ClearContext(hContext);
    }

    return result;
}

int32_t Http_ParseHeader(HttpContextHandle hContext)
{
    char *p, *startLineEnd;
    const char *pszTemp, *headerEnd;
    HttpStream *pStream;
    HttpMessage *pMessage;

    pStream = &hContext->stream;
    pMessage = &hContext->message;

    p = pStream->buffer + pStream->position;

    headerEnd = cmsUtl_strstr(p, "\r\n\r\n" );
    if ( headerEnd == NULL )
        return UPNP_E_INCOMPLETE;

    /* HTTP spec says to ignore CRLF if they show up at the beginning */
    HTTP_SKIP_WHITESPACE(p);
    while (*p == '\r' && *p == '\n')
        p++;

    if ( cmsUtl_strstr(p, "HTTP/1.0") != NULL )
    {
        hContext->version = HttpVersion_10;
        hContext->isPersistent = false;
    }
    else if ( cmsUtl_strstr(p, "HTTP/1.1") )
    {
        hContext->version = HttpVersion_11;
        hContext->isPersistent = true;
    }
    else
        return UPNP_E_BAD_REQUEST; /* Invalid Http Version */

    /* Process and tokenize the start line */
    startLineEnd = cmsUtl_strstr(p, "\r\n" );
    pMessage->startLine.offset = pStream->position;
    pMessage->startLine.length = startLineEnd-p;
    pszTemp = strsep((char**)&p, " \t");
    pszTemp = strsep((char**)&p, " \t");
    pszTemp = strsep((char**)&p, "\r\n");
    
    /* Process and tokenize the header fields */
    p = startLineEnd + 2;
    pMessage->headerFields.offset = pMessage->startLine.offset + pMessage->startLine.length + 2;
    pMessage->headerFields.length = headerEnd-p;
    Http_TokenizeHeader(p, pMessage->headerFields.length);

    pStream->position = (headerEnd + 4 - pStream->buffer);

    /* Parse CONNECTION header item figure out if the connection should not be persistent */
    pszTemp = Http_GetHeaderValue(hContext, "CONNECTION");
    if ( pszTemp != NULL && IMATCH(pszTemp, "CLOSE") )
        hContext->isPersistent = false;

    /* If both chunk encoding and content length are set, we treat it as chunk encoding */
    pszTemp = Http_GetHeaderValue(hContext, "TRANSFER-ENCODING");
    if ( pszTemp != NULL )
    {
        if (IMATCH(pszTemp, "CHUNKED"))
        {
            hContext->message.body.encoding = HttpTransferCoding_Chunked;
            hContext->parser.position = HttpParserPosition_Body;
        }
    }

    /* If not chunked, we check content length still */
    if (( hContext->message.body.encoding == HttpTransferCoding_Chunked ) == 0)
    {
        pszTemp = Http_GetHeaderValue(hContext, "CONTENT-LENGTH");
        if ( pszTemp != NULL )
        {
            Http_AddBodyEntity(hContext, pStream->position, atol(pszTemp));
            hContext->parser.position = HttpParserPosition_Body;
        }
    }
    return UPNP_E_SUCCESS;
}

static int32_t Http_ParseChunkedBody(HttpContextHandle hContext)
{
    HttpEntity *chunk;
    int32_t val;
    size_t len;
    char *pos, *end, *endptr;

    pos = hContext->stream.buffer + hContext->stream.position;

    if ( hContext->message.body.parser.position == HttpParserPosition_Header )
    {
        end = cmsUtl_strstr(pos, "\r\n" );
        if ( end == NULL )
            return UPNP_E_INCOMPLETE;
        end+=2;

        val = strtol(pos, &endptr, 16);
        if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))) 
            return UPNP_E_PARSER;

        if ( val == 0 )
        {
            end = cmsUtl_strstr(pos, "\r\n\r\n" );
            if ( end == NULL )
                return UPNP_E_INCOMPLETE;
            end+=4;
        }
        else
        {
            Http_AddBodyEntity(hContext, (end-pos)+hContext->stream.position, val);
            hContext->message.body.parser.bytesRemaining = val;
            hContext->message.body.parser.position = HttpParserPosition_Body;
        }

        hContext->stream.position += (end - pos);
        if ( val == 0 )
            return UPNP_E_SUCCESS;
    }

    if ( hContext->message.body.parser.position == HttpParserPosition_Body )
    {
        chunk = BLST_FIRST(&hContext->message.body.entities);

        pos = hContext->stream.buffer + hContext->stream.position;
        len = hContext->stream.length - hContext->stream.position;

        if (len >= hContext->message.body.parser.bytesRemaining) 
        {
            /* all data are already in stream */
            hContext->stream.position += (hContext->message.body.parser.bytesRemaining + 2);
            hContext->message.body.parser.bytesRemaining = 0;
            hContext->message.body.parser.position = HttpParserPosition_Header;
        }
        else
        {
            hContext->stream.position += len;
            hContext->message.body.parser.bytesRemaining -= len;
        }

    }


    return UPNP_E_INCOMPLETE;
}

static int32_t Http_ParseBody(HttpContextHandle hContext)
{
    char *p;
    size_t len;
    HttpEntity *body;

    if ( hContext->message.body.encoding == HttpTransferCoding_Chunked )
        return Http_ParseChunkedBody(hContext);
    else
    {
        body = BLST_FIRST(&hContext->message.body.entities);

        /* if no body entity exists then the client did not send a content length yet we still expect content. */
        /* this is OK if it's HTTP 1.0 where the spec says to read content until the client closes the connection */
        if ( body == NULL )
            body = Http_AddBodyEntity(hContext, hContext->stream.position, HTTP_MAX_BODY_SIZE);

        p = hContext->stream.buffer + body->offset;
        len = hContext->stream.length - body->offset;

        if (len >= body->length)
        {
            hContext->stream.position += body->length;

            /* Put parser position back to header in case we have pipelined request */
            hContext->parser.position = HttpParserPosition_Header; 
            return UPNP_E_SUCCESS; /* We have the entire body */
        }
        else
        {
            /* if bodyLength is 0 then there's no way to know when the body ends */
            /* in this case we keep appending to the body until the client closes the connect per Http spec */
            hContext->stream.position += len;
        }
    }

    return UPNP_E_INCOMPLETE;
}

static char* Http_TokenizeHeader(char *buf, int32_t length)
{
    char *name, *value;
    char *pos, *end;

    pos = buf;
    end = pos + length;

    while ( pos != NULL && pos < end )
    {
        HTTP_SKIP_WHITESPACE(pos);
        name = strsep(&pos, ":");

        HTTP_SKIP_WHITESPACE(pos);
        value = strsep(&pos, "\r\n");

        /* scan to the begining of the next line */
        while ( pos != NULL && (*pos == '\r' || *pos == '\n') )
            pos++;
    }

    return pos;
}

/**************************************************************************************************
 Functions Specific To UPNP Engine 
 *************************************************************************************************/

static int32_t Http_ProcessGet(HttpContextHandle hContext)
{
    cstring_t content;
    Url uri;
    char header[UPNP_MAX_PACKET_SIZE];
    char date[UPNP_MAX_DATE_SIZE];

    /* startLine is delimited by NULL's so cmsUtl_strlen will point to the uri */
    Url_Create(&uri, Http_GetRequestUri(hContext));

    CString_Create(&content);

    assert(uri.path);
    BUPnPDevice_GetDescriptionXml(uri.path, &content);

    if ( content.length == 0 )
    {
         Http_SendResponse(hContext, HTTP_BAD_REQUEST, NULL, NULL);
    }
    else
    {
        snprintf(header, sizeof(header)-1, 
                    "CONTENT-LENGTH: %d\r\n"
                    "CONTENT-TYPE: text/xml\r\n"
                    "DATE:  %s\r\n\r\n", content.length, BUPnP_GetRfc1123Date(date, sizeof(date)));

        Http_SendResponse(hContext, HTTP_OK, header, content.bytes);
    }

    CString_Destroy(&content);
    Url_Destroy(&uri);

    return UPNP_E_SUCCESS;
}

static int32_t Http_ProcessRequest(HttpContextHandle hContext)
{
    const char *method;
    int32_t result=UPNP_E_BAD_REQUEST;

    method = Http_GetRequestMethod(hContext);
    if ( method == NULL )
        return UPNP_E_INCOMPLETE;

    if ( IMATCH_PREFIX(method, "GET") )
        result = Http_ProcessGet(hContext);
    else if ( IMATCH_PREFIX(method, "POST") )
        result = Soap_ProcessPost(hContext);
    else if ( IMATCH_PREFIX(method, "SUBSCRIBE") )
        result = Gena_ProcessSubscribe(hContext);
    else if ( IMATCH_PREFIX(method, "UNSUBSCRIBE") )
        result = Gena_ProcessUnsubscribe(hContext);
    else if ( IMATCH_PREFIX(method, "NOTIFY") )
        result = Gena_ProcessNotify(hContext);

    return result;
}

bool Http_RequestHandler(BUPnPConnectionHandle hConnection, void* args)
{
    int32_t result;
    HttpContextHandle hContext;
    UNUSED_ARGUMENT(args);

    hContext = &hConnection->context;

    result = Http_Receive(hContext);
    if ( result == UPNP_E_SOCKET_CLOSED )
    {
        /* process whatever we have left (if anything) then close the connection */
        Http_ProcessRequest(hContext);
        return false;
    }
    else if (result != UPNP_E_SUCCESS )
    {
        return false;
    }

    result = Http_ParseRequest(hContext, Http_ProcessRequest);
    if ( result == UPNP_E_SUCCESS )
    {
        return ( Http_IsPersistent(hContext) );
    }
    else if ( result == UPNP_E_INCOMPLETE )
        return true; /* need more data */
    else
    {
        return false; /* catastrophic error so close the connection */
    }
}

