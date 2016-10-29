//Wireless Security
var G_Wireless = [];
var m = 0;
var encryption = '';

var G_Standard = '<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Standard?>' ;

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
	`	G_WEPKey[n] = ["$01"];  // get WEPkey1,WEPkey2,WEPkey3,WEPkey4
		n++;
	`?>
`?>
//WPS
var G_WPS = [];
var t = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "WPS.Enable  WPS.Version"
`	G_WPS[t] = [':InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00.',//path
				'$01', //Enable
				'$02' //Version
				];
	t++;
`?>
var Idx = 0;

function uiOnload(_arc){
<?if eq `advanced` `$var:menu` `	
	Idx = getSSID();
	if(_arc !== 'false'){
		createSSIDOpt();
		$('SELECT_SSID').value = Idx;
	}
`?>	
	$('SELECT_Mode').value = G_Wireless[Idx][2] == 'Basic' && G_Wireless[Idx][6] == 'None' ? 'None' : G_Wireless[Idx][2];

	ctrlSecMode();
	if(G_Wireless[Idx][15] != '1'){
		$('wlan_mode').style.display = 'none';
		$('Menu').style.height=560 + 'px';
	    $('Content').style.height=560+ 'px';
	} else {
		$('wlan_mode').style.display = 'block';
		$('Menu').style.height=1100+ 'px';
	    $('Content').style.height=1100 + 'px';
	}
	onChgWepLen_auto();
}

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

function createEncryptionOpt(){
	var _text = [], _value = [];
	
	_text.push("AES"); 
	_value.push("AESEncryption");

    if(G_Standard.indexOf('n') == -1 )
	{
		_text.push("TKIP"); 
		_value.push("TKIPEncryption");
	}
	
	_text.push("TKIP+AES"); 
	_value.push("TKIPandAESEncryption");
	
	$S('SELECT_EncryptionModes', _text, _value);
}

function ctrlSecMode(){
	var Type = $('SELECT_Mode').value; 
	var node_wpa = $('SELECT_WPAMode');
	var nodes = $('wep','wpa');
	var mode_wpa = mode_lli = mode_WPAand11i = '';
	var mode_secury = $('lang_secure_mode');
	
	createEncryptionOpt();
	
	switch(Type){
		case 'None' : {
			nodes[0].style.display = nodes[1].style.display = 'none';
			mode_secury.style.display = 'none';
			
			$('Menu').style.height=560 + 'px';
	        $('Content').style.height=560 + 'px';
			break;
		}
		case 'Basic' : {
			nodes[0].style.display = 'block';
			nodes[1].style.display = 'none';
			mode_secury.style.display = 'none';
			//For WEP Key
			var _id = Idx*4;
			setJSONValue({
				'SELECT_Auth'       : G_Wireless[Idx][5],
				'SELECT_WEPIndex'   : G_Wireless[Idx][3],
				'SELECT_WEPKey'     : G_Wireless[Idx][4],
				//wep key
				'INPUT_WEPKey1'     : G_WEPKey[_id+0][0],
				'INPUT_WEPKey2'     : G_WEPKey[_id+1][0],
				'INPUT_WEPKey3'     : G_WEPKey[_id+2][0],
				'INPUT_WEPKey4'     : G_WEPKey[_id+3][0]
			});
			$('Menu').style.height=1100 + 'px';
	        $('Content').style.height=1100 + 'px';
			break;
		}
		case 'WPA' : {
			mode_wpa = G_Wireless[Idx][7];
			mode_secury.style.display = 'block';
		}
		case '11i' : {
			mode_lli = G_Wireless[Idx][9];
			mode_secury.style.display = 'block';
			Form.Select('SELECT_EncryptionModes', G_Wireless[Idx][10]);
		}
		case 'WPAand11i' : {
			mode_WPAand11i = G_Wireless[Idx][9];
			Form.Select('SELECT_EncryptionModes', G_Wireless[Idx][10]);
			if(G_Wireless[Idx][10].length==0)
			{
			    $('SELECT_EncryptionModes').selectedIndex=0;
			    //Form.Select('SELECT_EncryptionModes', "AESEncryption");
			}
			if (Type == "WPA")
			{
			     Form.Select('SELECT_EncryptionModes', G_Wireless[Idx][8]);
				 if(G_Wireless[Idx][8].length==0)
				{
			    	$('SELECT_EncryptionModes').selectedIndex=0;
				}
			}
			mode_secury.style.display = 'block';

			var _word = Type == 'WPA' ? 'WPA' :
						Type == '11i' ? 'WPA2' : 'Auto(WPA or WPA2)';
			node_wpa.options[0].text = _word + '-PSK';
			node_wpa.options[1].text = _word + '-Enterprise';

			setJSONValue({
				'SELECT_WPAMode'        : mode_wpa || mode_lli || mode_WPAand11i,
				'INPUT_GroupKey'        : G_Wireless[Idx][14],
				//PSK
				'INPUT_PreShareKey'     : G_Wireless[Idx][11] == 'KeyPassphrase' ? G_Wireless[Idx][12] : G_Wireless[Idx][13],
				//EAP
				'INPUT_RadiusIPAddr'    : G_Wireless[Idx][16],
				'INPUT_RadiusPort'      : G_Wireless[Idx][17],
				'INPUT_RadiusSharedSec' : G_Wireless[Idx][18]
			});

			nodes[0].style.display = 'none';
			nodes[1].style.display = 'block';			
			onChgWapMode();
			$('Menu').style.height=1100 + 'px';
	        $('Content').style.height=1100 + 'px';
			break;
		}
	}
}

function onChgWepLen_auto(){ //about wep length
	var value_wep = G_Wireless[Idx][3];
	var node_keys = $('INPUT_WEPKey1','INPUT_WEPKey2','INPUT_WEPKey3','INPUT_WEPKey4');
	
	if(value_wep == '104-bit'){
		node_keys[0].maxLength = node_keys[1].maxLength = node_keys[2].maxLength = node_keys[3].maxLength = '26';
	} else {
		node_keys[0].maxLength = node_keys[1].maxLength = node_keys[2].maxLength = node_keys[3].maxLength = '10';

	}
}

function onChgWepLen(){ //about wep length
	var value_wep = $('SELECT_WEPIndex').value;
	var node_keys = $('INPUT_WEPKey1','INPUT_WEPKey2','INPUT_WEPKey3','INPUT_WEPKey4');
	
	if(value_wep == '104-bit'){
		node_keys[0].maxLength = node_keys[1].maxLength = node_keys[2].maxLength = node_keys[3].maxLength = '26';
	} else {
		node_keys[0].maxLength = node_keys[1].maxLength = node_keys[2].maxLength = node_keys[3].maxLength = '10';

	}
}

function onChgWapMode(){ // WPA Mode
	var value_wpamode = $('SELECT_WPAMode').value;
	var nodes = $('prekey','eap');
	
	if(value_wpamode == 'PSKAuthentication'){
		nodes[0].style.display = 'block';
		nodes[1].style.display = 'none';
	}
	else {
		nodes[0].style.display = 'none';
		nodes[1].style.display = 'block';
	}
	
}

function uiSubmit(){
	var _nodes = $('SELECT_Mode',
				   'SELECT_WEPIndex','SELECT_WEPKey','INPUT_WEPKey1','INPUT_WEPKey1','INPUT_WEPKey1','INPUT_WEPKey1','SELECT_Auth',
				   'SELECT_WPAMode','INPUT_GroupKey','INPUT_PreShareKey',
				   'INPUT_RadiusIPAddr','INPUT_RadiusPort','INPUT_RadiusSharedSec');

	/*根据中兴秘鲁反馈的问题，WEP加密方式下，SSID2~4的密码不应该设置成空，因此在这里做了一个判断，如果为空，则返回错误*/
	if(_nodes[0].value=='Basic' &&  $('INPUT_WEPKey'+$('SELECT_WEPKey').value).value.length==0)
	{
        alert("WEPKeyIndex"+$('SELECT_WEPKey').value+" should not be empty");
		return false;
	}
	
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'wlsecurity',
		'var:errorpage' : 'wlsecurity',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'var:arrayid'   : Idx,
		'obj-action'    : 'set'
	});

	var _path = G_Wireless[Idx][0];
	//WPS2.x
	if((G_WPS[Idx][1] ==1) && (G_WPS[Idx][2]==2))
	{

		if(_nodes[0].value == 'Basic')
		{
			if(confirm("If the security is WEP, the WPS would be disabled, Do you sure?"))
			{
				$F(_path + '.WPS.Enable','0');
			}
			else
			{
				return ;
			}
		}
		if(_nodes[0].value == 'WPA')
		{
			if(confirm("If the security mode is WPA only, the WPS would be disabled, Do you sure?"))
			{
				$F(_path + '.WPS.Enable','0');
			}
			else
			{
				return ;
			}
		}
		if($('SELECT_EncryptionModes').value=='TKIPEncryption')
		{
			if(confirm("If the encryption mode is TKIP only, the WPS would be disabled, Do you sure?"))
			{
				$F(_path + '.WPS.Enable','0');
			}
			else
			{
				return ;
			}
		}	
	}

	switch(_nodes[0].value){
		case 'None' : {
			$F(_path + '.BeaconType'             , 'None');
			$F(_path + '.BasicEncryptionModes'   , 'None');
			$F(_path + '.BasicAuthenticationMode', 'None');
			break;
		}
		case 'Basic' : {
			$F(_path + '.BeaconType'             , 'Basic');
			$F(_path + '.BasicEncryptionModes'   , 'WEPEncryption');
			$F(_path + '.BasicAuthenticationMode', $('SELECT_Auth').value);
			$F(_path + '.WEPEncryptionLevel'     , $('SELECT_WEPIndex').value);
			$F(_path + '.WEPKeyIndex'            , $('SELECT_WEPKey').value);
			$F(_path + '.WEPKey.1.WEPKey'        , $('INPUT_WEPKey1').value);
			$F(_path + '.WEPKey.2.WEPKey'        , $('INPUT_WEPKey2').value);
			$F(_path + '.WEPKey.3.WEPKey'        , $('INPUT_WEPKey3').value);
			$F(_path + '.WEPKey.4.WEPKey'        , $('INPUT_WEPKey4').value);
			break;
		}
		case 'WPAand11i' : {
			$F(_path + '.BeaconType', 'WPAand11i');
			$F(_path + '.IEEE11iEncryptionModes', $('SELECT_EncryptionModes').value);
		//	$F(_path + '.IEEE11iEncryptionModes', 'TKIPandAESEncryption');
			$F(_path + '.IEEE11iAuthenticationMode', $('SELECT_WPAMode').value);
			$F(_path + '.X_TWSZ-COM_WPAGroupRekey', $('INPUT_GroupKey').value);
			if($('SELECT_WPAMode').value == 'PSKAuthentication'){
				//Pre-Shared Key
				if($('INPUT_PreShareKey').value.length < 64){
					$F(_path + '.PreSharedKey.1.KeyPassphrase', $('INPUT_PreShareKey').value);
					$F(_path + '.X_TWSZ-COM_PSKExpression', "KeyPassphrase");
				} else {
					$F(_path + '.PreSharedKey.1.PreSharedKey', $('INPUT_PreShareKey').value);
					$F(_path + '.X_TWSZ-COM_PSKExpression', "PreSharedKey");
				}
			} else {
					$F(_path + '.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_IPAddress' , $('INPUT_RadiusIPAddr').value);
					$F(_path + '.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_Port'      , $('INPUT_RadiusPort').value);
					$F(_path + '.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_ShareKey'  , $('INPUT_RadiusSharedSec').value);				
			}
			//WPS
	//		$F(_path + '.WPS.Enable',Form.Checkbox('INPUT_WpsEnable'));
			break;
		}
		case '11i' : {
			$F(_path + '.BeaconType', '11i');
				
			$F(_path + '.IEEE11iEncryptionModes', $('SELECT_EncryptionModes').value);
		//	$F(_path + '.IEEE11iEncryptionModes', 'AESEncryption');
			$F(_path + '.IEEE11iAuthenticationMode', $('SELECT_WPAMode').value);
			$F(_path + '.X_TWSZ-COM_WPAGroupRekey', $('INPUT_GroupKey').value);
			if($('SELECT_WPAMode').value == 'PSKAuthentication'){
				//Pre-Shared Key
				if($('INPUT_PreShareKey').value.length < 64){
					$F(_path + '.PreSharedKey.1.KeyPassphrase', $('INPUT_PreShareKey').value);
					$F(_path + '.X_TWSZ-COM_PSKExpression', "KeyPassphrase");
				} else {
					$F(_path + '.PreSharedKey.1.PreSharedKey', $('INPUT_PreShareKey').value);
					$F(_path + '.X_TWSZ-COM_PSKExpression', "PreSharedKey");
				}
			} else {
					$F(_path + '.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_IPAddress' , $('INPUT_RadiusIPAddr').value);
					$F(_path + '.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_Port'      , $('INPUT_RadiusPort').value);
					$F(_path + '.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_ShareKey'  , $('INPUT_RadiusSharedSec').value);				
			}
			//WPS
	//		$F(_path + '.WPS.Enable',Form.Checkbox('INPUT_WpsEnable'));
			break;
		}
		case 'WPA' : {
			$F(_path + '.BeaconType', 'WPA');
			$F(_path + '.WPAEncryptionModes', $('SELECT_EncryptionModes').value);
			//$F(_path + '.WPAEncryptionModes', 'TKIPEncryption');
			$F(_path + '.WPAAuthenticationMode', $('SELECT_WPAMode').value);
			$F(_path + '.X_TWSZ-COM_WPAGroupRekey', $('INPUT_GroupKey').value);
			if($('SELECT_WPAMode').value == 'PSKAuthentication'){
				//Pre-Shared Key
				if($('INPUT_PreShareKey').value.length < 64){
					$F(_path + '.PreSharedKey.1.KeyPassphrase', $('INPUT_PreShareKey').value);
					$F(_path + '.X_TWSZ-COM_PSKExpression', "KeyPassphrase");
				} else {
					$F(_path + '.PreSharedKey.1.PreSharedKey', $('INPUT_PreShareKey').value);
					$F(_path + '.X_TWSZ-COM_PSKExpression', "PreSharedKey");
				}
			} else {
					$F(_path + '.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_IPAddress' , $('INPUT_RadiusIPAddr').value);
					$F(_path + '.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_Port'      , $('INPUT_RadiusPort').value);
					$F(_path + '.X_TWSZ-COM_RadiusServer.1.X_TWSZ-COM_ShareKey'  , $('INPUT_RadiusSharedSec').value);				
			}
			//WPS
	//		$F(_path + '.WPS.Enable',Form.Checkbox('INPUT_WpsEnable'));
	//		if ($("wps").style.display == "block")
	//		{
	//			if ($("INPUT_WpsEnable").checked == true)
	//				$F(_path + '.WPS.PeerPassword'   ,$('INPUT_DevicePin').value);
	//		}
			
			break;
		}
	}
	$('uiPostForm').submit();
}

//WPS
function newPin(){ 
	var _path = G_Wireless[Idx][0];
	
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'wlsecurity',
		'var:errorpage' : 'wlsecurity',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);

	$F(_path + '.WPS.GeneratePin','1');
	
	$('uiPostForm').submit();
}

function enabledPbc(){
	var _path = G_Wireless[Idx][0];
		
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'wlsecurity',
		'var:errorpage' : 'wlsecurity',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);

	$F(_path + '.WPS.StartWpsSession','1');
	
	$('uiPostForm').submit();	
}

function enabledPin(){
	var _path = G_Wireless[Idx][0];
		
	$H({
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:subpage'   : 'wlsecurity',
		'var:errorpage' : 'wlsecurity',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html',
		'obj-action'    : 'set'
	},true);

	$F(_path + '.WPS.StartWpsSession','2');
	$F(_path + '.WPS.PeerPassword'   ,$('INPUT_DevicePin').value);
	
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
