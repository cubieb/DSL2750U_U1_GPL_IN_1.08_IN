
var IPIDEnabled = "<?get :InternetGatewayDevice.X_TWSZ-COM_ShareProtection.IPIDEnable?>";
var TTLEnabled = "<?get :InternetGatewayDevice.X_TWSZ-COM_ShareProtection.TTLEnable?>";
var CONNLIMITEnabled = "<?get :InternetGatewayDevice.X_TWSZ-COM_ShareProtection.CONNLIMITEnable?>";
var TTLValue = "<?get :InternetGatewayDevice.X_TWSZ-COM_ShareProtection.TTLValue?>";
var TCPCONNValue = "<?get :InternetGatewayDevice.X_TWSZ-COM_ShareProtection.TCPCONNValue?>";
var UDPCONNValue = "<?get :InternetGatewayDevice.X_TWSZ-COM_ShareProtection.UDPCONNValue?>";

function uiOnload(){
	Form.Checkbox('INPUT_IPIDEnable', IPIDEnabled);
	Form.Checkbox('INPUT_TTLEnable', TTLEnabled);
	Form.Checkbox('INPUT_CONNLIMITEnable', CONNLIMITEnabled);
	
	$('INPUT_TTLValue').value = TTLValue;
	$('INPUT_TCPCONNValue').value = TCPCONNValue;
	$('INPUT_UDPCONNValue').value = UDPCONNValue;
}

function uiSubmit(){

	$F(":InternetGatewayDevice.X_TWSZ-COM_ShareProtection.IPIDEnable", Form.Checkbox('INPUT_IPIDEnable'));
	$F(":InternetGatewayDevice.X_TWSZ-COM_ShareProtection.TTLEnable", Form.Checkbox('INPUT_TTLEnable'));
	$F(":InternetGatewayDevice.X_TWSZ-COM_ShareProtection.CONNLIMITEnable", Form.Checkbox('INPUT_CONNLIMITEnable'));
	$F(":InternetGatewayDevice.X_TWSZ-COM_ShareProtection.TTLValue", $('INPUT_TTLValue').value);
	$F(":InternetGatewayDevice.X_TWSZ-COM_ShareProtection.TCPCONNValue", $('INPUT_TCPCONNValue').value);
	$F(":InternetGatewayDevice.X_TWSZ-COM_ShareProtection.UDPCONNValue", $('INPUT_UDPCONNValue').value);
	
	$H({
		'obj-action': 'set',
		'var:menu'  : 'advanced',
		'var:page'  : 'shareprotection',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html',
		'var:errorpage'    : 'shareprotection',
		'var:CacheLastData': ViewState.Save()
	});
	
	$('uiPostForm').submit();
}

function dealWithError(){
	if (G_Error != 1){ return false;}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);