/* ------------Wan Config-----------*/
//ATM
var G_ATM = [];
var m = 0;
<?if neq 1 $var:mod_WAN_SERVICETYPE
	`	
	var G_Service_Type = 'Internet';//'Internet';
`?>
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANDSLLinkConfig.LinkType WANDSLLinkConfig.DestinationAddress WANDSLLinkConfig.ATMEncapsulation WANDSLLinkConfig.ATMQoS WANDSLLinkConfig.ATMPeakCellRate WANDSLLinkConfig.ATMSustainableCellRate WANDSLLinkConfig.ATMMaximumBurstSize X_TWSZ-COM_VLANID X_TWSZ-COM_ProxyArp X_TWSZ-COM_QinQEnable X_TWSZ-COM_VLANPriority X_TWSZ-COM_FirewallEnable"
`	G_ATM[m] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$00.",//Path
				"$01",//LinkType
				"$02",//DestinationAddress
				"$03",//ATMEncapsulation
				"$04",//ATMQoS
				"$05",//ATMPeakCellRate
				"$06",//ATMSustainableCellRate
				"$07",//ATMMaximumBurstSize
				"$08", //X_TWSZ-COM_VLANID
				"$09",//X_TWSZ-COM_ProxyArp
				"$0a",//X_TWSZ-COM_QinQ
				"$0b", //X_TWSZ-COM_VLANPriority
				"$0c" //X_TWSZ-COM_FirewallEnable
				];
	m++;
`?>

//Wan ConnService List
var G_WanConnServiceList = [];
var m = 0;
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "X_TWSZ-COM_ServiceList"
		`	G_WanConnServiceList[m] = "$01";
			++m;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "X_TWSZ-COM_ServiceList"
		`	G_WanConnServiceList[m] = "$01"
;
			++m;
		`?>
	`?>
`?>
//Wan Conns----$var:secIdx ->第二层；$var:thdIdx -> 第三层
var G_WanConns = [];
<?if neq $var:G_IPv6Enable 1
`
	<?if eq $var:conn_type `IP`
	`	
		<?mget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx. "Enable Name NATEnabled ExternalIPAddress SubnetMask DefaultGateway DNSOverrideAllowed DNSServers AddressingType ConnectionType X_TELEFONICA-ES_NATType X_TWSZ-COM_ServiceList Backup3G"
		`	G_WanConns['Path']						= "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx."; //Path
			G_WanConns['Enable']					= "$01";//Enable
			G_WanConns['Name']						= "$02";//Name
			G_WanConns['NATEnabled']				= "$03";//NATEnabled
			G_WanConns['ExternalIPAddress']			= "$04";//ExternalIPAddress
			G_WanConns['SubnetMask']				= "$05";//SubnetMask
			G_WanConns['DefaultGateway']			= "$06";//DefaultGateway
			G_WanConns['DNSOverrideAllowed']		= "$07";//DNSOverrideAllowed
			G_WanConns['X_TWSZ-COM_UsrDNSServers']	= "$08";//X_TWSZ-COM_UsrDNSServers
			G_WanConns['AddressingType']			= "$09";//AddressingType
			G_WanConns['ConnectionType']			= "$0a";//ConnectionType
			G_WanConns['X_TELEFONICA-ES_NATType']	= '$0b'; //X_TELEFONICA-ES_NATType
			G_WanConns['X_TWSZ-COM_ServiceList']	= "$0c";//X_TWSZ-COM_ServiceList
			G_WanConns['Backup3G']					= "$0h";//Backup3G
		`?>
	`?>
`?>
<?if eq $var:G_IPv6Enable 1
`
	<?if eq $var:conn_type `IP`
	`	
		<?mget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx. "Enable Name NATEnabled ExternalIPAddress SubnetMask DefaultGateway DNSOverrideAllowed DNSServers AddressingType ConnectionType X_TELEFONICA-ES_NATType X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv6Enabled X_TELEFONICA-ES_IPv4Enabled X_TELEFONICA-ES_IPv6PrefixDelegationEnabled X_TELEFONICA-ES_DHCP6cForAddress Backup3G"
		`	G_WanConns['Path'] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx."; //Path
			G_WanConns['Enable']										= "$01";//Enable
			G_WanConns['Name']											= "$02";//Name
			G_WanConns['NATEnabled']									= "$03";//NATEnabled
			G_WanConns['ExternalIPAddress']								= "$04";//ExternalIPAddress
			G_WanConns['SubnetMask']									= "$05";//SubnetMask
			G_WanConns['DefaultGateway']								= "$06";//DefaultGateway
			G_WanConns['DNSOverrideAllowed']							= "$07";//DNSOverrideAllowed
			G_WanConns['X_TWSZ-COM_UsrDNSServers']						= "$08";//X_TWSZ-COM_UsrDNSServers
			G_WanConns['AddressingType']								= "$09";//AddressingType
			G_WanConns['ConnectionType']								= "$0a";//ConnectionType
			G_WanConns['X_TELEFONICA-ES_NATType']						= '$0b'; //X_TELEFONICA-ES_NATType
			G_WanConns['X_TWSZ-COM_ServiceList']						= "$0c";//X_TWSZ-COM_ServiceList
			G_WanConns['X_TELEFONICA-ES_IPv6Enabled']					= "$0d"; //ipv6 enable
			G_WanConns['X_TELEFONICA-ES_IPv4Enabled']					= "$0e"; //ipv4 enable
			G_WanConns['X_TELEFONICA-ES_IPv6PrefixDelegationEnabled']	= "$0f";//prefix delegation enable
			G_WanConns['X_TELEFONICA-ES_DHCP6cForAddress']				= "$0g";//prefix delegation enable
			G_WanConns['Backup3G']										= "$0h";//Backup3G
		`?>
	`?>
`?>
<?if neq $var:G_IPv6Enable 1
`
    <?if eq $var:conn_type `PPP`
	`	<?mget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx. "Enable NATEnabled Username PPPAuthenticationProtocol ConnectionTrigger IdleDisconnectTime MaxMRUSize PPPLCPEchoRetry X_TWSZ-COM_StaticIPAddress X_TELEFONICA-ES_NATType PPPLCPEcho X_TWSZ-COM_ServiceList MaxMTUSize ConnectionType PassThrough Backup3G"
		`	G_WanConns['Path'] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx."; //Path
			G_WanConns['Enable']						= "$01";//Enable
			G_WanConns['NATEnabled']					= "$02";//NATEnabled
			G_WanConns['Username']						= "$03";//Username
			G_WanConns['Password']						= "**********";//Password
			G_WanConns['PPPAuthenticationProtocol']		= "$04";//PPPAuthenticationProtocol
			G_WanConns['ConnectionTrigger']				= "$05";//ConnectionTrigger
			G_WanConns['IdleDisconnectTime']			= "$06";//IdleDisconnectTime
			G_WanConns['MaxMRUSize']					= "$07";//MaxMRUSize
			G_WanConns['PPPLCPEchoRetry']				= "$08";//PPPLCPEchoRetry
			G_WanConns['X_TWSZ-COM_StaticIPAddress']	= "$09"; //X_TWSZ-COM_StaticIPAddress
			G_WanConns['X_TELEFONICA-ES_NATType']		= "$0a";	//X_TELEFONICA-ES_NATType
			G_WanConns['PPPLCPEcho']					= "$0b";//PPPLCPEcho
			G_WanConns['X_TWSZ-COM_ServiceList']		= "$0c"; //X_TWSZ-COM_ServiceList
			G_WanConns['MaxMTUSize']					= "$0d";//MaxMTUSize
			G_WanConns['ConnectionType']				= "$0e";//ConnectionType
			G_WanConns['PassThrough']					= "$0f";//PassThrough
			G_WanConns['Backup3G']						= "$0g";//Backup3G
		`?>
	`?>
`?>
<?if eq $var:G_IPv6Enable 1
`
	<?if eq $var:conn_type `PPP`
	`
		<?mget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx. "Enable NATEnabled Username PPPAuthenticationProtocol ConnectionTrigger IdleDisconnectTime MaxMRUSize PPPLCPEchoRetry X_TWSZ-COM_StaticIPAddress X_TELEFONICA-ES_NATType PPPLCPEcho X_TWSZ-COM_ServiceList X_TELEFONICA-ES_IPv6Enabled X_TELEFONICA-ES_IPv4Enabled X_TELEFONICA-ES_IPv6PrefixDelegationEnabled X_TELEFONICA-ES_DHCP6cForAddress MaxMTUSize ConnectionType PassThrough Backup3G"
		`	G_WanConns['Path'] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx."; //Path
			G_WanConns['Enable']										= "$01";//Enable
			G_WanConns['NATEnabled']									= "$02";//NATEnabled
			G_WanConns['Username']										= "$03";//Username
			G_WanConns['Password']										= "**********";//Password
			G_WanConns['PPPAuthenticationProtocol']						= "$04";//PPPAuthenticationProtocol
			G_WanConns['ConnectionTrigger']								= "$05";//ConnectionTrigger
			G_WanConns['IdleDisconnectTime']							= "$06";//IdleDisconnectTime
			G_WanConns['MaxMRUSize']									= "$07";//MaxMRUSize
			G_WanConns['PPPLCPEchoRetry']								= "$08";//PPPLCPEchoRetry
			G_WanConns['X_TWSZ-COM_StaticIPAddress']					= "$09"; //X_TWSZ-COM_StaticIPAddress
			G_WanConns['X_TELEFONICA-ES_NATType']						= "$0a";	//X_TELEFONICA-ES_NATType
			G_WanConns['PPPLCPEcho']									= "$0b";//PPPLCPEcho
			G_WanConns['X_TWSZ-COM_ServiceList']						= "$0c"; //X_TWSZ-COM_ServiceList
			G_WanConns['X_TELEFONICA-ES_IPv6Enabled']					= "$0d"; //ipv6 enable
			G_WanConns['X_TELEFONICA-ES_IPv4Enabled']					= "$0e"; //ipv4 enable
			G_WanConns['X_TELEFONICA-ES_IPv6PrefixDelegationEnabled']	= "$0f";//prefix delegation enable
			G_WanConns['X_TELEFONICA-ES_DHCP6cForAddress']				= "$0g";//X_TELEFONICA-ES_DHCP6cForAddress
			G_WanConns['MaxMTUSize']									= "$0h";//MaxMTUSize
			G_WanConns['ConnectionType']								= "$0i";//ConnectionType
			G_WanConns['PassThrough']									= "$0j";//PassThrough
			G_WanConns['Backup3G']										= "$0k";//Backup3G
		`?>
	`?>
