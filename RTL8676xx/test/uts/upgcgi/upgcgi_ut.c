#include <stdarg.h>
#include <assert.h>
#include "Automated.h"

#include "common.h"
#include "upg_cgi.h"
#include "upgcgi_ut.h"


#define TEST_TRACE(param...)        printf("\n<UT, %s, %d> ", __FUNCTION__, __LINE__); printf(param);
//#define TEST_TRACE(param...)   
//extern char sVarName[MAX_PATH] ; 
extern char sDivider[MAX_LINE] ;
//----------------------begin--------------------------------------------------------------------------
void ResetEnv()
{
	fail_MSG_CreateMessage=0 ;

	fail_MSG_ReleaseMessage=0;
	fail_MSG_SendMessage=0 ;
	fail_MSG_ReceiveMessage=0;
	fail_MSG_RegModule=0;
 	fail_MSG_UnregModule=0;
 	fork_result =1;
	var_name_index=0;
	post_content_index=0;
	content_type_index=0;
	msg_result=0;
	fail_open=0;
	read_result=0;
	fail_fprintf=0;
	fail_fclose=0;
	setenv("REQUEST_METHOD", "POST", 1 );
	setenv("CONTENT_LENGTH", "50", 1 );
}


void UT_UPGCGI_Main_001()
{
	ResetEnv();
	fail_fopen=1;
	CU_ASSERT(UPGCGI_Main() == FOPEN_LOG_ERROR);
	
}


void UT_UPGCGI_Main_002()
{
	ResetEnv();
	fail_MSG_RegModule=1;
	CU_ASSERT(UPGCGI_Main() == INIT_MODULE_ERROR);
	
}

void UT_UPGCGI_Main_003()
{
	ResetEnv();
	setenv("REQUEST_METHOD", "abc", 1 );
	CU_ASSERT(UPGCGI_Main() == REQUEST_METHOD_ERROR);
	
}

void UT_UPGCGI_Main_004()
{
	//int iRet;
	ResetEnv();
	//setenv("REQUEST_METHOD", "POST", 1 );
	setenv("CONTENT_LENGTH", "10", 1 );
	var_name_index=1;
	//iRet= UPGCGI_Main();
	//printf("########%d#######\n", iRet);
	CU_ASSERT(UPGCGI_Main() == MIN_CONTENT_LENGTH_ERROR);
	
}


void UT_UPGCGI_Main_005()
{	
	ResetEnv();
		
	var_name_index=2;
	fork_result=-1;
	CU_ASSERT(UPGCGI_Main() == UPG_GENERAL_ERROR);
	
}

void UT_UPGCGI_Main_006()
{	
	ResetEnv();

	var_name_index=3;
	
	CU_ASSERT(UPGCGI_Main() == INVALID_VARIABLE_ERROR);
	
}



void UT_UPGCGI_Main_007()
{	
	ResetEnv();

	var_name_index=3;
	fail_MSG_UnregModule=1;
	CU_ASSERT(UPGCGI_Main() == DESTROY_MODULE_ERROR);
	
}

//添加正常的流程。
void UT_UPGCGI_Main_008()
{
	
	ResetEnv();
	setenv("CONTENT_LENGTH", "100", 1 );
	var_name_index=1;
	//
	content_type_index=0;
	strcpy(sDivider, "#");
	post_content_index=0;
	//
	fork_result=1;	
	msg_result=0;
	
	//
	CU_ASSERT(UPGCGI_Main() == UPG_SUCCESS);
	
}

void UT_UPGCGI_Main_009()
{
	
	ResetEnv();
	setenv("CONTENT_LENGTH", "100", 1 );
	var_name_index=0;
	//
	content_type_index=0;
	strcpy(sDivider, "#");
	post_content_index=0;
	//
	fork_result=1;	
	msg_result=0;
	
	//
	CU_ASSERT(UPGCGI_Main() == UPG_SUCCESS);
	
}


void UT_UPGCGI_Main()
{
	TEST_TRACE("\nEnter UT_main\n");
	
   	UT_UPGCGI_Main_001();
   	UT_UPGCGI_Main_002();
	UT_UPGCGI_Main_003();
	UT_UPGCGI_Main_004();
	UT_UPGCGI_Main_005();
	UT_UPGCGI_Main_006();
	UT_UPGCGI_Main_007();
	UT_UPGCGI_Main_008();
	UT_UPGCGI_Main_009();    	    	       	    	    	  	 	
   	TEST_TRACE("\nEnd UT_mainn");
	
}
//

void UT_handle_read_timeout_001()
{	
	ResetEnv();
	fail_fclose=1;
	handle_read_timeout(1);
			
}


