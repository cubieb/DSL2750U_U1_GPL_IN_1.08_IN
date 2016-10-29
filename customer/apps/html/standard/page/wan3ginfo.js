var G_stModemCfg = [];

<?mget :InternetGatewayDevice.WANDevice.2.X_TWSZ-COM_WAN3GInterfaceConfig. "DongleSerialNumber DongleVendorId DongleProductId DongleModelName UpstreamMaxRate DownstreamMaxRate" 
`	G_stModemCfg[0]                 =       "$01";
	G_stModemCfg[1]               =	 "$02";	
	G_stModemCfg[2]            		   =	 "$03";
	G_stModemCfg[3]               =	 "$04";	
	G_stModemCfg[4]               =	 "$05";	
	G_stModemCfg[5]               =	 "$06";	

`?>
function uiOnload(){

 	$('lang_dsn').innerHTML =G_stModemCfg[0];
	$('lang_dvi').innerHTML =G_stModemCfg[1];
	$('lang_dpi').innerHTML = G_stModemCfg[2];
	$('lang_dmn').innerHTML =G_stModemCfg[3];
	//$('lang_umr').innerHTML =G_stModemCfg[4];
	$('lang_dmr').innerHTML =G_stModemCfg[5];


}


function uiSubmit(){
	var _url;	
	
	_url = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wan3gview';
	
	document.location.href = _url;	
}

function dealWithError(){
	if (G_Error != 1)
		return false;
	
	var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,Form.Action);
 
