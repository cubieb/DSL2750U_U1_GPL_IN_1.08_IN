// UPNP
<?mget :InternetGatewayDevice.X_TWSZ-COM_UPNP. "Enable"
`	var G_UpnpEnable  = "$01"; // Enable
`?>

function uiOnload(){
	Form.Checkbox('INPUT_EnableUPnP',G_UpnpEnable);
}

function uiSubmit(){
	
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_UPNP.Enable'      : Form.Checkbox('INPUT_EnableUPnP'),
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:subpage'  : G_SubPage,
		'var:errorpage': G_SubPage,
		'obj-action':'set',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}


function dealWithError(){
	if(G_Error != 1){
		return false;
	}
	
	var arrayHint = [];	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
