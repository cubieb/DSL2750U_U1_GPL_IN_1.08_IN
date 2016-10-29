
/*******************************************all device info***************************************************/
<?mget :InternetGatewayDevice.DeviceInfo. "Manufacturer ManufacturerOUI ModelName Description SerialNumber HardwareVersion SoftwareVersion ModemFirmwareVersion UpTime"
`	var G_Manufacturer 		    = "$01";
	var G_ManufacturerOUI 		= "$02";
	var G_ModelName 			= "$03";
	var G_Descripstion 			= "$04";
	var G_SerialNumber 			= "$05";
	var G_HarewareVersion 		= "$06";
	var G_SoftwareVersion 		= "$07";
	var G_ModemFirmwareVersion 	= "$08";
	var G_UpTime               = "$09";
`?>

var G_CurrentLocalTime      = "<?get :InternetGatewayDevice.Time.CurrentLocalTime?>";
	
/*************************************************************************************************************/

/**************************************all wan info***********************************************************/
var G_ATM = [];
var G_ATM_3G = [];
var G_ATM_ETH = [];
var m = 0;
var n = 0;
var num_3g = 0;
var G_dsl_uprate = "N/A";
var G_dsl_downrate = "N/A";
var G_myType = "Ethernet";
var G_buttonName = data_language.value.clear_time_counter;
var G_CurrentTime = "<?echo $var:sys_CurrentTime ?>";

<?if eq 1 $var:mod_IGMP
`	
	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANDSLLinkConfig.LinkType WANDSLLinkConfig.DestinationAddress WANDSLLinkConfig.ATMEncapsulation WANDSLLinkConfig.ATMQoS"
	`	G_ATM[m] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$00.",//Path
			"$01",//LinkType
			"$02",//DestinationAddress
			"$03",//ATMEncapsulation
			"$04", //ATMQoS
			"$04" //X_TELEFONICA-ES_IGMPEnabled
			];
			m++;
	`?>
	<?if eq 1 $var:mod_NEW3G
	`
		<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice. "WANDSLLinkConfig.LinkType WANDSLLinkConfig.DestinationAddress WANDSLLinkConfig.ATMEncapsulation WANDSLLinkConfig.ATMQoS"
		`	G_ATM_3G[num_3g] = ["InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$00.",//Path
				"$01",//LinkType
				"$02",//DestinationAddress
				"$03",//ATMEncapsulation
				"$04", //ATMQoS
				"$04" //X_TELEFONICA-ES_IGMPEnabled
				];
				num_3g++;
		`?>
	`?>
	G_dsl_uprate = "<?get :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.UpstreamCurrRate ?>";
	G_dsl_downrate = "<?get :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.DownstreamCurrRate ?>";
`?>

<?if neq 1 $var:mod_IGMP
`
	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANDSLLinkConfig.LinkType WANDSLLinkConfig.DestinationAddress WANDSLLinkConfig.ATMEncapsulation WANDSLLinkConfig.ATMQoS WANDSLLinkConfig.X_TELEFONICA-ES_IGMPEnabled"
	`	G_ATM[m] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$00.",//Path
			"$01",//LinkType
			"$02",//DestinationAddress
			"$03",//ATMEncapsulation
			"$04", //ATMQoS
			"$05" //X_TELEFONICA-ES_IGMPEnabled
			];
			m++;
	`?>
	<?if eq 1 $var:mod_NEW3G
	`
		<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice. "WANDSLLinkConfig.LinkType WANDSLLinkConfig.DestinationAddress WANDSLLinkConfig.ATMEncapsulation WANDSLLinkConfig.ATMQoS WANDSLLinkConfig.X_TELEFONICA-ES_IGMPEnabled"
		`	G_ATM_3G[num_3g] = ["InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$00.",//Path
				"$01",//LinkType
				"$02",//DestinationAddress
				"$03",//ATMEncapsulation
				"$04", //ATMQoS
				"$05" //X_TELEFONICA-ES_IGMPEnabled
				];
				num_3g++;
		`?>
	`?>
	
	<?if eq 1 $var:mod_ETHWAN
	`			
		<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANEthernetLinkConfig.X_TELEFONICA-ES_IGMPEnabled"
		`	G_ATM_ETH[n] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$00.",//Path
				"$01" //X_TELEFONICA-ES_IGMPEnabled
				];
				n++;
		`?>
	`?>
	G_dsl_uprate = "<?get :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.UpstreamCurrRate ?>";
	G_dsl_downrate = "<?get :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.DownstreamCurrRate ?>";
`?>

