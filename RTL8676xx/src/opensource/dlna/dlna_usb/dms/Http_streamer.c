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
 * $brcm_Workfile: Http_streamer.c $
 * $brcm_Revision: 8 $
 * $brcm_Date: 9/25/09 3:38p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/Http_streamer.c $
 * 
 * 8   9/25/09 3:38p ismailk
 * SW7405-3080: DMS IP Streaming (Live channel) Feature.
 * 
 * 7   7/31/09 6:34p ajitabhp
 * PR55165: Signed Unsigned warnings
 * 
 * 6   7/31/09 4:31p ajitabhp
 * PR55165: Removed Extra Prints from httpstreamer files.
 * 
 * 5   7/30/09 6:48p ajitabhp
 * PR55165: Some clips behave wierd.
 * 
 * 4   7/15/09 3:04p ajitabhp
 * PR55165: Connection Stalling
 * 
 * 3   7/10/09 1:16p ajitabhp
 * PR55165: Increasing the socket timeout to 10 seconds.
 * 
 * 2   7/9/09 5:33p ajitabhp
 * PR55165: Fix the bug for non DLNA content.
 * 
 * 2   7/9/09 5:32p ajitabhp
 * PR55165: Fix the bug for non DLNA content.
 * 
 * 2   7/9/09 5:31p ajitabhp
 * PR55165: Bug Fixes for The following:
 * 1. Some files were not detected as DLNA profile files.
 * 2. There were invalid characters passed to the client when a NON dlna
 *  file exists with DLNA files.
 * 3. The server crashed when there was a DLNA file without the DLNA
 *  profile.
* 
***************************************************************************/
#include "http_streamer.h"
#include "bsocket.h"
#include "upnp_priv.h"
#include "cstring.h"
#include "CDS.h"
#include "http.h"
#include "trace.h"

static unsigned int ConnCnt=0;
static bool HttpContentTx_NewConnectionHandler(BUPnPConnectionHandle hConnection, void* args);

#define RFC1123_DATE "%a, %d %b %Y %H:%M:%S GMT"
#define TXSZ    4*1024*1024
#ifdef HAVE_DLNA
#define MAX_DLNA_HEADER_LEN    8*1024
#endif



#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

static
BC_STATUS
HttpSendErrorResponse(HttpContextHandle    hHttpContext,
                      HttpStatusCode HttpCode)
{
    BC_STATUS sts;
    sts = Http_SendResponse(hHttpContext,HttpCode,NULL,NULL);
    if(sts  != BC_UPNP_E_SUCCESS)
    {
        BcmLogMsg("\n Sending HTTP Response failed with err:%d",sts);
#ifdef WIN32
        BcmLogMsg( "Http_Send - Error %d sending body", WSAGetLastError());
#endif
    }
    return sts;
}

