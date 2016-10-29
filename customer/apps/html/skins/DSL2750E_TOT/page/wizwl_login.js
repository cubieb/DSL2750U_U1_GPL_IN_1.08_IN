//Wireless Security
var G_WanConns = [];
var G_Wireless = [];
var m = 0;
var G_WanConns_num = 0;
var encryption = '';
var Isp, Sec ;
var G_Standard = '<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Standard?>' ;
var old_wan_username;
var old_wan_password;
var edit_wan_num;
var edit_adsl = 0; 

<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
   `<?if gt $12 0
	 `	 <?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. " Name  ConnectionType Username TmpPassword"
		 `	  G_WanConns[G_WanConns_num] = [":InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00.", //Path
							   "$01",      //name
							   "$02",      //connectiontype
							   "$03",		//Username
							   "$04"		//TmpPassword
							   ];
							   ++G_WanConns_num;
	  `?>
	`?>
`?>

<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "SSID BeaconType WEPEncryptionLevel WEPKeyIndex BasicAuthenticationMode BasicEncryptionModes WPAAuthenticationMode WPAEncryptionModes IEEE11iAuthenticationMode IEEE11iEncryptionModes X_TWSZ-COM_PSKExpression PreSharedKey.1.KeyPassphrase PreSharedKey.1.PreSharedKey X_TWSZ-COM_WPAGroupRekey Enable SSIDAdvertisementEnabled"
`	G_Wireless[m] = [":InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00.", //Path
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
					 '<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.$10.SSIDAdvertisementEnabled?>', //IP
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

function chgHTML(){
	var main_menu = document.getElementsByClassName('main_menu');
	var td_menu   = document.getElementsByClassName('td_menu');
	//Hide the main menu
	if(main_menu[0]){
		main_menu[0].style.display = 'none';
	}
	//Hidden secondary menu
	if(td_menu[0]){
		td_menu[0].style.display = 'none';
	}
}

function uiOnload(_arc){
	
	chgHTML();
	
	$('display_process').style.display = "none"; 
	$('display_process_stop').style.display = "none";
	$('display_process_error').style.display = "none";
	
	Isp = getJsonObjn('nsp');
	Sec = getJsonObjn('nec');
	
	setJSONValue({
		'INPUT_WlanEnable'   : (Sec && Sec['INPUT_WlanEnable']) || G_Wireless[Idx][15] || '1',
		'INPUT_SSID'     : (Sec && Sec['INPUT_SSID']) || G_Wireless[Idx][1] || 'TOT',
		'vb_status'      : (Sec && Sec['vb_status']) || G_Wireless[Idx][16] || '1',
		'SELECT_Mode'    : (Sec && Sec['SELECT_Mode']) || G_Wireless[Idx][2] || 'None'
	});
	
	ctrlSecMode();
	
	onChgWepLen_auto();
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
	var nodes = $('wep','wpa');
	var mode_wpa = mode_lli = mode_WPAand11i = '';
	var mode_secury = $('lang_secure_mode');
	
	createEncryptionOpt();
	
	switch(Type){
		case 'None' : {
			nodes[0].style.display = nodes[1].style.display = 'none';
			mode_secury.style.display = 'none';	
			break;
		}
		case 'Basic' : {
			nodes[0].style.display = 'block';
			nodes[1].style.display = 'none';
			mode_secury.style.display = 'none';
			//For WEP Key
			var _id = Idx*4;
			setJSONValue({
				'SELECT_Auth'       : (Sec && Sec['SELECT_Auth']) || G_Wireless[Idx][5],
				'SELECT_WEPIndex'   : (Sec && Sec['SELECT_WEPIndex']) || G_Wireless[Idx][3],
				'SELECT_WEPKey'     : (Sec && Sec['SELECT_WEPKey']) || G_Wireless[Idx][4],
				//wep key
				'INPUT_WEPKey1'     : (Sec && Sec['INPUT_WEPKey1']) || G_WEPKey[_id+0][0],
				'INPUT_WEPKey2'     : (Sec && Sec['INPUT_WEPKey2']) || G_WEPKey[_id+1][0],
				'INPUT_WEPKey3'     : (Sec && Sec['INPUT_WEPKey3']) || G_WEPKey[_id+2][0],
				'INPUT_WEPKey4'     : (Sec && Sec['INPUT_WEPKey4']) || G_WEPKey[_id+3][0]
			});	
			break;
		}
		case 'WPA' : {
			mode_wpa = G_Wireless[Idx][7];
			mode_secury.style.display = 'block';
		}
		case '11i' : {
			mode_lli = G_Wireless[Idx][9];
			mode_secury.style.display = 'block';
			
			setJSONValue({
				'SELECT_EncryptionModes'       : (Sec && Sec['SELECT_EncryptionModes']) || G_Wireless[Idx][10],
			});
		}
		case 'WPAand11i' : {
			mode_WPAand11i = G_Wireless[Idx][9];
			
			setJSONValue({
				'SELECT_EncryptionModes'       : (Sec && Sec['SELECT_EncryptionModes']) || G_Wireless[Idx][10],
			});
			
			if(G_Wireless[Idx][10].length==0)
			{
			    $('SELECT_EncryptionModes').selectedIndex=0;
			    //Form.Select('SELECT_EncryptionModes', "AESEncryption");
			}
			if (Type == "WPA")
			{
				
				setJSONValue({
					'SELECT_EncryptionModes'       : (Sec && Sec['SELECT_EncryptionModes']) || G_Wireless[Idx][8]
				});
				
				 if(G_Wireless[Idx][8].length==0)
				{
			    	$('SELECT_EncryptionModes').selectedIndex=0;
				}
			}
			mode_secury.style.display = 'block';

			setJSONValue({
				//PSK
				'INPUT_PreShareKey'     : G_Wireless[Idx][11] == 'KeyPassphrase' ? G_Wireless[Idx][12] : G_Wireless[Idx][13]
			});

			nodes[0].style.display = 'none';
			nodes[1].style.display = 'block';			
			
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

function createJsonData(){
	var _json = '{';
		var nodes_isp = $('SELECT_WEPIndex','SELECT_WEPKey','SELECT_Auth','SELECT_EncryptionModes','SELECT_Mode');
	
	
		_json += 'INPUT_WlanEnable:"' + Form.Checkbox('INPUT_WlanEnable') + '",';
		_json += 'INPUT_SSID:"' + $('INPUT_SSID').value + '",';
		_json += 'vb_status:"' + Form.Radio('vb_status') + '",';
		_json += 'SELECT_Mode:"' + nodes_isp[4].value + '",';
		
		switch(nodes_isp[4].value){
			case 'Basic' : {
				_json += 'SELECT_WEPIndex:"' + nodes_isp[0].value + '",';
				_json += 'SELECT_WEPKey:"' + nodes_isp[1].value + '",';
				_json += 'INPUT_WEPKey1:"' + $('INPUT_WEPKey1').value + '",';
				_json += 'INPUT_WEPKey2:"' + $('INPUT_WEPKey2').value + '",';
				_json += 'INPUT_WEPKey3:"' + $('INPUT_WEPKey3').value + '",';
				_json += 'INPUT_WEPKey4:"' + $('INPUT_WEPKey4').value + '",';
				_json += 'SELECT_Auth:"' + nodes_isp[2].value + '",';
				break;
			}
			case 'WPAand11i' : {
				_json += 'SELECT_EncryptionModes:"' + nodes_isp[3].value + '",';
				break;
			}
			case '11i' : {
				_json += 'SELECT_EncryptionModes:"' + nodes_isp[3].value + '",';
				break;
			}
			case 'WPA' : {
				_json += 'SELECT_EncryptionModes:"' + nodes_isp[3].value + '",';
				break;
			}
		}
	
		_json = _json.delcomma();
	_json += '}';
	addJsonObjn('nec', _json);
	return true;
}


function uiApply(){
	$('display_process').style.display = "block"; 
	$('display_all').style.display = "none"; 
	uiAjax_Add();
	
	if(Cookie.Get('cache_wiz') != null){
		Cookie.Delete('cache_wiz');
	}
	
	$('back').disabled = true;
	$('submit').disabled = true;
	$('cancle').disabled = true;
}

function uiAjax_Add(){
	
	for(var i = 0; i < G_WanConns.length; i++){
		if(G_WanConns_num != 0 && G_WanConns[i][1] == "PVC:1/32"){
			edit_adsl = 1; 
			edit_wan_num = i;
		}
	}
	if(edit_adsl == 1){
		old_wan_username = G_WanConns[edit_wan_num][3];
		old_wan_password = G_WanConns[edit_wan_num][4];
		ajax_edit_adsl(edit_wan_num);
	}
	else{
		ajax_add_obj();
	}
}

function ajax_edit_adsl(num){
	
	var _path = G_WanConns[num][0];
	
	
	$H({
		'obj-action' : 'set',
		'var:page'   : 'wizwl_login',
		'getpage': 'html/page/wiz_ajax.js'
	}, true);

	$F(_path + 'Username',Isp['INPUT_Username']);
		
	if(Isp['INPUT_Psd'] != '********'){
		$F(_path + 'Password', Isp['INPUT_Psd']);
		$F(_path + 'TmpPassword', Isp['INPUT_Psd']);
	}
	
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_add_obj_wait);
	ajax.post($('uiPostForm'));		
}

function ajax_add_obj(){
	
	var _path = ':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANPPPConnection.0.';

	$H({
	   	':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.DestinationAddress':'PVC:1/32',//PVC
		':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.ATMEncapsulation' : "LLC", //LLC
		':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.LinkType' : "EoA", //Protocol		
		':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.Enable' : "1",
		':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.ATMPeakCellRate' : "0",
		':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.ATMMaximumBurstSize' : "0",
		':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.ATMSustainableCellRate' : "0",
		':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.X_TWSZ-COM_VLANID' : "0",
		':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.ATMQoS' : "UBR",
		'mid': '0438',
		'obj-action' : 'set',
		'var:page'   : 'wizwl_login',
		'getpage': 'html/page/wiz_ajax.js'
	}, true);

	$F(_path + 'Name', 'PVC:1/32');
	$F(_path + 'Enable','1'); 
	
	$F(_path + 'MaxMRUSize', '1492');
	$F(_path + 'Username',Isp['INPUT_Username']);
	
	$F(_path + 'Password', Isp['INPUT_Psd']);
	
	$F(_path + 'NATEnabled','1');

	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_add_obj_wait);
	ajax.post($('uiPostForm'));		
}

function ajax_add_obj_wait(_text){
	//成功, 则set
	try{
		eval(_text);
	} catch (ex){
		G_Error = '1';
		return false;
	}
	
	if (G_Error == '0'){
		uiAjax_set_wlan();
	} else if (G_Error == '1'){
		$('display_process').style.display = "none"; 
		$('display_process_stop').style.display = "block"; 
		dealWithError();
		document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wizard';
	}
}

function uiAjax_set_wlan(){

	/*根据中兴秘鲁反馈的问题，WEP加密方式下，SSID2~4的密码不应该设置成空，因此在这里做了一个判断，如果为空，则返回错误*/
	if($('SELECT_Mode').value =='Basic' &&  $('INPUT_WEPKey'+$('SELECT_WEPKey').value).value.length==0)
	{
        alert("WEPKeyIndex"+$('SELECT_WEPKey').value+" should not be empty");
		return false;
	}
	
	$H({	
	   	':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Enable'                    : Form.Checkbox('INPUT_WlanEnable'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSID'                      : $('INPUT_SSID').value,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSIDAdvertisementEnabled'  : Form.Radio('vb_status'),
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BeaconType'  : $('SELECT_Mode').value,
		'obj-action' : 'set',
		'var:page'   : 'wizwl_login',
		'getpage': 'html/page/wiz_ajax.js'
	},true);

	var _path = G_Wireless[Idx][0];
	
	//WPS2.x
	if((G_WPS[Idx][1] ==1) && (G_WPS[Idx][2]==2))
	{

		if($('SELECT_Mode').value == 'Basic')
		{
			if(confirm("If the security is WEP, the WPS would be disabled, Do you sure?"))
			{
				$F(_path + 'WPS.Enable','0');
			}
			else
			{
				return ;
			}
		}
		if($('SELECT_Mode').value == 'WPA')
		{
			if(confirm("If the security mode is WPA only, the WPS would be disabled, Do you sure?"))
			{
				$F(_path + 'WPS.Enable','0');
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
				$F(_path + 'WPS.Enable','0');
			}
			else
			{
				return ;
			}
		}	
	}

	switch($('SELECT_Mode').value){
		case 'None' : {
			$F(_path + 'BasicAuthenticationMode', 'None');
			break;
		}
		case 'Basic' : {
			$F(_path + 'BasicEncryptionModes'   , 'WEPEncryption');
			$F(_path + 'BasicAuthenticationMode', $('SELECT_Auth').value);
			$F(_path + 'WEPEncryptionLevel'     , $('SELECT_WEPIndex').value);
			$F(_path + 'WEPKeyIndex'            , $('SELECT_WEPKey').value);
			$F(_path + 'WEPKey.1.WEPKey'        , $('INPUT_WEPKey1').value);
			$F(_path + 'WEPKey.2.WEPKey'        , $('INPUT_WEPKey2').value);
			$F(_path + 'WEPKey.3.WEPKey'        , $('INPUT_WEPKey3').value);
			$F(_path + 'WEPKey.4.WEPKey'        , $('INPUT_WEPKey4').value);
			break;
		}
		case 'WPAand11i' : {
			$F(_path + 'IEEE11iEncryptionModes', $('SELECT_EncryptionModes').value);
			$F(_path + 'IEEE11iAuthenticationMode', 'PSKAuthentication');
			$F(_path + 'X_TWSZ-COM_PSKExpression', "KeyPassphrase");
			$F(_path + 'PreSharedKey.1.KeyPassphrase', $('INPUT_PreShareKey').value);
			break;
		}
		case '11i' : {
			$F(_path + 'IEEE11iEncryptionModes', $('SELECT_EncryptionModes').value);
			$F(_path + 'IEEE11iAuthenticationMode', 'PSKAuthentication');
			$F(_path + 'PreSharedKey.1.KeyPassphrase', $('INPUT_PreShareKey').value);
			$F(_path + 'X_TWSZ-COM_PSKExpression', "KeyPassphrase");
			break;
		}
		case 'WPA' : {
			$F(_path + 'WPAEncryptionModes', $('SELECT_EncryptionModes').value);
			$F(_path + 'WPAAuthenticationMode'   , 'PSKAuthentication');
			$F(_path + 'PreSharedKey.1.KeyPassphrase', $('INPUT_PreShareKey').value);
			$F(_path + 'X_TWSZ-COM_PSKExpression', "KeyPassphrase");
			break;
		}
	}
	
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, uiAjax_set_wlan_wait);
	ajax.post($('uiPostForm'));
}

function ajax_delProc(_text){
		try{
		eval(_text);
	} catch (ex){
		G_Error == '1';
		return false;
	}
	/*删除实例无论是成功和失败，最后都要跳回到开始的页面*/
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wizard';
}

function uiajax_Delete(_text){
	
	try{
		eval(_text);
	} catch (ex){
		G_Error == '1';
		return false;
	}
	/*读取实例成功，删除实例*/
	if (G_Error == '0'){
		
		var _path = G_Conns_DSL[DSL_num_ajax-1][0];//DSL_num_ajax和G_Conns_DSL[DSL_num_ajax-1][0]是执行wizsum_ajax.js获取到的值

		$H({
			'mid': '0430',
			'del-obj'       : _path,
			'obj-action' 	: 'del',
			'var:page'   	: 'wizsum',
			'getpage'		: 'html/page/wiz_ajax.js'
		}, true);
		
		var _url = "/cgi-bin/webproc";
		ajax = Ajax.getInstance(_url, "", 0, ajax_delProc);
		ajax.post($('uiPostForm'));	
	} else if (G_Error == '1'){
		dealWithError();
		document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wizard';
	}
}
/**获取已经建好的wan连接的路径**/
function read_object(){
	$('display_process_stop').style.display = "none";
	$('display_process_error').style.display = "block";
	$H({
		'mid': '0430',
		'obj-action' 	: 'get',
		'var:page'   	: 'wizsum',
		'getpage'		: 'html/page/wizsum_ajax.js'
	}, true);
	
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, uiajax_Delete);
	ajax.post($('uiPostForm'));	
}


function ajax_reset_wan_wait(_text){
	try{
		eval(_text);
	} catch (ex){
		G_Error == '1';
		return false;
	}
	/*恢复实例无论是成功和失败，最后都要跳回到开始的页面*/
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wizard';
}

function uiajax_reset_wan(){
	
	$('display_process_stop').style.display = "none";
	$('display_process_error').style.display = "block";
	
	var _path = G_WanConns[edit_wan_num][0];
	
	$H({
		'obj-action' : 'set',
		'var:page'   : 'wizwl_login',
		'getpage': 'html/page/wiz_ajax.js'
	}, true);

	$F(_path + 'Username',old_wan_username);
		
	if(Isp['INPUT_Psd'] != '********'){
		$F(_path + 'Password', old_wan_password);
		$F(_path + 'TmpPassword', old_wan_password);
	}
	
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_reset_wan_wait);
	ajax.post($('uiPostForm'));			
}

function uiAjax_set_wlan_wait(_text){
	//成功,则跳转页雿
	
	try{
		eval(_text);
	} catch (ex){
		G_Error == '1';
		return false;
	}
	
	if (G_Error == '0'){
		$G('/cgi-bin/webproc',{
		'getpage'			: 'html/index.html',
		'var:menu'			: 'status',
		'var:page'			: 'deviceinfo'
		});	
	} else if(G_Error == '1'){
		$('display_process').style.display = "none"; 
		$('display_process_stop').style.display = "block";
		dealWithError();
		if(edit_adsl == 0){
			read_object();
		}
		else{
			uiajax_reset_wan();      //如果wan连接已经设置成功，但后面的参数设置失败，为了保持整个快速配置的原子性，需要将已经建好的wan恢复配置。
		}
	}
}

function uiBack(){
	
	createJsonData();
	
	$H({
		'var:menu'	: 'setup',
		'var:subpage' : 'wizisp_login',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	});
	
	$('uiPostForm').submit();
}

function uiCancle(){
	if(confirm("Quit setup wizard and discard settings?")){
		$H({
			'var:menu' : 'status',
			'var:page' : 'deviceinfo',
			'getpage'  : 'html/index.html'
		});
		$('uiPostForm').submit();
	}
	else{
		$H({
			'var:menu' : 'advanced',
			'var:page' : 'wadvance',
			'getpage'  : 'html/index.html'
		});
		$('uiPostForm').submit();		
	}
}


function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
	
}

addListeners(uiOnload, dealWithError);