//Wan Conns List
var G_wanConnction = [];
var G_wanConnction_adsl = [];
var G_wanConnction_3g = [];
var G_wanConnction_eth = [];
var m = 0;
var j = 0;
var n = 0;
var list_3g = 0;
<?objget :InternetGatewayDevice.WANDevice. ""
`
	<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANPPPConnectionNumberOfEntries"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionType ExternalIPAddress RemoteIPAddress DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers X_TWSZ-COM_ConnectedDeviceName ConnectionTime X_TWSZ-COM_ServiceList"
			`	G_wanConnction[m] = ["InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00.", //Path
								 "$01"=="Connected" ? data_language.dynamic.connection : data_language.dynamic.disconnection ,//ConnectionStatus
								 "$02",//name;
								 "$03",//Enable;
								 "$04", ////ConnectionType
								 "PPPoE", //address type
								 "",       //  igmp
								 "Enable",       //qos
								 "$05",		// ip addr
								 "$06" ,     // default gw
								 "$07"  ,    // dnsoverride
								 "$08"  ,     // dnsserver
								 "$09" ,   //user dns
								 "$0a" ,  //connection device
								 "$0b" ,   //ConnectionTime
								 "$0c"     //service list
								 ];
				++m;
			`?>
		`?>
	`?>
`?>

<?objget :InternetGatewayDevice.WANDevice. ""
`
	<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable ConnectionType AddressingType ExternalIPAddress DefaultGateway DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers X_TWSZ-COM_ConnectedDeviceName ConnectionTime X_TWSZ-COM_ServiceList"
			`	G_wanConnction[m] = ["InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00.", //Path
								 "$01"=="Connected" ? data_language.dynamic.connection : data_language.dynamic.disconnection ,   //ConnectionStatus
								 "$02",    //name
								 "$03",    //enalbe
								 "$04",    //ConnectionType
								 "$05",       //AddressingType
								 "",		//  igmp
								 "Enable",		// qos
								 "$06",      //  ip addr 
								 "$07" ,        //default gw
								  "$08" ,     // dnsoverride
								 "$09",       // dnsserver
								 "$0a"  ,  //user dns
								 "$0b",//connection device
								 "$0c",//ConnectionTime
								 "$0d" //service list
								 ];
				++m;
			`?>
		`?>
	`?>	
`?>	


<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionType ExternalIPAddress RemoteIPAddress DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers X_TWSZ-COM_ConnectedDeviceName ConnectionTime X_TWSZ-COM_ServiceList"
		`	G_wanConnction_adsl[j] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00.", //Path
							 "$01"=="Connected" ? data_language.dynamic.connection : data_language.dynamic.disconnection ,//ConnectionStatus
							 "$02",//name;
							 "$03",//Enable;
							 "$04", ////ConnectionType
							 "PPPoE", //address type
							 "",       //  igmp
							 "Enable",       //qos
							 "$05",		// ip addr
							 "$06" ,     // default gw
							 "$07"  ,    // dnsoverride
							 "$08"  ,     // dnsserver
							 "$09" ,   //user dns
							 "$0a" ,  //connection device
							 "$0b" ,   //ConnectionTime
							 "$0c"     //service list
							 ];
			++j;
		`?>
	`?>
`?>


