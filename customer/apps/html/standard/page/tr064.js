//TR064
<?mget :InternetGatewayDevice.X_TWSZ-COM_TR064. "Enable Status"
`	var G_TR064Enable 		= "$01";
    var G_TR064Status 		= "$02";
`?>

function uiOnload(){
	setJSONValue({
		'INPUT_TR064Enable'   : G_TR064Enable
	});
	
	dealWithError();
}

function uiSubmit(){
	var node_tr064 = $('INPUT_TR064Enable');
	
	$H({
		":InternetGatewayDevice.X_TWSZ-COM_TR064.Enable"       : $('INPUT_TR064Enable').checked ? 1 : 0,
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