static
HttpStatusCode
HttpProcessRangeField(    HttpContextHandle    hHttpContext,
                        pbcm_upnp_object    pObj,
                        unsigned int        *StartOffset,
                        unsigned int        *EndOffset,
                        unsigned int        *XferSz)                
{
    const char *pRangeField=NULL;
    char *x=NULL,*ByteRangeStr=NULL,*StByteOff=NULL,*EndByteOff=NULL;
    unsigned int ItemSz=0;

    if(!pObj) 
        return HTTP_PRECONDITION_FAILED;

    *StartOffset=0;
    *EndOffset=0;
    ItemSz = ITEM_GET_SIZE(pObj);
    pRangeField = Http_GetHeaderValue(hHttpContext, "Range");    
    if(!pRangeField)
    {
        *XferSz = ItemSz;    
        *EndOffset = ItemSz - 1;
        return HTTP_NOT_FOUND;
    }

#ifdef HAVE_DLNA
    if(!HAS_RANGE_SUPPORT(pObj))
        return HTTP_NOT_ACCEPTABLE; /*Object does not support Range Request*/
#endif

    x = strdup(pRangeField);
    ByteRangeStr = strtok(x,"=-");
    ByteRangeStr = strtok(NULL,"=");
    StByteOff = strtok(ByteRangeStr,"-");

    if(!StByteOff) 
    {
        free(x);
        return HTTP_BAD_REQUEST; /*Range Header without start byte offset*/
    }

    *StartOffset = atol(StByteOff);
    EndByteOff = strtok(NULL,"-");

    /*HTTP RFC-2616:A missing value after "-" means the whole file-1*/
    if(EndByteOff) 
    {    
        *EndOffset = atol(EndByteOff);
        /*End Offset Present But Greater Case*/ /*HTTP RFC-2616*/
        if(*EndOffset > ItemSz)
        {
            *EndOffset = ItemSz - 1;
            free(x);
            return HTTP_REQUEST_RANGE_INVALID;
        }
    }else{
        /*EndOffset Not Present Case*//*HTTP RFC-2616*/
        *EndOffset = ItemSz-1;     
    }

    /*Validate the request*/
    if( (*StartOffset < 0) || 
        (*StartOffset > ItemSz) || (*EndOffset < 0) || (*StartOffset > *EndOffset))
    {
        free(x);
        return HTTP_BAD_REQUEST;
    }

    *XferSz = (*EndOffset - *StartOffset) + 1;         

    if(*XferSz > ItemSz)
    {
        BcmLogMsg("RangeRequst Err StOff:%x End:%x ComputedXferSz:%x\n",
            *StartOffset,*EndOffset,XferSz);
        free(x);
        return HTTP_REQUEST_RANGE_INVALID;
    }

    free(x);
    return HTTP_OK;
}

#ifdef HAVE_DLNA

static 
int 
GetPlaySpeed(const char * PlaySpeedField)
{
    int RetVal=-1;
    char *x=NULL,*temp=NULL;
    x = strdup(PlaySpeedField);    
    temp = strtok(x,"=");
    temp = strtok(NULL,"=");
    RetVal = atoi(temp);
    free(x);
    return RetVal;
}