<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable ConnectionType AddressingType ExternalIPAddress DefaultGateway DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers X_TWSZ-COM_ConnectedDeviceName ConnectionTime X_TWSZ-COM_ServiceList"
		`	G_wanConnction_adsl[j] = ["InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00.", //Path
							 "$01"=="Connected" ? data_language.dynamic.connection : data_language.dynamic.disconnection ,   //ConnectionStatus
							 "$02",    //name
							 "$03",    //enalbe
							 "$04",    //ConnectionType
							 "$05",       //AddressingType
							 "",		//  igmp
							 "Enable",		// qos
							 "$06",      //  ip addr 
							 "$07" ,        //default gw
							  "$08" ,     // dnsoverride
							 "$09",       // dnsserver
							 "$0a"  ,  //user dns
							 "$0b",//connection device
							 "$0c",//ConnectionTime
							 "$0d" //service list
							 ];
			++j;
		`?>
	`?>
`?>	
<?if eq 1 $var:mod_NEW3G
`
	<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice. "WANPPPConnectionNumberOfEntries"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionType ExternalIPAddress RemoteIPAddress DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers X_TWSZ-COM_ConnectedDeviceName ConnectionTime X_TWSZ-COM_ServiceList"
			`	G_wanConnction_3g[list_3g] = ["InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$20.WANPPPConnection.$00.", //Path
								 "$01"=="Connected" ? data_language.dynamic.connection : data_language.dynamic.disconnection ,//ConnectionStatus
								 "$02",//name;
								 "$03",//Enable;
								 "$04", ////ConnectionType
								 "PPPoE", //address type
								 "",       //  igmp
								 "Enable",       //qos
								 "$05",		// ip addr
								 "$06" ,     // default gw
								 "$07"  ,    // dnsoverride
								 "$08"  ,     // dnsserver
								 "$09" ,   //user dns
								 "$0a" ,  //connection device
								 "$0b" ,   //ConnectionTime
								 "$0c"     //service list
								 ];
				++list_3g;
			`?>
		`?>
	`?>
	
	
	<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice. "WANIPConnectionNumberOfEntries"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable ConnectionType AddressingType ExternalIPAddress DefaultGateway DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers X_TWSZ-COM_ConnectedDeviceName ConnectionTime X_TWSZ-COM_ServiceList"
			`	G_wanConnction_3g[list_3g] = ["InternetGatewayDevice.WANDevice.2.WANConnectionDevice.$20.WANIPConnection.$00.", //Path
								 "$01"=="Connected" ? data_language.dynamic.connection : data_language.dynamic.disconnection ,   //ConnectionStatus
								 "$02",    //name
								 "$03",    //enalbe
								 "$04",    //ConnectionType
								 "$05",       //AddressingType
								 "",		//  igmp
								 "Enable",		// qos
								 "$06",      //  ip addr 
								 "$07" ,        //default gw
								  "$08" ,     // dnsoverride
								 "$09",       // dnsserver
								 "$0a"  ,  //user dns
								 "$0b",//connection device
								 "$0c",//ConnectionTime
								 "$0d" //service list
								 ];
				++list_3g;
			`?>
		`?>
	`?>	
`?>
<?if eq 1 $var:mod_ETHWAN
`
	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANPPPConnectionNumberOfEntries"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionType ExternalIPAddress RemoteIPAddress DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers X_TWSZ-COM_ConnectedDeviceName ConnectionTime X_TWSZ-COM_ServiceList"
			`	G_wanConnction_eth[n] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection.$00.", //Path
								 "$01"=="Connected" ? data_language.dynamic.connection : data_language.dynamic.disconnection ,//ConnectionStatus
								 "$02",//name;
								 "$03",//Enable;
								 "$04", ////ConnectionType
								 "PPPoE", //address type
								 "",       //  igmp
								 "Enable",       //qos
								 "$05",		// ip addr
								 "$06" ,     // default gw
								 "$07"  ,    // dnsoverride
								 "$08"  ,     // dnsserver
								 "$09" ,   //user dns
								 "$0a" ,  //connection device
								 "$0b" ,   //ConnectionTime
								 "$0c"     //service list
								 ];
				++n;
			`?>
		`?>
	`?>
	
	
	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANIPConnectionNumberOfEntries"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable ConnectionType AddressingType ExternalIPAddress DefaultGateway DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers X_TWSZ-COM_ConnectedDeviceName ConnectionTime X_TWSZ-COM_ServiceList"
			`	G_wanConnction_eth[n] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection.$00.", //Path
								 "$01"=="Connected" ? data_language.dynamic.connection : data_language.dynamic.disconnection ,   //ConnectionStatus
								 "$02",    //name
								 "$03",    //enalbe
								 "$04",    //ConnectionType
								 "$05",       //AddressingType
								 "",		//  igmp
								 "Enable",		// qos
								 "$06",      //  ip addr 
								 "$07" ,        //default gw
								  "$08" ,     // dnsoverride
								 "$09",       // dnsserver
								 "$0a"  ,  //user dns
								 "$0b",//connection device
								 "$0c",//ConnectionTime
								 "$0d" //service list
								 ];
				++n;
			`?>
		`?>
	`?>	
`?>

//ipv6
var G_IPv6_Addr = [];
m=0;

<? if eq 1 `$var:mod_SUPPORT_IPV6`
`
	<?objget :InternetGatewayDevice.WANDevice. ""
		`
		<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries"
		`	<?if gt $11 0
			`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. " X_TWSZ-COM_IPv6Config.IPv6AddressList.1.IPv6Address X_TWSZ-COM_IPv6Config.IPv6AddressList.2.IPv6Address  X_TWSZ-COM_IPv6Config.IPv6AddressList.3.IPv6Address X_TWSZ-COM_ProtocolClass X_TWSZ-COM_IPv6Config.ConnectionStatus X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed X_TWSZ-COM_IPv6Config.IPv6DNSServers X_TWSZ-COM_IPv6Config.UsrIPv6DNSServers"
				`	G_IPv6_Addr[m] = ["InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00.", //Path
									 "$01",        //dhcp
									 ""    ,     //link local
									 "$03",     //ra
									 "$04",   //class type
									 "$05",     //ipv6 status
									  "$06",	// dns override
									  "$07",     //dns server
									  "$08"    //user dns
									 ];
					++m;							
				`?>
			`?>
		`?>
	`?>
`?>