void UT_handle_read_timeout()
{
	
	TEST_TRACE("\nEnter UT_handle_read_timeout\n");
	
   	UT_handle_read_timeout_001();   	
	    	    	       	    	    	  	 	
   	TEST_TRACE("\nEnd UT_handle_read_timeout");
	
}
//
void UT_UPGCGI_UploadFile_001()
{	
	ResetEnv();
	unsetenv("CONTENT_LENGTH");
		
	CU_ASSERT(UPGCGI_UploadFile(MIN_CONFIG_SIZE, MAX_CONFIG_SIZE) == CONTENT_LENGTH_ERROR);
		
}


void UT_UPGCGI_UploadFile_002()
{	
	ResetEnv();
	setenv("CONTENT_LENGTH", "10", 1 );
		
	CU_ASSERT(UPGCGI_UploadFile(MIN_CONFIG_SIZE, MAX_CONFIG_SIZE) == MIN_CONTENT_LENGTH_ERROR);
		
}

void UT_UPGCGI_UploadFile_003()
{	
	ResetEnv();
	setenv("CONTENT_LENGTH", "8493466", 1 );
		
	CU_ASSERT(UPGCGI_UploadFile(MIN_CONFIG_SIZE, MAX_CONFIG_SIZE) == MAX_CONTENT_LENGTH_ERROR);
		
}

void UT_UPGCGI_UploadFile_004()
{	
	ResetEnv();
	fail_fopen=1;		
	CU_ASSERT(UPGCGI_UploadFile(MIN_CONFIG_SIZE, MAX_CONFIG_SIZE) == FOPEN_FW_ERROR);
		
}


void UT_UPGCGI_UploadFile_005()
{	
	ResetEnv();
	content_type_index=0;
	strcpy(sDivider, "#");
	post_content_index=0;
	CU_ASSERT(UPGCGI_UploadFile(MIN_CONFIG_SIZE, MAX_CONFIG_SIZE) == UPG_SUCCESS);
		
}


void UT_UPGCGI_UploadFile_006()
{	
	ResetEnv();
	content_type_index=1;	
	strcpy(sDivider, "#");
	post_content_index=0;	
	CU_ASSERT(UPGCGI_UploadFile(MIN_CONFIG_SIZE, MAX_CONFIG_SIZE) == UPG_SUCCESS);
		
}


void UT_UPGCGI_UploadFile_007()
{	
	ResetEnv();
	content_type_index=0;
	strcpy(sDivider, "#");
	post_content_index=4;
	CU_ASSERT(UPGCGI_UploadFile(MIN_CONFIG_SIZE, MAX_CONFIG_SIZE) == FILE_UPLOAD_ERROR);
		
}


void UT_UPGCGI_UploadFile_008()
{	
	ResetEnv();
	content_type_index=1;	
	strcpy(sDivider, "#");
	post_content_index=4;	
	CU_ASSERT(UPGCGI_UploadFile(MIN_CONFIG_SIZE, MAX_CONFIG_SIZE) == FILE_UPLOAD_ERROR);
		
}


void UT_UPGCGI_UploadFile_009()
{	
	ResetEnv();
	content_type_index=0;
	strcpy(sDivider, "#");
	post_content_index=2;
	CU_ASSERT(UPGCGI_UploadFile(MIN_CONFIG_SIZE, MAX_CONFIG_SIZE) == UPG_SUCCESS);
		
}


void UT_UPGCGI_UploadFile_010()
{	
	ResetEnv();
	content_type_index=1;	
	strcpy(sDivider, "#");
	post_content_index=2;	
	CU_ASSERT(UPGCGI_UploadFile(MIN_CONFIG_SIZE, MAX_CONFIG_SIZE) == UPG_SUCCESS);
		
}


void UT_UPGCGI_UploadFile()
{
	TEST_TRACE("\nEnter UT_UPGCGI_UploadFile\n");
	
   	UT_UPGCGI_UploadFile_001();
   	UT_UPGCGI_UploadFile_002();
   	UT_UPGCGI_UploadFile_003();
   	UT_UPGCGI_UploadFile_004();
	UT_UPGCGI_UploadFile_005();
	UT_UPGCGI_UploadFile_006();  
	UT_UPGCGI_UploadFile_007();
	UT_UPGCGI_UploadFile_008();  
	UT_UPGCGI_UploadFile_009();
	UT_UPGCGI_UploadFile_010(); 	    	       	    	    	  	 	
   	TEST_TRACE("\nEnd UT_UPGCGI_UploadFile");
	
}
//
void UT_UPGCGI_DoFirmwareUpgrade_001()
{	
	ResetEnv();
	fork_result=-1;	
	CU_ASSERT(UPGCGI_DoFirmwareUpgrade() == UPG_GENERAL_ERROR);
		
}

