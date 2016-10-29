#ifndef _SCAN_DIR_H_
#define _SCAN_DIR_H_
//#include "bcm_types.h"
/* 
 * The Callback that the caller of the function
 * can supply to this API for notification when 
 * a requested file is found.
 */

typedef struct _FOUND_FILE_INFO_{
    char    *FileName;                        /* Full Name Of File*/
    char    *FullFilePath;                    /* Full Path Of File, Including Name*/
    size_t    FileSize;                        /* Size of the file.*/
}FOUND_FILE_INFO,*PFOUND_FILE_INFO;

typedef char    
(*NEW_FILE_FOUND)    (void *,                /*The context passed while stating the scan*/
                     PFOUND_FILE_INFO);        /*File information to be conveyed*/    

char
ScanDir(char *StartDir,                        /* The Root for starting the Scan*/
        char ScanSubDir,                    /* Scan The Sub Directories*/
        char *Extension,                    /* The Files To Report eg. mpg*/
        NEW_FILE_FOUND pfnFoundFile,        /* The callback function to call when the matched file is found*/
        void            *cb_context);            /* Context To be passed to the call back*/


#endif


