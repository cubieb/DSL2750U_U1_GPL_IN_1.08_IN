//Wireless Security
var G_Wireless = [];
var m = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "SSID BeaconType WEPEncryptionLevel WEPKeyIndex BasicAuthenticationMode BasicEncryptionModes WPAAuthenticationMode WPAEncryptionModes IEEE11iAuthenticationMode IEEE11iEncryptionModes X_TWSZ-COM_PSKExpression PreSharedKey.1.KeyPassphrase PreSharedKey.1.PreSharedKey X_TWSZ-COM_WPAGroupRekey Enable"
`	G_Wireless[m] = [":InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00", //Path
					 "$01", //SSID
					 "$02", //BeaconType
					 "$03", //WEPEncryptionLevel
					 "$04", //WEPKeyIndex
					 "$05", //BasicAuthenticationMode
					 "$06", //BasicEncryptionModes
					 "$07", //WPAAuthenticationMode
					 "$08", //WPAEncryptionModes
					 "$09", //IEEE11iAuthenticationMode
					 "$0a", //IEEE11iEncryptionModes
					 "$0b", //X_TWSZ-COM_PSKExpression
					 "$0c", //KeyPassphrase
					 "$0d", //PreSharedKey
					 "$0e", //X_TWSZ-COM_WPAGroupRekey
					 "$0f", //Enable
					 '<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.$10.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_IPAddress?>', //IP
					 '<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.$10.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_Port?>', //Port
					 '<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.$10.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_ShareKey?>'
					 ];

	m++;
`?>
//Wireless Wep Key
var G_WEPKey = [];
var n = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. ""
`	<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration.$10.WEPKey. "WEPKey"
	`	G_WEPKey[n] = ["$01"]; // 用来区别第几个WEBKey;
		n++;
	`?>
`?>
//WPS
var G_WPS = [];
var t = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "WPS.Enable WPS.DevicePassword WPS.PeerPassword WPS.StartWpsSession WPS.GeneratePin WPS.WpsSessionStatus WPS.GeneratePinStatus WPS.IsMatchCondition WPS.X_TWSZ-COM_Role WPS.ConfigurationState WPS.Version"
`	G_WPS[t] = [':InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00.',//path
				'$01',//Enable
				'$02',//DevicePassword
				'$03',//PeerPassword
				'$04',//StartWpsSession
				'$05',//GeneratePin
				'$06',//WpsSessionStatus
				'$07',//GeneratePinStatus
				'$08',//IsMatchCondition
				'$09',//X_TWSZ-COM_Role
				'$0a',
				'$0b'  //Version
				];
	t++;
`?>

//当前的实例对应的数组下标，默认为第一�?
var Idx = 0;

function getSSID(){
	var idx_ssid = $('SELECT_SSID') && $('SELECT_SSID').value;

	if(idx_ssid >= '0'){
		return idx_ssid;
	}
	return '<?echo $var:arrayid?>' == '-' ? 0 : Number('<?echo $var:arrayid?>');
}

function createSSIDOpt(){
	var _text = [], _value = [];
	
	for(var i = 0, _len = G_Wireless.length; i < _len; i++){
		_text.push(G_Wireless[i][1]); //SSID
		_value.push(i);
	}
	
	$S('SELECT_SSID', _text, _value);
}


function enable_all_button()
{
	if ((G_WPS[Idx][8] == 1) && (Form.Checkbox('INPUT_WpsEnable') == 1)) {
		$('INPUT_WpsEnabled').disabled = false;
		$('INPUT_EnabledPin').disabled = false;
		$('INPUT_EnablePin_dev').disabled = false;
		$('INPUT_NewPin').disabled = false;
	} else {
		$('INPUT_WpsEnabled').disabled = true;
		$('INPUT_EnabledPin').disabled = true;
		$('INPUT_EnablePin_dev').disabled = true;
		$('INPUT_NewPin').disabled = true;
	}
}



function get_wps(str)
{
	if (str == 'INPROESS')
		return data_language.dynamic.INPROESS;
	else if (str == 'ERROR')
		return data_language.dynamic.ERROR;
	else if (str == 'OVERLAP')
		return data_language.dynamic.OVERLAP;
	else if (str == 'SUCCESS')
		return data_language.dynamic.SUCCESS;
	else
		return '';
}

//function uiOnload(_arc){
function uiOnload(){
	
	//获取SSID
	//Idx = getSSID();
	Idx = 0;
	
	//if(_arc !== 'false'){
	//	createSSIDOpt();
	//	$('SELECT_SSID').value = Idx;
	//}
	Form.Checkbox('INPUT_WpsEnable',G_WPS[Idx][1]);
	enable_all_button();
	$('INPUT_SSID').disabled = true;
	setJSONValue({
   //           'wps_auth'          : get_auth_mode(),
        'INPUT_SSID'        : G_Wireless[Idx][1],
		'SPAN_CrtlyPin'     : G_WPS[Idx][2],
  //            'SPAN_GenPinSt'     : G_WPS[Idx][7],
		'INPUT_DevicePin'   : G_WPS[Idx][3],
        'SELECT_Mode'   	: G_WPS[Idx][9],
        'SELECT_ConfigurationState'   	: G_WPS[Idx][10],      
		'SPAN_WpsSessionSt' : get_wps(G_WPS[Idx][6])
	});
	
	 <?if eq 1 $var:mod_WPS2X
	`
		$('SELECT_Version').value = G_WPS[Idx][11];
	`?>
		
	onChgApRole();
}