<? if neq 1 `$var:mod_SUPPORT_IPV6`
`
	<?objget :InternetGatewayDevice.WANDevice. ""
	`
		<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries"
		`	<?if gt $11 0
			`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. " X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass  X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass"
				`	G_IPv6_Addr[m] = ["InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00.", //Path
									 "$01",        //dhcp
									 ""    ,     //link local
									 "$03",     //ra
									 "$04",   //class type
									 "$05",     //ipv6 status
									  "$06",	// dns override
									  "$07",     //dns server
									  "$08"    //user dns
									 ];
					++m;		
				`?>
			`?>
		`?>
	`?>
`?>

<? if eq 1 `$var:mod_SUPPORT_IPV6`
`
	<?objget :InternetGatewayDevice.WANDevice. ""
	`
		<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANPPPConnectionNumberOfEntries"
		`	<?if gt $11 0
			`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. " X_TWSZ-COM_IPv6Config.IPv6AddressList.1.IPv6Address X_TWSZ-COM_IPv6Config.IPv6AddressList.2.IPv6Address  X_TWSZ-COM_IPv6Config.IPv6AddressList.3.IPv6Address X_TWSZ-COM_ProtocolClass X_TWSZ-COM_IPv6Config.ConnectionStatus X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed X_TWSZ-COM_IPv6Config.IPv6DNSServers X_TWSZ-COM_IPv6Config.UsrIPv6DNSServers"
				`	G_IPv6_Addr[m] = ["InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00.", //Path
									 "$01",           //dhcp
									  ""    ,        //link local
									  "$03",            //ra
									  "$04",      //class type
									  "$05",     // ipv6 status
									  "$06",	// dns override
									  "$07",     //dns server
									  "$08"     //user dns
									 ];
					++m;
				`?>
			`?>
		`?>
	`?>
`?>

<? if neq 1 `$var:mod_SUPPORT_IPV6`
`
	<?objget :InternetGatewayDevice.WANDevice. ""
	`
		<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANPPPConnectionNumberOfEntries"
		`	<?if gt $11 0
			`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. " X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass  X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass X_TWSZ-COM_ProtocolClass"
				`	G_IPv6_Addr[m] = ["InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00.", //Path
									 "$01",           //dhcp
									  ""    ,        //link local
									  "$03",            //ra
									  "$04",      //class type
									  "$05",     // ipv6 status
									  "$06",	// dns override
									  "$07",     //dns server
									  "$08"     //user dns
									 ];
					++m;
				`?>
			`?>
		`?>
	`?>
`?>

var G_DefaultRouter = "<?get :InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW?>";
 
var G_RouteInfo = new Array();
m = 0;

<?objget :InternetGatewayDevice.X_TWSZ-COM_RouteInfo. "DestIPAddress GatewayIPAddress DestSubnetMask  Flags Ref ForwardingMetric Interface"
	`	G_RouteInfo[m] = new Array();
		G_RouteInfo[m][0] = "$01"; //DestIPAddress
		G_RouteInfo[m][1] = "$02"; //GatewayIPAddress
		G_RouteInfo[m][2] = "$03"; //DestSubnetMask
		G_RouteInfo[m][3] = "$04"; //Flags
		G_RouteInfo[m][4] = "$05"; //Ref
		G_RouteInfo[m][5] = "$06"; //ForwardingMetric
		G_RouteInfo[m][6] = "$07"; //Interface
		G_RouteInfo[m][7] = "$00"; //index
		m++;
`?>
		

/*************************************************************************************************************/

/*****************************************All Wlan device info************************************************/

var G_Wlan_Info = [];
var t = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "SSID BSSID BeaconType TotalAssociations Enable SSIDAdvertisementEnabled"
`	G_Wlan_Info[t] = [];
	G_Wlan_Info[t][0] = "InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00";
	G_Wlan_Info[t][1] = "$01";		//SSID
	G_Wlan_Info[t][2] = "$02";		//BSSID
	G_Wlan_Info[t][3] = "$03"=='None' ? data_language.dynamic.none : "$03";		//BeaconType
	G_Wlan_Info[t][4] = "$04";		//TotalAssociations
	G_Wlan_Info[t][5] = "$05";		//Enable
	G_Wlan_Info[t][6] = "$06";		//hide
	t++;
`?>

var G_WirelessEnable   = "<?get :InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Enable?>";