`?>
<?if eq $var:mod_NATPUBIPADDR 1
`
    <?if eq $var:conn_type `PPP`
	`
	    <?mget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx. "Natipaddr"
	    `    G_WanConns['Natipaddr']								    = "$01";//Natipaddr
	    `?>
	`?>
	<?if eq $var:conn_type `IP`
	`
	    <?mget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx. "Natipaddr"
	    `    G_WanConns['Natipaddr']								    = "$01";//Natipaddr
	    `?>
	`?>
`?>

var G_Conns = [];
var n = 0;
<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable AddressingType"
		`	G_Conns[n] = [n,
						"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00.",
						"$02",
						"$20"
						];
			n++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger"
		`	G_Conns[n] = [n,
						"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00.",
						"$02",
						"$20"
						];
			n++;
		`?>
	`?>
`?>

var G_IP6_info = [];
//var m = 0;

<?if eq $var:G_IPv6Enable 1
`	<?if eq $var:conn_type `IP`
	`	<?mget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx.X_TWSZ-COM_IPv6Config. "IPv6AddressList.1.IPv6Address IPv6AddressList.1.PrefixLength IPv6AddressList.1.AddressStatus IPv6DNSOverrideAllowed IPv6DNSServers UsrIPv6DNSServers IPv6DefaultRouterList.1.DefaultRouter IPv6DefaultRouterList.2.DefaultRouter IPv6SitePrefixInfo.IPv6SitePrefix IPv6SitePrefixInfo.IPv6SitePrefixLength"
		`	G_IP6_info['Path'] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx."; //Path
			G_IP6_info['address']			= "$01";//Enable
			G_IP6_info['addrprefixlen']		= "$02";//Name
			G_IP6_info['status']			= "$03";//NATEnabled
			G_IP6_info['override']			= "$04"; 
			G_IP6_info['server']			= "$05";
			G_IP6_info['userserver']		= "$06";
			G_IP6_info['gateway']			= "$07";
			G_IP6_info['gatewayauto']		= "$08";
			G_IP6_info['prefix']			= "$09";
			G_IP6_info['prefixlen']			= "$0a";
		`?>
	`?>
`?>



var wan_service_type = 0;

<?if eq 1 $var:mod_WAN_SERVICETYPE
`
   wan_service_type += 1;
`?>

var _m = -1; //璁板綍ATM鐨勬暟缁勪笅?

//鍔ㄦ€佺敓鎴愭湇鍔″悕绉扮殑缂栧彿:n
(function (){
	if('<?echo $var:conn_type?>' != '-'){
		for(var i = 0, _len = G_Conns.length; i < _len; i++){
			if(G_WanConns['Path'] == G_Conns[i][1]){
				n = i;
			}
		}
	}
})();

//鍔ㄦ€佺敓鎴怶an Interface
function createWanInfOpt(select_name){
	var _text = [], _value = [];
	for(var i = 0, _len = G_WanConn.length; i < len; i++){
		_text.push(G_WanConn[i][0]);
		_value.push(G_WanConn[i][1]);
	}
	
	$S(select_name, _text, _value);
}
function change_ipv4_enable()
{
	var ipv4_enable = Form.Checkbox('INPUT_EnableIPv4');
	$('ip_addr_ipv4').style.display = (ipv4_enable == '1') ? 'block' : 'none';
	if (ipv4_enable == '1') 
	{
		Form.Checkbox('INPUT_EnableNat', '1');
	}
<?if eq $var:G_IPv6Enable 1 
` 
	var ipv6_enable = Form.Checkbox('INPUT_EnableIPv6');
	if ((ipv6_enable == '1') && (ipv4_enable == '0')) 
	{
		Form.Checkbox('INPUT_EnableNat', '0');
	}
	
`?>
	onclkNatEnabled();
}
function change_ipv6_enable()
{
	var ipv4_enable = Form.Checkbox('INPUT_EnableIPv4');

<?if eq $var:G_IPv6Enable 1 
` 
	var ipv6_enable = Form.Checkbox('INPUT_EnableIPv6');
	if($('SELECT_Protocol').value != 'IPoA')
	{
		$('ip_addr_ipv6').style.display = (ipv6_enable == '1') ? 'block' : 'none';
		if ((ipv6_enable == '1') && (ipv4_enable == '0')) 
		{
			Form.Checkbox('INPUT_EnableNat', '0');
		}
		if ($('SELECT_Protocol').value != 'Bridging')
		{
			$("IPV6_CFG").style.display = ((ipv6_enable == '1') ? 'block' : 'none'); 
		}
       
	}
	else
	{
		$('ip_addr_ipv6').style.display = 'none';
	}

	if (ipv6_enable == '1')
	{
		Form.Checkbox('INPUT_IPv6PrefixDelegationEnabled', '1');
		Form.Checkbox('INPUT_IPv6IANAEnabled', '1');
	}
`?>
	onclkNatEnabled();
}

//查找当前PVC及相应的WAN连接
function matchPVC(){
	var secIdx = '<?echo $var:secIdx?>';
	
	if(secIdx != '-'){
		for(var i = 0, _len = G_ATM.length; i < _len; i++){
			if(secIdx == G_ATM[i][0].split('.')[4]){
				_m = i;
				break;
			}
		}
	}
}

function WanConnServiceListCount()
{
	var count= 0;
	for(i = 0; i < G_WanConnServiceList.length; i++)
	{
				if(G_WanConnServiceList[i] =='TR069')
					count ++;
	}
	return count;
}

<?if eq $var:G_IPv6Enable 1

`function ipv6_info_load()
{
	var secIdx = '<?echo $var:secIdx?>';
	if(secIdx != '-'){
		//$('SELECT_Protocol_R').disabled = true;
		if (G_WanConns['X_TELEFONICA-ES_IPv6Enabled'] == '1') {
			if (G_WanConns['AddressingType'] == 'Static') {
				setJSONValue({
					 'INPUT_WanIpAddr_IP6'		:     G_IP6_info['address'],
					 'INPUT_WanPrefix_IP6'		:	  G_IP6_info['addrprefixlen'],
					 'INPUT_IPv6_DefGateway'    :	  G_IP6_info['gateway'] ? G_IP6_info['gateway'] : G_IP6_info['gatewayauto'],
					 'INPUT_IPv6_Prefix'        :	  G_IP6_info['prefix'],
					 'INPUT_IPv6_PrefixLen'     :	  G_IP6_info['prefixlen'],
					 'INPUT_IPv6_DnsServer_P'   :	  G_IP6_info['userserver'] ? G_IP6_info['userserver'].split(',')[0] : (G_IP6_info['server'] ? G_IP6_info['server'].split(',')[0] : ''),
					 'INPUT_IPv6_DnsServer_A'   :	  G_IP6_info['userserver'] ? (G_IP6_info['userserver'].split(',')[1]||'') : (G_IP6_info['server'] ? (G_IP6_info['server'].split(',')[1]||'') : '')
				});
			}
		}
		$("IPV6_CFG").style.display = ((G_WanConns['X_TELEFONICA-ES_IPv6Enabled'] == '1') ? 'block' : 'none'); 
		Form.Checkbox('INPUT_IPv6PrefixDelegationEnabled', G_WanConns['X_TELEFONICA-ES_IPv6PrefixDelegationEnabled']);
		Form.Checkbox('INPUT_IPv6IANAEnabled', G_WanConns['X_TELEFONICA-ES_DHCP6cForAddress']);
	}		
}
`?>

<?if eq $var:mod_NATPUBIPADDR 1
`
function onchgNatMode()
{
    var value_natmode = $('SELECT_NatIPMode').value;
	if(value_natmode == 'AUTO_IP')
	{
		$('INPUT_NatIPAddr').value = '';
		$('INPUT_NatIPAddr').disabled = true
	}
	else if(value_natmode == 'MENU_IP')
	{
		$('INPUT_NatIPAddr').disabled = false;
		$('INPUT_NatIPAddr').value = G_WanConns['Natipaddr']? ((G_WanConns['Natipaddr'] == '0.0.0.0')? '' : G_WanConns['Natipaddr']) : '';
	}
}
`?>