function onChgApRole(){
	var _apRole = $('SELECT_Mode').value;
	var _Registrar = _Enrollee = _Proxy = 'none';
	var _index = 0;
	
	switch(_apRole){
		case 'Registrar' :
			_Registrar = 'block';
			break;
		case 'Enrollee' :
			_Enrollee = 'block';
			break;
		case 'Proxy' :
			_Proxy = 'block';

			break;
		}
	$('wps_registrar').style.display = _Registrar;
	$('wps_enrollee').style.display = _Enrollee;
}

function get_auth_mode()
{
	if (G_Wireless[Idx][2] == 'None') 
		return 'None';
	else if (G_Wireless[Idx][2] == 'Basic') 
		return 'WEP';
	else if (G_Wireless[Idx][2] == 'WPAand11i') 
		return  'Auto(WPA or WPA2)';
	else if (G_Wireless[Idx][2] == '11i')
		return 'WPA2 only';
	else if (G_Wireless[Idx][2] == 'WPA')
		return 'WPA only';
	else
		return '';
}

function uiSubmit()
{
	var _path = G_Wireless[Idx][0];
	
	var WPSVersion = 1;
	
 	<?if eq 1 $var:mod_WPS2X
	`
		WPSVersion = $('SELECT_Version').value;
	
		if(Form.Checkbox('INPUT_WpsEnable'))
		{
			if($('SELECT_Version').value == 2){
				if((G_Wireless[Idx][2]) == 'Basic' || (G_Wireless[Idx][2])== 'WPA' )
				{
					alert("The current security mode is not supproted WPS 2.0 version, please change!");
					return;
				}
			}
		}
	`?>
		
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'wps',
		'var:errorpage' : 'wps',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'var:arrayid'   : Idx,
		'obj-action'    : 'set'
	});
	$F(_path + '.WPS.Enable',Form.Checkbox('INPUT_WpsEnable'));
	$F(_path + '.WPS.X_TWSZ-COM_Role',$('SELECT_Mode').value);
	$F(_path + '.WPS.Version',WPSVersion);
	$F(_path + '.WPS.ConfigurationState',$('SELECT_ConfigurationState').value);
	$('uiPostForm').submit();
}


function uiResetOOB(){
	var _path = G_Wireless[Idx][0];
		
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'wps',
		'var:errorpage' : 'wps',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);

	$F(_path + '.WPS.X_TWSZ-COM_ResetOob','1');
	
	$('uiPostForm').submit();	
}

function enabledPbc(){
	var _path = G_Wireless[Idx][0];
		
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'wps',
		'var:errorpage' : 'wps',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);

	$F(_path + '.WPS.StartWpsSession','1');
	$F(_path + '.WPS.ConfigurationState',$('SELECT_ConfigurationState').value);
	$F(_path + '.WPS.X_TWSZ-COM_Role',$('SELECT_Mode').value);
	
	$('uiPostForm').submit();	
}

function newPin(){ //�����µ�pin��
        var _path = G_Wireless[Idx][0];

        $H({
                'var:menu'      : '<?echo $var:menu?>',
                'var:page'      : '<?echo $var:page?>',
                'var:subpage'   : 'wps',
                'var:errorpage' : 'wps',
                'getpage'       : 'html/index.html',
                'errorpage'     : 'html/index.html',
                'obj-action'    : 'set'
        },true);

        $F(_path + '.WPS.GeneratePin','1');
	 	$F(_path + '.WPS.X_TWSZ-COM_Role',$('SELECT_Mode').value);
		$F(_path + '.WPS.ConfigurationState',$('SELECT_ConfigurationState').value);
		
        $('uiPostForm').submit();
}


function enabledPin(){
	var _path = G_Wireless[Idx][0];
	
	var pin = $('INPUT_DevicePin').value.replace(/[\x20-]/g, '');

	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'wps',
		'var:errorpage' : 'wps',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);

	$F(_path + '.WPS.StartWpsSession','2');
	$F(_path + '.WPS.PeerPassword',pin);
	$F(_path + '.WPS.X_TWSZ-COM_Role',$('SELECT_Mode').value);
	$F(_path + '.WPS.ConfigurationState',$('SELECT_ConfigurationState').value);
	
	$('uiPostForm').submit();	
}

function enabledPinDev(){
	var _path = G_Wireless[Idx][0];
		
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'wps',
		'var:errorpage' : 'wps',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);

	$F(_path + '.WPS.StartWpsSession','3');
	$F(_path + '.WPS.X_TWSZ-COM_Role',$('SELECT_Mode').value);
	$F(_path + '.WPS.ConfigurationState',$('SELECT_ConfigurationState').value);
	
	$('uiPostForm').submit();	
}


function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
	
}

addListeners(uiOnload, dealWithError);
