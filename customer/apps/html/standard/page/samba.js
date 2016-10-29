
//SAMBA
<?mget :InternetGatewayDevice.X_TWSZ-COM_SAMBA. "SambaEnable Workgroup NetbiosName Newpassword USBEnable AnonymousEnable"
`	var G_SambaEnable 		= "$01";
	var G_Workgroup			= "$02";
	var G_NetbiosName 		= "$03";
	var G_Newpassword 		= "$04";
	var G_USBEnable    		= "$05";
	var G_AnonymousEnable	= "$06";
`?>

function uiOnload(){
	setJSONValue({
		'INPUT_SMBEnable'   : G_SambaEnable,
		'INPUT_Workgroup'  : G_Workgroup,
		'INPUT_NetbiosName'  : G_NetbiosName,
		'INPUT_NewSMBPasswd'  : G_Newpassword,
		'INPUT_RNewSMBPasswd'  : G_Newpassword,
		'INPUT_USBEnable'     : G_USBEnable,
		'INPUT_AnonymousEnable'     : G_AnonymousEnable
	});
	
	dealWithError();
}

function uiSubmit(){
	var node_samb = $('INPUT_SMBEnable','INPUT_Workgroup','INPUT_NetbiosName','INPUT_NewSMBPasswd','INPUT_USBEnable','INPUT_AnonymousEnable');
	
	if($('INPUT_NewSMBPasswd').value != $('INPUT_RNewSMBPasswd').value){
		alert(SEcode[1010]);
		return false;
	}
	$H({
		":InternetGatewayDevice.X_TWSZ-COM_SAMBA.SambaEnable"       : node_samb[0].checked ? 1 : 0,
		":InternetGatewayDevice.X_TWSZ-COM_SAMBA.Workgroup"  : node_samb[1].value,
		":InternetGatewayDevice.X_TWSZ-COM_SAMBA.NetbiosName"  : node_samb[2].value,
		":InternetGatewayDevice.X_TWSZ-COM_SAMBA.Newpassword"     : node_samb[3].value,
		":InternetGatewayDevice.X_TWSZ-COM_SAMBA.USBEnable": node_samb[4].checked ? 1 : 0,
		":InternetGatewayDevice.X_TWSZ-COM_SAMBA.AnonymousEnable": node_samb[5].checked ? 1 : 0,
		'obj-action'   : 'set',
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:errorpage': G_Page,
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

