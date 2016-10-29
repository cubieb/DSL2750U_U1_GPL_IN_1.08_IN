/* ------------Wan Config-----------*/
//ATM
var G_ATM = [];
var m = 0;
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "X_TWSZ-COM_VLANID"
`	G_ATM[m] = ["InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$00.",//Path
				"$01" //X_TWSZ-COM_VLANID
				];
	m++;
`?>

//Wan Conns----$var:secIdx ->第二层；$var:thdIdx -> 第三层
var G_WanConns = [];
//var m = 0;
<?if eq $var:conn_type `IP`
`	<?mget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx. "Enable Name NATEnabled ExternalIPAddress SubnetMask DefaultGateway DNSOverrideAllowed X_TWSZ-COM_UsrDNSServers AddressingType ConnectionType"
	`	G_WanConns['Path'] = "InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx."; //Path
	    G_WanConns['Enable']                   = "$01";//Enable
	    G_WanConns['Name']                     = "$02";//Name
	    G_WanConns['NATEnabled']               = "$03";//NATEnabled
	    G_WanConns['ExternalIPAddress']        = "$04";//ExternalIPAddress
	    G_WanConns['SubnetMask']               = "$05";//SubnetMask
	    G_WanConns['DefaultGateway']           = "$06";//DefaultGateway
		G_WanConns['DNSOverrideAllowed']       = "$07";//DNSOverrideAllowed
	    G_WanConns['X_TWSZ-COM_UsrDNSServers'] = "$08";//X_TWSZ-COM_UsrDNSServers
		G_WanConns['AddressingType']           = "$09";//AddressingType
		G_WanConns['ConnectionType']           = "$0a";//ConnectionType
	`?>
`?>
<?if eq $var:conn_type `PPP`
`	<?mget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx. "Enable NATEnabled Username PPPAuthenticationProtocol ConnectionTrigger IdleDisconnectTime MaxMTUSize PPPLCPEchoRetry X_TWSZ-COM_StaticIPAddress X_TWSZ-COM_Isp X_TWSZ-COM_Apn X_TWSZ-COM_DialNumber X_TWSZ-COM_ModemBaud Backup3GEnable IdleDelayTime"
	`	G_WanConns['Path'] = "InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx.", //Path
		G_WanConns['Enable']                    = "$01",//Enable
		G_WanConns['NATEnabled']                = "$02",//NATEnabled
		G_WanConns['Username']                  = "$03",//Username
	    G_WanConns['Password']                  = "**********",//Password
	    G_WanConns['PPPAuthenticationProtocol'] = "$04",//PPPAuthenticationProtocol
	    G_WanConns['ConnectionTrigger']         = "$05",//ConnectionTrigger
	    G_WanConns['IdleDisconnectTime']        = "$06",//IdleDisconnectTime
	    G_WanConns['MaxMTUSize']                = "$07",//MaxMRUSize
	    G_WanConns['PPPLCPEchoRetry']           = "$08",//PPPLCPEchoRetry
	    G_WanConns['X_TWSZ-COM_StaticIPAddress']= "$09" //X_TWSZ-COM_StaticIPAddress
		G_WanConns['X_TWSZ-COM_Isp']     		= "$0a";
		G_WanConns['X_TWSZ-COM_Apn']     		= "$0b";
		G_WanConns['X_TWSZ-COM_DialNumber']     = "$0c";
		G_WanConns['X_TWSZ-COM_ModemBaud']     	= "$0d";
		G_WanConns['Backup3GEnable']     		= "$0e";
		G_WanConns['IdleDelayTime']     		= "$0f";
	`?>
`?>

var G_Conns = [];
var n = 0;
<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable AddressingType"
		`	G_Conns[n] = [n,
						  "InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$20.WANIPConnection.$00.",
						  "$02"
						  ];
			n++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger"
		`	G_Conns[n] = [n,
						  "InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$20.WANPPPConnection.$00.",
						  "$02"
						  ];
			n++;
		`?>
	`?>