static
HttpStatusCode 
HttpProcessDLNAFileds(HttpContextHandle        hContext,
                      pbcm_upnp_object        pObj,
                      char                    *Response,
                      unsigned int            ResSize,
                      unsigned int            *UsedSz)
{
    const char *pszTemp=NULL;
    char *pNextRes=NULL;
    unsigned int len=0,bytes_left;
    bytes_left = ResSize;
    pNextRes = Response;
    
    *UsedSz=0;

    /*
     * HACK: We needs a get next header function from framework to verify 
     * all the headers in the HTTP request are the ones that we are supporting.
     * We are just using this hack to pass CTT.
     */
    pszTemp = Http_GetHeaderValue(hContext, "Background");    
    pszTemp = Http_GetHeaderValue(hContext, "Interactive");    
    pszTemp = Http_GetHeaderValue(hContext, "Streaming");    
    pszTemp = Http_GetHeaderValue(hContext, "realTimeInfo.dlna.org");    
    if(pszTemp)
    {
        BcmLogMsg("UnKnown Header Requests\n");
        return HTTP_BAD_REQUEST;
    }

    pszTemp = Http_GetHeaderValue(hContext, "getcontentFeatures.dlna.org");    
    if(pszTemp)
    {
        if(atol(pszTemp)!=1 )
        {
            BcmLogMsg("Error In Filed getcontentFeatures.dlna.org\n");
            return HTTP_BAD_REQUEST;

        }else{
            len = snprintf(    pNextRes,
                            bytes_left,
                            "contentFeatures.dlna.org: %s\r\n",
                            GET_DLNA_FEATURES(pObj));
            pNextRes += len;
            bytes_left -= len;
        }
    }

    pszTemp = Http_GetHeaderValue(hContext, "PlaySpeed.dlna.org");    
    if(pszTemp)
    {
        int PlaySpeed=GetPlaySpeed(pszTemp);
        if(PlaySpeed & GET_PLAY_SPEED(pObj)) /*We support This Play Speed*/
        {
            /*We support This Play Speed*/
            len = snprintf(    pNextRes,
                            bytes_left,
                            "PlaySpeed.dlna.org:speed=%d \r\n",
                            GET_PLAY_SPEED(pObj));
            pNextRes += len;
            bytes_left -= len;
        }else{
            /*We do not support This Play Speed*/
            BcmLogMsg("UnSupported Play Speed Request\n");
            return HTTP_NOT_ACCEPTABLE;
        }
    }

    pszTemp = Http_GetHeaderValue(hContext, "TimeSeekRange.dlna.org");
    if(pszTemp)
    {
        if(!HAS_TIMESEEK_SUPPORT(pObj))
        {
            BcmLogMsg("Streaming Transfer Mode Not Supported");
            return HTTP_NOT_ACCEPTABLE;
        }
        BcmLogMsg("!!Need TO handle Time Seek Operations Here!!");
        BcmLogMsg("!!Need TO handle Time Seek Operations Here!!");
    }
    
    pszTemp = Http_GetHeaderValue(hContext, "transferMode.dlna.org");
    if(pszTemp)
    {
        if(IMATCH(pszTemp,"Streaming"))
        {
            //if(IS_DLNA_CLASS_IMAGE(pObj))
            //{
            //    BcmLogMsg("Streaming Transfer Mode Not Supported");
            //    return HTTP_NOT_ACCEPTABLE;
            //}

            if(!IS_STREAMING_XFER_MODE(pObj))
            {
                BcmLogMsg("Streaming Transfer Mode Not Supported");
                return HTTP_NOT_ACCEPTABLE;
            }

            len = snprintf(    pNextRes,
                            bytes_left,
                            "transferMode.dlna.org: Streaming \r\n");
            pNextRes += len;
            bytes_left -= len;

        }else if(IMATCH(pszTemp,"Interactive")){

            /*if((IS_DLNA_CLASS_AUDIO(pObj)) 
               ||(IS_DLNA_CLASS_AV(pObj)))
            {
                BcmLogMsg("Streaming Transfer Mode Not Supported");
                return HTTP_NOT_ACCEPTABLE;
            }*/

            if(!IS_INTERACTIVE_XFER_MODE(pObj))
            {
                BcmLogMsg("Interactive Transfer Mode Not Supported");
                return HTTP_NOT_ACCEPTABLE;
            }

            len = snprintf(    pNextRes,
                            bytes_left,
                            "transferMode.dlna.org: Interactive \r\n");
            pNextRes += len;
            bytes_left -= len;

        }else if(IMATCH(pszTemp,"Background")){

            if(!IS_BACKGROUND_XFER_MODE(pObj))
            {
                BcmLogMsg("Background Transfer Mode Not Supported");
                return HTTP_NOT_ACCEPTABLE;
            }

            len = snprintf(    pNextRes,
                            bytes_left,
                            "transferMode.dlna.org: Background \r\n");
            pNextRes += len;
            bytes_left -= len;

        }else{
            BcmLogMsg("Invalid Transfer Mode In Request");
            return HTTP_BAD_REQUEST;
        }    
    }

    pszTemp = Http_GetHeaderValue(hContext, "pragma");
    if(pszTemp)
    {
        //PRAGMA: ifoFileURI.dlna.org="http://192.168.0.1:8080/IFO_101.ifo"
        if(IMATCH(pszTemp,"getIfoFileURI.dlna.org"))
        {
            len = snprintf(    pNextRes,
                            bytes_left,
                            "PRAGMA: ifoFileURI.dlna.org= \"%s\" \r\n",ITEM_GET_FILE_URL(pObj));
            pNextRes += len;
            bytes_left -= len;
        }
    }

    *UsedSz = ResSize - bytes_left;
    return HTTP_OK;
}
#endif

