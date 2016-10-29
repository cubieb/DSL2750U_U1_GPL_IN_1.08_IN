//WAN连接信息
var G_MaxBitRate     = "";
var G_DuplexMode     = "";
var G_Bitrate_Duplex = G_MaxBitRate == 'Auto' ? 'Auto' : (G_MaxBitRate + 'Mbps ' + G_DuplexMode);
var G_wanConnction = [];
var m = 0;


<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. ""
` 	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$10.WANIPConnection. "Name AddressingType ExternalIPAddress SubnetMask DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers ConnectionStatus LastConnectionError DefaultGateway ConnectionType X_TELEFONICA-ES_IPv6Enabled"
	`	<?if eq $1c `1`
		`	G_wanConnction[m] = [];
			G_wanConnction[m][0] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $30?>.WANIPConnection.<?echo $20?>.";
			G_wanConnction[m][1] = "<?echo $21?>"; //Name
			G_wanConnction[m][2] = "<?echo $22?>"; //AddressingType
			G_wanConnction[m][3] = G_Bitrate_Duplex;
			G_wanConnction[m][4] = '<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$30.WANIPConnection.$20.X_TWSZ-COM_IPv6Config.IPv6AddressList. "IPv6Address PrefixLength" `$01/$02,`?>'; //IPv6Address/PrefixLength
			G_wanConnction[m][5] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANIPConnection.$20.X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed?>" == "0"?"<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DNSServers?>":"<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.UsrIPv6DNSServers?>";//DDNS
			G_wanConnction[m][6] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.ConnectionStatus?>"; //
			G_wanConnction[m][7] = ""; //LastConnectionError
			G_wanConnction[m][8] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.1.DefaultRouter?>";
			
			G_wanConnction[m][10] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.ConnectionType?>";//ConnectionType
			G_wanConnction[m][11] = "IPv6";
			G_wanConnction[m][12] = "<?echo $2d?>".replace('_','&'); 
			G_wanConnction[m][13] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6SitePrefixInfo.IPv6SitePrefix?>/<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6SitePrefixInfo.IPv6SitePrefixLength?>";
			G_wanConnction[m][14] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.2.DefaultRouter?>"; //RemoteIPAddress
			G_wanConnction[m][15] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.1.ConfigType?>";

			m++;
		`?>
	`?>

	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$10.WANPPPConnection. "Name ExternalIPAddress DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers ConnectionStatus LastConnectionError RemoteIPAddress ConnectionType X_TELEFONICA-ES_IPv6Enabled"
	`	<?if eq $1a `1`
		`	G_wanConnction[m] = [];
			G_wanConnction[m][0] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $30?>.WANPPPConnection.<?echo $20?>.";
			G_wanConnction[m][1] = "<?echo $21?>";
			G_wanConnction[m][2] = "PPPoEv6";
			G_wanConnction[m][3] = G_Bitrate_Duplex;
			G_wanConnction[m][4] = '<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$30.WANPPPConnection.$20.X_TWSZ-COM_IPv6Config.IPv6AddressList. "IPv6Address PrefixLength" `$01/$02,`?>'; //IPv6Address/PrefixLength
			G_wanConnction[m][5] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed?>" == "0"?"<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DNSServers?>":"<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.UsrIPv6DNSServers?>";//DDNS
			G_wanConnction[m][6] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.ConnectionStatus?>"; //
			G_wanConnction[m][7] = ""; //LastConnectionError
			G_wanConnction[m][8] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.1.DefaultRouter?>";
			
			G_wanConnction[m][10] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.ConnectionType?>";//ConnectionType
			G_wanConnction[m][11] = "IPv6";
			G_wanConnction[m][12] = "<?echo $2b?>".replace('_','&'); //unique
			/*G_wanConnction[m][13] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6SitePrefixInfo.IPv6SitePrefix?>/<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6SitePrefixInfo.IPv6SitePrefixLength?>";*/
			G_wanConnction[m][13] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TELEFONICA-ES_IPv6SitePrefix?>";
			G_wanConnction[m][14] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.2.DefaultRouter?>"; //RemoteIPAddress
 			G_wanConnction[m][15] = "<?get :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.1.ConfigType?>";

			m++;
		`?>
	`?>
`?>
<?if eq 1 $var:mod_ETHWAN
`
	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. ""
	` 	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$10.WANIPConnection. "Name AddressingType ExternalIPAddress SubnetMask DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers ConnectionStatus LastConnectionError DefaultGateway ConnectionType X_TELEFONICA-ES_IPv6Enabled"
		`	<?if eq $1c `1`
			`	G_wanConnction[m] = [];
				G_wanConnction[m][0] = "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $30?>.WANIPConnection.<?echo $20?>.";
				G_wanConnction[m][1] = "<?echo $21?>"; //Name
				G_wanConnction[m][2] = "<?echo $22?>"; //AddressingType
				G_wanConnction[m][3] = G_Bitrate_Duplex;
				G_wanConnction[m][4] = '<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$30.WANIPConnection.$20.X_TWSZ-COM_IPv6Config.IPv6AddressList. "IPv6Address PrefixLength" `$01/$02,`?>'; //IPv6Address/PrefixLength
				G_wanConnction[m][5] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANIPConnection.$20.X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed?>" == "0"?"<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DNSServers?>":"<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.UsrIPv6DNSServers?>";//DDNS
				G_wanConnction[m][6] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.ConnectionStatus?>"; //
				G_wanConnction[m][7] = ""; //LastConnectionError
				G_wanConnction[m][8] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.1.DefaultRouter?>";
				
				G_wanConnction[m][10] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.ConnectionType?>";//ConnectionType
				G_wanConnction[m][11] = "IPv6";
				G_wanConnction[m][12] = "<?echo $2d?>".replace('_','&'); 
				G_wanConnction[m][13] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6SitePrefixInfo.IPv6SitePrefix?>/<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6SitePrefixInfo.IPv6SitePrefixLength?>";
				G_wanConnction[m][14] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.2.DefaultRouter?>"; //RemoteIPAddress
				G_wanConnction[m][15] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANIPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.1.ConfigType?>";

				m++;
			`?>
		`?>

		<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$10.WANPPPConnection. "Name ExternalIPAddress DNSOverrideAllowed DNSServers X_TWSZ-COM_UsrDNSServers ConnectionStatus LastConnectionError RemoteIPAddress ConnectionType X_TELEFONICA-ES_IPv6Enabled"
		`	<?if eq $1a `1`
			`	G_wanConnction[m] = [];
				G_wanConnction[m][0] = "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $30?>.WANPPPConnection.<?echo $20?>.";
				G_wanConnction[m][1] = "<?echo $21?>";
				G_wanConnction[m][2] = "PPPoEv6";
				G_wanConnction[m][3] = G_Bitrate_Duplex;
				G_wanConnction[m][4] = '<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$30.WANPPPConnection.$20.X_TWSZ-COM_IPv6Config.IPv6AddressList. "IPv6Address PrefixLength" `$01/$02,`?>'; //IPv6Address/PrefixLength
				G_wanConnction[m][5] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed?>" == "0"?"<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DNSServers?>":"<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.UsrIPv6DNSServers?>";//DDNS
				G_wanConnction[m][6] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.ConnectionStatus?>"; //
				G_wanConnction[m][7] = ""; //LastConnectionError
				G_wanConnction[m][8] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.1.DefaultRouter?>";
				
				G_wanConnction[m][10] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.ConnectionType?>";//ConnectionType
				G_wanConnction[m][11] = "IPv6";
				G_wanConnction[m][12] = "<?echo $2b?>".replace('_','&'); //unique
				/*G_wanConnction[m][13] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6SitePrefixInfo.IPv6SitePrefix?>/<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6SitePrefixInfo.IPv6SitePrefixLength?>";*/
				G_wanConnction[m][13] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TELEFONICA-ES_IPv6SitePrefix?>";
				G_wanConnction[m][14] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.2.DefaultRouter?>"; //RemoteIPAddress
	 			G_wanConnction[m][15] = "<?get :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.<?echo $40?>.WANPPPConnection.<?echo $30?>.X_TWSZ-COM_IPv6Config.IPv6DefaultRouterList.1.ConfigType?>";

				m++;
			`?>
		`?>
	`?>
`?>

