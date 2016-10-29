/* ------------Wan Config-----------*/
//ATM
var G_ATM = [];
var m = 0;
<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "X_TWSZ-COM_VLANID"
`	G_ATM[m] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$00.",//Path
				"$01" //X_TWSZ-COM_VLANID
				];
	m++;
`?>

//Wan Conns----$var:secIdx ->第二层；$var:thdIdx -> 第三层
var G_WanConns = [];
//var m = 0;
<?if eq $var:conn_type `Bridge`
`	<?mget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx. "Enable Name NATEnabled ExternalIPAddress SubnetMask DefaultGateway DNSOverrideAllowed X_TWSZ-COM_UsrDNSServers AddressingType ConnectionType X_TELEFONICA-ES_NATType DNSServers"
	`	G_WanConns['Path'] = "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx."; //Path
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
		G_WanConns['X_TELEFONICA-ES_NATType']  = "$0b"; //X_TELEFONICA-ES_NATType
	    G_WanConns['DNSServers']               = "$0c";//DNSServers
	`?>
`?>
<?if eq $var:conn_type `DHCP`
`	<?mget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx. "Enable Name NATEnabled ExternalIPAddress SubnetMask DefaultGateway DNSOverrideAllowed X_TWSZ-COM_UsrDNSServers AddressingType ConnectionType X_TELEFONICA-ES_NATType DNSServers"
	`	G_WanConns['Path'] = "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx."; //Path
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
		G_WanConns['X_TELEFONICA-ES_NATType']  = "$0b"; //X_TELEFONICA-ES_NATType
	    G_WanConns['DNSServers']               = "$0c";//DNSServers
	`?>
`?>
<?if eq $var:G_IPv6Enable 1
`   <?if eq $var:conn_type `DHCP`
    `
        <?mget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx. "X_TELEFONICA-ES_IPv6Enabled X_TELEFONICA-ES_IPv4Enabled X_TELEFONICA-ES_IPv6PrefixDelegationEnabled X_TELEFONICA-ES_DHCP6cForAddress"
        `   G_WanConns['X_TELEFONICA-ES_IPv6Enabled']					= "$01"; //ipv6 enable
			G_WanConns['X_TELEFONICA-ES_IPv4Enabled']					= "$02"; //ipv4 enable
			G_WanConns['X_TELEFONICA-ES_IPv6PrefixDelegationEnabled']	= "$03";//prefix delegation enable
			G_WanConns['X_TELEFONICA-ES_DHCP6cForAddress']				= "$04";//X_TELEFONICA-ES_DHCP6cForAddress
		`?>
    `?>
`?>
<?if eq $var:conn_type `Static`
`	<?mget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx. "Enable Name NATEnabled ExternalIPAddress SubnetMask DefaultGateway DNSOverrideAllowed X_TWSZ-COM_UsrDNSServers AddressingType ConnectionType X_TELEFONICA-ES_NATType DNSServers"
	`	G_WanConns['Path'] = "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx."; //Path
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
		G_WanConns['X_TELEFONICA-ES_NATType']  = "$0b"; //X_TELEFONICA-ES_NATType
	    G_WanConns['DNSServers']               = "$0c";//DNSServers
	`?>
`?>
<?if eq $var:G_IPv6Enable 1
`   <?if eq $var:conn_type `Static`
    `
        <?mget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx. "X_TELEFONICA-ES_IPv6Enabled X_TELEFONICA-ES_IPv4Enabled X_TELEFONICA-ES_IPv6PrefixDelegationEnabled X_TELEFONICA-ES_DHCP6cForAddress"
        `   G_WanConns['X_TELEFONICA-ES_IPv6Enabled']					= "$01"; //ipv6 enable
			G_WanConns['X_TELEFONICA-ES_IPv4Enabled']					= "$02"; //ipv4 enable
			G_WanConns['X_TELEFONICA-ES_IPv6PrefixDelegationEnabled']	= "$03";//prefix delegation enable
			G_WanConns['X_TELEFONICA-ES_DHCP6cForAddress']				= "$04";//X_TELEFONICA-ES_DHCP6cForAddress
		`?>
    `?>
`?>
<?if eq $var:conn_type `PPPOE`
`	<?mget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx. "Enable NATEnabled Username PPPAuthenticationProtocol ConnectionTrigger IdleDisconnectTime MaxMRUSize PPPLCPEchoRetry X_TWSZ-COM_StaticIPAddress X_TELEFONICA-ES_NATType PPPLCPEcho"
	`	G_WanConns['Path'] = "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx.";//Path
		G_WanConns['Enable']                    = "$01";//Enable
		G_WanConns['NATEnabled']                = "$02";//NATEnabled
		G_WanConns['Username']                  = "$03";//Username
	    G_WanConns['Password']                  = "**********";//Password
	    G_WanConns['PPPAuthenticationProtocol'] = "$04";//PPPAuthenticationProtocol
	    G_WanConns['ConnectionTrigger']         = "$05";//ConnectionTrigger
	    G_WanConns['IdleDisconnectTime']        = "$06";//IdleDisconnectTime
	    G_WanConns['MaxMRUSize']                = "$07";//MaxMRUSize
	    G_WanConns['PPPLCPEchoRetry']           = "$08";//PPPLCPEchoRetry
	    G_WanConns['X_TWSZ-COM_StaticIPAddress']= "$09";//X_TWSZ-COM_StaticIPAddress
	    G_WanConns['X_TELEFONICA-ES_NATType']        = "$0a";//X_TELEFONICA-ES_NATType
	    G_WanConns['PPPLCPEcho']                = "$0b";//PPPLCPEcho
	`?>
`?>
<?if eq $var:G_IPv6Enable 1
`   <?if eq $var:conn_type `PPPOE`
    `
        <?mget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANPPPConnection.$var:thdIdx. "X_TELEFONICA-ES_IPv6Enabled X_TELEFONICA-ES_IPv4Enabled X_TELEFONICA-ES_IPv6PrefixDelegationEnabled X_TELEFONICA-ES_DHCP6cForAddress"
        `   G_WanConns['X_TELEFONICA-ES_IPv6Enabled']					= "$01"; //ipv6 enable
			G_WanConns['X_TELEFONICA-ES_IPv4Enabled']					= "$02"; //ipv4 enable
			G_WanConns['X_TELEFONICA-ES_IPv6PrefixDelegationEnabled']	= "$03";//prefix delegation enable
			G_WanConns['X_TELEFONICA-ES_DHCP6cForAddress']				= "$04";//X_TELEFONICA-ES_DHCP6cForAddress
		`?>
    `?>
`?>