/*************************************************************************************************************/



/**********************************************all lan info***************************************************/
//all lan info
var G_lan_info = []
<?mget :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement. "DHCPServerEnable IPInterface.1.IPInterfaceIPAddress IPInterface.1.IPInterfaceSubnetMask"
`
	var G_lan_dhcp_enable = "$01";
	var G_lan_ip_addr     = "$02";
	var G_lan_mask        = "$03";
`?>
	/*vport1 or vport4 May as LAN ascending mouth use, cannot get local MAC address*/
	var G_lan_mac = "<?get :InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.2.MACAddress ?>";
 
/*************************************************************************************************************/ 
<?if eq 1 $var:mod_IGMP
`	
	var igmp_enable = "<?get :InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.Enable ?>"
	var igmp_wan = "<?get :InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.WANPathName ?>"
`?>

/***********************************        wan process***************************************/

function find_default_gw()
{
//	if (G_DefaultRouter == '') return 'NULL';
//	for (var i =0; i <  G_wanConnction.length; i++) {
//		if (G_wanConnction[i][0].indexOf(G_DefaultRouter) > -1)
//			return G_wanConnction[i][9];
//	}
//	return 'NULL';

	var default_gw = '';
	for (var i = 0; i < G_RouteInfo.length; i++) {
		if (G_RouteInfo[i][0] == '0.0.0.0' && G_RouteInfo[i][2] == '0.0.0.0') {
			if (G_RouteInfo[i][1] != '0.0.0.0') return G_RouteInfo[i][1];
			else {
				default_gw = G_RouteInfo[i][6];
				break;
			}
		}
	}
	if (default_gw == '') return '';
	for (i = 0;  i <  G_wanConnction.length; i++) {
		if (G_wanConnction[i][13] == default_gw) 
			return G_wanConnction[i][9];
	}
	return '';
}

function select_wan()
{
	//var default_gw = find_default_gw();
	var index = Form.Select('select_wan_interface');
	if (index < 0) return;
 
	if (G_wanConnction[index][4] != "IP_Routed")         
	{
		$('internet_info_bridge').style.display = 'block';
		$('internet_info').style.display = 'none';
		//alert(G_wanConnction[index][4]);
		if(G_wanConnction[index][4] != "IP_Bridged"){
			//$('wan_service_type1').value = G_wanConnction[index][15];
			//alert(G_wanConnction[index][15);
			setJSONValue({
				'wan_service_type1' : G_wanConnction[index][15]
			});
		}else{
			setJSONValue({
				'wan_service_type1' : 'Internet'
			});
		}

		if(G_wanConnction[index][0].indexOf('WANDevice.3') <= -1){
			setJSONValue({
				'internet_connection_status_bridge'    : G_wanConnction[index] ? G_wanConnction[index][1] : ''
			});
		}
	}
	else
	{
		var dns = [];
		var currentTime = '0';
		if ( G_IPv6_Addr[index][4] == "IPv4") {
			/*if (G_wanConnction[index][10] == '0') {
				dns = G_wanConnction[index][11].split(',');
			}
			else {
				dns = G_wanConnction[index][12].split(',');
			}*/

			dns = G_wanConnction[index][11].split(',');
		} else {
			if (G_IPv6_Addr[index][6] == '0') {
				dns = G_IPv6_Addr[index][7].split(',');
			}
			else {
				dns = G_IPv6_Addr[index][8].split(',');
			}
		}

		if((G_wanConnction[index][1] == "Connected") || (G_wanConnction[index][1] == data_language.dynamic.connection)) 
		{
			var timeGet = parseInt(G_wanConnction[index][14]);
			var nowTime = parseInt(G_CurrentTime);
			var timeTemp=nowTime - timeGet;
			if(timeTemp<0)
			{
				timeTemp=0;
			}
			currentTime = timeTemp;
		}else{
			currentTime = '';
		}
		$('internet_info_bridge').style.display = 'none';
		$('internet_info').style.display = 'block';
		<?if eq 1 $var:mod_TR069
		`
			if(G_wanConnction[index][5] == "PPPoE"){
				//alert(G_wanConnction[index][15]);
				setJSONValue({
						'wan_service_type' : G_wanConnction[index][15]
				});
			}else{
				//alert(G_wanConnction[index][14]);
				setJSONValue({
						'wan_service_type' : G_wanConnction[index][15]
				});
			}
		`?>
		<?if neq 1 $var:mod_TR069
		`
			if(G_wanConnction[index][15] != "Internet_TR069")
			{
				if(G_wanConnction[index][5] == "PPPoE"){
					//alert(G_wanConnction[index][15]);
					setJSONValue({
							'wan_service_type' : G_wanConnction[index][15]
					});
				}else{
					//alert(G_wanConnction[index][14]);
					setJSONValue({
							'wan_service_type' : G_wanConnction[index][15]
					});
				}
			}
			else
			{
				setJSONValue({
							'wan_service_type' : "Internet"
				});
			}
		`?>

		if ((G_wanConnction[index][1] == "Connected") || (G_wanConnction[index][1] == data_language.dynamic.connection))
		{
			setJSONValue({
				'internet_connection_status'    :  G_IPv6_Addr[index][4] == "IPv4" ? (G_wanConnction[index] ? G_wanConnction[index][1] : ''):G_IPv6_Addr[index][5],
				'internet_first_dns'			:  dns[0],
				'internet_second_dns'			:  dns.length>1 ? dns[1] : '', 
				<? if eq 1 `$var:mod_SUPPORT_IPV6`
				`
					'internet_link_local'	: "",
					'internet_RA'			: G_IPv6_Addr[index][3],
					'internet_dhcp'			: G_IPv6_Addr[index][1],
				`?>
				'internet_gateway'				: G_wanConnction[index][9]});
		}else{
		setJSONValue({
		 		'internet_connection_status'    :  G_IPv6_Addr[index][4] == "IPv4" ? (G_wanConnction[index] ? G_wanConnction[index][1] : ''):G_IPv6_Addr[index][5],
				'internet_first_dns'			:  "",
				'internet_second_dns'			:  "", 
				<? if eq 1 `$var:mod_SUPPORT_IPV6`
				`
				'internet_link_local'	: "",
				'internet_RA'			: '',
				'internet_dhcp'			: '',
				`?>
				'internet_gateway'				: ''});				
		}
		if(currentTime != '')
		{
			var hour = Math.floor(currentTime/3600);
			var min = currentTime%3600;
			var sec = min%60;
			min = Math.floor(min/60);
			$('internet_time_counter').innerHTML = ""+hour+':'+ min+':' + sec + '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="button" id="clear_time_counter" style="width:50px" value="'+G_buttonName+'" onclick="javascript:return Clear_Internet_Time_Counter()">';
		}
		else
		{
			$('internet_time_counter').innerHTML = currentTime;
		}
	}
}