void UT_UPGCGI_DoFirmwareUpgrade_002()
{	
	ResetEnv();
	fork_result=1;	
	fail_MSG_ReceiveMessage=1;
	CU_ASSERT(UPGCGI_DoFirmwareUpgrade() == UPG_GENERAL_ERROR);
		
}


void UT_UPGCGI_DoFirmwareUpgrade_003()
{	
	ResetEnv();
	fork_result=1;	
	msg_result=0;
	CU_ASSERT(UPGCGI_DoFirmwareUpgrade() == TBS_SUCCESS);
			
}



void UT_UPGCGI_DoFirmwareUpgrade()
{
	TEST_TRACE("\nEnter UT_UPGCGI_DoFirmwareUpgrade\n");
	
   	UT_UPGCGI_DoFirmwareUpgrade_001();
   	UT_UPGCGI_DoFirmwareUpgrade_002();
   	UT_UPGCGI_DoFirmwareUpgrade_003();
   		    	       	    	    	  	 	
   	TEST_TRACE("\nEnd UT_UPGCGI_DoFirmwareUpgrade");
	
}

//
void UT_UPGCGI_DoConfigUpgrade_001()
{	
	ResetEnv();
	fork_result=-1;	
	CU_ASSERT(UPGCGI_DoConfigUpgrade() == UPG_GENERAL_ERROR);
		
}

void UT_UPGCGI_DoConfigUpgrade_002()
{	
	ResetEnv();
	fork_result=1;	
	fail_MSG_ReceiveMessage=1;
	CU_ASSERT(UPGCGI_DoConfigUpgrade() == UPG_GENERAL_ERROR);
		
}


void UT_UPGCGI_DoConfigUpgrade_003()
{	
	ResetEnv();
	fork_result=1;	
	msg_result=0;
	CU_ASSERT(UPGCGI_DoConfigUpgrade() == TBS_SUCCESS);
			
}



void UT_UPGCGI_DoConfigUpgrade()
{
	TEST_TRACE("\nEnter UT_UPGCGI_DoConfigUpgrade\n");
	
   	UT_UPGCGI_DoConfigUpgrade_001();
   	UT_UPGCGI_DoConfigUpgrade_002();
   	UT_UPGCGI_DoConfigUpgrade_003();
   		    	       	    	    	  	 	
   	TEST_TRACE("\nEnd UT_UPGCGI_DoConfigUpgrade");
	
}
//

//
void UT_UPGCGI_DownloadFile_001()
{	
	ResetEnv();
	fork_result=-1;	
	CU_ASSERT(UPGCGI_DownloadFile() == UPG_GENERAL_ERROR);
		
}

void UT_UPGCGI_DownloadFile_002()
{	
	ResetEnv();
	fork_result=1;	
	fail_MSG_ReceiveMessage=1;
	CU_ASSERT(UPGCGI_DownloadFile() == UPG_GENERAL_ERROR);
		
}


void UT_UPGCGI_DownloadFile_003()
{	
	ResetEnv();
	fork_result=1;	
	msg_result=0;
	fail_open = 1;
	CU_ASSERT(UPGCGI_DownloadFile() == FOPEN_FW_ERROR);
			
}

void UT_UPGCGI_DownloadFile_004()
{	
	ResetEnv();
	fork_result=1;	
	msg_result=0;
	
	CU_ASSERT(UPGCGI_DownloadFile() == UPG_SUCCESS);
			
}



void UT_UPGCGI_DownloadFile()
{
	TEST_TRACE("\nEnter UT_UPGCGI_DownloadFile\n");
	
   	UT_UPGCGI_DownloadFile_001();
   	UT_UPGCGI_DownloadFile_002();
   	UT_UPGCGI_DownloadFile_003();
   	UT_UPGCGI_DownloadFile_004();	
   	    	       	    	    	  	 	
   	TEST_TRACE("\nEnd UT_UPGCGI_DownloadFile");
	
}
//
void UT_UPGCGI_Header_001()
{	
	ResetEnv();
	content_type_index=0;
				
}

void UT_UPGCGI_Header_002()
{	
	ResetEnv();
	content_type_index=1;
				
}



void UT_UPGCGI_Header()
{
	TEST_TRACE("\nEnter UT_UPGCGI_Header\n");
	
   	UT_UPGCGI_Header_001();
   	UT_UPGCGI_Header_002();
   		    	       	    	    	  	 	
   	TEST_TRACE("\nEnd UT_UPGCGI_Header");
	
}
//

void UT_UPGCGI_SendErrorPage_001()
{	
	char*  buff="abc";
	ResetEnv();
	content_type_index=0;
	UPGCGI_SendErrorPage(buff);			
}

