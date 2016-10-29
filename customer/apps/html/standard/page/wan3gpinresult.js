<?mget :InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig. "ActResult Enable ActNeedCaptival "
`	
	/*sim card status:NO USB CARD/lock disable/
	* lock enable/NEED PIN CODE/NEED PUK CODE 
	*/
	var G_ActResult	 		= "$01";
	var G_Enable				= "$02";
	var G_ActNeedCaptival		= "$03";
`?>
	
function uiSwitchPage(page)
{
	var url = 'webproc?getpage=html/index.html&var:menu=advanced&var:page=' + page;
	document.location = url;
}

function uiOnload(){
 	$('lang_introduction').innerHTML = G_ActResult;

	if (G_Enable == '1' && G_ActNeedCaptival == '1' && 
			G_ActResult.indexOf('OK') != -1) 
	{
		window.setTimeout("uiSwitchPage('wan3g_connect_status')", 3000);
	} else if (G_ActResult.indexOf('NONE') != -1) {
		window.setTimeout("uiSwitchPage('wan3gview')", 3000);
	} else 
		window.setTimeout("uiSwitchPage('wan3gpin')", 3000);
}

function dealWithError(){
	if (G_Error != 1)
		return false;
	
	var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,Form.Action);
 