function Clear_Internet_Time_Counter()
{
	var index = Form.Select('select_wan_interface');
	var nowTime = parseInt(G_CurrentTime);
	var path=':'+G_wanConnction[index][0]+'ConnectionTime';
	$H({
		'var:menu'				: G_Menu,
		'var:page'				: G_Page,
		'var:subpage'			: G_SubPage,
		'var:errorpage'			: G_SubPage,
		'obj-action'			: 'set',
		'getpage'				: 'html/index.html',
		'errorpage'				: 'html/index.html',
		'var:CacheLastData'		: ViewState.Save()
		});

	$F(path , nowTime);
	
	$('uiPostForm').submit();
}

function load_wan_select()
{
	var _text = [], _value = [];
	for(var i = 0; i < G_wanConnction.length; i++){
		_text.push(G_wanConnction[i][2]);
		_value.push(i);
	}
	$S('select_wan_interface',_text,_value);
	if (i == 0) return;
	select_wan();
}

function getAtmParam_adsl(_i){
	var path_wan = G_wanConnction_adsl[_i][0];

	for(var i = 0; i < G_ATM.length; i++){
		if(path_wan.indexOf(G_ATM[i][0]) > -1){
			return i;
		}
	}
	
	return -1;
}

function getAtmParam_3g(_i)
{
	var path_wan = G_wanConnction_3g[_i][0];

	for(var i = 0; i < G_ATM_3G.length; i++){
		if(path_wan.indexOf(G_ATM_3G[i][0]) > -1){
			return i;
		}
	}	
	return -1;
}

function getAtmParam_eth(_i)
{
	var path_wan = G_wanConnction_eth[_i][0];

	for(var i = 0; i < G_ATM_ETH.length; i++){
		if(path_wan.indexOf(G_ATM_ETH[i][0]) > -1){
			return i;
		}
	}	
	return -1;
}

function get_conn_type_adsl()
{
		var i = arguments[0];
		var j = getAtmParam_adsl(i);
		if (G_ATM[j][1] == 'EoA') {
			if (G_wanConnction_adsl[i][0].indexOf('IP') > -1) {
				if (G_wanConnction_adsl[i][4] == 'IP_Bridged')
					return 'BRIDGE';
				else 
					return G_wanConnction_adsl[i][5].toUpperCase();
			}
			else
			{
				if (G_wanConnction_adsl[i][4] == 'PPPoE_Bridged')
					return 'BRIDGE';
				else
					return 'PPPOE';
			}
		}
		else
			return G_ATM[j][1];
}

