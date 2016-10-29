
#include "cfg_comm.h"
#include "cfg_file.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif


int main(int argc, char *argv[])
{
    char *pcBuf = NULL;
    unsigned long ulLen = 0;
    CFG_RET ret = CFG_OK;

    if (argc < 2)
    {
        CFG_ERR(-1, "Please Input the source xml file...");
        return -1;
    }

    /* 读取文件 */
    ret = CFG_ReadFile(argv[1], &pcBuf, 0, 0, &ulLen);
    CFG_COMM_ERR_PROC(ret, "%s", argv[1]);

    /* 调用函数写成flash文件 */
    ret = CFG_SaveCurCfg(argv[2], pcBuf, ulLen);
    free(pcBuf);
    CFG_COMM_ERR_PROC(ret, "%s", argv[3]);

    return 0;
}






#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


