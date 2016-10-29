//LOG
<?mget :InternetGatewayDevice.X_TWSZ-COM_Logger. "LoggerEnabled LogTFTPServer LogUploadTrigger LogClearTrigger DebugLogEnabled DebugLogFileSize RemoteLogEnabled RemoteLogger RemotePort"
`	var G_LoggerEnabled		= "$01";
	var G_LogTFTPServer		= "$02";
	var G_LogUploadTrigger	= "$03";
	var G_LogClearTrigger	= "$04";
	var G_DebugLogEnabled	= "$05";
	var G_DebugLogFileSize	= "$06";
	var G_RemoteLogEnabled 	= "$07";
	var G_RemoteLogger		= "$08";
	var G_RemotePort        = "$09"
`?>

function uiOnload(){
	 logenabdisply();
	 logmoddisply();	
}


function uiSubmit(){	
		
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_Logger.LoggerEnabled'	: LoggerEnabled(),
	//	':InternetGatewayDevice.X_TWSZ-COM_Logger.LogTFTPServer'	: Form.Checkbox('INPUT_logEnable') ? $('INPUT_TFTPServer').value : undefined,
		':InternetGatewayDevice.X_TWSZ-COM_Logger.DebugLogEnabled' 	: DebugLogEnabled(),
		//":InternetGatewayDevice.X_TWSZ-COM_Logger.DebugLogFileSize" : Form.Checkbox('INPUT_DebugLogEnable') ? $('INPUT_DebugLogSize').value : undefined,
		":InternetGatewayDevice.X_TWSZ-COM_Logger.RemoteLogEnabled" : RemoteLogEnabled(),
		":InternetGatewayDevice.X_TWSZ-COM_Logger.RemoteLogger"     : RemoteLogEnabled() ? $('INPUT_RemoteHost').value : undefined,
		":InternetGatewayDevice.X_TWSZ-COM_Logger.RemotePort"     : RemoteLogEnabled() ? $('INPUT_RemotePort').value : undefined,
		'obj-action'       :'set',
		'var:menu'         :'maintenance',
		'var:page'         :'syslog',
		'var:errorpage'    :'syslog',
		'getpage'          :'html/index.html',
		'errorpage'        :'html/index.html',
		'var:CacheLastData': ViewState.Save()
	},true);
	$('uiPostForm').submit();
}



function   LoggerEnabled(){
	
	if($('INPUT_logEnable').checked==false) 
	{
		return 0;
					
	}	 
	else
	{
		if(($('INPUT_logEnable').checked==true) && ($('INPUT_logmode').value==1))
		     return 0;
		else
		     return 1; 
	}	
}

function  RemoteLogEnabled(){
	
	if($('INPUT_logEnable').checked==false) 
	{
		return 0;
					
	}	 
	else
	{
		if(($('INPUT_logmode').value == 1)  || ($('INPUT_logmode').value == 2))
		     return 1;
		else
		     return 0; 
	}	
	
}

function  DebugLogEnabled(){
	
	if($('INPUT_logEnable').checked==false) 
	{
		return 0;
					
	}	 
	else
	{
		if(($('INPUT_logmode').value == 1)  || ($('INPUT_logmode').value == 2))
		     return 1;
		else
		     return 0; 
	}	
	
}


function   logenabdisply(){
	
	  if((G_RemoteLogEnabled==0)&&(G_LoggerEnabled==0 ))
	{
	     $('INPUT_logEnable').checked=false;	
		 $('INPUT_logmode').disabled=true;
		 $('inputmode').style.color = '#aaaaaa';
	     $('serveripaddr').style.color = '#aaaaaa';
	     $('severudpport').style.color = '#aaaaaa';
	}
	else
	{
	      $('INPUT_logEnable').checked=true;		 
	}
	
}
function  logmoddisply(){
	
	if((G_RemoteLogEnabled==0)&&(G_LoggerEnabled==1 ))
	{
		$('INPUT_logmode').value=0;
		$('INPUT_RemoteHost').disabled = true;
		$('INPUT_RemotePort').disabled = true;
	    $('serveripaddr').style.color = '#aaaaaa';
	    $('severudpport').style.color = '#AAAAAA';		
	}
	else
	{
	    if((G_RemoteLogEnabled==1)&&(G_LoggerEnabled==0 ))
	    {
		     $('INPUT_logmode').value=1;
		     $('INPUT_RemoteHost').value = G_RemoteLogger;
		     $('INPUT_RemotePort').value = G_RemotePort || 514;
	    }
		else
		{
			 if((G_RemoteLogEnabled==1)&&(G_LoggerEnabled==1))
			 {
			     $('INPUT_logmode').value=2;
		         $('INPUT_RemoteHost').value = G_RemoteLogger;
		         $('INPUT_RemotePort').value = G_RemotePort || 514;
			 }
			 else
			 {
				 $('INPUT_logmode').disabled=true;
		         $('INPUT_RemoteHost').disabled = true;
		         $('INPUT_RemotePort').disabled = true;
				 
			 }
		}
	}
	
}


function dealWithError(){

         if (G_Error != 1){ return false; }
         var arrayHint = [];
         dealErrorMsg(arrayHint, G_Error_Msg);
}



function  uiOnLogEnableClick(){
	
	if($('INPUT_logEnable').checked==true){
	
	      $('INPUT_logmode').disabled = false;	
		  $('inputmode').style.color = '#000000';
           modechange();    
	}
	else
	{
	     $('inputmode').style.color = '#aaaaaa';
	     $('serveripaddr').style.color = '#aaaaaa';
	     $('severudpport').style.color = '#aaaaaa';
	     $('INPUT_logmode').disabled = true;
	     $('INPUT_RemoteHost').disabled = true;
	     $('INPUT_RemotePort').disabled = true;
	//	 $('INPUT_logmode').value = " ";
	     $('INPUT_RemoteHost').value = " ";
	     $('INPUT_RemotePort').value = " ";
    }
}

function  modechange(){
	
   	if($('INPUT_logmode').value==0)
	{
	    $('INPUT_RemoteHost').disabled = true;
	    $('INPUT_RemotePort').disabled = true;
		$('INPUT_RemoteHost').value = " ";
	    $('INPUT_RemotePort').value = " ";	
	    $('serveripaddr').style.color = '#aaaaaa';
	    $('severudpport').style.color = '#aaaaaa';		
	}
	else
	{
	    $('INPUT_RemoteHost').disabled = false;
	    $('INPUT_RemotePort').disabled = false;
		$('INPUT_RemoteHost').value = G_RemoteLogger;
	    $('INPUT_RemotePort').value = G_RemotePort||514;
	    $('serveripaddr').style.color = '#000000';
	    $('severudpport').style.color = '#000000';				
	}
			
}

function loggerviewlog(){
    document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=status&var:page=logview';
}

function firewallviewlog(){
    document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=status&var:page=securitylogview';
}

addListeners(uiOnload, dealWithError);
