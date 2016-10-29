//TR069
var G_TR069_Enable = "<?get :InternetGatewayDevice.ManagementServer.EnableCWMP?>";
<?mget :InternetGatewayDevice.ManagementServer. "URL Username PeriodicInformEnable PeriodicInformInterval ConnectionRequestUsername ConnectionRequestEnable Password ConnectionRequestPassword"
`	var G_TR069_URL = "$01";                    //URL
	var G_TR069_Username = "$02";
	var G_TR069_PeriodicInformEnable = "$03";
	var G_TR069_PeriodicInformInterval = "$04";
	var G_TR069_ConnectionRequestUsername = "$05";
	var G_TR069_ConnectionRequestEnable = "$06";
	var G_TR069_Password = "$07";
	var G_TR069_ConnectionRequestPassword = "$08";
`?>

function controlEnable(_enable,_tag){
	//根据bool的状态来控制状态
	var bool = _enable == '1' ? true : false;
	//管理所有的输入框
	var nodes = document.getElementsByClassName(_tag);
	var Parent,Previous;
	
	for(var i = 0, _len = nodes.length; i < _len; i++){
		Parent   = nodes[i].parentNode;
		Previous = Parent.previousSibling;
		Parent.style.color   = bool ? '#000' : '#aaa';
		Previous.style.color = bool ? '#000' : '#aaa';
		nodes[i].disabled = !bool;
	}
	return _enable;
}

function CancleEntry(){
	var _input=$('inputarea').getElementsByTagName('INPUT');
	
	for(var k = 0, _len = _input.length; k < _len; k++){
			_input[k].value = '';
		}


}

function ChangeInfromInterval(value){
	
	if(value=='1'){
		$('INPUT_PeriodicInformInterval').disabled=false;
	}else{
		$('INPUT_PeriodicInformInterval').disabled=true;
	}
	
}

function ChangeCWMPInterval(value){
	
	if(value=='1'){
		$('INPUT_PeriodicInformInterval').disabled=false;
	}else{
		$('INPUT_PeriodicInformInterval').disabled=true;
	}
	
}
function EnableAuth(){
	
    if($('Enable_Request_Auth').checked){
		$('conn_auth').style.display='block';
	}else
		$('conn_auth').style.display='none';
}

var g_PwdShow = "#$^%.";
function uiOnload(){
	var enable_interval=G_TR069_PeriodicInformEnable;
	Form.Radio('InfromEnable', enable_interval);
	if(enable_interval=='1'){
		$('INPUT_PeriodicInformInterval').disabled=false;
	}else{
		$('INPUT_PeriodicInformInterval').disabled=true;
	}

	var enable_tr069 = G_TR069_Enable;
	Form.Radio('CwmpEnable', enable_tr069);
	
	if(G_TR069_ConnectionRequestEnable=='1'){
		$('conn_auth').style.display='block';
		$('Enable_Request_Auth').checked=true;
		$('INPUT_ConnectionRequestUsername').value=G_TR069_ConnectionRequestUsername;
		$('INPUT_ConnectionRequestPassword').value=g_PwdShow;//G_TR069_ConnectionRequestPassword;
	}else{
		$('conn_auth').style.display='none';
		$('Enable_Request_Auth').checked=false;
	}
	setJSONValue({
		'INPUT_URL'      : G_TR069_URL,
		'INPUT_PeriodicInformInterval'	: G_TR069_PeriodicInformInterval,
		'INPUT_Username' : G_TR069_Username,
		//'INPUT_Password'  : G_TR069_Password
		'INPUT_Password'  : g_PwdShow
	});
}

function uiSubmit(){	
	var value_array   = $('INPUT_URL','INPUT_Username','INPUT_Password','INPUT_PeriodicInformInterval','INPUT_ConnectionRequestUsername','INPUT_ConnectionRequestPassword','Enable_Request_Auth');

	var enable_infrom = Form.Radio('InfromEnable');
	var enable_TR069 = Form.Radio('CwmpEnable');
	//var enable_request_auth = $('Enable_Request_Auth').checked;
	$H({
		':InternetGatewayDevice.ManagementServer.EnableCWMP'   : enable_TR069 =='1' ? 1 : 0,
		':InternetGatewayDevice.ManagementServer.PeriodicInformEnable'     : enable_infrom=='1' ? 1 : 0,
		':InternetGatewayDevice.ManagementServer.PeriodicInformInterval'   : enable_infrom=='1' ? value_array[3].value : undefined,
		':InternetGatewayDevice.ManagementServer.Password'                 : value_array[2].value != g_PwdShow ? value_array[2].value : undefined,
		':InternetGatewayDevice.ManagementServer.ConnectionRequestPassword': Form.Checkbox('Enable_Request_Auth') ? (value_array[5].value != g_PwdShow?value_array[5].value:undefined) : undefined,
		':InternetGatewayDevice.ManagementServer.URL'                      : value_array[0].value,
		':InternetGatewayDevice.ManagementServer.Username'                 : value_array[1].value,
		':InternetGatewayDevice.ManagementServer.ConnectionRequestUsername': Form.Checkbox('Enable_Request_Auth') ? value_array[4].value : 'abcde12345',
		':InternetGatewayDevice.ManagementServer.ConnectionRequestEnable'  : Form.Checkbox('Enable_Request_Auth') ? 1:0,
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:subpage'  : G_SubPage,
		'var:errorpage': G_SubPage,
		'getpage'      :'html/index.html',
		'errorpage'    :'html/index.html',
		'obj-action'   :'set',
		'var:CacheLastData': ViewState.Save()
	},true);
	$('uiPostForm').submit();
}


function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,dealWithError,Form.Action);
