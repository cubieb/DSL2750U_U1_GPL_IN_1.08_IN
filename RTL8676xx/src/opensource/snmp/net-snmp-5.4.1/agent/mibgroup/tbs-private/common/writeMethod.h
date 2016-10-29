
#ifndef WRITE_METHOD_H_
#define WRITE_METHOD_H_

typedef struct writeMethod_struct_tag
{
    unsigned char ucVar_expect_type; /*destionation OID type we defined, from vp*/
    unsigned short usAccessType; /*if this var is writable, from vp*/    

    unsigned char ucDataType;  /*the type of data to be written:1 long, 0 charpointer*/
    union
    {
	    struct /*if destination type is long, we specify its min and max value*/
	    {
	        long lMin;
	        long lMax;
	        long *pulDest; /*pointer to room where new value(long) will be saved, if it is NULL, we set value to CMM*/
	    } longRange;
	    struct /*if destination type is char*, we specify its varaible size*/
	    {
	        unsigned long ulValueSize; /*destination room size if pszDest isn't NULL, or Node size defined in config.xml if pszDest is NULL*/
	        long ulReserve;
	        char *pszDest; /*pointer to room where new value(char*) will be saved, if it is NULL, we set value to CMM*/
	    }charpSize;
    }range;

    char **pv;
    int pvLen;
    
    char *pszNodeWholePath; /*Pointer to the node path for which we will set new value to CMM*/
}writeMethod_struct_data;

static writeMethod_struct_data *pstWtMethodStruct = NULL; /*if can be allocated room when init, free it when shutdown*/

/*
  init public variable pstWriteMethodStruct for calling WriteMethod_Comm
  return: nonzero when error occurs
             zero when succeed
*/
int Init_WriteMethodStruct();
void Free_WriteMethodStruct();
int Active_WriteMethod(unsigned char ucVar_expect_type,
						  unsigned short usAccessType,
						  unsigned char ucDataType,
						   
						  long la,
						  long lb, /*when ucDataType=0, retain it to be 0*/
						  void *p,

						  char *pszNodeWholePath,
                                            char **pv,
                                            int pvLen);
int WriteMethod_Comm(int action,
                                             unsigned char *var_val,
                                             unsigned char var_val_type,
                                             size_t var_val_len, 
                                             unsigned char *statP, 
                                             oid *name, size_t length);
#endif
