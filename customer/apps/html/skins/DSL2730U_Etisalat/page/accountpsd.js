//get user info
var G_UserInfo = new Array();
var m = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_Authentication.UserList. "UserName Password Level"
`	G_UserInfo[m] = new Array();
	G_UserInfo[m][0] = "$01"; //UserName
	G_UserInfo[m][1] = ""; //Password
	G_UserInfo[m][2] = "$03"; //Level
	G_UserInfo[m][3] = "$00"; //Index
	m++;
`?>

//get session timeout
var G_SessionTimeout = "<?get :InternetGatewayDevice.X_TWSZ-COM_Authentication.SessionTimeout?>";
var UserLevel = <?get :InternetGatewayDevice.X_TWSZ-COM_Authentication.UserList.<?echo $var:sys_UserIndex?>.Level?>;

function uiOnload()
{
    setJSONValue({
		'INPUT_SessionTimeOut'   : G_SessionTimeout/60
	});

	setJSONValue({
   	      'SELECT_UserLevel'       : UserLevel
	});
	ctrlChageUser();
	
	dealWithError();
}

function ctrlChageUser()
{
   var text = [], value = [];
   var index = $('SELECT_UserLevel').value;

   if (1 == UserLevel)
   	text = [G_UserInfo[0][0],G_UserInfo[1][0],G_UserInfo[2][0]], value = ['1','2','3'];
   else
   {
   	   if (3 == UserLevel)
   	   	text = [G_UserInfo[1][0],G_UserInfo[2][0]], value = ['2','3'];
	   else
	   	text = [G_UserInfo[1][0]], value = ['2'];
   }

   $S('SELECT_UserLevel', text, value);
   setJSONValue({
   	      'SELECT_UserLevel'       : index,
		'INPUT_NewUsername'   : G_UserInfo[index-1][0]
	});
}

function uiSubmit()
{
	if(!checkValue()) 
		return false;
	
	var value_node = $('SELECT_UserLevel','INPUT_OldPassword','INPUT_NewPassword','INPUT_NewUsername');
	$F(':InternetGatewayDevice.X_TWSZ-COM_Authentication.UserList.' + value_node[0].value + '.Password', value_node[1].value + ':' +value_node[2].value);
	$F(':InternetGatewayDevice.X_TWSZ-COM_Authentication.UserList.' + value_node[0].value + '.UserName', value_node[3].value);

	if($('SELECT_UserLevel').value == UserLevel)
	{
		$H({
			'obj-action'   : 'set',
			'var:nodeIndex': value_node[0].value,
			'var:menu'     : 'maintenance',
			'var:page'     : 'accessctrl',
			'var:subpage'  : 'accountpsd',
			'var:errorpage': 'accountpsd',
			'getpage'      : 'html/main.html',
			'errorpage'    : 'html/index.html',
			'var:noredirect':'0',
			'var:CacheLastData': ViewState.Save()
		});
	}
	else
	{
		$H({
			'obj-action'   : 'set',
			'var:nodeIndex': value_node[0].value,
			'var:menu'     : 'maintenance',
			'var:page'     : 'accessctrl',
			'var:subpage'  : 'accountpsd',
			'var:errorpage': 'accountpsd',
			'getpage'      : 'html/index.html',
			'errorpage'    : 'html/index.html',
			'var:noredirect':'0',
			'var:CacheLastData': ViewState.Save()
		});
	}
	$('uiPostForm').submit();
}

function uiSubmit_TO()
{
	$H({
		":InternetGatewayDevice.X_TWSZ-COM_Authentication.SessionTimeout" : $('INPUT_SessionTimeOut').value*60,
		'obj-action'   	:'set',
		'var:menu'     	:'maintenance',
		'var:page'     	:'accessctrl',
		'var:subpage'   :'accountpsd',
		'var:errorpage': 'accountpsd',
		'getpage'   	:'html/index.html',
		'errorpage'    : 'html/index.html',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}

function checkValue()
{
	//check NewPassword
	var _NewPassword = String($("INPUT_NewPassword").value);	
	//check ConfirmPassword
	var _ConfirmPassword = String($('INPUT_ConfirmPassword').value);
	if(_NewPassword.length < 6)
	{
		alert("The new password must be at lest 6 strings");
		return false;
	}
	else if(_ConfirmPassword != _NewPassword)
	{
		alert(SEcode[1010]);
		return false;
	}else if( -1 != _NewPassword.indexOf(" ") )
	{
		alert(SEcode[1013]);
		return false;
	}
	
	return true;
}

function dealWithError()
{
	if (G_Error != 1)
	{
		return false;
	}
	
	var arrayHint = [];
//	arrayHint['SessionTimeout']  = 'INPUT_SessionTimeOut';
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload);
