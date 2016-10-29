<?mget :InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig. "Inform ActJumpToPage"
`
	var G_Inform 				= "$01";
	var G_ActJumpToPage		= "$02"; 
`?>

var G_Url_Pre = 'webproc?getpage=html/index.html&var:page=';

function uiSwitchPage(page)
{
	var url = '';

	if (page == G_ActJumpToPage) 
		url = G_ActJumpToPage;
	else
		url = G_Url_Pre + page;

	document.location = url;
}

function uiOnload(){
 	$('lang_introduction').innerHTML = G_Inform;

	if (G_Inform.indexOf('DISCONNECT') != -1)
		window.setTimeout("uiSwitchPage('wan3g_connect_status')", 3000);
	else if (G_Inform.indexOf('CONNECTTED') != -1 || G_Inform.indexOf('Connected') != -1)
		window.setTimeout("uiSwitchPage(G_ActJumpToPage)", 2000);
	else
		window.setTimeout("uiSwitchPage('wan3g_connect_status')", 3000);
		
}

addListeners(uiOnload, Form.Action);
