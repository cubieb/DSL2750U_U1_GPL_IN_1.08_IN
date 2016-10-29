/* IPv6 Interface Address */
var G_IPv6InterfaceAddress = "<?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.X_TWSZ-COM_IPv6InterfaceAddress?>";

//IPv6 DHCP
<?mget :InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement. "AutoConfigurationAddress RADVD.Enabled RadVDConfigManagement.ULAPrefixInfo.Prefix IPv6SitePrefixConfigType.StaticDelegated ServerType.Stateless ServerType.StatefullDHCPv6.MinInterfaceID ServerType.StatefullDHCPv6.MaxInterfaceID ServerType.StatefullDHCPv6.DHCPv6LeaseTime ServerType.StatefullDHCPv6.IPv6DNSConfigType ServerType.StatefullDHCPv6.IPv6DNSServers"
`	var G_DHCPV6            = "$01"; //AutoConfigurationAddress
	var G_RADVD             = "$02"; //RADVD.Enabled
	var G_ULAPrefix         = "$03"; //RadVDConfigManagement.ULAPrefixInfo.Prefix
	var G_IPv6SiteType      = "$04"; //IPv6SitePrefixConfigType.StaticDelegated
	var G_LanAddrDistribute = "$05"; //ServerType.Stateless
	var G_MinInterfaceID    = "$06"; //ServerType.StatefullDHCPv6.MinInterfaceID
	var G_MaxInterfaceID    = "$07"; //ServerType.StatefullDHCPv6.MaxInterfaceID
	var G_Dhcpv6LeaseTime   = "$08"; //ServerType.StatefullDHCPv6.DHCPv6LeaseTime
	var G_IPv6DNSType       = "$09"; //ServerType.StatefullDHCPv6.IPv6DNSConfigType
	var G_IPv6DNSServers    = "$0a"; //ServerType.StatefullDHCPv6.IPv6DNSServers
`?>

//uiOnload
function uiOnload() {
	setJSONValue({
		'INPUT_InterfaceAddress'      : G_IPv6InterfaceAddress,
		'INPUT_EnableDhcpv6'          : G_DHCPV6 || '0',
		'INPUT_EnableRadvd'           : G_RADVD || '0',
		'INPUT_SitePrefix'            : G_ULAPrefix,
		'Enable'                      : G_IPv6SiteType || '0',
		'RADIO_LanAddrDistributeType' : G_LanAddrDistribute || '0',
		'INPUT_MinInterfaceID'        : G_MinInterfaceID,
		'INPUT_MaxInterfaceID'        : G_MaxInterfaceID,
		'INPUT_Dhcpv6LeaseTime'       : G_Dhcpv6LeaseTime,
		'IPv6_DNS_Mode'               : G_IPv6DNSType || 'DHCP',
		'INPUT_IPv6DNSServers'        : G_IPv6DNSServers
	});

	onDistributeTypeChg();
	onDNSModeChg();
	setEnable();
}

function onDistributeTypeChg()
{
	var ipv6_address_type = Form.Radio('RADIO_LanAddrDistributeType') == '1' ? false : true;

	disCtrl('Dhcpv6Pool', ipv6_address_type);
}

function onDNSModeChg()
{
	var ipv6_dns_mode = Form.Radio('IPv6_DNS_Mode') == 'DHCP' ? false : true;
	disCtrl('StaticDNS', ipv6_dns_mode);
}

function setEnable() {
	var ipv6_enable = Form.Radio('Enable') == '0' ? false : true;
	
	disCtrl('Static', ipv6_enable);
}

function isValidLLAddress(address) {

    var num, result;
    var addrParts;
    var ipParts = address.split('/');
    if (ipParts.length > 2) return false;
    if (ipParts.length == 2) {
        num = parseInt(ipParts[1]);
        if (num <= 0 || num > 128)
            return false;
    }

    addrParts = ipParts[0].split(':');
    if (addrParts.length < 3 || addrParts.length > 8)
        return false;
    if (addrParts[0] == '')
        return false;
    else
    {
        num = parseInt(addrParts[0], 16);
        
        /* LLA fe80::/10 */
        if ( (num & 0xffb0) != 0xfe80 )
            return false;    
    }
   return true;
}
function uiSubmit() {
	if ($('INPUT_SitePrefix').value != '') {
		if ($('INPUT_SitePrefix').value.indexOf('/') < 0) {
			alert("prefix error, e.g. 2001:1234::/64.");
			return false;
		}
	}

	/* Ö»ÄÜÊÇLLA */
	//if ($('INPUT_InterfaceAddress').value.indexOf('ff') == 0)
	if (!isValidLLAddress($('INPUT_InterfaceAddress').value))
	{
		alert($('INPUT_InterfaceAddress').value + "is not a valid link local address.");
		return false;
	}
	
	$H({
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.X_TWSZ-COM_IPv6InterfaceAddress'                         : $('INPUT_InterfaceAddress').value,
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement.AutoConfigurationAddress'                          : Form.Checkbox('INPUT_EnableDhcpv6'),
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement.RADVD.Enabled'                                     : Form.Checkbox('INPUT_EnableRadvd'),
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement.IPv6SitePrefixConfigType.StaticDelegated'          : Form.Radio('Enable'),
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement.RadVDConfigManagement.ULAPrefixInfo.Prefix'        : Form.Radio('Enable') != "1" ? undefined : $('INPUT_SitePrefix').value,
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement.ServerType.Stateless'                              : Form.Radio('RADIO_LanAddrDistributeType'),
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement.ServerType.StatefullDHCPv6.MinInterfaceID'         : Form.Radio('RADIO_LanAddrDistributeType') == "1" ? undefined : $('INPUT_MinInterfaceID').value,
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement.ServerType.StatefullDHCPv6.MaxInterfaceID'         : Form.Radio('RADIO_LanAddrDistributeType') == "1" ? undefined : $('INPUT_MaxInterfaceID').value,
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement.ServerType.StatefullDHCPv6.DHCPv6LeaseTime'        : Form.Radio('RADIO_LanAddrDistributeType') == "1" ? undefined : $('INPUT_Dhcpv6LeaseTime').value,
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement.ServerType.StatefullDHCPv6.IPv6DNSConfigType'      : Form.Radio('IPv6_DNS_Mode'),
		':InternetGatewayDevice.LANDevice.1.X_TELEFONICA-ES_IPv6LANHostConfigManagement.ServerType.StatefullDHCPv6.IPv6DNSServers'         : Form.Radio('IPv6_DNS_Mode') == "DHCP" ? undefined : $('INPUT_IPv6DNSServers').value,
		
		'obj-action'   :'set',
		'var:menu'     :'setup',
		'var:page'     :'ipv6_lan',
		'var:errorpage':'ipv6_lan',
		'getpage'      :'html/index.html',
		'errorpage'    :'html/index.html',
		'var:CacheLastData': ViewState.Save()
	},true);
	
	$('uiPostForm').submit();
}

function dealWithError() {
	if (G_Error != 1)
		return false;
	
	var arrayHint = [];
	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);