function createWanConnsOpt(){
	var _text = [], _value = [];
	var _wanidx;

	for(var i = 0, _len = G_wanConnction.length; i < _len; i++){
		_text[i]  = G_wanConnction[i][1];
		_value[i] = G_wanConnction[i][0];
	}

	$S('SELECT_WanConns', _text, _value);

	if((_wanidx = Cookie.Get('WanIdx')) != null && G_wanConnction[_wanidx]){
		$('SELECT_WanConns').value = G_wanConnction[_wanidx][0];
		_w_i = _wanidx;
	}
}
		
var _w_i = 0;
function Wan_Port_Info_Table_Create(){
	var DefaultGw ;
	var AddressingType;
	if (m > 0){
	    if(G_wanConnction[_w_i][15] == "Static")
	    {
            DefaultGw = G_wanConnction[_w_i][8];
	    }
	    else
	    {
             if(G_wanConnction[_w_i][14] != "")
		    {
                 DefaultGw = G_wanConnction[_w_i][14];
		    }
		    else
		    {
                 DefaultGw = G_wanConnction[_w_i][8];
		    }
	    }
		
		if(G_wanConnction[_w_i] && G_wanConnction[_w_i][2] == 'PPPoEv6')
		{
			AddressingType = "PPPoEv6";
		}
		else if(G_wanConnction[_w_i] && G_wanConnction[_w_i][2] == 'DHCP')
		{
			AddressingType = "DHCPv6";
		}
		else if(G_wanConnction[_w_i] && G_wanConnction[_w_i][2] == 'Static')
		{
			AddressingType = "Static";
		}
		else
		{
			AddressingType = "Bridged";
		}
		setJSONValue({
			'td_type'                : G_wanConnction[_w_i] && G_wanConnction[_w_i][2] == 'PPPoEv6' && G_wanConnction[_w_i][10] == 'PPPoE_Bridged' ? 'Bridged' : AddressingType || '&nbsp;',
			'td_ipAddress'           : G_wanConnction[_w_i] && (CheckNeedShow(_w_i) ? splitIpv6Addr(G_wanConnction[_w_i][4]) : '') || '&nbsp;',
			'td_gateway'             : G_wanConnction[_w_i] && (CheckNeedShow(_w_i) ? DefaultGw : '') || '&nbsp;',
			'td_dns_1'               : G_wanConnction[_w_i] && (CheckNeedShow(_w_i) ? (G_wanConnction[_w_i][5].split(',')[0] || '') : '') || '&nbsp;',
			'td_dns_2'               : G_wanConnction[_w_i] && (CheckNeedShow(_w_i) ? (G_wanConnction[_w_i][5].split(',')[1] || '') : '') || '&nbsp;',
			'td_prefixinfo'          : G_wanConnction[_w_i] && (CheckNeedShow(_w_i) ? (G_wanConnction[_w_i][13] == '/' ? '' : G_wanConnction[_w_i][13]) : '') || '&nbsp;',
			'td_status'              : G_wanConnction[_w_i] && GetConnStatus(_w_i) || '&nbsp;'			

		});	
	}
}