function uiOnload(){
	$('qinq_enable').style.display = 'none';
	matchPVC();
	//ATM赋值
	<?if eq 1 $var:mod_NEW3G
	`
	setJSONValue({
		'INPUT_VPI'          : G_ATM[_m] ? G_ATM[_m][2].split(':')[1].split('/')[0] || '0' : '0',
		'INPUT_VCI'          : G_ATM[_m] ? G_ATM[_m][2].split('/')[1] || '0' : '35',
		'SELECT_Service'     : G_ATM[_m] ? G_ATM[_m][4] : 'UBR',
		'INPUT_PeakCellRate' : G_ATM[_m] ? G_ATM[_m][5] : '0',
		'INPUT_SbCellRate'   : G_ATM[_m] ? G_ATM[_m][6] : '0',
		'INPUT_MaxBurstSize' : G_ATM[_m] ? G_ATM[_m][7] : '0',
		//'INPUT_EnableQoS'    : '1'
		
		'SELECT_EncapMode'   : G_ATM[_m] ? G_ATM[_m][3] : 'LLC',
		'SELECT_Protocol'    : findProtocol(),
		'INPUT_VlanId'       : G_ATM[_m] ? G_ATM[_m][8] : '0',
		'INPUT_QinQEnable'   : G_ATM[_m] ? G_ATM[_m][10]: '0',
		'INPUT_Priority'     : G_ATM[_m] ? G_ATM[_m][11]: '0',
		'INPUT_FirewallEnable'     : G_ATM[_m] ? G_ATM[_m][12]: '1',
		'en_proxyarp'        : G_ATM[_m] ? G_ATM[_m][9] : '0',
		'INPUT_backup3G_enable' : G_WanConns['Backup3G'] == '0' ? '0' : '1'
	});	
	`?>
	<?if neq 1 $var:mod_NEW3G
	`
		setJSONValue({
		'INPUT_VPI'          : G_ATM[_m] ? G_ATM[_m][2].split(':')[1].split('/')[0] || '0' : '0',
		'INPUT_VCI'          : G_ATM[_m] ? G_ATM[_m][2].split('/')[1] || '0' : '35',
		'SELECT_Service'     : G_ATM[_m] ? G_ATM[_m][4] : 'UBR',
		'INPUT_PeakCellRate' : G_ATM[_m] ? G_ATM[_m][5] : '0',
		'INPUT_SbCellRate'   : G_ATM[_m] ? G_ATM[_m][6] : '0',
		'INPUT_MaxBurstSize' : G_ATM[_m] ? G_ATM[_m][7] : '0',
		//'INPUT_EnableQoS'    : '1'
		
		'SELECT_EncapMode'   : G_ATM[_m] ? G_ATM[_m][3] : 'LLC',
		'SELECT_Protocol'    : findProtocol(),
		'INPUT_VlanId'       : G_ATM[_m] ? G_ATM[_m][8] : '0',
		'INPUT_QinQEnable'   : G_ATM[_m] ? G_ATM[_m][10]: '0',
		'INPUT_Priority'     : G_ATM[_m] ? G_ATM[_m][11]: '0',
		'INPUT_FirewallEnable'     : G_ATM[_m] ? G_ATM[_m][12]: '1',
		'en_proxyarp'        : G_ATM[_m] ? G_ATM[_m][9] : '0'
	});	
	`?>
	onchgService();

<?if eq $var:mod_NATPUBIPADDR 1
`	if((G_WanConns['Natipaddr'] != '0.0.0.0') && G_WanConns['Natipaddr'])
	{
		$('SELECT_NatIPMode').value = 'MENU_IP';
		$('INPUT_NatIPAddr').disabled = false;
		$('INPUT_NatIPAddr').value = G_WanConns['Natipaddr']? G_WanConns['Natipaddr'] : '';
	}
	else
	{
		$('SELECT_NatIPMode').value = 'AUTO_IP';
		$('INPUT_NatIPAddr').disabled = true;
	}
	
`?>
	//WAN赋值
	onchgProtocol();
	//
	onchgWANServiceType();
	onclkIpAddr_1();

	<?if eq $var:G_IPv6Enable 1	
	`	
		if($('SELECT_Protocol').value != "Bridging")
			ipv6_info_load();
	`?>
}

//ATM -> Service
function onchgService(){
	var value_service = $('SELECT_Service').value;
	var peak = false, cells = false, qos = false;
	
	switch(value_service){
		case 'UBR+' : {
			qos = true;
			break;
		}
		case 'UBR' : {
			peak = qos = true;
			break;
		}
		case 'CBR' : {
			peak = true;
			break;
		}
		case 'VBR-nrt' : {
			peak = cells = qos = true;
			break;
		}
		case 'VBR-rt' : {
			peak = cells = true;
			break;
		}
	}
	//绠＄悊鎺т?
	disCtrl('peak',peak);
	disCtrl('cells',cells);
	//disCtrl('qos',qos);鏆傛椂鍏抽棴QOS閫夐?
	$('qos').style.display = 'none';
}
//鎷ㄥ彿鏂瑰紡
function uiDialUp(){
	var mode = $('SELECT_DialupMode').value;
	var bool = false;
	
	if(mode == 'OnDemand'){
		bool = true;
	}
	
	disCtrl('inatimeout',bool);
	
}

//鎵惧嚭Protocol绫诲?
function findProtocol(){
	var _protocol = 'Bridging';
	var _linktype = G_ATM[_m] ? G_ATM[_m][1] : '';
	
	if('<?echo $var:conn_type?>' == 'IP' && _linktype == 'EoA' && G_WanConns['ConnectionType'] != 'IP_Bridged'){
		_protocol = 'MER';
	} else if('<?echo $var:conn_type?>' == 'PPP' && _linktype == 'EoA' &&G_WanConns['ConnectionType'] != 'PPPoE_Bridged'){
		_protocol = 'PPPoE';
	} else if(_linktype == 'IPoA'){
		_protocol = 'IPoA';
	} else if(_linktype == 'PPPoA'){
		_protocol = 'PPPoA';
	}
	
	return _protocol;
}
function onClickWanSeviceType(protocol){

	if(protocol.indexOf('PPP') > -1)
	{
	    var value_wanServiceType1 = $('SELECT_WANServiceType_1').value;
	    var value_name = $('INPUT_ServiceName').value;
	
	    var _split = value_name.split('_');
	    if(_split){
		 $('INPUT_ServiceName').value = _split[0] + '_' + _split[1]+ '_' + _split[2] + '_' + _split[3] + '_' + _split[4]+ '_' +value_wanServiceType1;
	    }
	 	if($('SELECT_WANServiceType_1').value=='TR069')
		{
 		       $('INPUT_EnableNat').disabled = true;
			$('SELECT_Nat').disabled =true;
		}else{
		       $('INPUT_EnableNat').disabled = false;
			$('SELECT_Nat').disabled = false;

		};
	}else{
	    var value_wanServiceType2 = $('SELECT_WANServiceType_2').value;
	    var value_name = $('INPUT_ServiceName').value;
	
	    var _split = value_name.split('_');
	    if(_split){
		 $('INPUT_ServiceName').value = _split[0] + '_' + _split[1]+ '_' + _split[2] + '_' + _split[3] + '_' + _split[4]+ '_' +value_wanServiceType2;
	    }
		if($('SELECT_WANServiceType_2').value=='TR069')
		{
 		       $('INPUT_EnableNat').disabled = true;
			$('SELECT_Nat').disabled =true;
		}else{
		       $('INPUT_EnableNat').disabled = false;
			$('SELECT_Nat').disabled = false;
		};
	    
	}
}
function onclkNatEnabled()
{
	var bool = Form.Checkbox('INPUT_EnableNat') == '1'?false:true;
	$('SELECT_Nat').disabled =bool;
}

function onchgWANServiceType()
{
	<?if eq 1 $var:mod_WAN_SERVICETYPE
	`
		if($('SELECT_WANServiceType_1').value == 'TR069' ||$('SELECT_WANServiceType_2').value == 'TR069')
		{
			$('INPUT_EnableNat').disabled = true;
			$('SELECT_Nat').disabled =true;
		}else{
			$('INPUT_EnableNat').disabled = false;
			$('SELECT_Nat').disabled = false;
		}
	`?>
	<?if neq  1 $var:mod_WAN_SERVICETYPE
	`
		$('INPUT_EnableNat').disabled = false;
		$('SELECT_Nat').disabled = false;
	`?>

}

