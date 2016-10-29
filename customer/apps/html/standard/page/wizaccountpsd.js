//get user info

var G_UserInfo1 = "<?get :InternetGatewayDevice.X_TWSZ-COM_Authentication.UserList.1.UserName?>";
function uiOnload()
{
	 chgHTML();
	 $('INPUT_Username').disabled = true;
    var user_psd = getJsonObj('psd');
	if(user_psd == null){
		alert('user_psd is null!');
		return false;
	}
    if(user_psd['INPUT_OldPassword'] != null)
	{
	setJSONValue({
   	      'INPUT_Username'         : G_UserInfo1,
		  'INPUT_OldPassword'      : user_psd && user_psd['INPUT_OldPassword'],
		  'INPUT_NewPassword'      : user_psd && user_psd['INPUT_NewPassword'],
		  'INPUT_ConfirmPassword'  : user_psd && user_psd['INPUT_ConfirmPassword']
	});
	}
	else
	{
			setJSONValue({
   	      'INPUT_Username'         : G_UserInfo1
	});
	}
	dealWithError();
}
function chgHTML(){
	var main_menu = document.getElementsByClassName('main_menu');
	var td_menu   = document.getElementsByClassName('td_menu');
	//隐藏主菜�?
	if(main_menu[0]){
		main_menu[0].style.display = 'none';
	}
	//隐藏二级菜单
	if(td_menu[0]){
		td_menu[0].style.display = 'none';
	}
}
function checkValue()
{
	//check NewPassword
	var _NewPassword = String($("INPUT_NewPassword").value);	
	//check ConfirmPassword
	var _ConfirmPassword = String($('INPUT_ConfirmPassword').value);
	if(_ConfirmPassword != _NewPassword)
	{
		alert(SEcode[1010]);
		return false;
	}
	else if( -1 != _NewPassword.indexOf(" ") )
	{
		alert(SEcode[1013]);
		return false;
	}
	
	return true;
}

function createJsonData(){
	var _json = '{';
	var nodes_isp = $(
					  'INPUT_OldPassword',
					  'INPUT_NewPassword',
					  'INPUT_ConfirmPassword',
					  'skip');
	_json += 'INPUT_OldPassword:"' + nodes_isp[0].value + '",';
	_json += 'INPUT_NewPassword:"' + nodes_isp[1].value + '",';	
	_json += 'INPUT_ConfirmPassword:"' + nodes_isp[2].value + '",';	
	_json += 'skip:"' + 1 + '",';
	_json = _json.delcomma();
	_json += '}';
	
	addJsonObj('psd', _json);
	return true;
}

function uiNextPage(){
	if(!checkValue())
	{
		return false;
	}
	
	if(!createJsonData()){
		return false;
	}
	
	
	$H({
	    'var:menu'	: 'setup',
		'var:subpage'  : 'wizsum',
		'var:page'  : 'wizard',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html'
	});
	
	$('uiPostForm').submit();	
}

function uiSkip(){
	
	var _json = '{';
	var nodes_isp = $( 'skip');
	_json += 'skip:"' + 0 + '",';
	_json = _json.delcomma();
	_json += '}';
	
	addJsonObj('psd', _json);
	
	$H({
	    'var:menu'	: 'setup',
		'var:subpage'  : 'wizsum',
		'var:page'  : 'wizard',
		'getpage'   : 'html/index.html',
		'errorpage' : 'html/index.html'
	});
	
	$('uiPostForm').submit();	
}


function uiBack(){
	
	if(!createJsonData()){
		return false;
	}
	
	$H({
	    'var:menu'	: 'setup',
		'var:subpage' : 'wizwl',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	});
	
	$('uiPostForm').submit();
}
function uiCancle(){
	if(!confirm(SEcode[1012])){
		return false;
	}
	
	$H({
		'var:menu' : 'setup',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	});
	
	$('uiPostForm').submit();
}
function dealWithError(){
	
}

addListeners(uiOnload,dealWithError);
