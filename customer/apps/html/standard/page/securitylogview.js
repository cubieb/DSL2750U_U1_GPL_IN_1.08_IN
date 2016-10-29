// JavaScript Document//LOG


function uiOnload(){
	ajax_GetLogText();
}



//启动PBC-ajax
function ajax_GetLogText()
{

	//$('INPUT_LogText').value = uError.lang_log_loading;

	var _url = "/cgi-bin/webupg";
	ajax = Ajax.getInstance(_url, "", 0, processResult);
	ajax.post($('uiShowLog'));
}

//将下载的日志文件,显示到页面
function processResult(responseText)
{

	$('INPUT_LogText').value = responseText;
	
}

function dealWithError(){

         if (G_Error != 1){ return false; }
         var arrayHint = [];
         dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);


