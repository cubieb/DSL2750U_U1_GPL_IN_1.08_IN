
#include "imgbuilder_ex.h"
int main(int argc, char *argv[])
{
	int i;
	int ret;
	ST_IMAGE_INFO m_stImgInfo;
	struct  sys_cfg m_stSysConfig;
	struct  image_tail	m_stImgTail;


	memset(&m_stImgInfo, 0, sizeof(m_stImgInfo));
	memset(&m_stSysConfig, 0, sizeof(m_stSysConfig));
	memset(&m_stImgTail, 0, sizeof(m_stImgTail));	

	for(i=1;i<argc;i++)
		debug("%s\n",argv[i]);

	ret = AnalyseParameter(argc, argv, &m_stImgInfo,  &m_stSysConfig);
	if( ret == 1 )    /* if analyse parameter fail,exit  */
	{    
	    if(m_stImgInfo.uImageType == TYPE_SINGLE_BIN)
	    {
            if(CreateImage(&m_stImgInfo, &m_stSysConfig))
            {
                m_stImgInfo.uImageType = TYPE_SINGLE_IMG;
                if(CreateImage(&m_stImgInfo, &m_stSysConfig))
                {
                    printf( "Generate BIN file and IMG file successfully!\n" );
                    debug( "BIN file checksum: 0x%x\n" , crc_checksum.bin_file_checksum);
                    debug( "IMG file checksum: 0x%x\n" ,  crc_checksum.img_file_checksum );
                }
            }
	    }
	    else if(m_stImgInfo.uImageType == TYPE_DUAL_BIN)
	    {
            if(CreateImage(&m_stImgInfo, &m_stSysConfig))
            {
                m_stImgInfo.uImageType = TYPE_DUAL_IMG;
                if(CreateImage(&m_stImgInfo, &m_stSysConfig))
                {
                    printf( "Generate BIN file and IMG file successfully!\n" );
                    debug( "BIN file checksum: 0x%x\n" , crc_checksum.bin_file_checksum);
                    debug( "IMG file checksum: 0x%x\n" ,  crc_checksum.img_file_checksum );
                }
            }
	    }
	    else
	    {
            m_stImgInfo.uImageType = TYPE_BACKUP_BIN;
            if(CreateBackupImage(&m_stImgInfo, &m_stSysConfig))
            {
                m_stImgInfo.uImageType = TYPE_BACKUP_IMG;
                if(CreateBackupImage(&m_stImgInfo, &m_stSysConfig))
                {
                    printf( "Generate BIN file and IMG file successfully!\n" );
                    debug( "BIN file checksum: 0x%x\n" , crc_checksum.bin_file_checksum);
                    debug( "IMG file checksum: 0x%x\n" ,  crc_checksum.img_file_checksum );
                }
            }
	    }
	}
	else if( ret == 0 )
	{
		printf("Generate BIN file and IMG file failed!\n");
	}
	
	return 0 ;
}