void UT_UPGCGI_SendErrorPage_002()
{	
	char*  buff="abc";
	ResetEnv();
	content_type_index=1;
	UPGCGI_SendErrorPage(buff);				
}



void UT_UPGCGI_SendErrorPage()
{
	TEST_TRACE("\nEnter UT_UPGCGI_SendErrorPage\n");
	
   	UT_UPGCGI_SendErrorPage_001();
   	UT_UPGCGI_SendErrorPage_002();
   		    	       	    	    	  	 	
   	TEST_TRACE("\nEnd UT_UPGCGI_SendErrorPage");
	
}
//
void UT_UPGCGI_SendHtmlPage_001()
{	
	ResetEnv();
	UPGCGI_SendHtmlPage("/var/www/upfile.html");				
}

void UT_UPGCGI_SendHtmlPage_002()
{	
	ResetEnv();
	fail_open=1;
	UPGCGI_SendHtmlPage("/var/www/upfile.html");				
}

void UT_UPGCGI_SendHtmlPage_003()
{	
	ResetEnv();
	read_result=1;
	UPGCGI_SendHtmlPage("/var/www/upfile.html");			
}

void UT_UPGCGI_SendHtmlPage()
{
	TEST_TRACE("\nEnter UT_UPGCGI_SendHtmlPage\n");
	
   	UT_UPGCGI_SendHtmlPage_001();
   	UT_UPGCGI_SendHtmlPage_002();
   	UT_UPGCGI_SendHtmlPage_003();
   		    	       	    	    	  	 	
   	TEST_TRACE("\nEnd UT_UPGCGI_SendHtmlPage");
	
}


//


void UT_UPGCGI_FlushBuffer_001()
{	
	ResetEnv();
	
	UPGCGI_FlushBuffer();				
}



void UT_UPGCGI_FlushBuffer()
{
	TEST_TRACE("\nEnter UT_UPGCGI_FlushBuffer\n");
	
   	UT_UPGCGI_FlushBuffer_001();
   	   		    	       	    	    	  	 	
   	TEST_TRACE("\nEnd UT_UPGCGI_FlushBuffer");
	
}

//

void UT_UPGCGI_LogTrace_001()
{	
	
	ResetEnv();
	
	UPGCGI_LogTrace("just a test!");				
}



void UT_UPGCGI_LogTrace()
{
	TEST_TRACE("\nEnter UT_UPGCGI_LogTrace\n");
	
   	UT_UPGCGI_LogTrace_001();
   		    	       	    	    	  	 	
   	TEST_TRACE("\nEnd UT_UPGCGI_LogTrace");
	
}//
//----------------------end--------------------------------------------------------------------------
CU_TestInfo test_cases[] = {
	
///*	
	{"备份配置文件", UT_UPGCGI_DownloadFile},
	{"上传文件", UT_UPGCGI_UploadFile},
	{"升级镜像文件", UT_UPGCGI_DoFirmwareUpgrade},
	{"升级配置文件", UT_UPGCGI_DoConfigUpgrade},
	
	{"打印文件头", UT_UPGCGI_Header},
	{"发送错误页面", UT_UPGCGI_SendErrorPage},
	
	
	{"发送html页面", UT_UPGCGI_SendHtmlPage},
	{"输出缓存", UT_UPGCGI_FlushBuffer},
	{"调试信息", UT_UPGCGI_LogTrace},
	{"超时处理", UT_handle_read_timeout},

	
	{"主函数", UT_UPGCGI_Main},
	
//*/
	
	
	CU_TEST_INFO_NULL   	
};

int init_suit()
{
	return 0;	
}

int end_suit()
{
	return 0;
}



CU_SuiteInfo suites[] = {
	{"Testing the module cpgcgi:", init_suit, end_suit, test_cases},
	CU_SUITE_INFO_NULL
};


void AddTests(void)
{
	assert(NULL != CU_get_registry());
	assert(!CU_is_test_running());

	if(CUE_SUCCESS != CU_register_suites(suites)){
		fprintf(stderr, "Register suites failed - %s ", CU_get_error_msg());
		exit(1);
	}
}

int ut_main( int argc, char *argv)
{
	setenv("MALLOC_TRACE","trace.log",1);

    	mtrace();
    
	if(CU_initialize_registry())
	{
		fprintf(stderr, " Initialization of Test Registry failed. ");
		exit(1);
	}
	else
	{
		AddTests();
//		CU_set_output_filename("DNSTest");
//		CU_list_tests_to_file();
//		CU_automated_run_tests();
		CU_basic_run_tests();
		CU_cleanup_registry();
		
	}
	
	muntrace();
	
	return 0;
}
//---------------------------the end-------------