function onchgProtocol(){
	var MContent = $('Content').offsetHeight;
	var _protocol = $('SELECT_Protocol').value;
	var nodes = $('ppp','ip','non-bridge','is-bridge','IPoA_MER');
	var pvc = false, q802 = false, vlan_prio = false, qinq = false, ppp = 'none', ip = 'none', non_bridge = 'block', is_bridge = 'none',IPoA_MER = 'none';
	var proxyarp = false;
    var index = 0;
  var WanManageListCount = 0;
	if(wan_service_type == 1){
		WanManageListCount = WanConnServiceListCount();
	}

	if(_protocol == 'IPoA')
	{
		$('SELECT_Protocol_V4_6').style.display = 'block';
	    disCtrl('SELECT_Protocol_V4_6', false);
		Form.Checkbox('INPUT_EnableIPv4', G_WanConns['X_TELEFONICA-ES_IPv4Enabled'] || '1');
<?if eq $var:G_IPv6Enable 1 
`		if(Form.Checkbox('INPUT_EnableIPv6') == '1')
		{
			$('IPV6_CFG').style.display = 'none';
			Form.Checkbox('INPUT_IPv6PrefixDelegationEnabled','0');
			Form.Checkbox('INPUT_IPv6IANAEnabled','0');
			Form.Checkbox('INPUT_EnableIPv6','0');
		}
`?>
	}
	else
	{
	    disCtrl('SELECT_Protocol_V4_6', true);
		Form.Checkbox('INPUT_EnableIPv4', G_WanConns['X_TELEFONICA-ES_IPv4Enabled'] || '1');
       	<?if eq $var:G_IPv6Enable 1 
    		`   Form.Checkbox('INPUT_EnableIPv6', G_WanConns['X_TELEFONICA-ES_IPv6Enabled']);
            `?>
	}

<?if eq $var:mod_NATPUBIPADDR 1
`   if(_protocol == 'Bridging')
    {
        $('nat_public_ip').style.display = 'none';
    }
    else
    {
        $('nat_public_ip').style.display = 'block';
    }
`?>
    if (n>0){
        index = G_Conns[n-1][3];
    }
	switch(_protocol){
		case 'PPPoA' : {
			ppp = 'block';
			if(wan_service_type == 1){
				$('SELECT_WANServiceType_1').value = G_WanConns['X_TWSZ-COM_ServiceList']||'Internet';	
			}
			
			//璧嬪?
			setJSONValue({
				'INPUT_UserName'      : G_WanConns['Username'] || '',
				'INPUT_Psd'           : (G_WanConns['Username'] == "")?"":G_WanConns['Password'] || '',
				'INPUT_ConfirmPsd'    : (G_WanConns['Username'] == "")?"":G_WanConns['Password'] || '',
				'SELECT_AuthMethod'   : G_WanConns['PPPAuthenticationProtocol'] || 'Auto',			
				'SELECT_DialupMode'   : G_WanConns['ConnectionTrigger'] || 'AlwaysOn',
				'INPUT_InaTimeout'    : G_WanConns['IdleDisconnectTime'] || '100',
				
				'INPUT_MTUSize'       : G_WanConns['MaxMRUSize'] || '1492',
				'INPUT_MaxMTUSize'       : G_WanConns['MaxMTUSize'] || '1400',
				'INPUT_KeepAlive'     : G_WanConns['PPPLCPEchoRetry'] == 0 ? '0' : '1',
				'INPUT_LCPEchoInterval' : G_WanConns['PPPLCPEcho'] || '30',
				'INPUT_LCPEchoRetry'  : G_WanConns['PPPLCPEchoRetry'] || '5',
				
				'INPUT_StaticIpAddr'  : (G_WanConns['X_TWSZ-COM_StaticIPAddress'] && G_WanConns['X_TWSZ-COM_StaticIPAddress'] != '') ? 1 : 0,
				'INPUT_IPAddress'     : G_WanConns['X_TWSZ-COM_StaticIPAddress'] || '',
							
				'INPUT_EnableNat'     : G_WanConns['NATEnabled'] || '1',
				'SELECT_Nat'		  : G_WanConns['X_TELEFONICA-ES_NATType'] || 'full cone',
				'INPUT_EnableWanSv'   : G_WanConns['Enable'] || '1',
				<?if neq 1 $var:mod_WAN_SERVICETYPE
				`	
				'INPUT_ServiceName'   : 'pppoa_' + ($('INPUT_VPI').value || '0') + '_' + ($('INPUT_VCI').value || '0') + '_' + ($('INPUT_VlanId').value || '0') + '_'  + index + '_' + (G_WanConns['X_TWSZ-COM_ServiceList'] || G_Service_Type)
				`?>
				<?if eq 1 $var:mod_WAN_SERVICETYPE
				`	
				'INPUT_ServiceName'   : 'pppoa_' + ($('INPUT_VPI').value || '0') + '_' + ($('INPUT_VCI').value || '0') + '_' + ($('INPUT_VlanId').value || '0') + '_'  + index + '_' + ($('SELECT_WANServiceType_1').value||'internet')
				`?>
			});
			if(G_WanConns['ConnectionType'] == 'IP_Bridged'){
				Form.Checkbox('INPUT_EnableNat', '1');
				<?if eq $var:G_IPv6Enable 1
				`
					Form.Checkbox('INPUT_EnableIPv6','0');
				`?>
				$('SELECT_Nat').value = 'full cone';
			}
			<?if eq $var:mod_PPPOE_PASSTHROUGH 1
			`   $('pass-through').style.display = 'none';
			`?>
			
			<?if neq $var:G_IPv6Enable 1
			`	
				$('SELECT_Protocol_V4_6').style.display = 'block';
				disCtrl('SELECT_Protocol_V4_6', false);
			`?>
			<?if eq $var:G_IPv6Enable 1
			`	
				$('SELECT_Protocol_V4_6').style.display = 'block';
				$('IPV6_CFG').style.display = 'block';
			`?>
			
            <?if eq 1 $var:mod_NEW3G
			`
			/*scb+ 2012-8-3 for bridge, not show backup 3g*/
			$('backup_3g_item').style.display = 'block';
			$('backup3G_enable').style.display = 'block';
			`?>
			
			break;
		}
		case 'PPPoE' : {
			pvc  = true;
			q802 = true;
			vlan_prio = true;
			ppp  = 'block';
			if(wan_service_type == 1){
				$('SELECT_WANServiceType_1').value = G_WanConns['X_TWSZ-COM_ServiceList']||'Internet';	
			}
			//璧嬪?
			setJSONValue({
				'INPUT_UserName'      : G_WanConns['Username'] || '',
				'INPUT_Psd'           : (G_WanConns['Username'] == "")?"":G_WanConns['Password'] || '',
				'INPUT_ConfirmPsd'    : (G_WanConns['Username'] == "")?"":G_WanConns['Password'] || '',
				'SELECT_AuthMethod'   : G_WanConns['PPPAuthenticationProtocol'] || 'Auto',
				'SELECT_DialupMode'  : G_WanConns['ConnectionTrigger'] || 'AlwaysOn',
				'INPUT_InaTimeout'    : G_WanConns['IdleDisconnectTime'] || '100',
				
				'INPUT_MTUSize'       : G_WanConns['MaxMRUSize'] || '1492',
				'INPUT_MaxMTUSize'       : G_WanConns['MaxMTUSize'] || '1400',
				'INPUT_KeepAlive'     : G_WanConns['PPPLCPEchoRetry'] == 0 ? '0' : '1',
				'INPUT_LCPEchoInterval' : G_WanConns['PPPLCPEcho'] || '30',
				'INPUT_LCPEchoRetry'  : G_WanConns['PPPLCPEchoRetry'] || '5',
				
				'INPUT_StaticIpAddr'  : (G_WanConns['X_TWSZ-COM_StaticIPAddress'] && G_WanConns['X_TWSZ-COM_StaticIPAddress'] != '') ? 1 : 0,
				'INPUT_IPAddress'     : G_WanConns['X_TWSZ-COM_StaticIPAddress'] || '',				
				'INPUT_EnableNat'     : G_WanConns['NATEnabled'] || '1',
				'SELECT_Nat'		: G_WanConns['X_TELEFONICA-ES_NATType'] || 'full cone',
				'INPUT_EnableWanSv'   : G_WanConns['Enable'] || '1',
				<?if neq 1 $var:mod_WAN_SERVICETYPE
				`
				'INPUT_ServiceName'   : 'pppoe_' + ($('INPUT_VPI').value || '0') + '_' + ($('INPUT_VCI').value || '0') + '_' + ($('INPUT_VlanId').value || '0') + '_' + index + '_' + (G_WanConns['X_TWSZ-COM_ServiceList'] || G_Service_Type)
				`?>
				<?if eq 1 $var:mod_WAN_SERVICETYPE
				`
				'INPUT_ServiceName'   : 'pppoe_' + ($('INPUT_VPI').value || '0') + '_' + ($('INPUT_VCI').value || '0') + '_' + ($('INPUT_VlanId').value || '0') + '_' + index + '_' + ($('SELECT_WANServiceType_1').value||'internet')
				`?>
			});
			if(G_WanConns['ConnectionType'] == 'IP_Bridged'){
				Form.Checkbox('INPUT_EnableNat', '1');
				<?if eq $var:G_IPv6Enable 1
				`
					Form.Checkbox('INPUT_EnableIPv6','0');
				`?>
				$('SELECT_Nat').value = 'full cone';
			}
			<?if neq $var:G_IPv6Enable 1
			`	
				$('SELECT_Protocol_V4_6').style.display = 'block';
				disCtrl('SELECT_Protocol_V4_6', false);
			`?>
			<?if eq $var:G_IPv6Enable 1
			`	
				$('SELECT_Protocol_V4_6').style.display = 'block';
				$('IPV6_CFG').style.display = 'block';
			`?>
			<?if eq $var:mod_PPPOE_PASSTHROUGH 1
			`   $('pass-through').style.display = 'block';
			    Form.Checkbox('INPUT_PassThrough', G_WanConns['PassThrough'] || '0');
			`?>
             <?if eq 1 $var:mod_NEW3G
			 `
			/*scb+ 2012-8-3 for bridge, not show backup 3g*/
			$('backup_3g_item').style.display = 'block';
			$('backup3G_enable').style.display = 'block';
			`?>
			break;
		}
		case 'MER' : {
			pvc  = true;
			q802 = true;
			vlan_prio = true;
			ip   = IPoA_MER = 'block';
			if(wan_service_type == 1){
				$('SELECT_WANServiceType_2').value = G_WanConns['X_TWSZ-COM_ServiceList']||'Internet';	
			}
			proxyarp = true;
			//璧嬪?
			setJSONValue({
				'INPUT_AutoIpAddr'      : (G_WanConns['AddressingType'] != 'DHCP') ? 'Static' : 'DHCP',
				'INPUT_WanIpAddr_IP'    : G_WanConns['ExternalIPAddress'] || '',
				'INPUT_WanMark_IP'      : G_WanConns['SubnetMask'] || '',

				'INPUT_IpAddr_IP'       : G_WanConns['DefaultGateway'] || '',
				'SELECT_WanInf_IP'      : G_WanConns[0] || '',
				
				'INPUT_DnsServer'       : G_WanConns['DNSOverrideAllowed'] || '0',
				'INPUT_DnsServer_P'     : G_WanConns['X_TWSZ-COM_UsrDNSServers'] ? G_WanConns['X_TWSZ-COM_UsrDNSServers'].split(',')[0] : '',
				'INPUT_DnsServer_A'     : G_WanConns['X_TWSZ-COM_UsrDNSServers'] ? (G_WanConns['X_TWSZ-COM_UsrDNSServers'].split(',')[1] || '') : '',
				
				'INPUT_EnableNat'       : G_WanConns['NATEnabled'] || '1',
				'SELECT_Nat'		: G_WanConns['X_TELEFONICA-ES_NATType'] || 'full cone',
				'INPUT_EnableWanSv'     : G_WanConns['Enable'] || '1',
				<?if neq 1 $var:mod_WAN_SERVICETYPE
				`
				'INPUT_ServiceName'     : 'mer_' + ($('INPUT_VPI').value || '0') + '_' + ($('INPUT_VCI').value || '0') + '_' + ($('INPUT_VlanId').value || '0') + '_' + index+ '_' + (G_WanConns['X_TWSZ-COM_ServiceList'] || G_Service_Type)
				`?>
				<?if eq 1 $var:mod_WAN_SERVICETYPE
				`
				'INPUT_ServiceName'     : 'mer_' + ($('INPUT_VPI').value || '0') + '_' + ($('INPUT_VCI').value || '0') + '_' + ($('INPUT_VlanId').value || '0') + '_' + index+ '_' + ($('SELECT_WANServiceType_2').value||'internet')
				`?>
			});
			if(G_WanConns['ConnectionType'] == 'IP_Bridged'){
				Form.Checkbox('INPUT_EnableNat', '1');
				<?if eq $var:G_IPv6Enable 1
				`
					Form.Checkbox('INPUT_EnableIPv6','0');
				`?>
				$('SELECT_Nat').value = 'full cone';
			}

<?if eq $var:G_IPv6Enable 1
`
			if (Form.Checkbox('INPUT_EnableIPv6') == 1)
			{
				setJSONValue({
					'INPUT_WanIpAddr_IP6'   : G_IP6_info['address']||'',
				    'INPUT_WanPrefix_IP6'   : G_IP6_info['addrprefixlen']||'',
				    'INPUT_IPv6_DefGateway' : G_IP6_info['gatewayauto'] ? G_IP6_info['gatewayauto'] : (G_IP6_info['gateway'] ? G_IP6_info['gateway'] : ''),
				    'INPUT_IPv6_Prefix'     : G_IP6_info['prefix']||'',
				    'INPUT_IPv6_PrefixLen'  : G_IP6_info['prefixlen']||'',
				    'INPUT_IPv6_DnsServer_P': G_IP6_info['server'] ? G_IP6_info['server'].split(',')[0] : (G_IP6_info['userserver'] ? G_IP6_info['userserver'].split(',')[0] : ''),
				    'INPUT_IPv6_DnsServer_A': G_IP6_info['server'] ? (G_IP6_info['server'].split(',')[1]||''):(G_IP6_info['userserver'] ? (G_IP6_info['userserver'].split(',')[1] || '') : '')
				});
			}
`?>

			<?if neq $var:G_IPv6Enable 1
			`	
				$('SELECT_Protocol_V4_6').style.display = 'block';
				disCtrl('SELECT_Protocol_V4_6', false);
			`?>

			<?if eq $var:G_IPv6Enable 1
			`	
				$('SELECT_Protocol_V4_6').style.display = 'block';
				$('IPV6_CFG').style.display = 'block';
			`?>		
             <?if eq 1 $var:mod_NEW3G
			 `
			/*scb+ 2012-8-3 for bridge, not show backup 3g*/
			$('backup_3g_item').style.display = 'block';
			$('backup3G_enable').style.display = 'block';
            `?>
			
			break;
		}
		case 'IPoA' : {
			ip = 'block';
			IPoA_MER = 'none';
			if(wan_service_type == 1){
				$('SELECT_WANServiceType_2').value = G_WanConns['X_TWSZ-COM_ServiceList']||'Internet';	
			}
			//璧嬪?
			setJSONValue({
				'INPUT_AutoIpAddr'      : 'Static',
				'INPUT_WanIpAddr_IP'    : G_WanConns['ExternalIPAddress'] || '',
				'INPUT_WanMark_IP'      : G_WanConns['SubnetMask'] || '',
				
//				'INPUT_DefautlGw_Ip'    : G_WanConns[4] || '',
//				'INPUT_RemoteIpAddr_IP' : G_WanConns[0] || '',
				'INPUT_IpAddr_IP'       : G_WanConns['DefaultGateway'] || '',
				'SELECT_WanInf_IP'      : G_WanConns[0] || '',
				
				'INPUT_DnsServer'       : G_WanConns['DNSOverrideAllowed'] || '0',
				'INPUT_DnsServer_P'     : G_WanConns['X_TWSZ-COM_UsrDNSServers'] ? G_WanConns['X_TWSZ-COM_UsrDNSServers'].split(',')[0] : '',
				'INPUT_DnsServer_A'     : G_WanConns['X_TWSZ-COM_UsrDNSServers'] ? (G_WanConns['X_TWSZ-COM_UsrDNSServers'].split(',')[1] || '') : '',
				
				'INPUT_EnableNat'       : G_WanConns['NATEnabled'] || '1',
				'SELECT_Nat'		: G_WanConns['X_TELEFONICA-ES_NATType'] || 'full cone',
				'INPUT_EnableWanSv'     : G_WanConns['Enable'] || '1',
				<?if neq 1 $var:mod_WAN_SERVICETYPE
				`
				'INPUT_ServiceName'     : 'ipoa_' + ($('INPUT_VPI').value || '0') + '_' + ($('INPUT_VCI').value || '0') + '_' + ($('INPUT_VlanId').value || '0') + '_' + index + '_' + (G_WanConns['X_TWSZ-COM_ServiceList'] || G_Service_Type)
				`?>
				<?if eq 1 $var:mod_WAN_SERVICETYPE
				`
				'INPUT_ServiceName'     : 'ipoa_' + ($('INPUT_VPI').value || '0') + '_' + ($('INPUT_VCI').value || '0') + '_' + ($('INPUT_VlanId').value || '0') + '_' + index + '_' + ($('SELECT_WANServiceType_2').value||'internet')
				`?>
			});
			if(G_WanConns['ConnectionType'] == 'IP_Bridged'){
				Form.Checkbox('INPUT_EnableNat', '1');
				<?if eq $var:G_IPv6Enable 1
				`
					Form.Checkbox('INPUT_EnableIPv6','0');
				`?>
				$('SELECT_Nat').value = 'full cone';
			}
<?if eq $var:G_IPv6Enable 1
`			if (Form.Checkbox('INPUT_EnableIPv6') == 1)
			{
				setJSONValue({
					'INPUT_WanIpAddr_IP6'   : G_IP6_info['address']||'',
				    'INPUT_WanPrefix_IP6'   : G_IP6_info['prefix']||'',
				    'INPUT_IPv6_DefGateway' : G_IP6_info['gateway']||'',
				    'INPUT_IPv6_DnsServer_P': G_IP6_info['userserver'] ? G_IP6_info['userserver'].split(',')[0] : '',
				    'INPUT_IPv6_DnsServer_A': G_IP6_info['userserver'] ? (G_IP6_info['userserver'].split(',')[1]||''):''
				});
			}
`?>
         
<?if eq 1 $var:mod_NEW3G
`
			/*scb+ 2012-8-3 for bridge, not show backup 3g*/
			$('backup_3g_item').style.display = 'block';
			$('backup3G_enable').style.display = 'block';
`?>
			break;
		}
		case 'Bridging' : {
			pvc = true;
			q802 = true;
			qinq = true;
			vlan_prio = true;
			non_bridge = 'none';
			is_bridge  = 'block';
			$('INPUT_ServiceName').value = 'br_' + ($('INPUT_VPI').value || '0') + '_' + ($('INPUT_VCI').value || '0') + '_' + ($('INPUT_VlanId').value || '0') + '_' + index;
			Form.Checkbox('INPUT_EnableBridgeSv',G_WanConns['Enable'] || '1');
			uiSynaEnable();
<?if eq 1 $var:mod_NEW3G
`
			/*scb+ 2012-8-3 for bridge, not show backup 3g*/
			$('backup_3g_item').style.display = 'none';
			$('backup3G_enable').style.display = 'none';
`?>
			
			$('SELECT_Protocol_V4_6').style.display = 'none';
			<?if eq $var:G_IPv6Enable 1
			`	$('IPV6_CFG').style.display = 'none';
			`?>			
			$('lang_net_config').style.display = 'none';
			break;
		}
	}
	//
	//disCtrl('pvc',pvc);
	disCtrl('802.1q',q802);
	disCtrl('qinq_enable', qinq);
	disCtrl('vlan_priority', vlan_prio);
	disCtrl('proxyarp', proxyarp);
	if (proxyarp == false)
		$('en_proxyarp').checked = false;
	$('pvc').style.display = 'none';
	nodes[0].style.display = ppp;
//	$('ppp_gw').style.display = 'none';
	nodes[1].style.display = ip;
	nodes[2].style.display = non_bridge;
	nodes[3].style.display = is_bridge;
	nodes[4].style.display = IPoA_MER;
	
	uiDialUp();
//	disCtrl('inatimeout',Form.Checkbox('INPUT_DialOnDemand'));
	disCtrl('s_LcpEchoConfig',Form.Checkbox('INPUT_KeepAlive'));
	disCtrl('s_ipAddr',Form.Checkbox('INPUT_StaticIpAddr'));
//	onclkPppAddr();
	onclkIpAddr_1();
	//onclkIpAddr_2();
	//onclkIpAddr_4();	
	onclkNatEnabled();
	var ContentHeigh;
	if(MContent==$('Content').offsetHeight)
	{
		ContentHeigh =FirthContentHeigh + nodes[0].offsetHeight + nodes[1].offsetHeight + nodes[2].offsetHeight + nodes[3].offsetHeight + nodes[4].offsetHeight;
	    //$('Menu').style.height=ContentHeigh + 'px';
	    //$('Content').style.height=ContentHeigh + 'px';
	}
	else
	{
	    Heigtduiqi();
	}
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
	<?if eq $var:G_IPv6Enable 1	
	`
		disCtrl('ip_addr_ipv6', bool);
	`?>
	disCtrl('ip_addr_ipv4', bool);
	change_ipv4_enable();
	change_ipv6_enable();
	Form.Checkbox('INPUT_EnableNat', G_WanConns['NATEnabled']);
	onclkNatEnabled();
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

function onchgSetVPI(){
	var value_vpi = $('INPUT_VPI').value;
	var value_name = $('INPUT_ServiceName').value;
	var _protocol = $('SELECT_Protocol').value;
	//for example: br_0_35_10
	var _split = value_name.split('_');
	if(_protocol=='Bridging'){
		if(_split){
			$('INPUT_ServiceName').value = _split[0] + '_' + value_vpi + '_' + _split[2] + '_' + _split[3] + '_' + _split[4];
		}
	}
	else{
		if(_split){
			if(_split[6] == "TR069")
			{
				$('INPUT_ServiceName').value = _split[0] + '_' + value_vpi + '_' + _split[2] + '_' + _split[3] + '_' + _split[4] + '_' + _split[5] + '_' + _split[6];
			}
			else
			{
				$('INPUT_ServiceName').value = _split[0] + '_' + value_vpi + '_' + _split[2] + '_' + _split[3] + '_' + _split[4] + '_' + _split[5];
			}
		}
	}
}

function onchgSetVCI(){
	var value_vci = $('INPUT_VCI').value;
	var value_name = $('INPUT_ServiceName').value;
	var _protocol = $('SELECT_Protocol').value;
	//for example: br_0_35_10
	var _split = value_name.split('_');
	if(_protocol=='Bridging'){
		if(_split){
			$('INPUT_ServiceName').value = _split[0] + '_' + _split[1] + '_' + value_vci + '_' + _split[3] + '_' + _split[4];
		}
	}
	else{
		if(_split){
			if(_split[6] == "TR069")
			{
				$('INPUT_ServiceName').value = _split[0] + '_' + _split[1] + '_' + value_vci + '_' + _split[3] + '_' + _split[4] + '_' + _split[5] + '_' + _split[6];
			}
			else
			{
				$('INPUT_ServiceName').value = _split[0] + '_' + _split[1] + '_' + value_vci + '_' + _split[3] + '_' + _split[4] + '_' + _split[5];
			}
		}
	}
}

function onchgSetVlan(){
	var value_vlanid = $('INPUT_VlanId').value;
	var value_name = $('INPUT_ServiceName').value;
	
	//for example: br_0_35_10
	var _split = value_name.split('_');
	if(_split){
		if(_split[5]!="Internet")
		{
			$('INPUT_ServiceName').value = _split[0] + '_' + _split[1] + '_' + _split[2] + '_' + value_vlanid + '_' + _split[4];
		}
		else
		{
			$('INPUT_ServiceName').value = _split[0] + '_' + _split[1] + '_' + _split[2] + '_' + value_vlanid + '_' + _split[4] + '_' + _split[5];
			if(_split[6] == "TR069")
			{
				$('INPUT_ServiceName').value = _split[0] + '_' + _split[1] + '_' + _split[2] + '_' + value_vlanid + '_' + _split[4] + '_' + _split[5] + '_' + _split[6];
			}
		}
	}
}

//鍒ゆ柇褰撳墠鐨勫崗璁槸IP, 杩樻槸PPP
function getProtocol(){
	var _protocol = $('SELECT_Protocol').value;
	
	switch(_protocol){
		case 'PPPoE' : {
			return ['EoA','PPP'];
		}
		case 'PPPoA' : {
			return ['PPPoA','PPP'];
		}
		case 'MER' : {
			return ['EoA','IP'];
		}
		case 'IPoA' : {
			return ['IPoA','IP'];
		}
		case 'Bridging' : {
			if(G_WanConns['ConnectionType'] == 'PPPoE_Bridged')
			   return ['EoA','PPP'];
			else
			   return ['EoA','IP'];
		}
	}
}
function checkpassword(){
	if($('INPUT_Psd').value != $('INPUT_ConfirmPsd').value){
		alert(SEcode[1010]);
		display_ctl();
		return false;
	}
	return true;
}
function checkValue(){
	//var node_type = $('INPUT_ServiceName').value.split('_');
	//for(var i = 0, _type, _len = G_Conns.length; i < _len; i++){
	//		if (i == n) continue;
	//		_type = G_Conns[i][2].split('_');
			// 涓€涓狿VC鍙兘瀵瑰簲涓€涓狪PConn
	//		if( _type[1] == node_type[1] && _type[2] == node_type[2]){
	//			alert(SEcode[9999]);
	//			return false;
	//		}
	//	}
	
	if (!$('INPUT_Priority').value.match(/^[0-7]{1}$/))
	{
		alert("Invalid Vlan priority");
		display_ctl();
		return false;
	}
	
	return true;
}

function uiSubmit(){
	$('Apply').disabled = 1;
	$('Cancel').disabled = 1;
	var _protocol = $('SELECT_Protocol').value;
	var _node; //瀛樻斁鑺傜偣鐨勬暟缁?
	var _path = ':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.';
	
	if(_protocol == "PPPoA" || _protocol == "PPPoE")
	{
		if(!checkpassword())
			return false;	
	}
	
	if(!checkValue()){
		return false;
	}
	
	var _pro = getProtocol(); //鍙栧緱涓€涓猘rray,?['IPoA','IP']
	//缁熶竴鐨凢orm鍐呭?
	$H({
		'obj-action'    : 'set',
		'var:menu'      : '<?echo $var:menu?>',
		'var:page'      : '<?echo $var:page?>',
		'var:errorpage' : 'wanadsl',
		'var:conn_type' : '<?echo $var:conn_type?>',
		'var:secIdx'    : '<?echo $var:secIdx?>',
		'var:thdIdx'    : '<?echo $var:thdIdx?>',
		'getpage'       : 'html/index.html',
		'errorpage'     : 'html/index.html'
	},true);
	
	//
	if(_m != -1){
		_path = ':' + G_ATM[_m][0];
	}
	//璁剧疆ATM妯″紡
	var _pvc = $('INPUT_VPI','INPUT_VCI','SELECT_Service','INPUT_PeakCellRate','INPUT_SbCellRate','INPUT_MaxBurstSize','INPUT_EnableQoS');
	
	$F(_path + 'WANDSLLinkConfig.Enable', '1');
	$F(_path + 'WANDSLLinkConfig.DestinationAddress', 'PVC:' + _pvc[0].value + '/' + _pvc[1].value);
	$F(_path + 'WANDSLLinkConfig.ATMQoS', _pvc[2].value);
	//LLC/VCMUX
	$F(_path + 'WANDSLLinkConfig.ATMEncapsulation', $('SELECT_EncapMode').value);
	//LinkType
	$F(_path + 'WANDSLLinkConfig.LinkType', _pro[0]);
	//Vlan ID
	$F(_path + 'X_TWSZ-COM_VLANID', $('INPUT_VlanId').value);
	//Vlan prio
	$F(_path + 'X_TWSZ-COM_VLANPriority', $('INPUT_Priority').value);
	//QinQ
	$F(_path + 'X_TWSZ-COM_QinQEnable', Form.Checkbox('INPUT_QinQEnable'));
	//Firewall
	$F(_path + 'X_TWSZ-COM_FirewallEnable', Form.Checkbox('INPUT_FirewallEnable'));
	//proxy arp
	$F(_path + 'X_TWSZ-COM_ProxyArp', Form.Checkbox('en_proxyarp'));
	
	switch(_pvc[2].value){
		case 'UBR+' : {
			//鍚庢潵涓轰簡妯″潡妫€鏌ヨ€屽姞鐨?
			$F(_path + 'WANDSLLinkConfig.ATMPeakCellRate'       , _pvc[3].value);
			$F(_path + 'WANDSLLinkConfig.ATMSustainableCellRate', _pvc[4].value);
			$F(_path + 'WANDSLLinkConfig.ATMMaximumBurstSize'   , _pvc[5].value);
			break;
		}
		case 'UBR' : {
			$F(_path + 'WANDSLLinkConfig.ATMPeakCellRate', _pvc[3].value);
			//鍚庢潵涓轰簡妯″潡妫€鏌ヨ€屽姞鐨?
			$F(_path + 'WANDSLLinkConfig.ATMSustainableCellRate', _pvc[4].value);
			$F(_path + 'WANDSLLinkConfig.ATMMaximumBurstSize'   , _pvc[5].value);
			break;
		}
		case 'CBR' : {
			$F(_path + 'WANDSLLinkConfig.ATMPeakCellRate', _pvc[3].value);
			//鍚庢潵涓轰簡妯″潡妫€鏌ヨ€屽姞鐨?
			$F(_path + 'WANDSLLinkConfig.ATMSustainableCellRate', _pvc[4].value);
			$F(_path + 'WANDSLLinkConfig.ATMMaximumBurstSize'   , _pvc[5].value);
			break;
		}
		case 'VBR-nrt' : {
			$F(_path + 'WANDSLLinkConfig.ATMPeakCellRate'       , _pvc[3].value);
			$F(_path + 'WANDSLLinkConfig.ATMSustainableCellRate', _pvc[4].value);
			$F(_path + 'WANDSLLinkConfig.ATMMaximumBurstSize'   , _pvc[5].value);
			break;
		}
		case 'VBR-rt' : {
			$F(_path + 'WANDSLLinkConfig.ATMPeakCellRate'       , _pvc[3].value);
			$F(_path + 'WANDSLLinkConfig.ATMSustainableCellRate', _pvc[4].value);
			$F(_path + 'WANDSLLinkConfig.ATMMaximumBurstSize'   , _pvc[5].value);
			break;
		}
	}
  
	//鍒ゆ柇鏄疘P杩樻槸PPP
	//鏄惁浜ょ敱webproxy澶勭?
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
			//涓洪厤鍚堟ā鍧楁鏌ワ紝灏咺P/PPP鐨勫墠鐨勫疄渚嬪彿鏀逛负0.
			_path = _path.substr(0, _path.length - 2) + '0.'; // -> InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANIPConnection.0.
		}
	}
	
	//鍏叡閰嶇疆
	$F(_path + 'NATEnabled', Form.Checkbox('INPUT_EnableNat'));
	$F(_path + 'X_TELEFONICA-ES_NATType', $('SELECT_Nat').value);
	$F(_path + 'Enable', Form.Checkbox('INPUT_EnableWanSv'));

	//2013-2-28 nat public ip addr