static
bool
SendBufferToSkt(HttpContextHandle hContext,
                char    *Buffer,
                size_t    BuffSz,
                char StallConn)
{
    size_t    DoneSz=0,XferSz=0;
    fd_set fds;
    struct timeval timeout;
    unsigned int Cnt=0,SendFlags=0;
    int SendSz=0,SelectRet=0;

    if(!BuffSz || !BuffSz)
        return false;

#ifdef LINUX
    SendFlags = MSG_MORE | MSG_NOSIGNAL;
#endif

    DoneSz =0;
    XferSz = BuffSz;
    while(DoneSz < BuffSz)
    {
        /*We need it this way for Linux compatibility.*/
        FD_ZERO (&fds);
        FD_SET (hContext->s, &fds);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;


        /* 
         * We are supporting Connection Stalling.
         * But for How much time the client stalls read??
         * We take 2 minutes as default. CTT passes with two 
         * minutes and is a good time for closing the connections.
         */

        if(StallConn) timeout.tv_sec=120;    
        SelectRet = select(FD_SETSIZE,
                            NULL,
                            &fds,
                            NULL,
                            &timeout);
        if(!SelectRet)
        {
            /*Selection Timeout*/
            BcmLogMsg("%d:STO\n",hContext->s); 
            Cnt++;
            if(Cnt > 2)    return false;
            continue;/*Try Again*/
        }
        
        if(SelectRet < 0)
        {
            /*Select Call Failed- Not Timeout*/
            BcmLogMsg("%d:SF",hContext->s); 
            return false;
        }
        
        /*Select Succeeded*/
        /*Try to send the whole remaining buffer*/
        SendSz = send(hContext->s, 
                        Buffer, 
                        XferSz, 
                        SendFlags);
        if(SendSz < 0)
        {
            BcmLogMsg("\n ***** Sending Buffer failure ***** \n");
            return false;
        }

        /*Update For Next Iteration*/
        DoneSz += SendSz;
        Buffer+=SendSz;
        XferSz = BuffSz - DoneSz;
        SendSz=0;
    }

    return true;
}

static
int
HttpOptimizeSktSz(HttpContextHandle hContext,
                  unsigned int NeededSz)
{
    unsigned int SendSz=0;
    int retVal=0;
#ifdef WIN32
    int paramSz = sizeof(int);
#else
    unsigned int paramSz = sizeof(int);

#endif
    retVal = getsockopt(hContext->s, 
                        SOL_SOCKET, 
                        SO_SNDBUF, 
                        (char *) &SendSz, 
                        &paramSz);
    if ( retVal == -1) 
    {
        BcmLogMsg("Failed To Get Socket Opt \n");
        return -1;
    }
 
 // Windows Single core 1.86 Ghz 2GB ram.
 // On windows we have seen that we are able to set any size we want
 //even 4 MB. But if the size is kept a multiple of 64K the CPU utilization 
 //will be less. 
 //for 64K size the CPU is about 10%
 //128K the CPU is about 18%
 //for 256 K the CPU is about 28%
 //
    if(SendSz < NeededSz)
        NeededSz = SendSz * 4;

    retVal = setsockopt(hContext->s,
                        SOL_SOCKET,
                        SO_SNDBUF,
                        (char *)&NeededSz,
                        paramSz);
    if(retVal == -1)
    {
        BcmLogMsg("Failed To Set Socket Sz:%x \n",NeededSz);
        return -1;
    }

    retVal = getsockopt(hContext->s, 
                            SOL_SOCKET, 
                            SO_SNDBUF, 
                            (char *)&SendSz, 
                            &paramSz);
    if ( retVal == -1) 
    {
        BcmLogMsg("Failed To Get Socket Opt\n");
        return -1;
    }

    return SendSz;
}