function get_conn_type_3g()
{
		var i = arguments[0];
		var j = getAtmParam_3g(i);
		if (G_ATM_3G[j][1] == 'EoA') {
			if (G_wanConnction_3g[i][0].indexOf('IP') > -1) {
				if (G_wanConnction_3g[i][4] == 'IP_Bridged')
					return 'BRIDGE';
				else 
					return G_wanConnction_3g[i][5].toUpperCase();
			}
			else
			{
				if (G_wanConnction_3g[i][4] == 'PPPoE_Bridged')
					return 'BRIDGE';
				else
					return 'PPPOE';
			}
		}
		else
			return G_ATM_3G[j][1];
}

function get_conn_type_eth()
{
	var i = arguments[0];

	if (G_wanConnction_eth[i][0].indexOf('IP') > -1) {
		if (G_wanConnction_eth[i][4] == 'IP_Bridged')
			return 'BRIDGE';
		else 
			return G_wanConnction_eth[i][5].toUpperCase();
		}
	else
		return 'PPPOE';
}

function load_adsl_list()
{
	//Table.Clear('wan_info');
	var array_value = [];
	var igmp = '';
	var type_adsl = '';
	//load_qos_dsl();	

	for(var i = 0; i < G_wanConnction_adsl.length; i++){
		<?if eq 1 $var:mod_IGMP
		`	
			igmp = 'Disable';
			if (igmp_enable == '1') {
				if (G_wanConnction_adsl[i][0].indexOf(igmp_wan) != -1)
					igmp = 'Enable';
			}
		`?>
		j = getAtmParam_adsl(i);
		type_adsl = get_conn_type_adsl(i);

		array_value[i] = [];
		array_value[i].push(G_ATM[j][2]);         //vci/vpi
		array_value[i].push(G_wanConnction_adsl[i][2]);  //name
		array_value[i].push(type_adsl);   //type
		<?if eq 1 $var:mod_IGMP
		`	
			array_value[i].push(igmp=='Enable'? data_language.dynamic.enalbe : data_language.dynamic.disable);   //igmp
		`?>
		
		<?if eq 1 $var:mod_TF_IGMP
		`	
			array_value[i].push(G_ATM[j][5] == '1' ? 'Enable' : 'Disable');   //igmp
		`?>
		array_value[i].push(G_wanConnction_adsl[i][7] == 'Enable'? data_language.dynamic.enalbe : data_language.dynamic.disable); //qos
		array_value[i].push(G_wanConnction_adsl[i][8]);   //ip addr

	}
	$T('wan_info',array_value);
}

function load_eth_list()
{
	var array_value_eth = [];
	var igmp = '';
	var type_eth = '';
		
	for(var k = 0; k < G_wanConnction_eth.length; k++){
		<?if eq 1 $var:mod_IGMP
		`	
			igmp = 'Disable';
			if (igmp_enable == '1') {
				if (G_wanConnction_eth[k][0].indexOf(igmp_wan) != -1)
					igmp = 'Enable';
			}
		`?>
		j = getAtmParam_eth(k);
		type_eth = get_conn_type_eth(k);

		array_value_eth[k] = [];
		array_value_eth[k].push("N/A");         //vci/vpi
		array_value_eth[k].push(G_wanConnction_eth[k][2]);  //name
		array_value_eth[k].push(type_eth);   //type
		<?if eq 1 $var:mod_IGMP
		`	
			array_value_eth[k].push(igmp=='Enable'? data_language.dynamic.enalbe : data_language.dynamic.disable);   //igmp
		`?>
		
		<?if eq 1 $var:mod_TF_IGMP
		`
			array_value_eth[k].push(G_ATM_ETH[j][1] == '1' ? 'Enable' : 'Disable');   //igmp
		`?>
		array_value_eth[k].push(G_wanConnction_eth[k][7] == 'Enable'? data_language.dynamic.enalbe : data_language.dynamic.disable); //qos
		array_value_eth[k].push(G_wanConnction_eth[k][8]);   //ip addr
	
	}
	$T('wan_info',array_value_eth);
}