var G_Conns = [];
var n = 0;
<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable AddressingType"
		`	G_Conns[n] = [n,
						  "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection.$00."
						  ];
			n++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger"
		`	G_Conns[n] = [n,
						  "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection.$00."
						  ];
			n++;
		`?>
	`?>
`?>

var G_IP6_info = [];
var _m = -1;
<?if eq $var:G_IPv6Enable 1
`	<?if eq $var:conn_type `Static`
	`	<?mget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$var:secIdx.WANIPConnection.$var:thdIdx.X_TWSZ-COM_IPv6Config. "IPv6AddressList.1.IPv6Address IPv6AddressList.1.PrefixLength IPv6AddressList.1.AddressStatus IPv6DNSOverrideAllowed IPv6DNSServers UsrIPv6DNSServers IPv6DefaultRouterList.1.DefaultRouter IPv6DefaultRouterList.2.DefaultRouter IPv6SitePrefixInfo.IPv6SitePrefix IPv6SitePrefixInfo.IPv6SitePrefixLength"
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

function change_ipv4_enable()
{
	var ipv4_enable = Form.Checkbox('INPUT_EnableIPv4');
	if($('SELECT_Protocol').value == 'Static')
	{
	    $('ip_addr1').style.display = (ipv4_enable == '1') ? 'block' : 'none';
		$('ip_addr2').style.display = (ipv4_enable == '1') ? 'block' : 'none';
		$('ip_addr4').style.display = (ipv4_enable == '1') ? 'block' : 'none';
	}
<?if eq $var:G_IPv6Enable 1 
` 
	var ipv6_enable = Form.Checkbox('INPUT_EnableIPv6');
	if ((ipv6_enable == '1') && (ipv4_enable == '0')) 
	{
		Form.Checkbox('INPUT_EnableNat', '0');
	}
	if (ipv4_enable == '1') 
	{
		Form.Checkbox('INPUT_EnableNat', '1');
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

	$('ip_addr_ipv6').style.display = (ipv6_enable == '1') ? 'block' : 'none';
	if ((ipv6_enable == '1') && (ipv4_enable == '0')) 
	{
		Form.Checkbox('INPUT_EnableNat', '0');
	}
	if ($('SELECT_Protocol').value != 'Bridging')
	{
		$("IPV6_CFG").style.display = ((ipv6_enable == '1') ? 'block' : 'none'); 
	}

	if (ipv6_enable == '1')
	{
		Form.Checkbox('INPUT_IPv6PrefixDelegationEnabled', '1');
		Form.Checkbox('INPUT_IPv6IANAEnabled', '1');
	}
	else if(ipv6_enable == '0')
	{
	    Form.Checkbox('INPUT_IPv6PrefixDelegationEnabled', '0');
		Form.Checkbox('INPUT_IPv6IANAEnabled', '0');
	}
`?>
	onclkNatEnabled();
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

function uiOnload(){
	matchPVC();
	//赋值
	setJSONValue({
		'SELECT_Protocol'    : findProtocol(),
		'INPUT_VlanId'       : G_ATM[_m] ? G_ATM[_m][1] : '0'
	});	

	//WAN赋值
	onchgProtocol();

	onclkIpAddr_1();

<?if eq $var:G_IPv6Enable 1	
`	ipv6_info_load();
`?>
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

//找出Protocol类型
function findProtocol(){
	var _protocol = 'Bridging';
	
	if('<?echo $var:conn_type?>' == 'DHCP'){
		_protocol = 'DHCP';
	}else if('<?echo $var:conn_type?>' == 'Static'){
		_protocol = 'Static';
	}else if('<?echo $var:conn_type?>' == 'PPPOE'){
		_protocol = 'PPPoE';
	}
	
	return _protocol;
}

function onclkNatEnabled()
{
	var bool = Form.Checkbox('INPUT_EnableNat') == '1'?false:true;
	$('SELECT_Nat').disabled =bool;
}

function onchgProtocol(){
	var _protocol = $('SELECT_Protocol').value;
	var nodes = $('pppoe','static','non-bridge','is-bridge','IPoA_MER');
	var pvc = false, q802 = false, ppp = 'none', ip = 'none', non_bridge = 'block', is_bridge = 'none',IPoA_MER = 'none';
	
    if(_protocol == 'Bridging')
	{
	    disCtrl('SELECT_Protocol_V4_6', false);
		Form.Checkbox('INPUT_EnableIPv4', G_WanConns['X_TELEFONICA-ES_IPv4Enabled'] || '1');
<?if eq $var:G_IPv6Enable 1 
`		if(Form.Checkbox('INPUT_EnableIPv6') == '1')
		{
			$('IPV6_CFG').style.display = 'none';
			Form.Checkbox('INPUT_IPv6PrefixDelegationEnabled','1');
			Form.Checkbox('INPUT_IPv6IANAEnabled','1');
			Form.Checkbox('INPUT_EnableIPv6','1');
		}
`?>
	}
	else
	{
	    disCtrl('SELECT_Protocol_V4_6', true);
		Form.Checkbox('INPUT_EnableIPv4', G_WanConns['X_TELEFONICA-ES_IPv4Enabled'] || '1');
       	<?if eq $var:G_IPv6Enable 1 
    		`   Form.Checkbox('INPUT_EnableIPv6', G_WanConns['X_TELEFONICA-ES_IPv6Enabled'] || '0');
        `?>
	}
	switch(_protocol){
		case 'PPPoE' : {
			pvc  = true;
			q802 = true;
			ppp  = 'block';
			//赋值
			setJSONValue({
				'INPUT_UserName'      : G_WanConns['Username'] || '',
				'INPUT_Psd'           : G_WanConns['Password'] || '',
				'INPUT_ConfirmPsd'    : G_WanConns['Password'] || '',
				'SELECT_AuthMethod'   : G_WanConns['PPPAuthenticationProtocol'] || 'Auto',
				
				'SELECT_DialupMode'  : G_WanConns['ConnectionTrigger'] || 'AlwaysOn',
				'INPUT_InaTimeout'    : G_WanConns['IdleDisconnectTime'] || '100',
				
				'INPUT_MTUSize'       : G_WanConns['MaxMRUSize'] || '1492',
				'INPUT_KeepAlive'     : G_WanConns['PPPLCPEchoRetry'] == 0 ? '0' : '1',
				'INPUT_LCPEchoInterval' : G_WanConns['PPPLCPEcho'] || '30',
				'INPUT_LCPEchoRetry'  : G_WanConns['PPPLCPEchoRetry'] || '5',
				
				'INPUT_StaticIpAddr'  : (G_WanConns['X_TWSZ-COM_StaticIPAddress'] && G_WanConns['X_TWSZ-COM_StaticIPAddress'] != '') ? 1 : 0,
				'INPUT_IPAddress'     : G_WanConns['X_TWSZ-COM_StaticIPAddress'] || '',
				
				'INPUT_EnableNat'     : G_WanConns['NATEnabled'] || '1',
				'SELECT_Nat'		: G_WanConns['X_TELEFONICA-ES_NATType'] || 'full cone',
				'INPUT_EnableWanSv'   : G_WanConns['Enable'] || '1',
				'INPUT_ServiceName'   : 'pppoe_' + ($('INPUT_VlanId').value || '0') + '_' + n
			});
			if(G_WanConns['ConnectionType'] == 'IP_Bridged'){
				Form.Checkbox('INPUT_EnableNat', '1');
				$('SELECT_Nat').value = 'full cone';
			}
			$('SELECT_Protocol_V4_6').style.display = 'block';
			<?if eq $var:G_IPv6Enable 1
			`	$('IPV6_CFG').style.display = 'block';
			`?>
			break;
		}
		case 'DHCP' : {
			q802 = true;
			setJSONValue({
				'INPUT_EnableNat'       : G_WanConns['NATEnabled'] || '1',
				'SELECT_Nat'			: G_WanConns['X_TELEFONICA-ES_NATType'] || 'full cone',
				'INPUT_EnableWanSv'     : G_WanConns['Enable'] || '1',
				'INPUT_ServiceName'     : 'mer_' + ($('INPUT_VlanId').value || '0') + '_' + n
			});
			if(G_WanConns['ConnectionType'] == 'IP_Bridged'){
				Form.Checkbox('INPUT_EnableNat', '1');
				$('SELECT_Nat').value = 'full cone';
			}
			$('SELECT_Protocol_V4_6').style.display = 'block';

			<?if eq $var:G_IPv6Enable 1
			`	$('IPV6_CFG').style.display = 'block';
			`?>		
			break;
		}
		case 'Static' : {
			pvc  = true;
			q802 = true;
			ip   = IPoA_MER = 'block';
			var dns;
			if(G_WanConns['DNSOverrideAllowed'])
			{
			    dns = G_WanConns['DNSOverrideAllowed'] == '1' ? G_WanConns['DNSServers'] : G_WanConns['X_TWSZ-COM_UsrDNSServers'];
			}
			
			//赋值
			setJSONValue({
				'INPUT_WanIpAddr_IP'    : G_WanConns['ExternalIPAddress'] || '',
				'INPUT_WanMark_IP'      : G_WanConns['SubnetMask'] || '',
				
				'INPUT_IpAddr_IP'       : G_WanConns['DefaultGateway'] || '',
				'SELECT_WanInf_IP'      : G_WanConns[0] || '',
				
				'INPUT_DnsServer_P'     : dns ? dns.split(',')[0] : '',
				'INPUT_DnsServer_A'     : dns ? (dns.split(',')[1] || '') : '',
				
				'INPUT_EnableNat'       : G_WanConns['NATEnabled'] || '1',
				'SELECT_Nat'			: G_WanConns['X_TELEFONICA-ES_NATType'] || 'full cone',
				'INPUT_EnableWanSv'     : G_WanConns['Enable'] || '1',
				'INPUT_ServiceName'     : 'mer_' + ($('INPUT_VlanId').value || '0') + '_' + n
			});
			if(G_WanConns['ConnectionType'] == 'IP_Bridged'){
				Form.Checkbox('INPUT_EnableNat', '1');
				$('SELECT_Nat').value = 'full cone';
			}
<?if eq $var:G_IPv6Enable 1
`
				setJSONValue({
					'INPUT_WanIpAddr_IP6'   : G_IP6_info['address']||'',
				    'INPUT_WanPrefix_IP6'   : G_IP6_info['addrprefixlen']||'',
				    'INPUT_IPv6_DefGateway' : G_IP6_info['gatewayauto'] ? G_IP6_info['gatewayauto'] : (G_IP6_info['gateway'] ? G_IP6_info['gateway'] : ''),
				    'INPUT_IPv6_Prefix'     : G_IP6_info['prefix']||'',
				    'INPUT_IPv6_PrefixLen'  : G_IP6_info['prefixlen']||'',
				    'INPUT_IPv6_DnsServer_P': G_IP6_info['server'] ? G_IP6_info['server'].split(',')[0] : (G_IP6_info['userserver'] ? G_IP6_info['userserver'].split(',')[0] : ''),
				    'INPUT_IPv6_DnsServer_A': G_IP6_info['server'] ? (G_IP6_info['server'].split(',')[1]||''):(G_IP6_info['userserver'] ? (G_IP6_info['userserver'].split(',')[1] || '') : '')
				});
`?>
			$('SELECT_Protocol_V4_6').style.display = 'block';

			<?if eq $var:G_IPv6Enable 1
			`	$('IPV6_CFG').style.display = 'block';
			`?>		
			break;
		}
		case 'Bridging' : {
			pvc = true;
			q802 = true;
			non_bridge = 'none';
			is_bridge  = 'block';
			$('INPUT_ServiceName').value = 'br_' + ($('INPUT_VlanId').value || '0') + '_' + n
			Form.Checkbox('INPUT_EnableBridgeSv',G_WanConns['Enable'] || '1');
			uiSynaEnable();
			$('SELECT_Protocol_V4_6').style.display = 'none';
			<?if eq $var:G_IPv6Enable 1
			`	$('IPV6_CFG').style.display = 'none';
			`?>			
			break;
		}
	}
	disCtrl('802.1q',q802);
	nodes[0].style.display = ppp;
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
	onclkIpAddr_4();	
	onclkNatEnabled();
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
	var bool = $('SELECT_Protocol').value == 'Static' ? true : false;
	<?if eq $var:G_IPv6Enable 1	
	`
		disCtrl('ip_addr_ipv6', bool);
	`?>
	change_ipv4_enable();
	change_ipv6_enable();
	Form.Checkbox('INPUT_EnableNat', G_WanConns['NATEnabled']);
	onclkNatEnabled();
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
	var _protocol = $('SELECT_Protocol').value;
	
	switch(_protocol){
		case 'PPPoE' : {
			return ['EoA','PPP'];
		}
		case 'DHCP' : {
			return ['EoA','IP'];
		}
		case 'Static' : {
			return ['EoA','IP'];
		}
		case 'Bridging' : {
			return ['EoA','IP'];
		}
	}
}

function checkValue(){
	if($('INPUT_Psd').value != $('INPUT_ConfirmPsd').value){
		alert(SEcode[1010]);
		$('Apply').disabled = 0;
		$('Cancel').disabled = 0;
		return false;
	}
	
	return true;
}

function uiSubmit(){
	
	$('Apply').disabled = 1;
	$('Cancel').disabled = 1;
	if(0 == n)
	{
		uiAjax_Set();
	}
	else
	{
		uiAjax_create_start();
	}
}

function uiAjax_Set(){
		$H({
		   ':InternetGatewayDevice.WANDevice.3.WANEthernetInterfaceConfig.Enable' : "1",
		   	'getpage'       : 'html/page/wiz_ajax.js',
			'var:page'      : 'wanethernet',
			'obj-action'    : 'set'
		}, true);	
		
		var _url = "/cgi-bin/webproc";
		ajax = Ajax.getInstance(_url, "", 0, uiAjax_create);
		ajax.post($('uiPostForm'));	
}

function uiAjax_create(_text){
	//Success, then jump to page
	try{
		eval(_text);
	} catch (ex){
		G_Error = '1';
	}

	if (G_Error == '0'){
		uiAjax_create_start();		
	} else if(G_Error == '1'){
		dealWithError();
		document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wanethernet';		
	}
}

function uiAjax_create_start(){
	var _protocol = $('SELECT_Protocol').value;
	var _node; //存放节点的数组
	var _path = ':InternetGatewayDevice.WANDevice.3.WANConnectionDevice.0.';
	if(_protocol == "PPPoE"){
		if(!checkValue()){
			return false;
		}
	}
	
	var _pro = getProtocol(); //取得一个array,如:['Route','IP']
	//统一的Form内容
	$H({
		'obj-action'    : 'set',
		'var:page'      : 'wanethernet',
		'var:conn_type' : '<?echo $var:conn_type?>',
		'var:secIdx'    : '<?echo $var:secIdx?>',
		'var:thdIdx'    : '<?echo $var:thdIdx?>',
		'getpage'		: 'html/page/wiz_ajax.js'
	},true);
	
	if(_m != -1){
		_path = ':' + G_ATM[_m][0];
	}
	
	//Vlan ID
	$F(_path + 'X_TWSZ-COM_VLANID', $('INPUT_VlanId').value);
	
  
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
	$F(_path + 'X_TELEFONICA-ES_NATType', $('SELECT_Nat').value);
	$F(_path + 'Enable', Form.Checkbox('INPUT_EnableWanSv'));
	<?if eq $var:G_IPv6Enable 1
	`	
	    $F(_path + 'X_TELEFONICA-ES_IPv6Enabled', (_protocol == 'Bridging')? '1' :Form.Checkbox('INPUT_EnableIPv6'));
		$F(_path + 'X_TELEFONICA-ES_IPv4Enabled', (_protocol == 'Bridging')? '1' :Form.Checkbox('INPUT_EnableIPv4'));
	`?>
	$F(_path + 'Name'  , $('INPUT_ServiceName').value);
	
	switch(_protocol){
		case 'PPPoE' : {
			_node = $('INPUT_UserName','INPUT_Psd','SELECT_AuthMethod','INPUT_AcName','INPUT_PPPServiceName','INPUT_InaTimeout','INPUT_MTUSize',
					  'INPUT_PPP_IP_Ext','INPUT_IPAddress','INPUT_IpAddr_PPP','SELECT_WanIntf_PPP','INPUT_LCPEchoInterval','INPUT_LCPEchoRetry');

			$F(_path + 'Username'                  , _node[0].value.strip());
			//Password
			if(_node[1].value != '**********'){
				$F(_path + 'Password'              , _node[1].value);
			}
			$F(_path + 'PPPAuthenticationProtocol' , _node[2].value);
			$F(_path + 'ConnectionTrigger'  , $('SELECT_DialupMode').value);
			if($('SELECT_DialupMode').value == 'OnDemand'){
				$F(_path + 'IdleDisconnectTime' , _node[5].value);
			}
			$F(_path + 'MaxMRUSize'                , _node[6].value);
		
			$F(_path + 'PPPLCPEchoRetry'           , Form.Checkbox('INPUT_KeepAlive') == '1' ? _node[12].value : '0');
			$F(_path + 'PPPLCPEcho'           , _node[11].value);
			//Use Static IP Address
			if(Form.Checkbox('INPUT_StaticIpAddr') == '1'){
				$F(_path + 'X_TWSZ-COM_StaticIPAddress'  , _node[8].value);
			} else {
				$F(_path + 'X_TWSZ-COM_StaticIPAddress'  , '');
			}
<?if eq $var:G_IPv6Enable 1
`
			if(Form.Checkbox('INPUT_EnableIPv6') == '1')
			{
				$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled', Form.Checkbox('INPUT_IPv6PrefixDelegationEnabled'));
				$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress', Form.Checkbox('INPUT_IPv6IANAEnabled'));
				$F(_path + 'X_TWSZ-COM_IPv6Config.IPv6AddressList.1.AddressingType', 'DHCP');
				$F(_path  + 'X_TWSZ-COM_IPv6Config.IPv6DNSEnabled', '1');
				$F(_path  + 'X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed', '0');
				$F(_path  + 'X_TWSZ-COM_IPv6Config.GetIPv6NTPServers', '0');
			}
			if(Form.Checkbox('INPUT_EnableIPv6') == '0')
			{
				$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled', '0');
				$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress', '0');				
			}	
`?>			
			break;
		}
		case 'DHCP' : {
			$F(_path + 'AddressingType'    , 'DHCP'); //DHCP
			if(Form.Checkbox('INPUT_EnableIPv4') == '1')
			{
				$F(_path + 'DNSOverrideAllowed'      , '0');
			}
			//这个用于IP_Bridged -> IP_Routed
			$F(_path + 'ConnectionType'         , 'IP_Routed');			
<?if eq $var:G_IPv6Enable 1
`           if (Form.Checkbox('INPUT_EnableIPv6') == '1')
			{
				var path_v6 = _path + "X_TWSZ-COM_IPv6Config.";
				$F(path_v6 + 'IPv6DefaultRouterList.1.ConfigType' , 'OTHER');
				$F(path_v6 + 'IPv6AddressList.1.AddressingType' , 'DHCP');
				$F(path_v6 + 'IPv6DNSOverrideAllowed'      , '0');
				/* prefix delegation */
				$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled', Form.Checkbox('INPUT_IPv6PrefixDelegationEnabled'));
				$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress', Form.Checkbox('INPUT_IPv6IANAEnabled'));
			}
			if (Form.Checkbox('INPUT_EnableIPv6') == '0')
			{
				$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled', '0');
				$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress', '0');
			}
`?>
			break;
		}
		case 'Static' : {
			_node   = $('INPUT_WanIpAddr_IP','INPUT_WanMark_IP','INPUT_IpAddr_IP','SELECT_WanInf_IP','INPUT_DnsServer_P','INPUT_DnsServer_A');
<?if eq $var:G_IPv6Enable 1
`           if (Form.Checkbox('INPUT_EnableIPv6') == '1')
			{
				var path_v6 = _path + "X_TWSZ-COM_IPv6Config.";
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
				/* prefix delegation */
				$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled', Form.Checkbox('INPUT_IPv6PrefixDelegationEnabled'));
				$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress', Form.Checkbox('INPUT_IPv6IANAEnabled'));
			}
			if (Form.Checkbox('INPUT_EnableIPv6') == '0')
			{
				$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled', '0');
				$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress', '0');
			}
`?>
			$F(_path + 'AddressingType'    , 'Static'); //Static
			//IP & Mark
			if(Form.Checkbox('INPUT_EnableIPv4') == '1')
			{
				$F(_path + 'ExternalIPAddress' , _node[0].value);
				$F(_path + 'SubnetMask'        , _node[1].value);
		
				$F(_path + 'DefaultGateway'    , _node[2].value);
				//DNS Server

				$F(_path + 'DNSServers', (_node[4].value + ',' + _node[5].value).delcomma());
				$F(_path + 'DNSOverrideAllowed'      , '1');
			}
	
			//这个用于IP_Bridged -> IP_Routed
			$F(_path + 'ConnectionType'         , 'IP_Routed');			
			break;
		}
		case 'Bridging' : {
			$F(_path + 'ConnectionType'         , 'IP_Bridged');
//			$F(_path + 'X_TWSZ-COM_ProtocolType', 'IP');
			$F(_path + 'X_TWSZ-COM_ServiceList' , 'Internet');
		}
	}
	
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_AddProc);
	ajax.post($('uiPostForm'));	
}

function ajax_AddProc(_text){
	//成功, 则set
	try{
		eval(_text);
	} catch (ex){
		G_Error = '1';
	}	
	if (G_Error == '0'){
		$G('/cgi-bin/webproc',{
			'var:menu'			: 'setup',
			'var:page'			: 'wancfg',
			'getpage'			: 'html/index.html',
			'errorpage'			: 'html/index.html'
		});	
	} else if (G_Error == '1'){
			ajax_disable_eth();//如果建立第一条lan上行失败，则要将lan上行功能关闭
	}
}
function ajax_disable_eth(){
	 if (0 == n){	
		$H({
		'obj-action'	: 'set',
		'var:page'		: 'wanethernet',
		'getpage'		: 'html/page/wiz_ajax.js'
		}, true);
		
		$F(':InternetGatewayDevice.WANDevice.3.WANEthernetInterfaceConfig.Enable' , 0);	
		
		var _url = "/cgi-bin/webproc";
		ajax = Ajax.getInstance(_url, "", 0, ajax_disable_eth_end);
		ajax.post($('uiPostForm'));	
	}
	else{
		dealWithError();
		document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wancfg';
	}
}

function ajax_disable_eth_end(){
	dealWithError();
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
//checkbox状态同步
function uiSynaEnable(){
	if(Form.Checkbox('INPUT_EnableBridgeSv') == '1'){
		Form.Checkbox('INPUT_EnableWanSv','1');
	} else {
		Form.Checkbox('INPUT_EnableWanSv','0');
	}
}

function uiPageRefresh(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=<?echo $var:menu?>&var:page=<?echo $var:page?>&var:subpage=wanethernet&var:conn_type=<?echo $var:conn_type?>&var:secIdx=<?echo $var:secIdx?>&var:thdIdx=<?echo $var:thdIdx?>';
}

function backtowancfg(){
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wancfg';
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