static 
bool
HttpXferFile(HttpContextHandle hContext,
             FILE            *FileHandle,        
             unsigned int    StartOffset,
             unsigned int    EndOffset,
             size_t            FileSz,
             size_t            XferSz,
             char            StallConn)
{
    bool    RetSts=TRUE;
    char *XferBuff=NULL;
    int ReadSz=0, ChunkSz=0;
    size_t DoneSz=0;
    
    if(!FileHandle || !XferSz ||  !FileSz || !EndOffset||
       (StartOffset >= EndOffset))
        return false;

    ChunkSz=min(TXSZ,XferSz);
    ChunkSz = HttpOptimizeSktSz(hContext,ChunkSz);

    if(ChunkSz == -1) ChunkSz=min(TXSZ,XferSz); /*Use whatever the send buffer size available*/
    XferBuff = malloc(ChunkSz);
    if(!XferBuff)
    {
        BcmLogMsg("%s:Memory Allocation Failed",__FUNCTION__);
        return false;
    }

    if(fseek(FileHandle,StartOffset,SEEK_SET))
    {
        BcmLogMsg("%s:Cannot Seek File To Start Position",__FUNCTION__);
        free(XferBuff);
        return false;
    }

    while(DoneSz < XferSz)
    {
        ReadSz = fread(XferBuff,1,ChunkSz,FileHandle);
        if(ReadSz)
        {
            RetSts = SendBufferToSkt(hContext,XferBuff,ReadSz,StallConn);
            if (false == RetSts) break;
            DoneSz += ReadSz;
        }


        if(ReadSz <= ChunkSz)
        {
            if(feof(FileHandle))
            {
                BcmLogMsg("File Transfer Completed\n");
                RetSts = true;
                break;
            }else if(ferror(FileHandle)){
                BcmLogMsg("File Transfer Error\n");
                RetSts = false;
                break;
            }else if(DoneSz == XferSz){
                BcmLogMsg("File Transfer Completed\n");
                RetSts = true;
            }
        }
    }

    free(XferBuff);
    return RetSts;
}

