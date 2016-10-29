#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "cmmif.h"

#include "writeMethod.h"


/*
  init public variable pstWtMethodStruct for calling WriteMethod_Comm
  return: nonzero when error occurs
             zero when succeed
*/
int Init_WriteMethodStruct()
{
    if(pstWtMethodStruct) return 0;
    
    pstWtMethodStruct = (writeMethod_struct_data*)malloc(sizeof(writeMethod_struct_data));
    if(!pstWtMethodStruct) return -1;
    
    pstWtMethodStruct->pszNodeWholePath = NULL;

    CMM_Init();
    
    return 0;
}

void Free_WriteMethodStruct()
{
    if(pstWtMethodStruct)
    {
        free(pstWtMethodStruct);
        pstWtMethodStruct = NULL;
    }
    CMM_Free();
}

/*p is output*/
int Active_WriteMethod(unsigned char ucVar_expect_type,
						   unsigned short usAccessType,
						   unsigned char ucDataType,
						   
						   long la,
						   long lb, /*when ucDataType=0, retain it to be 0*/
						   void *p,

						   char *pszNodeWholePath,
                                             char **pv,
                                             int pvLen)
{
    if(!pstWtMethodStruct) return -1;

    pstWtMethodStruct->ucVar_expect_type = ucVar_expect_type;
    pstWtMethodStruct->usAccessType = usAccessType;
    pstWtMethodStruct->ucDataType = ucDataType;
    pstWtMethodStruct->pszNodeWholePath = pszNodeWholePath;
    pstWtMethodStruct->pv = pv;
    pstWtMethodStruct->pvLen = pvLen;    
    
    if(ucDataType == 1) /*long*/
    {
        pstWtMethodStruct->range.longRange.lMin = la;
        pstWtMethodStruct->range.longRange.lMax = lb;
        pstWtMethodStruct->range.longRange.pulDest = (unsigned long*)p;
    }
    else if(ucDataType == 0) /*char pointer*/
    {
        pstWtMethodStruct->range.charpSize.ulValueSize = la;
        pstWtMethodStruct->range.charpSize.ulReserve = lb;
        pstWtMethodStruct->range.charpSize.pszDest = (char*)p;
    }
    else return -1;
}

int WriteMethod_Comm(int action,
                     unsigned char *var_val,
                     unsigned char var_val_type,
                     size_t var_val_len, 
                     unsigned char *statP, 
                     oid *name, size_t length)
{
     if(!pstWtMethodStruct) return SNMP_ERR_GENERR;

     switch(action)
     {
         case RESERVE1:
              /*writable ?*/
              if(pstWtMethodStruct->usAccessType != RWRITE) return SNMP_ERR_NOTWRITABLE;

              /*oid type match?*/
			  if (var_val_type != pstWtMethodStruct->ucVar_expect_type) return SNMP_ERR_WRONGTYPE;

			  /*value range is valid?*/
			  if(pstWtMethodStruct->ucDataType == 1) /* long */
			  {
				   long lNewVal = *((long *) var_val);
				   if (var_val_len > sizeof(unsigned long)) return SNMP_ERR_WRONGLENGTH;

				   if( pstWtMethodStruct->range.longRange.lMax > pstWtMethodStruct->range.longRange.lMin)
				   {
				       if(lNewVal > pstWtMethodStruct->range.longRange.lMax || 
				               lNewVal < pstWtMethodStruct->range.longRange.lMin)
				       return SNMP_ERR_WRONGVALUE;
				   }
			  }
			  else if(pstWtMethodStruct->ucDataType == 0) /* char* */
			  {
				   if(var_val)
				   {
				       if(var_val_len > pstWtMethodStruct->range.charpSize.ulValueSize)
				            return SNMP_ERR_WRONGLENGTH;
				   }
				   else 
				   {
				       return SNMP_ERR_WRONGLENGTH;
				   }
			  }
			  else 
			  {
			      return SNMP_ERR_GENERR;  
			  }
			  	
              break;
			  
         case RESERVE2:
               break;
			   
         case ACTION:
              if(pstWtMethodStruct->ucDataType == 1) /*long*/
              {
                  if(pstWtMethodStruct->range.longRange.pulDest) /*return to a variable*/
                  {
                      *pstWtMethodStruct->range.longRange.pulDest = *((unsigned long *) var_val);
                  }
                  else /*set to CMM*/
                  {
                      unsigned long ulValue = *((unsigned long *) var_val);
					  if(CMM_SetUlong(pstWtMethodStruct->pszNodeWholePath, ulValue,
		                                                  pstWtMethodStruct->pv, 
		                                                  pstWtMethodStruct->pvLen) != CMM_SUCCESS)
					  {
					     SNMPA_TRACE("Error returned after calling CMM_SetUlong. node:%s, value:%lu\n", pstWtMethodStruct->pszNodeWholePath, ulValue);
					     return SNMP_ERR_GENERR;
					  }
                  }
              }
              else if(pstWtMethodStruct->ucDataType == 0) /*char pointer*/
              {
                  if(pstWtMethodStruct->range.charpSize.pszDest) /*return to a variable*/
                  {
                      strncpy(pstWtMethodStruct->range.charpSize.pszDest, var_val, pstWtMethodStruct->range.charpSize.ulValueSize);
                  }
                  else /*set to CMM*/
                  {
                      char my_var_val[257] = {0};
                      if(var_val_len > 0)
                      {
                          if(var_val_type == ASN_IPADDRESS)
                          {
                            if(intIPAddr_TO_CHARS(my_var_val, *((unsigned long *) var_val)) == NULL) return SNMP_ERR_WRONGVALUE;
                          }
                          else 
                          strncpy(my_var_val, var_val, var_val_len);
                      }
					  if(CMM_SetStr(pstWtMethodStruct->pszNodeWholePath, my_var_val, 
		                                              pstWtMethodStruct->pv, pstWtMethodStruct->pvLen) != CMM_SUCCESS)
					  {
					      SNMPA_TRACE("Error returned after calling CMM_SetStr. node:%s, value:%s\n", pstWtMethodStruct->pszNodeWholePath, my_var_val);
					      return SNMP_ERR_GENERR;
					  }
                  }
              }
              else return SNMP_ERR_GENERR;
         break;
         case UNDO:
         break;
         case COMMIT:
         break;
         case FREE:
         break;
     }

     return SNMP_ERR_NOERROR;
}

