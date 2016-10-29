// Global Options
var G_QosGlobalEnable = "<? get :InternetGatewayDevice.QueueManagement.Enable ?>";

function uiOnload(){
	Form.Checkbox('INPUT_QosEnable', G_QosGlobalEnable || 0);
}

function uiSubmit(){
	$H({
		'getpage'    : 'html/index.html',
		'errorpage'  : 'html/index.html',
		'var:menu'   : 'advanced',
		'var:page'   : 'qos_config',
		'var:subpage': 'qos_global',
		'obj-action' : 'set',
		'var:errorpage': 'qos_global',
		'var:CacheLastData': ViewState.Save()
	},true);
	
	$F(':InternetGatewayDevice.QueueManagement.Enable', Form.Checkbox('INPUT_QosEnable'));
	
	$('uiPostForm').submit();
}

function dealWithError(){
	var arrayHint = [];
	if (G_Error != 1){ return false;}
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,dealWithError);