static 
int32_t 
HttpTxProcessHeadOrGet(HttpContextHandle hContext, 
                       bool        XferFileData)    /*If this is a get request transfer content with header*/
{
    const char            *uri;
    char                PartialXfer=0,freeMime=0,header[UPNP_MAX_PACKET_SIZE],date[UPNP_MAX_DATE_SIZE];
    unsigned int        StartOff=0,EndOff=0,XFerSz=0, pos=0,len=0;
    pbcm_upnp_object    pObj=NULL;
    const char            *pszTemp =NULL;
    size_t                bytes_left=UPNP_MAX_PACKET_SIZE;
    HttpStatusCode        HttpSts;
    BC_STATUS            sts =BC_UPNP_E_SUCCESS;
    FILE                *fp=NULL;
    char                StallConn=0;

    memset(header,0,sizeof(header));

    /* startLine is delimited by NULL's so cmsUtl_strlen will point to the uri */
    uri = Http_GetRequestUri(hContext);
    pObj = GetFileFromUrl(uri);
    if(pObj== NULL )
    {
        return HttpSendErrorResponse(hContext, HTTP_BAD_REQUEST);
    }

    pos = 0;
    bytes_left = UPNP_MAX_PACKET_SIZE;

#ifdef HAVE_DLNA
    if(HAS_DLNA_PROFILE(pObj))
        pszTemp = GET_DLNA_MIME(pObj);
#endif
    
    if(!pszTemp){
        /*Uses strdup need to free after use*/
        pszTemp = GET_MIME_FROM_PROTO(pObj);
        freeMime=1;
    }

    len = snprintf(header, bytes_left, 
                    "CONTENT-TYPE: %s\r\n"
                    "SERVER: Broadcom DMS\r\n",
                    pszTemp);
    if(freeMime) free((void *)pszTemp);

    pos += len;
    bytes_left -= pos;

    HttpSts = HttpProcessRangeField(hContext,
                                    pObj,
                                    &StartOff,
                                    &EndOff,
                                    &XFerSz);
    
    if(HttpSts == HTTP_NOT_FOUND)    
    {
        /*This means that the Range Header Was not found*/
        len = snprintf(&header[pos], 
                        bytes_left, 
                        "CONTENT-LENGTH: %d\r\n",
                        ITEM_GET_SIZE(pObj));

    }else if(HttpSts == HTTP_OK){    
        /*Range Header Was Found OK*/
        len = snprintf(&header[pos], 
                        bytes_left, 
                        "CONTENT-LENGTH: %d\r\n"
                        "CONTENT-RANGE: bytes %d-%d/%d\r\n",
                        XFerSz,StartOff,EndOff,
                        ITEM_GET_SIZE(pObj));
        PartialXfer=1;
    }else if(HttpSts == HTTP_REQUEST_RANGE_INVALID){
        /*Valid Range Header With Invalid Request Range*/
        return HttpSendErrorResponse(hContext, HttpSts);
    }else {                        
        /*Unparseable Range Header*/
        return HttpSendErrorResponse(hContext, HTTP_BAD_REQUEST);
    }

    pos += len;
    bytes_left -= pos;

#ifdef HAVE_DLNA
    HttpSts = HttpProcessDLNAFileds(hContext,
                                        pObj,
                                        &header[pos],
                                        bytes_left,
                                        &len);
    if(HTTP_OK != HttpSts)
    {
        return HttpSendErrorResponse(hContext, HttpSts);
    }

    if (HAS_CONN_STALL(pObj)) StallConn=1;
    pos += len;
    bytes_left -= pos;
#endif 
    
    fp = fopen(pObj->u.item_info.u.file.fullpath,"rb");
    if(fp==NULL)
    {
        BcmLogMsg("\n File not found");
        return Http_SendResponse(hContext, HTTP_NOT_FOUND, NULL, NULL);
    }

    /*
     * Everything Went well. The request was OK and we have access to file.
     * Send the header response and transfer data if you have to
     */
    snprintf(&header[pos],
                bytes_left,
                "DATE:  %s\r\n\r\n",
                BUPnP_GetRfc1123Date(date, sizeof(date)));


    if(PartialXfer)
        HttpSts = HTTP_PARTIAL_XFER;

    sts = Http_SendResponse(hContext, HttpSts, header, NULL);

    if(sts!=BC_UPNP_E_SUCCESS) 
        BcmLogMsg("Sending HTTP Response Header err:%d\n",sts);

    /*
     * Transfer the File Content If you have to
     */
    if(XferFileData && XFerSz)
    {
        // StartOff has the starting position of the file
        // EndOff has the starting position of the transfer
        // XferSz has the size that we need to transfer.

        if(false == HttpXferFile(hContext,fp,StartOff,
                                 EndOff,ITEM_GET_SIZE(pObj),
                                 XFerSz,
                                 StallConn))
        {
            BcmLogMsg("File Transfer Failure\n");
        }
    }

    fclose(fp);
    return sts;
}

static
bool
HttpValidateRequest(HttpContextHandle hContext)
{
    HttpVersion    HttpVer;
    const char *pSzTemp=NULL,*pSzBody=NULL;
    HttpVer = Http_GetVersion(hContext);
    pSzBody = Http_GetBody(hContext);
    if(HttpVer == HttpVersion_11)
    {    
        pSzTemp = Http_GetHeaderValue(hContext,"Host");
        if(!pSzTemp) return false; /* HTTP-1.1 must have the Host header.*/
            
    }else if(HttpVer == HttpVersion_10){
        return true;
    }else{
        return false;
    } 

    return true;
}

static 
int32_t 
HttpContentTx_ProcessRequest(HttpContextHandle hContext)
{
    const char *method;
    int32_t result=UPNP_E_BAD_REQUEST;

    method = Http_GetRequestMethod(hContext);
    
    if ( method == NULL )
        return UPNP_E_INCOMPLETE;

    if(!HttpValidateRequest(hContext))
    {
        return HttpSendErrorResponse(hContext, HTTP_BAD_REQUEST);
    }

    if ( IMATCH_PREFIX(method, "GET") )
    {
        result = HttpTxProcessHeadOrGet(hContext,true);
    }else if ( IMATCH_PREFIX(method, "HEAD") ){
        result = HttpTxProcessHeadOrGet(hContext,false);
    }
    return result;
}