function load_3g_list()
{
	//Table.Clear('wan_info');
	var array_value = [];
	var igmp = '';
	var type_3g = '';
	//load_qos_dsl();	

	for(var i = 0; i < G_wanConnction_3g.length; i++){
		<?if eq 1 $var:mod_IGMP
		`	
			igmp = 'Disable';
			if (igmp_enable == '1') {
				if (G_wanConnction_3g[i][0].indexOf(igmp_wan) != -1)
					igmp = 'Enable';
			}
		`?>
		j = getAtmParam_3g(i);
		type_3g = get_conn_type_3g(i);

		array_value[i] = [];
		array_value[i].push("N/A");         //vci/vpi
		array_value[i].push(G_wanConnction_3g[i][2]);  //name
		array_value[i].push(type_3g);   //type
		<?if eq 1 $var:mod_IGMP
		`	
			array_value[i].push(igmp=='Enable'? data_language.dynamic.enalbe : data_language.dynamic.disable);   //igmp
		`?>
		
		<?if eq 1 $var:mod_TF_IGMP
		`	
			array_value[i].push(G_ATM_3G[j][5] == '1' ? 'Enable' : 'Disable');   //igmp
		`?>
		array_value[i].push(G_wanConnction_3g[i][7] == 'Enable'? data_language.dynamic.enalbe : data_language.dynamic.disable); //qos
		array_value[i].push(G_wanConnction_3g[i][8]);   //ip addr

	}
	$T('wan_info',array_value);
}


function load_LineRate()
{
	setJSONValue({
		'internet_down_rate'			:  G_dsl_downrate == "" ? "0" : G_dsl_downrate,
		'internet_up_rate'				:  G_dsl_uprate == "" ? "0" : G_dsl_uprate
	});
}
/**************************************************************************************************************/

/*************************************         lan process ***************************************************/
function timeForm(number)
{ 
	if(number > 9) 
		return number; 
	else 
		return ("0" + number); 
} 

function load_other()
{
	var second = G_UpTime % 60;
	G_UpTime = parseInt(G_UpTime/60);
	var minute = G_UpTime % 60;
	G_UpTime = parseInt(G_UpTime/60);
	var hour = G_UpTime;
	second = timeForm(second);
	minute = timeForm(minute);
	hour = timeForm(hour);
	var UpTime = hour + ":" + minute + ":" + second;
		
	setJSONValue({
		'system_info_model_name' : G_ModelName,
		'system_info_time'  : G_CurrentLocalTime.replace('T',' '),
		'serial_number'     : G_SerialNumber,
		'system_info_firmware' : G_ModemFirmwareVersion,
		'system_info_uptime' : UpTime,
		'local_mac_address' : G_lan_mac,
		'local_ip_address'  :  G_lan_ip_addr,
		'local_netmask'        :  G_lan_mask,
		'lccal_dhcp_enable'  :   G_lan_dhcp_enable == '1' ? data_language.dynamic.enalbe : data_language.dynamic.disable,
		'system_info_HardwareVersion'     : G_HarewareVersion,
		'system_info_SoftwareVersion' : G_SoftwareVersion
	});

}

/**************************************************************************************************/


/**************************     wireless process   **************************************************/

function select_wireless()
{
	var BeaconType = '';
	var i = Form.Select('select_wireless');
	if (i < 0) return;

	switch(G_Wlan_Info[i][3]){
		case 'None' : {
			BeaconType = "None";
			break;
		}
		case 'Basic' : {
			BeaconType = "WEP";
			break;
		}
		case 'WPAand11i' : {
			BeaconType = "Auto(WPA or WPA2)";
			break;
		}
		case '11i' : {
			BeaconType = "WPA2 only";
			break;
		}
		case 'WPA' : {
			BeaconType = "WPA only";
			break;
		}
	}	
	setJSONValue({
			'wireless_mac_address' : G_Wlan_Info[i][2],
			'wireless_status' : (G_Wlan_Info[i][5] == '1' && G_WirelessEnable == '1') ? data_language.dynamic.enalbe : data_language.dynamic.disable ,
			'wireless_ssid' : G_Wlan_Info[i][1],
			'wireless_visibility'  : G_Wlan_Info[i][6] == '1' ? data_language.dynamic.visable : data_language.dynamic.hide,
			'wireless_security': BeaconType
		});
}

function load_wireless_select()
{
	var _text = [], _value = [];
	for(var i = 0; i < G_Wlan_Info.length; i++){
		_text.push(G_Wlan_Info[i][1]);
		_value.push(i);
	}
	$S('select_wireless',_text,_value);
	$('select_wireless').value = '0';
	select_wireless();
}


/**************************************************************************************/

function uiOnload() 
{
	load_wan_select();

	load_adsl_list();

	<?if eq 1 $var:mod_ETHWAN
	`	
		load_eth_list();
	`?>
	
	<?if eq 1 $var:mod_NEW3G
	`	
		load_3g_list();
	`?>

	load_LineRate();
	
	load_wireless_select();
	load_other();
	dealWithError();
	var tr = document.getElementsByClassName('SoftwareVersion_info');
	tr[0].style.display = 'none';
}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload);
 
