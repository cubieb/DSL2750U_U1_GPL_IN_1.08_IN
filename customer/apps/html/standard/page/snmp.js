//SNMP
<?mget :InternetGatewayDevice.X_TWSZ-COM_SNMPAgent. "Enable Status ROCommunity RWCommunity TrapHost TrapCommunity TrapVersion"
`	var G_SNMPEnable 		= "$01";
	var G_SNMPStatus 		= "$02";
	var G_ROCommunity 		= "$03";
	var G_RWCommunity 		= "$04";
	var G_TrapHost    		= "$05";
	var G_TrapCommunity 	= "$06";
	var G_TrapVersion 		= "$07";
`?>

function uiOnload(){
	setJSONValue({
		'INPUT_SNMPEnable'   : G_SNMPEnable,
		'INPUT_ROCommunity'  : G_ROCommunity,
		'INPUT_RWCommunity'  : G_RWCommunity,
		'INPUT_TrapHost'     : G_TrapHost,
		'INPUT_TrapCommunity': G_TrapCommunity,
		'INPUT_TrapVersion'  : G_TrapVersion || '1'
	});
	
	dealWithError();
}

function uiSubmit(){
	var node_snmp = $('INPUT_SNMPEnable','INPUT_ROCommunity','INPUT_RWCommunity','INPUT_TrapHost','INPUT_TrapCommunity','INPUT_TrapVersion');
	
	$H({
		":InternetGatewayDevice.X_TWSZ-COM_SNMPAgent.Enable"       : node_snmp[0].checked ? 1 : 0,
		":InternetGatewayDevice.X_TWSZ-COM_SNMPAgent.ROCommunity"  : node_snmp[1].value,
		":InternetGatewayDevice.X_TWSZ-COM_SNMPAgent.RWCommunity"  : node_snmp[2].value,
		":InternetGatewayDevice.X_TWSZ-COM_SNMPAgent.TrapHost"     : node_snmp[3].value,
		":InternetGatewayDevice.X_TWSZ-COM_SNMPAgent.TrapCommunity": node_snmp[4].value,
		":InternetGatewayDevice.X_TWSZ-COM_SNMPAgent.TrapVersion"  : node_snmp[5].value,
		'obj-action'   : 'set',
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:subpage'  : G_SubPage,
		'var:errorpage': G_SubPage,
		'getpage'      : 'html/index.html',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}

function dealWithError(){
	if (G_Error != 1){ return false;}
	
	var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,Form.Action);