static
void
IncConnCnt()
{
    ConnCnt++;
    BcmLogMsg("= Conn:%d = ",ConnCnt);
}

static
void
DecConnCnt()
{
    ConnCnt--;
    BcmLogMsg("= Conn:%d =",ConnCnt);
}

void
HttpContentTx_ProcessReceive(void *s)
{
    int32_t result = UPNP_E_INCOMPLETE;
    HttpContextHandle hContext;
    hContext = (HttpContext*) malloc(sizeof(HttpContext));
    if(hContext==NULL)
    {
        DecConnCnt();
        BcmLogMsg("\n malloc failed..");    
        return;
    }
    memset(hContext, 0, sizeof(*hContext));
    Http_CreateContext(hContext);
    hContext->s = (SOCKET)s;
    BcmLogMsg("\n\n");

    while(1)
    {
        result = Http_Receive(hContext);
        if(result != UPNP_E_SUCCESS)
        {
            BcmLogMsg("%s: Failed\n","Http_Recieve");
            if ( result == UPNP_E_SOCKET_CLOSED )
            {
                /*Process Whatever We Have*/
                result = HttpContentTx_ProcessRequest(hContext);
            }
            /*Receive Failed....better to close the socket.*/
            shutdown(hContext->s,2);
            closesocket(hContext->s);
            break;
        }

        BcmLogMsg("%s",hContext->stream.buffer);

        result = Http_ParseRequest(hContext, 
                                    HttpContentTx_ProcessRequest);

        if (result == UPNP_E_BAD_REQUEST)
        {
            HttpSendErrorResponse(hContext, 
                                HTTP_BAD_REQUEST);

            Http_DestroyContext(hContext);
            free(hContext);
            DecConnCnt();
            return;
        }

        if(result == UPNP_E_INCOMPLETE) 
            continue;/*You Need More Data From Socket. Read Next Chunk And Try Again*/
        
        if(result == UPNP_E_SUCCESS)
        {
            /*If the connection NOT persistent then close socket*/
            if(!Http_IsPersistent(hContext)) 
            {
                shutdown(hContext->s,2);
                closesocket(hContext->s);
                break;
            }
        }else{
            /*In Error condition Always close the socket*/            
            shutdown(hContext->s,2);
            closesocket(hContext->s);
            break;
        }
    }

    Http_DestroyContext(hContext);
    free(hContext);
    DecConnCnt();
    return;
}


static 
bool 
HttpContentTx_NewConnectionHandler(BUPnPConnectionHandle hConnection, 
                                   void* args)
{
    SOCKET s;
    bthread *thHandle=NULL;

    UNUSED_ARGUMENT(args);
    
    s = accept(hConnection->context.s, NULL, NULL);
    BcmLogMsg("\nRequest on socket %d, accepted on %d",
                    hConnection->context.s,    s);
        
    if (s != -1)
    {
        IncConnCnt();
        thHandle = bthread_create(HttpContentTx_ProcessReceive,(void *)s);
        if(!thHandle)
            BcmLogMsg("%s:Failed to create and start thread\n",__FUNCTION__);
        else 
            BcmLogMsg("%s:done\n",__FUNCTION__);
    }
    
    return TRUE;
}

BUPnPError 
http_streamer_init(char* IpAddr, 
                  unsigned short PortNum,
                  BUPnPConnectionHandle *pContentTxListner)
{
    SOCKET s;
    
    s = BSocket_CreateTcpListener(IpAddr, PortNum);
    if ( s <= 0 )
        return UPNP_E_SOCKET;

    *pContentTxListner = BUPnP_AddConnection(s, 0, HttpContentTx_NewConnectionHandler, NULL);
    return 0;
}


BUPnPError 
http_streamer_close(BUPnPConnectionHandle pContentTxListner)
{
    BUPnP_RemoveConnection(pContentTxListner);
    return 0;
}