function onchgWanConns(_this){
	for(var i = 0, _len = G_wanConnction.length; i < _len; i++){
		if(G_wanConnction[i][0] == _this.value){
			_w_i = i;
			Cookie.Set('WanIdx',i);
			break;
		}
	}

	Wan_Port_Info_Table_Create();
}
		
function uiOnload()
{	
	createWanConnsOpt();
	Wan_Port_Info_Table_Create();
}

function UserLimit(){
	$('wan_pvc').style.display = 'none';
}

function CheckNeedShow(index) {
	if (G_wanConnction[index][10] == 'PPPoE_Bridged' || G_wanConnction[index][10] == 'IP_Bridged') {
		return false;
	}
	if (G_wanConnction[index][6].indexOf('Connected') < 0 && G_wanConnction[index][2] != 'Static') {
		return false;
	}
	return true;
}

function GetConnStatus(index) {
	if (G_wanConnction[index][10] == 'PPPoE_Bridged' || G_wanConnction[index][10] == 'IP_Bridged') {
		return G_wanConnction[index][9];
	}
	if (G_wanConnction[index][6].indexOf('Connected') < 0 && G_wanConnction[index][2] == 'PPPoEv6' && G_wanConnction[index][7] != 'ERROR_NONE')	{
		return G_wanConnction[index][7];
	}
	return G_wanConnction[index][6];
}

function splitIpv6Addr(_addr){
	var s_addr = _addr.split(',');
	var n_addr = '';
	
	for(var i = 0, _len = s_addr.length; i < _len; i++){
		if(s_addr[i] == '/' || s_addr[i] == ''){
			continue;
		}
		
		n_addr += s_addr[i] + ',';
	}
	
	return n_addr.delcomma();
}

addListeners(uiOnload);