`?>

var _m = -1;

(function (){
	if('<?echo $var:conn_type?>' != '-'){
		for(var i = 0, _len = G_Conns.length; i < _len; i++){
			if(G_WanConns['Path'] == G_Conns[i][1]){
				n = i;
			}
		}
	}
})();

var _protocoltype;

function uiOnload(){
	//find _m's value
	/*for(var i = 0, _i; i < G_ATM.length; i++){
		if(G_WanConns['Path'] &&  G_WanConns['Path'].indexOf(G_ATM[i][0])){
			_m = i;
			break;
		}
	}*/

	var secIdx = '<?echo $var:secIdx?>';
	
	if(secIdx != '-'){
		for(var i = 0, _len = G_ATM.length; i < _len; i++){
			if(secIdx == G_ATM[i][0].split('.')[4]){
				_m = i;
				break;
			}
		}
	}
	
	//setJSONValue({
	//	   : 
		
	//});	
	_protocoltype = findProtocol();
	//WAN赋?
	onchgProtocol();

	//onclkIpAddr_1();
}

//拨号方式
function uiDialUp(){
	var mode = $('SELECT_DialupMode').value;
	var bool = false;
	
	if(mode == 'OnDemand'){
		bool = true;
	}
	
	disCtrl('inatimeout',bool);
	
}

function uiBackupEnable(){
	if(Form.Checkbox('INPUT_EnableBackup') == '1'){
		disCtrl('indelay_timeout',1);
		if ($('INPUT_DelayTimeout').value < 0){
			$('INPUT_DelayTimeout').value = '5';
		}
	} else {
		disCtrl('indelay_timeout',0);
	}
}

//找出Protocol类型
function findProtocol(){
	var _protocol = 'PPPoU';
	
	return _protocol;
}

function onchgProtocol(){
			setJSONValue({
				'INPUT_PPPOU_Isp'   		: G_WanConns['X_TWSZ-COM_Isp'] || 'CDMA2000',
				'INPUT_PPPOU_Apn'   		: G_WanConns['X_TWSZ-COM_Apn'] || 'UNINET',
				'INPUT_PPPOU_DialNumber'   	: G_WanConns['X_TWSZ-COM_DialNumber'] || '#777',
				'INPUT_PPPOU_ModemBaud'   	: G_WanConns['X_TWSZ-COM_ModemBaud'] || '460800',

				'INPUT_PPPOU_Act'   		: G_WanConns['Username'] || '',
				'INPUT_PPPOU_Psd'   		: G_WanConns['Password'] || '',
				'INPUT_PPPOU_ConfirmPsd' 	: G_WanConns['Password'] || '',
				'INPUT_PPPOU_MTU'  			: G_WanConns['MaxMTUSize'] || '1400',
				'SELECT_DialupMode'  : G_WanConns['ConnectionTrigger'] || 'OnDemand',
				'INPUT_InaTimeout'    : G_WanConns['IdleDisconnectTime'] || '60',

				'INPUT_EnableBackup'		: G_WanConns['Backup3GEnable'] || '1',
				'INPUT_DelayTimeout'		: G_WanConns['IdleDelayTime'] || '60',
				
				'SELECT_PPPOU_AuthMethod' 	: G_WanConns['PPPAuthenticationProtocol'] || 'Auto',
				'INPUT_EnableNat'     : G_WanConns['NATEnabled'] || '1',
				'INPUT_EnableWanSv'   : G_WanConns['Enable'] || '1',
				'INPUT_ServiceName'   : 'pppou_' + n
				});
				
	uiDialUp();
	uiBackupEnable();
	//disCtrl('INPUT_DelayTimeout',Form.Checkbox('INPUT_EnableBackup'));
//	disCtrl('inatimeout',Form.Checkbox('INPUT_DialOnDemand'));
	//disCtrl('s_ipAddr',Form.Checkbox('INPUT_StaticIpAddr'));
//	onclkPppAddr();
//	onclkIpAddr_1();
	//onclkIpAddr_2();
	//onclkIpAddr_4();	
}

//PPP Address -> all
function onclkPppAddr(){	
	var bool = Form.Radio('ppp_gw') == '0' ? false : true;	
	
	if(bool){
		onclkGwPpp();
	} else {
		disCtrl('ppp_addr1', bool);
		disCtrl('ppp_addr2', bool);		
	}
}

//PPP Address -> 1/2
function onclkGwPpp(){
	var bool = Form.Radio('ppp_ipaddr') == '0' ? false : true;
	
	disCtrl('ppp_addr1', !bool);
	disCtrl('ppp_addr2', bool);
	$('INPUT_WanIpAddr_PPP').disabled = $('INPUT_RemoteIpAddr_PPP').disabled = false;
}

//IP Address -> 1
function onclkIpAddr_1(){
	var bool = Form.Radio('INPUT_AutoIpAddr') == 'Static' ? true : false;
	
	disCtrl('ip_addr1', bool);
	disCtrl('ip_addr2', bool);
}

//IP Address -> 2
function onclkIpAddr_2(){	
	var bool = Form.Radio('INPUT_DefautlGw_Ip') == '0' ? false : true;	
	
	if(bool){
		onclkIpAddr_3();
	} else {
		disCtrl('ip_addr2', bool);
		disCtrl('ip_addr3', bool);		
	}
}

//IP Address -> 3
function onclkIpAddr_3(){
	var bool = Form.Radio('INPUT_RemoteIpAddr_IP') == '0' ? false : true;
	var node_name = document.getElementsByName('INPUT_RemoteIpAddr_IP');
	
	disCtrl('ip_addr2', !bool);
	disCtrl('ip_addr3', bool);
	node_name[0].disabled = node_name[1].disabled = false;
}

//IP Address -> 4
function onclkIpAddr_4(){
	var bool = Form.Radio('INPUT_DnsServer') == '0' ? false : true;
	
	disCtrl('ip_addr4', bool);	
}

function onchgSetVlan(){
	var value_vlanid = $('INPUT_VlanId').value;
	var value_name = $('INPUT_ServiceName').value;
	
	//for example: br_0_35_10
	var _split = value_name.split('_');
	if(_split){
		$('INPUT_ServiceName').value = _split[0] + '_' + value_vlanid + '_' + _split[2];
	}	
}
//判断当前的协议是IP, 还是PPP
function getProtocol(){
	return ['PPPoU','PPP'];
}

function checkValue(){
	if($('INPUT_PPPOU_Psd').value != $('INPUT_PPPOU_ConfirmPsd').value){
		alert(SEcode[1010]);
		return false;
	}
	
	return true;
}

function uiSubmit(){
	var _protocol = _protocoltype;
	var _node; //存放节点的数?
	var _path = ':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.';
	
	
	if(!checkValue()){
		return false;
	}
	
	var _pro = getProtocol(); //取得一个array,?['Route','IP']
	//统一的Form内容
	$H({
		'obj-action'    : 'set',
		'var:menu'      : G_Menu,
		'var:page'      : G_Page,
		'var:errorpage' : G_Page,
		'var:conn_type' : '<?echo $var:conn_type?>',
		'var:secIdx'    : '<?echo $var:secIdx?>',
		'var:thdIdx'    : '<?echo $var:thdIdx?>',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html'
	},true);
	
	if(_m != -1){
		_path = ':' + G_ATM[_m][0];
	}

	$F(_path + 'WANDSLLinkConfig.Enable', '1');
	//Vlan ID
	//$F(_path + 'X_TWSZ-COM_VLANID', $('INPUT_VlanId').value);
	
  
	//判断是IP还是PPP
	//是否交由webproxy处理
	if(_m == -1){ // -> new & to webproxy
		$F('mid','0438');
		_path = _path + 'WAN' + _pro[1] + 'Connection.0.';
	} else { // -> modify
		if(G_WanConns['Path'] && G_WanConns['Path'].indexOf(_pro[1]) > -1){ // -> not to webproxy
			_path = ':' + G_WanConns['Path'];
		} else { // -> to webproxy
			$F('mid','0438');
			_path = ':' + G_WanConns['Path'].replace(_pro[1] == 'IP' ? 'PPP' : 'IP', _pro[1]);
			$F(_path + 'X_TWSZ-COM_ProtocolType', G_WanConns['Path'].indexOf('IP') > 0 ? 'PPP' : 'IP');
			
			//为配合模块检查，将IP/PPP的前的实例号改为0.
			_path = _path.substr(0, _path.length - 2) + '0.'; // -> InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANIPConnection.0.
		}
	}
	//公共配置
	$F(_path + 'NATEnabled', Form.Checkbox('INPUT_EnableNat'));
	$F(_path + 'Enable', Form.Checkbox('INPUT_EnableWanSv'));
	$F(_path + 'Name'  , $('INPUT_ServiceName').value);

	$F(_path + 'X_TWSZ-COM_Isp', 			$('INPUT_PPPOU_Isp').value);
	$F(_path + 'X_TWSZ-COM_Apn', 			$('INPUT_PPPOU_Apn').value);
	$F(_path + 'X_TWSZ-COM_DialNumber',  	$('INPUT_PPPOU_DialNumber').value);
	$F(_path + 'X_TWSZ-COM_ModemBaud',  	$('INPUT_PPPOU_ModemBaud').value);
	$F(_path + 'Username', 					$('INPUT_PPPOU_Act').value);
	if ($('INPUT_PPPOU_Psd').value != '**********')
	{
		$F(_path + 'Password', 				$('INPUT_PPPOU_Psd').value);
	}
	$F(_path + 'ConnectionType', 			'PPPoU');
	$F(_path + 'PPPAuthenticationProtocol', $('SELECT_PPPOU_AuthMethod').value);
	$F(_path + 'MaxMTUSize', 				$('INPUT_PPPOU_MTU').value);
	$F(_path + 'ConnectionTrigger'  , $('SELECT_DialupMode').value);
	if($('SELECT_DialupMode').value == 'OnDemand'){
		$F(_path + 'IdleDisconnectTime' , $('INPUT_InaTimeout').value);
	}

	$F(_path + 'Backup3GEnable'  , Form.Checkbox('INPUT_EnableBackup'));
	if(Form.Checkbox('INPUT_EnableBackup') == '1'){
		$F(_path + 'IdleDelayTime' , $('INPUT_DelayTimeout').value);
	}
	
	$('uiPostForm').submit();
}

//checkbox状态同?
function uiSynaEnable(){
	if(Form.Checkbox('INPUT_EnableBridgeSv') == '1'){
		Form.Checkbox('INPUT_EnableWanSv','1');
	} else {
		Form.Checkbox('INPUT_EnableWanSv','0');
	}
}

function uiPageRefresh(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wan3g&var:conn_type=<?echo $var:conn_type?>&var:secIdx=<?echo $var:secIdx?>&var:thdIdx=<?echo $var:thdIdx?>';
}

function dealWithError(){
	if (G_Error != 1){
		return false; 
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
	
}

addListeners(uiOnload, dealWithError);
