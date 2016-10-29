var IPv6Enable = "<?get :InternetGatewayDevice.X_TWSZ-COM_IPv6Enable.Enable ?>";


function uiSubmit(){
    	$H({
		//总开关
		':InternetGatewayDevice.X_TWSZ-COM_IPv6Enable.Enable' :Form.Checkbox('INPUT_EnableIPv6'),		
		
		'obj-action':'set',
		'var:menu'  :'<?echo $var:menu?>',
		'var:page'  :'<?echo $var:page?>',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html',
		'var:errorpage' : 'globalipv6',
		'var:CacheLastData': ViewState.Save()
	},true);
	$('uiPostForm').submit();
}

function uiOnload(){
    Form.Checkbox('INPUT_EnableIPv6',IPv6Enable);
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