<?if eq $var:mod_NATPUBIPADDR 1
`	if($('SELECT_NatIPMode').value == 'MENU_IP')
	{
		$F(_path + 'Natipaddr', $('INPUT_NatIPAddr').value);
	}
	else
	{
		$F(_path + 'Natipaddr', '0.0.0.0');
	}
`?>
	/*scb+ 2012-8-2*/
	<?if eq 1 $var:mod_NEW3G
	`
	$F(_path + 'Backup3G', Form.Checkbox('INPUT_backup3G_enable'));
	`?>
	<?if eq $var:G_IPv6Enable 1
	`	
	    $F(_path + 'X_TELEFONICA-ES_IPv6Enabled', (_protocol == 'Bridging')? '1' :Form.Checkbox('INPUT_EnableIPv6'));
		$F(_path + 'X_TELEFONICA-ES_IPv4Enabled', (_protocol == 'Bridging')? '1' :Form.Checkbox('INPUT_EnableIPv4'));
	`?>
	//$F(_path + 'Name'  , $('INPUT_ServiceName').value);
	$F(_path + 'Name'  , 'PVC:' + _pvc[0].value + '/' + _pvc[1].value);
	
	switch(_protocol){
		case 'PPPoA' : {
			_node = $('INPUT_UserName','INPUT_Psd','SELECT_AuthMethod','INPUT_AcName','INPUT_PPPServiceName','INPUT_InaTimeout','INPUT_MTUSize',
					  'INPUT_PPP_IP_Ext','INPUT_IPAddress','INPUT_IpAddr_PPP','SELECT_WanIntf_PPP','INPUT_LCPEchoInterval','INPUT_LCPEchoRetry','INPUT_MaxMTUSize');

			$F(_path + 'Username'                  , _node[0].value.strip());
			//Password
			if(_node[1].value != '**********'){
				$F(_path + 'Password'              , _node[1].value);
			}
			$F(_path + 'PPPAuthenticationProtocol' , _node[2].value);
			//$F(_path + 'PPPoEACName'               , _node[3].value);
			//$F(_path + 'PPPoEServiceName'          , _node[4].value);
			$F(_path + 'ConnectionTrigger'  , $('SELECT_DialupMode').value);
			if($('SELECT_DialupMode').value == 'OnDemand'){
				$F(_path + 'IdleDisconnectTime' , _node[5].value);
			}
			$F(_path + 'MaxMRUSize'                , _node[6].value);
			$F(_path + 'MaxMTUSize'                , _node[13].value);
			$F(_path + 'PPPLCPEchoRetry'           , Form.Checkbox('INPUT_KeepAlive') == '1' ? _node[12].value : '0');
			$F(_path + 'PPPLCPEcho'           , _node[11].value);
			//Use Static IP Address
			if(Form.Checkbox('INPUT_StaticIpAddr') == '1'){
				$F(_path + 'X_TWSZ-COM_StaticIPAddress'  , _node[8].value);
			} else {
				$F(_path + 'X_TWSZ-COM_StaticIPAddress'  , '');
			}
//			$F(_path + 'RouteProtocolRx'           , _node[8].value);
//			$F(_path + 'X_TWSZ-COM_PppoeRetryNum'  , _node[9].value);
			if(wan_service_type == 1){
				$F(_path + 'X_TWSZ-COM_ServiceList' , $('SELECT_WANServiceType_1').value);
			}else{
				$F(_path + 'X_TWSZ-COM_ServiceList' , G_Service_Type);
			}
			break;
		}
		case 'PPPoE' : {
			_node = $('INPUT_UserName','INPUT_Psd','SELECT_AuthMethod','INPUT_AcName','INPUT_PPPServiceName','INPUT_InaTimeout','INPUT_MTUSize',
					  'INPUT_PPP_IP_Ext','INPUT_IPAddress','INPUT_IpAddr_PPP','SELECT_WanIntf_PPP','INPUT_LCPEchoInterval','INPUT_LCPEchoRetry','INPUT_MaxMTUSize');

			$F(_path + 'Username'                  , _node[0].value.strip());
			//Password
			if(_node[1].value != '**********'){
				$F(_path + 'Password'              , _node[1].value);
				$F(_path + 'TmpPassword'              , _node[1].value);
			}
			$F(_path + 'PPPAuthenticationProtocol' , _node[2].value);
//			$F(_path + 'PPPoEACName'               , _node[3].value);
//			$F(_path + 'PPPoEServiceName'          , _node[4].value);
			$F(_path + 'ConnectionTrigger'  , $('SELECT_DialupMode').value);
			if($('SELECT_DialupMode').value == 'OnDemand'){
				$F(_path + 'IdleDisconnectTime' , _node[5].value);
			}
			$F(_path + 'MaxMRUSize'                , _node[6].value);
			$F(_path + 'MaxMTUSize'                , _node[13].value);
			$F(_path + 'PPPLCPEchoRetry'           , Form.Checkbox('INPUT_KeepAlive') == '1' ? _node[12].value : '0');
			$F(_path + 'PPPLCPEcho'           , _node[11].value);
			//Use Static IP Address
			if(Form.Checkbox('INPUT_StaticIpAddr') == '1'){
				$F(_path + 'X_TWSZ-COM_StaticIPAddress'  , _node[8].value);
			} else {
				$F(_path + 'X_TWSZ-COM_StaticIPAddress'  , '');
			}
//			$F(_path + 'RouteProtocolRx'           , _node[8].value);
//			$F(_path + 'X_TWSZ-COM_PppoeRetryNum'  , _node[9].value);
			if(wan_service_type == 1){
				$F(_path + 'X_TWSZ-COM_ServiceList' , $('SELECT_WANServiceType_1').value);
			}else{
				$F(_path + 'X_TWSZ-COM_ServiceList' , G_Service_Type);
			}
			$F(_path + 'ConnectionType'         , 'IP_Routed');
<?if eq $var:mod_PPPOE_PASSTHROUGH 1
`			$F(_path + 'PassThrough'            , Form.Checkbox('INPUT_PassThrough'));
`?>

			//ipv6			
<?if eq $var:G_IPv6Enable 1
`
			if(Form.Checkbox('INPUT_EnableIPv6') == '1'){
				$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled', Form.Checkbox('INPUT_IPv6PrefixDelegationEnabled'));
				$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress', Form.Checkbox('INPUT_IPv6IANAEnabled'));
				$F(_path + 'X_TWSZ-COM_IPv6Config.IPv6AddressList.1.AddressingType', 'DHCP');
				$F(_path  + 'X_TWSZ-COM_IPv6Config.IPv6DNSEnabled', '1');
				$F(_path  + 'X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed', '0');
				$F(_path  + 'X_TWSZ-COM_IPv6Config.GetIPv6NTPServers', '0');
				
			}
			if(Form.Checkbox('INPUT_EnableIPv6') == '0'){
				$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled', '0');
				$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress', '0');				
			}	
`?>			
			break;
		}
		case 'IPoA' : {
			_node   = $('INPUT_WanIpAddr_IP','INPUT_WanMark_IP','INPUT_IpAddr_IP','SELECT_WanInf_IP','INPUT_DnsServer_P','INPUT_DnsServer_A','INPUT_IPv6_DnsServer_P','INPUT_IPv6_DnsServer_A');
			
			if (Form.Checkbox('INPUT_EnableIPv4') == '1')
			{
				$F(_path + 'ExternalIPAddress' , _node[0].value);
				$F(_path + 'SubnetMask'        , _node[1].value);
				$F(_path + 'AddressingType'    , 'Static'); //Static
				$F(_path + 'DefaultGateway'    , _node[2].value);
				$F(_path + 'DNSServers', (_node[4].value + ',' + _node[5].value).delcomma());
				$F(_path + 'DNSOverrideAllowed' , '1');
			}
			//[BUG:28789]zzh add.DNSOverrideAllowedΪ1ԶDNSΪա
			$F(_path + 'X_TWSZ-COM_UsrDNSServers', (_node[4].value + ',' + _node[5].value).delcomma());
<?if eq $var:G_IPv6Enable 1			
`			if(Form.Checkbox('INPUT_EnableIPv6') == '0')
			{
				$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled','0');
				$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress','0');
			}
`?>			
			
			$F(_path + 'ConnectionType'         , 'IP_Routed');
			if(wan_service_type == 1){
				$F(_path + 'X_TWSZ-COM_ServiceList' , $('SELECT_WANServiceType_2').value);
			}else{
				$F(_path + 'X_TWSZ-COM_ServiceList' , G_Service_Type);
			}
			break;
		}
		case 'MER' : {
			_node   = $('INPUT_WanIpAddr_IP','INPUT_WanMark_IP','INPUT_IpAddr_IP','SELECT_WanInf_IP','INPUT_DnsServer_P','INPUT_DnsServer_A');

<?if eq $var:G_IPv6Enable 1
`           if (Form.Checkbox('INPUT_EnableIPv6') == '1')
			{
				var path_v6 = _path + "X_TWSZ-COM_IPv6Config.";
				if(Form.Radio('INPUT_AutoIpAddr') == 'Static')
				{
					if($('INPUT_WanIpAddr_IP6').value == '')
					{
						alert("Please input the WAN IP6 Address !");
						display_ctl();
						return false;
					}
					if($('INPUT_WanPrefix_IP6').value == '')
					{
						alert("Please input the WAN IP6 Prefix length !");
						display_ctl();
						return false;
					}
					if($('INPUT_IPv6_DefGateway').value == '')
					{
						alert("Please input the WAN IP6 Gateway !");
						display_ctl();
						return false;
					}
					if($('INPUT_IPv6_Prefix').value == '')
					{
						alert("Please input the WAN IP6 Prefix !");
						display_ctl();
						return false;
					}
					if($('INPUT_IPv6_PrefixLen').value == '')
					{
						alert("Please input the WAN IP6 Prefix length !");
						display_ctl();
						return false;
					}
					if(($('INPUT_IPv6_DnsServer_P').value == '') && ($('INPUT_IPv6_DnsServer_A').value == ''))
					{
						alert("Please input the Preferred IP6 DNS server or Alternate IP6 DNS server!");
						display_ctl();
						return false;
					}
					$F(_path + 'AddressingType',                 'Static');
				
					$F(path_v6 + 'IPv6AddressList.1.IPv6Address', $('INPUT_WanIpAddr_IP6').value);
					$F(path_v6 + 'IPv6AddressList.1.PrefixLength', $('INPUT_WanPrefix_IP6').value);
					$F(path_v6 + 'IPv6DefaultRouterList.1.ConfigType', 'Static');
					$F(path_v6 + 'IPv6DefaultRouterList.1.DefaultRouter', $('INPUT_IPv6_DefGateway').value);
					$F(path_v6 + 'IPv6SitePrefixInfo.IPv6SitePrefix', $('INPUT_IPv6_Prefix').value);
					$F(path_v6 + 'IPv6SitePrefixInfo.IPv6SitePrefixLength', $('INPUT_IPv6_PrefixLen').value);
					$F(path_v6 + 'IPv6SitePrefixInfo.ValidLifeTime', '172800');
					$F(path_v6 + 'IPv6SitePrefixInfo.PreferredLifeTime', '7200');
					$F(path_v6 + 'UsrIPv6DNSServers', ($('INPUT_IPv6_DnsServer_P').value + ',' + $('INPUT_IPv6_DnsServer_A').value).delcomma());
					$F(path_v6 + 'IPv6AddressList.1.AddressingType', 'Static');
					$F(path_v6 + 'IPv6DNSOverrideAllowed'      , '1');
					
					$F(path_v6 + 'IPv6DNSEnabled', '1');
					//$F(path_v6 + 'IPv6DNSOverrideAllowed', '1');
					$F(path_v6 + 'GetIPv6NTPServers', '0');	                
				}
				else 
				{
					$F(path_v6 + 'IPv6DefaultRouterList.1.ConfigType' , 'OTHER');
					$F(path_v6 + 'IPv6AddressList.1.AddressingType' , 'DHCP');
					$F(_path + 'AddressingType',                 'DHCP');
					$F(path_v6 + 'IPv6DNSOverrideAllowed'      , '0');
				}
				/* prefix delegation */
				$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled', Form.Checkbox('INPUT_IPv6PrefixDelegationEnabled'));
				$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress', Form.Checkbox('INPUT_IPv6IANAEnabled'));
				if(wan_service_type == 1){
					$F(_path + 'X_TWSZ-COM_ServiceList' , $('SELECT_WANServiceType_2').value);
				}else{
					$F(_path + 'X_TWSZ-COM_ServiceList' , G_Service_Type);
				}
			}
			 if (Form.Checkbox('INPUT_EnableIPv6') == '0')
			 {
				$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled', '0');
				$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress', '0');
			 }
`?>
			 if (Form.Checkbox('INPUT_EnableIPv4') == '1')			
			{
				//IP & Mark
				if(Form.Radio('INPUT_AutoIpAddr') == 'Static')
				{ //鍒ゆ柇鏄疭tatic, 杩樻槸DHCP
					if(_node[0].value == '')
					{
						alert("Please input the WAN IP address!");
						display_ctl();
						return false;
					}
					if(_node[1].value == '')
					{
						alert("Please input the WAN Subnet Mask!");
						display_ctl();
						return false;
					}
					if(_node[2].value == '')
					{
						alert("Please input the Default gateway!");
						display_ctl();
						return false;
					}
					if((_node[4].value == '')&&(_node[5].value == ''))
					{
						alert("Please input the Preferred DNS server address or Alternate DNS server address!");
						display_ctl();
						return false;
					}
					$F(_path + 'ExternalIPAddress' , _node[0].value);
					$F(_path + 'SubnetMask'        , _node[1].value);
					$F(_path + 'AddressingType'    , 'Static'); //Static
					$F(_path + 'DefaultGateway'    , _node[2].value);
					$F(_path + 'DNSServers', (_node[4].value + ',' + _node[5].value).delcomma());
					$F(_path + 'DNSOverrideAllowed'      , '1');
					//[BUG:28789]zzh add.DNSOverrideAllowedΪ1ԶDNSΪա
					$F(_path + 'X_TWSZ-COM_UsrDNSServers', (_node[4].value + ',' + _node[5].value).delcomma());
				} 
				else 
				{
					$F(_path + 'AddressingType'    , 'DHCP'); //DHCP
					$F(_path + 'DNSOverrideAllowed'      , '0');
				}
				if(wan_service_type == 1){
					$F(_path + 'X_TWSZ-COM_ServiceList' , $('SELECT_WANServiceType_2').value);
				}else{
					$F(_path + 'X_TWSZ-COM_ServiceList' , G_Service_Type);
				}
			}
			$F(_path + 'ConnectionType'         , 'IP_Routed');	
			break;
		}
		case 'Bridging' : {
			if(Form.Checkbox('INPUT_QinQEnable') == '1' 
				&& $('INPUT_VlanId').value == 0)
			{
				alert("When vlan id is 0, QinQ is invalid!");
				display_ctl();
				return false;
			}
			if(G_WanConns['ConnectionType'] == 'PPPoE_Bridged')
			   $F(_path + 'ConnectionType'         , 'PPPoE_Bridged');
			else
			   $F(_path + 'ConnectionType'         , 'IP_Bridged');
			//set bridge connecttion servicelist Internet as default
			$F(_path + 'X_TWSZ-COM_ServiceList' , 'Internet');
		}
	}

	$('uiPostForm').submit();
}

function backtowancfg(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wancfg';
}

function uiKeepAliveEnable(){
	if(Form.Checkbox('INPUT_KeepAlive') == '1'){
		disCtrl('s_LcpEchoConfig',1);
		if ($('INPUT_LCPEchoRetry').value <= 0){
			$('INPUT_LCPEchoRetry').value = '5';
		}
	} else {
		disCtrl('s_LcpEchoConfig',0);
	}
}

//checkbox鐘舵€佸悓姝?
function uiSynaEnable(){
	if(Form.Checkbox('INPUT_EnableBridgeSv') == '1'){
		Form.Checkbox('INPUT_EnableWanSv','1');
	} else {
		Form.Checkbox('INPUT_EnableWanSv','0');
	}
}

function uiPageRefresh(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wanadsl&var:conn_type=<?echo $var:conn_type?>&var:secIdx=<?echo $var:secIdx?>&var:thdIdx=<?echo $var:thdIdx?>';
}

function display_ctl(){
	$('Apply').disabled = 0;
	$('Cancel').disabled = 0;
}
function dealWithError(){
	if (G_Error != 1){
		return false; 
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
	
}

addListeners(uiOnload, dealWithError);
