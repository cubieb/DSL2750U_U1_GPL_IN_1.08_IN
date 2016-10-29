var G_WANAccessType = "<?get :InternetGatewayDevice.WANDevice.1.WANCommonInterfaceConfig.WANAccessType?>";

//proxy arp
var en_proxyarp = "<?get :InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_ProxyArp?>";

//lan clients --

var LanHosts = [];
var m = 0;
var checkboxched=0;
var drcheckboxched=0;
var cdopcheckboxched=0;
<?objget :InternetGatewayDevice.LANDevice. "Hosts.HostNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.LANDevice.$20.Hosts.Host. "HostName  MACAddress IPAddress LeaseTimeRemaining VendorClassID AddressSource"
             `	<?if eq `DHCP` `<?echo $26?>` 	
			`	LanHosts[m] = [];
				LanHosts[m][0] = "<?echo $21?>";
				LanHosts[m][1] = "<?echo $22?>";
				LanHosts[m][2] = "<?echo $23?>";
				LanHosts[m][3] = "<?echo $24?>";
				//LanHosts[m][4] = "<?echo $25?>";
				++m;
			`?>
		`?>
	`?>
`?>

//LAN
<?setvaronce var:IPIntfNum <?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterfaceNumberOfEntries?>?>
<? if lt 0 `<?echo $var:IPIntfNum?>`
`	<?mget :InternetGatewayDevice.LANDevice.1. "LANHostConfigManagement.IPInterface.1.Enable LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask"
	`	var G_EbAddress  = "$01"; 
		var G_IPAddress  = "$02";
		var G_SubAddress = "$03";
	`?>
`
`		var G_EbAddress  = ""; 
		var G_IPAddress  = "";
		var G_SubAddress = "";
`?>
<?mget :InternetGatewayDevice.LANDevice.1. "LANHostConfigManagement.DomainName LANHostConfigManagement.DHCPServerEnable LANHostConfigManagement.MinAddress LANHostConfigManagement.MaxAddress LANHostConfigManagement.DHCPLeaseTime LANHostConfigManagement.DHCPRelay LANHostConfigManagement.X_TWSZ-COM_DHCPRelayAddress LANHostConfigManagement.X_TWSZ-COM_UseIPRoutersAsDNSServer LANHostConfigManagement.DNSServers LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable LANHostConfigManagement.SubnetMask LANHostConfigManagement.IPRouters"
`	var G_DomainName       = "$01";
	var G_DHCPServerEnable = "$02";
	var G_MinAddress       = "$03";
	var G_MaxAddress       = "$04";
	var G_DHCPLeaseTime    = "$05";
	//RELAY
	var G_DHCPRelayEnable = "$06";
	var G_DHCPRelayIP     = "$07";

	//LAN DNS
	var G_LANDNSEnalbe  = "$08";
	var G_LANDNSAddress = "$09";
	var G_DNSRelayEnable = "$0a";

        var G_DHCPMask = "$0b";
        var G_DHCPRouterIP = "$0c";
`?>

var G_Addlanclient = new Array();
var n = 0;
<?if eq $var:mod_ClubWifi 1
`	<?objget :InternetGatewayDevice.LANDevice. "LANHostConfigManagement.IPRouters LANHostConfigManagement.SubnetMask LANHostConfigManagement.DomainName LANHostConfigManagement.MinAddress LANHostConfigManagement.MaxAddress LANHostConfigManagement.DHCPLeaseTime"
`    G_Addlanclient[n] = ["$00",
                          "$01", //IPRouters
                          "$02", //SubnetMask
                          "$03", //DomainName
                          "$04", //MinAddress
                          "$05", //MaxAddress
                          "$06"  //DHCPLeaseTime,
                          ];
     n++;
`?>
`?>

var G_NextAddress = new Array();
var x = 0;

<?setvaronce var:IPIntfNum <?get :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterfaceNumberOfEntries?>?>
<? if lt 1 `<?echo $var:IPIntfNum?>`
` <?objget :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface. "Enable IPInterfaceIPAddress IPInterfaceSubnetMask "
`       G_NextAddress[x] = ["$00",
                            "$01",
                            "$02",
                            "$03"
                            ];
        x++;
`?>
`
`		G_NextAddress[x] = ["",
                            "",
                            "",
                            ""
                            ];
        x++;
`?>	
//DHCP Client Class
<?setvaronce var:objindex $var:newobjindex?>
var pathIndex = '<?echo $var:pathIndex?>' == '-' ? 0 : '<?echo $var:pathIndex?>';
	
var G_DHCP_LanPortEnabe = [];
var m = 0;
<?objget :InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig. "X_TWSZ-COM_Description DhcpServerEnable" 
`       G_DHCP_LanPortEnabe[m] = [];
		G_DHCP_LanPortEnabe[m][0] = "$01"; 
        G_DHCP_LanPortEnabe[m][1] = "$02"; 
        G_DHCP_LanPortEnabe[m][2] = "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.$00."; 
		m++;
`?>

var G_LanToIndex = [];


var G_DHCP_WlanPortEnabe = [];
var m = 0;
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "X_TWSZ-COM_DHCPEnabled"
`       G_DHCP_WlanPortEnabe[m] = [];
		G_DHCP_WlanPortEnabe[m][0] = "$01"; 
		m++;
`?>
	
var G_DHCP_ClientClass = [];
var G_COND_DHCP_Option = [];
var m = 0;
var n = 0;
<?objget :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPConditionalServingPool. "VendorClassID MinAddress MaxAddress DNSServers PoolOrder"
`       G_DHCP_ClientClass[m] = [];
		G_DHCP_ClientClass[m][0] = "$01";  //filterip
		G_DHCP_ClientClass[m][1] = "$02";   // filterMAC
		G_DHCP_ClientClass[m][2] = "$03";   // filtersta
		G_DHCP_ClientClass[m][3] = "$04";   // filtersta
		G_DHCP_ClientClass[m][4] = "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPConditionalServingPool.$00." ;
        G_DHCP_ClientClass[m][5] = "$05";
		<?if gt $15 0
	    `	<?objget :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPConditionalServingPool.$20.DHCPOption. "Enable Tag Value"
			`       G_COND_DHCP_Option[n] = [];		
					G_COND_DHCP_Option[n][0] = "$01";
					G_COND_DHCP_Option[n][1] = "$02";
					G_COND_DHCP_Option[n][2] = "$03";
					G_COND_DHCP_Option[n][3] = "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPConditionalServingPool.$20.DHCPOption." ;
					G_COND_DHCP_Option[n][4] = "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPConditionalServingPool.$20.DHCPOption.$00." ;
					G_COND_DHCP_Option[n][5] = G_DHCP_ClientClass[m][0]; //client class
		       n++;
	 		`?>
	    `?>
		m++;
`?>
<?if eq $var:mod_ClubWifi 1
`	var landevicenum = "<?get :InternetGatewayDevice.LANDeviceNumberOfEntries?>";
`?>
var G_DHCP_WhiteList = [];
var m = 0;
<?objget :InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DHCPFilter.WhiteList. "FilterIP FilterMAC PerEnabled FilterPCName"
`       G_DHCP_WhiteList[m] = [];
		G_DHCP_WhiteList[m][0] = "$01";  //filterip
		G_DHCP_WhiteList[m][1] = "$02";   // filterMAC
		G_DHCP_WhiteList[m][2] = "$03";   // filtersta
		G_DHCP_WhiteList[m][3] = "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DHCPFilter.WhiteList.$00." ;
		G_DHCP_WhiteList[m][4] = "$04";
		m++;
`?>



function jsonFstMap(){
	return {
		'INPUT_DomainName'    :G_DomainName,
		'INPUT_MinAddress'    :G_MinAddress,
		'INPUT_MaxAddress'    :G_MaxAddress,
		'INPUT_DHCPLeaseTime' :G_DHCPLeaseTime,
		'INPUT_DhcpMask' : G_DHCPMask,
		'INPUT_DhcpRouter' : G_DHCPRouterIP,
	    'INPUT_RelayIP'  :G_DHCPRelayIP 
	};
}

function ipchange()
{
	if('1' == G_DHCPServerEnable)
	{
		var len = $('INPUT_IPAddress').value.lastIndexOf(".");
		$('INPUT_MinAddress').value = $('INPUT_IPAddress').value.substr(0, len) + G_MinAddress.substr(G_MinAddress.lastIndexOf("."), G_MinAddress.length - G_MinAddress.lastIndexOf("."));
		$('INPUT_MaxAddress').value = $('INPUT_IPAddress').value.substr(0, len) + G_MaxAddress.substr(G_MaxAddress.lastIndexOf("."), G_MaxAddress.length - G_MaxAddress.lastIndexOf("."));
	}
}

function uiOnload(){

	if (en_proxyarp == '1')
		$('en_proxyarp').checked = true;
	else
		$('en_proxyarp').checked = false;

	if (G_DHCPServerEnable == '1'){
		$('INPUT_DHCPServerEnable').checked = true;
		$('INPUT_DHCPRelayEnable').checked = false;
	}
	if (G_DHCPRelayEnable == '1'){
		
		$('INPUT_DHCPServerEnable').checked = false;
		$('INPUT_DHCPRelayEnable').checked = true;
	}

	$('INPUT_IPAddress').value = G_IPAddress;
	$('INPUT_SubAddress').value = G_SubAddress;
	if((G_NextAddress[0][0] != "") && (G_NextAddress[1][1]=='1'))
	{
	    $('en_secipadd').checked = true;
		if ( G_DHCPRouterIP.indexOf(G_NextAddress[0][2]) != -1 )
		{
			$('INPUT_IPAddress2').value = G_NextAddress[1][2] ;
			$('INPUT_SubnetMask2').value = G_NextAddress[1][3];
		}else
		{
			$('INPUT_IPAddress2').value = G_NextAddress[0][2] ;
			$('INPUT_SubnetMask2').value = G_NextAddress[0][3];
			$('INPUT_IPAddress').value = G_NextAddress[1][2] ;
			$('INPUT_SubAddress').value = G_NextAddress[1][3];
		}
	}
	else
	{
		 $('en_secipadd').checked = false;
		$('INPUT_IPAddress2').disabled = true ;
		$('INPUT_SubnetMask2').disabled= true;
	}

	if(G_LANDNSEnalbe == '1')
	{   
	    $('INPUT_LANDNSEnable').checked = false;
	    $('INPUT_FSTDNS').disabled = true;
	    $('INPUT_SECDNS').disabled = true;
	    $('LANFSTDNSserver').style.color = '#AAAAAA';	
	    $('LANAltDNSserver').style.color = '#AAAAAA';	
	
	}
	else
	{
	    $('INPUT_LANDNSEnable').checked = true;
	    $('INPUT_FSTDNS').disabled = false;
	    $('INPUT_SECDNS').disabled = false; 
	    $('LANFSTDNSserver').style.color = '#000000';	
	    $('LANAltDNSserver').style.color = '#000000';		
	}

  if(G_DNSRelayEnable == '1')
  {
  	$('en_DNSRelayEnable').checked = true;
  }
  else
  {
    $('en_DNSRelayEnable').checked = false;
  }
  
	setJSONValue({
		'INPUT_FSTDNS' : G_LANDNSAddress.split(',')[0] || '',
		'INPUT_SECDNS' : G_LANDNSAddress.split(',')[1] || ''
	});
	var array_clientclasslist = [] ;
	for(var i = 0, _len = G_DHCP_ClientClass.length; i < _len; i++){
		 array_clientclasslist[i]= [
		 	            '<input type="checkbox" id="choose'+i+'" name="choose'+i+'">',
						G_DHCP_ClientClass[i][0], 
						G_DHCP_ClientClass[i][1], 
						G_DHCP_ClientClass[i][2],
						G_DHCP_ClientClass[i][3]
						];
	}
	$T('td_clientclasslist',array_clientclasslist);
	var array_conddhcpoptionlist = [];
	for(var i = 0, _len = G_COND_DHCP_Option.length; i<_len; i++){
		var conddhcpoptionenable = G_COND_DHCP_Option[i][0]=='1' ? data_language.dynamic.enable : data_language.dynamic.disable;
		array_conddhcpoptionlist[i] = [
								   '<input type="checkbox" id="choose4'+i+'" name="choose'+i+'">',
								   conddhcpoptionenable,
								   G_COND_DHCP_Option[i][5],
								   G_COND_DHCP_Option[i][1],
								   Base64.Decode(G_COND_DHCP_Option[i][2])
								   ];
		}
	$T('td_conddhcpoptionlist',array_conddhcpoptionlist);
								   
	var array_whitelist = [] ;
	for(var i = 0, _len = G_DHCP_WhiteList.length; i < _len; i++){

		var dhcpfilterenable = G_DHCP_WhiteList[i][2]=='1' ? data_language.dynamic.enable : data_language.dynamic.disable;
		 array_whitelist[i]= [
						'<input type="checkbox" id="choose2'+i+'" name="choose'+i+'">',
						dhcpfilterenable, //state
						G_DHCP_WhiteList[i][4], //pcname
						G_DHCP_WhiteList[i][1], //MAC
						G_DHCP_WhiteList[i][0]	  //IP			
						];
	}
	$T('td_reseverlist',array_whitelist);
	setJSONValue(jsonFstMap());
	//create table
	$('DHCP_Client').innerHTML = LanHosts.length;
    $T('td_lanClients',LanHosts);

	for(var i = 0, _len = G_DHCP_LanPortEnabe.length; i < _len; i++)
    {
        if (G_DHCP_LanPortEnabe[i][0] == "ethernet1")
        {
            G_LanToIndex[i] = [];
            G_LanToIndex[i][0] = G_DHCP_LanPortEnabe[i][2];
            G_LanToIndex[i][1] = 1;
            Form.Checkbox('INPUT_Lan1',G_DHCP_LanPortEnabe[i][1]);
        }
        if (G_DHCP_LanPortEnabe[i][0] == "ethernet2")
        {
            G_LanToIndex[i] = [];  
            G_LanToIndex[i][0] = G_DHCP_LanPortEnabe[i][2];
            G_LanToIndex[i][1] = 2;            
            Form.Checkbox('INPUT_Lan2',G_DHCP_LanPortEnabe[i][1]);        
        }
        if (G_DHCP_LanPortEnabe[i][0] == "ethernet3")
        {
            G_LanToIndex[i] = [];   
            G_LanToIndex[i][0] = G_DHCP_LanPortEnabe[i][2];
            G_LanToIndex[i][1] = 3;            
            Form.Checkbox('INPUT_Lan3',G_DHCP_LanPortEnabe[i][1]);
        }
        if (G_DHCP_LanPortEnabe[i][0] == "ethernet4")
        {
            G_LanToIndex[i]= [];
            G_LanToIndex[i][0] = G_DHCP_LanPortEnabe[i][2];
            G_LanToIndex[i][1] = 4;            
            Form.Checkbox('INPUT_Lan4',G_DHCP_LanPortEnabe[i][1]);        
        }
	}

<?if eq $var:mod_ClubWifi 1	
`	$('SecIPRouterRange').style.display = 'none';
	$('ThiIPRouterRange').style.display = 'none';
	$('ForIPRouterRange').style.display = 'none';
	$('FiveIPRouterRange').style.display = 'none';
	
	if(landevicenum > 1)
	{
	    $('SecIPRouterRange').style.display = 'block';
		$('INPUT_SecIPAddress').value = G_Addlanclient[1][1];
		$('INPUT_SecSubAddress').value = G_Addlanclient[1][2];
		$('INPUT_SecDomainName').value = G_Addlanclient[1][3];
		$('INPUT_SecMinAddress').value = G_Addlanclient[1][4];
		$('INPUT_SecMaxAddress').value = G_Addlanclient[1][5];
		$('INPUT_SecDHCPLeaseTime').value = G_Addlanclient[1][6];
		if(landevicenum > 2)
		{
		    $('ThiIPRouterRange').style.display = 'block';
			$('INPUT_ThiIPAddress').value = G_Addlanclient[2][1];
			$('INPUT_ThiSubAddress').value = G_Addlanclient[2][2];
			$('INPUT_ThiDomainName').value = G_Addlanclient[2][3];
			$('INPUT_ThiMinAddress').value = G_Addlanclient[2][4];
			$('INPUT_ThiMaxAddress').value = G_Addlanclient[2][5];
			$('INPUT_ThiDHCPLeaseTime').value = G_Addlanclient[2][6];
			if(landevicenum > 3)
			{
			    $('ForIPRouterRange').style.display = 'block';
				$('INPUT_ForIPAddress').value = G_Addlanclient[3][1];
				$('INPUT_ForSubAddress').value = G_Addlanclient[3][2];
				$('INPUT_ForDomainName').value = G_Addlanclient[3][3];
				$('INPUT_ForMinAddress').value = G_Addlanclient[3][4];
				$('INPUT_ForMaxAddress').value = G_Addlanclient[3][5];
				$('INPUT_ForDHCPLeaseTime').value = G_Addlanclient[3][6];
				if(landevicenum > 4)
				{
				    $('FiveIPRouterRange').style.display = 'block';
					$('INPUT_FiveIPAddress').value = G_Addlanclient[4][1];
					$('INPUT_FiveSubAddress').value = G_Addlanclient[4][2];
					$('INPUT_FiveDomainName').value = G_Addlanclient[4][3];
					$('INPUT_FiveMinAddress').value = G_Addlanclient[4][4];
					$('INPUT_FiveMaxAddress').value = G_Addlanclient[4][5];
					$('INPUT_FiveDHCPLeaseTime').value = G_Addlanclient[4][6];
				}
			}
		}
	}
`?>

	var array_wlanportenablelist = [] ;
	for(var i = 0, _len = G_DHCP_WlanPortEnabe.length; i < _len; i++){
		 array_wlanportenablelist[i]= G_DHCP_WlanPortEnabe[i][0];
	}
	Form.Checkbox('INPUT_Wlan1',array_wlanportenablelist[0]);
	Form.Checkbox('INPUT_Wlan2',array_wlanportenablelist[1]);
	Form.Checkbox('INPUT_Wlan3',array_wlanportenablelist[2]);
	Form.Checkbox('INPUT_Wlan4',array_wlanportenablelist[3]);			
	
	if(G_WANAccessType == 'Ethernet'){
		$('lan4').style.display = 'none';
		$('lang_Lan4').style.display = 'none';
	}	
	
	setEnable();
	setRelayEnable();
	//Form.Action();
	$("arp_agency").style.display = 'none';
	$("lang_dhcprelay").style.display = 'none';
	$("DHCP_relay").style.display = 'none';
	$("DNS_service").style.display = 'none';
	$("lang_landnssetting").style.display = 'none';
	$("DHCP_customer_info").style.display = 'none';
	$("DhcpClientClass").style.display = 'none';
	$("dhcpcoptlist").style.display = 'none';
	$("Dhcpcondoptionclass").style.display = 'none';
	$("Dhcpreservation").style.display = 'none';
	$("dhcp_nddc").style.display = 'none';
	
}

function setEnable(){
	
    var _text = $('INPUT_MinAddress','INPUT_MaxAddress','INPUT_DHCPLeaseTime','INPUT_DhcpMask', 'INPUT_DhcpRouter');
	var _node = $('INPUT_DHCPServerEnable','DhcpipaddrR','DhcpleaseT','todispaly','secdisplay', 'lang_DhcpMask','lang_DhcpRouter','DhcpMaskDisplay', 'DhcpRouterDisplay');
	var _dispvalue=[G_MinAddress ,G_MaxAddress,G_DHCPLeaseTime,G_DHCPMask,G_DHCPRouterIP];
	var valuechange = 0;
	if(!_node[0].checked){
		for(var i = 0; i < _text.length; i++){
			_text[i].value = " ";			
		}		
		_node[1].style.color = '#AAAAAA';	
		_node[2].style.color = '#AAAAAA';
		_node[3].style.color = '#AAAAAA';	
		_node[4].style.color = '#AAAAAA';		
		_node[5].style.color = '#AAAAAA';
		_node[6].style.color = '#AAAAAA';
		_node[7].style.color = '#AAAAAA';
		_node[8].style.color = '#AAAAAA';
		
		$('INPUT_MinAddress').disabled = true;
	    $('INPUT_MaxAddress').disabled = true;
	    $('INPUT_DHCPLeaseTime').disabled = true;	
        $('INPUT_DhcpMask').disabled = true;
        $('INPUT_DhcpRouter').disabled = true;
		
		$('INPUT_DHCPRelayEnable').disabled = false;		
	} else if ((_node[0].checked)){
	    if (G_DHCPServerEnable == '0')
    	{
            valuechange = 1;
		}

		
		for(var i = 0; i < _text.length; i++){
			_text[i].value = _dispvalue[i];
			_text[i].disabled = false;
		}
		_node[1].style.color = '#000000';
		_node[2].style.color = '#000000';
		_node[3].style.color = '#000000';
		_node[4].style.color = '#000000';
		_node[5].style.color = '#AAAAAA';
		_node[6].style.color = '#AAAAAA';
		_node[7].style.color = '#AAAAAA';
		_node[8].style.color = '#AAAAAA';
		$('INPUT_DhcpMask').disabled = true;
		$('INPUT_DhcpRouter').disabled = true;
		$('INPUT_DHCPRelayEnable').disabled = true;			
	} 
	
}

function setRelayEnable(){
	
    var _text = $('INPUT_RelayIP');
	var _node = $('INPUT_DHCPRelayEnable','lang_RealyIPaddr');
	var _dispvalue=G_DHCPRelayIP;
	if(!_node[0].checked){
		_text.value = " ";			
		_node[1].style.color = '#AAAAAA';
		$('INPUT_RelayIP').disabled = true;
		$('INPUT_DHCPServerEnable').disabled = false;
		
	} else if ((_node[0].checked) && !($('INPUT_DHCPServerEnable').checked)){
		_text.value = _dispvalue;
		_text.disabled = false;
		_node[1].style.color = '#000000';
		$('INPUT_DHCPServerEnable').disabled = true;
		
	} 
}

function setLANDNSEnable(){
	var nodes = $('INPUT_FSTDNS','INPUT_SECDNS');
	if($('INPUT_LANDNSEnable').checked )
	{
	   nodes[0].disabled = false;
	   nodes[1].disabled = false;	
	   $('LANFSTDNSserver').style.color = '#000000';
	   $('LANAltDNSserver').style.color = '#000000';
	}
	else
	{
	   nodes[0].disabled = true;
	   nodes[1].disabled = true;
	   $('LANFSTDNSserver').style.color = '#AAAAAA';
	   $('LANAltDNSserver').style.color = '#AAAAAA';
	}
}

function IPtoInt(ip){
	var a = ip.split('.');

	var b = parseInt(a[1],10) << 16;
	var c = parseInt(a[2],10) << 8;
	var d = parseInt(a[3],10); 
	
	return (b + c + d);	
}
//Obtain the subnet address,lbw
function getNetwork(ip, submask)
{
    var c = new Array(4);
    var a = ip.split('.');
    var b = submask.split('.');

    c[0] = a[0]&b[0];
    c[1] = a[1]&b[1];
    c[2] = a[2]&b[2];
    c[3] = a[3]&b[3];

    var d = c[0] + '.' + c[1] + '.' + c[2] + '.' + c[3];
    return d;
}


function uiSubmit(){
	var ValueArray = $('INPUT_IPAddress','INPUT_SubAddress','INPUT_MinAddress','INPUT_MaxAddress','INPUT_DHCPLeaseTime','btn','INPUT_IPAddress2','INPUT_SubnetMask2', 'INPUT_DomainName', 'INPUT_RelayIP');	

	var node_dns  = $('INPUT_FSTDNS','INPUT_SECDNS');
	var value_dns = node_dns[0].value.strip() + ',' + node_dns[1].value.strip();
	
	var INPUT_IPAdd_INT  = IPtoInt(ValueArray[0].value);
	var INPUT_MinAdd_INT = IPtoInt(ValueArray[2].value);
	var INPUT_MaxAdd_INT = IPtoInt(ValueArray[3].value);
	if((INPUT_MinAdd_INT <= INPUT_IPAdd_INT) && (INPUT_IPAdd_INT <= INPUT_MaxAdd_INT))
	{
		alert("The DHCP Router IP should not be in the DHCP IP Address Range");
		return false;
	}
	
	if($('INPUT_DHCPServerEnable').checked)
	{
        var ip_route_subnet,ip_dhcp_subnet1,ip_dhcp_subnet2;
	    ip_route_subnet = getNetwork(ValueArray[0].value,ValueArray[1].value);
        ip_dhcp_subnet1 = getNetwork( ValueArray[2].value,ValueArray[1].value);
        ip_dhcp_subnet2 = getNetwork( ValueArray[3].value,ValueArray[1].value);

		if(ip_route_subnet!=ip_dhcp_subnet1 || ip_route_subnet!=ip_dhcp_subnet2 )
		{
			alert(data_language.dynamic.Subnetmask);
			return false;
		}
	}
	<?if eq $var:mod_ClubWifi 1	
	`	if(landevicenum > 1)
		{
			var ValueArray1 = $('INPUT_SecIPAddress','INPUT_SecSubAddress','INPUT_SecMinAddress','INPUT_SecMaxAddress');
			var ip_first_subnet = getNetwork(ValueArray[0].value,ValueArray[1].value);
			var ip_sec_subnet,ip_sec_subnet1,ip_sec_subnet2;
			
			var INPUT_SecIPAdd_INTT  = IPtoInt(ValueArray1[0].value);
			var INPUT_SecMinAdd_INTT = IPtoInt(ValueArray1[2].value);
			var INPUT_SecMaxAdd_INTT = IPtoInt(ValueArray1[3].value);
			if((INPUT_SecMinAdd_INTT <= INPUT_SecIPAdd_INTT) && (INPUT_SecIPAdd_INTT <= INPUT_SecMaxAdd_INTT))
			{
				 alert("The DHCP Router IP should not be in the DHCP IP Address Range");
		         return false;
			}
			
			if($('INPUT_DHCPServerEnable').checked)
			{
			    ip_sec_subnet = getNetwork(ValueArray1[0].value,ValueArray1[1].value);
		        ip_sec_subnet1 = getNetwork( ValueArray1[2].value,ValueArray1[1].value);
		        ip_sec_subnet2 = getNetwork( ValueArray1[3].value,ValueArray1[1].value);

				if(ip_sec_subnet != ip_sec_subnet1 || ip_sec_subnet != ip_sec_subnet2 )
				{
					alert("DHCP IP Address must be at the same subnet as DHCP Router IP");
					return false;
				}
				if(ip_sec_subnet == ip_first_subnet)
				{
				    alert("DHCP IP segment must be not at the same!");
					return false;
				}
			}

			$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.IPRouters',$('INPUT_SecIPAddress').value);
			$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress',$('INPUT_SecIPAddress').value);
			$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.SubnetMask',$('INPUT_SecSubAddress').value);
			$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask',$('INPUT_SecSubAddress').value);
			$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.DomainName',$('INPUT_SecDomainName').value);
			$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.MinAddress',$('INPUT_SecMinAddress').value);
			$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.MaxAddress',$('INPUT_SecMaxAddress').value);
			$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.DHCPLeaseTime',$('INPUT_SecDHCPLeaseTime').value);
			$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.DHCPServerEnable',Form.Checkbox('INPUT_DHCPServerEnable'));
			if($('en_DNSRelayEnable').checked == true)
			{
			    $F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.DNSServers',$('INPUT_SecIPAddress').value);
			}
			if(landevicenum > 2)
			{
				var ValueArray2 = $('INPUT_ThiIPAddress','INPUT_ThiSubAddress','INPUT_ThiMinAddress','INPUT_ThiMaxAddress');
				var ip_thi_subnet,ip_thi_subnet1,ip_thi_subnet2;
				
				var INPUT_ThiIPAdd_INT  = IPtoInt(ValueArray2[0].value);
				var INPUT_ThiMinAdd_INT = IPtoInt(ValueArray2[2].value);
				var INPUT_ThiMaxAdd_INT = IPtoInt(ValueArray2[3].value);
				if((INPUT_ThiMinAdd_INT <= INPUT_ThiIPAdd_INT) && (INPUT_ThiIPAdd_INT <= INPUT_ThiMaxAdd_INT))
				{
					alert("DHCP IP Address Range can not include Router IP Address");
					return false;
				}
				
				if($('INPUT_DHCPServerEnable').checked)
				{
				    ip_thi_subnet = getNetwork(ValueArray2[0].value,ValueArray2[1].value);
			        ip_thi_subnet1 = getNetwork( ValueArray2[2].value,ValueArray2[1].value);
			        ip_thi_subnet2 = getNetwork( ValueArray2[3].value,ValueArray2[1].value);
					if(ip_thi_subnet != ip_thi_subnet1 || ip_thi_subnet != ip_thi_subnet2)
					{
						alert("DHCP IP Address must be at the same subnet as DHCP Router IP");
						return false;
					}
					if((ip_thi_subnet == ip_first_subnet) || (ip_thi_subnet == ip_sec_subnet))
					{
					    alert("DHCP IP segment must be not at the same!");
						return false;
					}
				}
				$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[2][0] + '.LANHostConfigManagement.IPRouters',$('INPUT_ThiIPAddress').value);
				$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[2][0] + '.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress',$('INPUT_ThiIPAddress').value);
				$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[2][0] + '.LANHostConfigManagement.SubnetMask',$('INPUT_ThiSubAddress').value);
				$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[2][0] + '.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask',$('INPUT_ThiSubAddress').value);
				$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[2][0] + '.LANHostConfigManagement.DomainName',$('INPUT_ThiDomainName').value);
				$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[2][0] + '.LANHostConfigManagement.MinAddress',$('INPUT_ThiMinAddress').value);
				$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[2][0] + '.LANHostConfigManagement.MaxAddress',$('INPUT_ThiMaxAddress').value);
				$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[2][0] + '.LANHostConfigManagement.DHCPLeaseTime',$('INPUT_ThiDHCPLeaseTime').value);
				$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[2][0] + '.LANHostConfigManagement.DHCPServerEnable',Form.Checkbox('INPUT_DHCPServerEnable'));
				if($('en_DNSRelayEnable').checked == true)
				{
				    $F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.DNSServers',$('INPUT_ThiIPAddress').value);
				}
				if(landevicenum > 3)
				{
					var ValueArray3 = $('INPUT_ForIPAddress','INPUT_ForSubAddress','INPUT_ForMinAddress','INPUT_ForMaxAddress');
					var ip_for_subnet,ip_for_subnet1,ip_for_subnet2;
					
					var INPUT_ForIPAdd_INT  = IPtoInt(ValueArray3[0].value);
					var INPUT_ForMinAdd_INT = IPtoInt(ValueArray3[2].value);
					var INPUT_ForMaxAdd_INT = IPtoInt(ValueArray3[3].value);
					if((INPUT_ForMinAdd_INT <= INPUT_ForIPAdd_INT) && (INPUT_ForIPAdd_INT <= INPUT_ForMaxAdd_INT))
					{
						alert("The DHCP Router IP should not be in the DHCP IP Address Range");
						return false;
					}
					
					if($('INPUT_DHCPServerEnable').checked)
					{
					    ip_for_subnet = getNetwork(ValueArray3[0].value,ValueArray3[1].value);
				        ip_for_subnet1 = getNetwork( ValueArray3[2].value,ValueArray3[1].value);
				        ip_for_subnet2 = getNetwork( ValueArray3[3].value,ValueArray3[1].value);
						if(ip_for_subnet != ip_for_subnet1 || ip_for_subnet != ip_for_subnet2)
						{
							alert("DHCP IP Address must be at the same subnet as DHCP Router IP");
							return false;
						}
						if((ip_for_subnet == ip_first_subnet) || (ip_for_subnet == ip_sec_subnet) || (ip_for_subnet == ip_thi_subnet))
						{
						    alert("DHCP IP segment must be not at the same!");
							return false;
						}
					}
					$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[3][0] + '.LANHostConfigManagement.IPRouters',$('INPUT_ForIPAddress').value);
					$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[3][0] + '.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress',$('INPUT_ForIPAddress').value);
					$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[3][0] + '.LANHostConfigManagement.SubnetMask',$('INPUT_ForSubAddress').value);
					$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[3][0] + '.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask',$('INPUT_ForSubAddress').value);
					$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[3][0] + '.LANHostConfigManagement.DomainName',$('INPUT_ForDomainName').value);
					$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[3][0] + '.LANHostConfigManagement.MinAddress',$('INPUT_ForMinAddress').value);
					$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[3][0] + '.LANHostConfigManagement.MaxAddress',$('INPUT_ForMaxAddress').value);
					$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[3][0] + '.LANHostConfigManagement.DHCPLeaseTime',$('INPUT_ForDHCPLeaseTime').value);
					$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[3][0] + '.LANHostConfigManagement.DHCPServerEnable',Form.Checkbox('INPUT_DHCPServerEnable'));
					if($('en_DNSRelayEnable').checked == true)
					{
					    $F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.DNSServers',$('INPUT_ForIPAddress').value);
					}
					if(landevicenum > 4)
					{
						var ValueArray4 = $('INPUT_FiveIPAddress','INPUT_FiveSubAddress','INPUT_FiveMinAddress','INPUT_FiveMaxAddress');
						var ip_five_subnet,ip_five_subnet1,ip_five_subnet2;
						
						var INPUT_FiveIPAdd_INT  = IPtoInt(ValueArray4[0].value);
						var INPUT_FiveMinAdd_INT = IPtoInt(ValueArray4[2].value);
						var INPUT_FiveMaxAdd_INT = IPtoInt(ValueArray4[3].value);
						if((INPUT_FiveMinAdd_INT <= INPUT_FiveIPAdd_INT) && (INPUT_FiveIPAdd_INT <= INPUT_FiveMaxAdd_INT))
						{
							alert("The DHCP Router IP should not be in the DHCP IP Address Range");
							return false;
						}
					
						if($('INPUT_DHCPServerEnable').checked)
						{
						    ip_five_subnet = getNetwork(ValueArray4[0].value,ValueArray4[1].value);
					        ip_five_subnet1 = getNetwork( ValueArray4[2].value,ValueArray4[1].value);
					        ip_five_subnet2 = getNetwork( ValueArray4[3].value,ValueArray4[1].value);
							if(ip_five_subnet != ip_five_subnet1 || ip_five_subnet != ip_five_subnet2)
							{
								alert("DHCP IP Address must be at the same subnet as DHCP Router IP");
								return false;
							}
							if((ip_five_subnet == ip_first_subnet) || (ip_five_subnet == ip_sec_subnet) || (ip_five_subnet == ip_thi_subnet) || (ip_five_subnet == ip_for_subnet))
							{
							    alert("DHCP IP segment must be not at the same!");
								return false;
							}
						}
						$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[4][0] + '.LANHostConfigManagement.IPRouters',$('INPUT_FiveIPAddress').value);
						$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[4][0] + '.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress',$('INPUT_FiveIPAddress').value);
						$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[4][0] + '.LANHostConfigManagement.SubnetMask',$('INPUT_FiveSubAddress').value);
						$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[4][0] + '.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask',$('INPUT_FiveSubAddress').value);
						$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[4][0] + '.LANHostConfigManagement.DomainName',$('INPUT_FiveDomainName').value);
						$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[4][0] + '.LANHostConfigManagement.MinAddress',$('INPUT_FiveMinAddress').value);
						$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[4][0] + '.LANHostConfigManagement.MaxAddress',$('INPUT_FiveMaxAddress').value);
						$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[4][0] + '.LANHostConfigManagement.DHCPLeaseTime',$('INPUT_FiveDHCPLeaseTime').value);
						$F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[4][0] + '.LANHostConfigManagement.DHCPServerEnable',Form.Checkbox('INPUT_DHCPServerEnable'));
						if($('en_DNSRelayEnable').checked == true)
						{
						    $F(':InternetGatewayDevice.LANDevice.' + G_Addlanclient[1][0] + '.LANHostConfigManagement.DNSServers',$('INPUT_FiveIPAddress').value);
						}
					}
				}
			}
		}
	`?>

	$F(':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.' + G_NextAddress[1][0] + '.Enable' ,Form.Checkbox('en_secipadd'));
	if(Form.Checkbox('en_secipadd') == 1)
	{
		$F(':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.' + G_NextAddress[1][0] + '.IPInterfaceIPAddress' ,$('en_secipadd').checked ? ValueArray[6].value : undefined);
	    $F(':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.' + G_NextAddress[1][0] + '.IPInterfaceSubnetMask',$('en_secipadd').checked ? ValueArray[7].value : undefined);
	}
	
    $H({
		':InternetGatewayDevice.LANDevice.1.X_TWSZ-COM_ProxyArp': Form.Checkbox('en_proxyarp'),
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DomainName':ValueArray[8].value,
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.Enable' :1,
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress' :ValueArray[0].value,
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceSubnetMask':ValueArray[1].value,
		
	<?if eq 1 $var:mod_DHCPS
	`	':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPServerEnable':Form.Checkbox('INPUT_DHCPServerEnable'),
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.MinAddress'      :$('INPUT_DHCPServerEnable').checked ? ValueArray[2].value : undefined,
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.MaxAddress'      :$('INPUT_DHCPServerEnable').checked ? ValueArray[3].value : undefined,
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPLeaseTime'   :$('INPUT_DHCPServerEnable').checked ? ValueArray[4].value : undefined,
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPRelay':Form.Checkbox('INPUT_DHCPRelayEnable'),
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.SubnetMask':ValueArray[1].value,
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPRouters':ValueArray[0].value,
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DHCPRelayAddress'      :$('INPUT_DHCPRelayEnable').checked ? ValueArray[9].value : undefined,
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_DHCPEnabled':Form.Checkbox('INPUT_Wlan1'),	
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.2.X_TWSZ-COM_DHCPEnabled':Form.Checkbox('INPUT_Wlan2'),	
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.3.X_TWSZ-COM_DHCPEnabled':Form.Checkbox('INPUT_Wlan3'),	
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.4.X_TWSZ-COM_DHCPEnabled':Form.Checkbox('INPUT_Wlan4'),

		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_UseIPRoutersAsDNSServer': $('INPUT_LANDNSEnable').checked ? 0:1,
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DNSServers'      :$('INPUT_LANDNSEnable').checked ? value_dns.delcomma() : undefined,
		':InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DNSRelayEnable': $('en_DNSRelayEnable').checked ? 1:0,
	
	`?>

	    'errorpage': 'html/index.html',
	    'getpage'  : 'html/index.html',
		'var:menu'    :'setup',
		'var:page'    :'lan',
		'obj-action'  :'set',
		'var:errorpage' : 'lan'
		//'var:CacheLastData': ViewState.Save()
	});

	for(var i = 0, _len = G_LanToIndex.length; i < _len; i++)
    {   
        var dhcpenablePath;
        switch(G_LanToIndex[i][1])
        {
            case 1:
                dhcpenablePath = ':' + G_LanToIndex[i][0];
                $F(dhcpenablePath+'DhcpServerEnable',Form.Checkbox('INPUT_Lan1'));
                break;
            case 2:
                dhcpenablePath = ':' + G_LanToIndex[i][0];
                $F(dhcpenablePath+'DhcpServerEnable',Form.Checkbox('INPUT_Lan2'));
                break;
            case 3:
                dhcpenablePath = ':' + G_LanToIndex[i][0];
                $F(dhcpenablePath+'DhcpServerEnable',Form.Checkbox('INPUT_Lan3'));
                break;
            case 4:
                dhcpenablePath = ':' + G_LanToIndex[i][0];
                $F(dhcpenablePath+'DhcpServerEnable',Form.Checkbox('INPUT_Lan4'));      
                break;
            default:
                alert("bad index!");
                break;
        }
    }
        
	$('uiPostForm').submit();
	ValueArray[5].disabled = 'true';
}

function  addcc(){
	checkboxched=0;
	$('DhcpClientClass').style.display='block';
	$('addcc').disabled = true;
	$('editcc').disabled = true;
	$('delcc').disabled = true;
	$('INPUT_CC_Name').value='';
	$('INPUT_CC_MinAddr').value='';
	$('INPUT_CC_MaxAddr').value='';
	$('INPUT_CC_DnsAddr').value='';
	
	Heigtduiqi();
}
    
function  editcc(){	
	checkboxched=1;
	for (var i = 0; i < G_DHCP_ClientClass.length; i++) {
		var tmpid = "choose" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{   
		    $('DhcpClientClass').style.display='block';
			$('addcc').disabled = true;
			$('editcc').disabled = true;
			$('delcc').disabled = true;
			$('INPUT_CC_Name').value=G_DHCP_ClientClass[i][0];
			$('INPUT_CC_MinAddr').value=G_DHCP_ClientClass[i][1];
			$('INPUT_CC_MaxAddr').value=G_DHCP_ClientClass[i][2];
			$('INPUT_CC_DnsAddr').value=G_DHCP_ClientClass[i][3];
			Heigtduiqi();
			return;
		}			
	}
	alert(SEcode[1009]);		
	
	
}

function delcc(){
	
	 if(!confirm(SEcode[1008])){
		return false;
	 }
    for (var i = 0; i < G_DHCP_ClientClass.length; i++) {
		var tmpid = "choose" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{  
		   $H({
		      'del-obj'    : G_DHCP_ClientClass[i][4],
	          'obj-action' : 'del',
		      'var:menu'   : 'setup',
		      'var:page'   : 'lan',
		      'getpage'    : 'html/index.html',
		      'errorpage'  : 'html/index.html',
		      'var:errorpage' : 'lan',
		      'var:CacheLastData' : ViewState.Save()
	        },true);
	     $('uiPostForm').submit();
	   }			
  }	
}

function uiCCSubmit(){

	if (checkboxched==0)
	{
		readdccsubmint();
	}
	else
	{
	    reeditccsubmint();		
	}
}

function readdccsubmint(){

	var i;
	var prorule =0;
	for(i=0; i<G_DHCP_ClientClass.length; i++)
	{
		if(G_DHCP_ClientClass[i][5] > prorule)
		{
           prorule = G_DHCP_ClientClass[i][5];
		}
	}
    prorule = prorule*1 + 1;
    $('INPUT_CC_PoolRode').value=prorule;

	$H({
		'add-obj'    : 'InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPConditionalServingPool.',
		':VendorClassID'  : $('INPUT_CC_Name').value,
		':MinAddress' : $('INPUT_CC_MinAddr').value,
	    ':MaxAddress'   : $('INPUT_CC_MaxAddr').value,
		':DNSServers'   :  $('INPUT_CC_DnsAddr').value,
		':Enable'    : 1,
		':PoolOrder' : $('INPUT_CC_PoolRode').value,
		':VendorClassIDExclude' : 'false',
		':VendorClassIDMode' : 'Exact',
		':LocallyServed' : 'true',
		'obj-action' : 'add-set',
		'var:menu'   : 'setup',
		'var:page'   : 'lan',
		'getpage'    : 'html/index.html',
		'errorpage'  : 'html/index.html',
		'var:errorpage' : 'lan',
		'var:CacheLastData' : ViewState.Save()
	},true);
	$('uiPostForm').submit();	
}

function reeditccsubmint(){
	var path;
	for (var i = 0; i < G_DHCP_ClientClass.length; i++) {
		var tmpid = "choose" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{   
		   $H({    
	          'obj-action' : 'set',
		      'var:menu'   : 'setup',
		      'var:page'   : 'lan',
		      'getpage'    : 'html/index.html',
		      'errorpage'  : 'html/index.html',
		      'var:errorpage' : 'lan',
		      'var:CacheLastData' : ViewState.Save()
	        },true);
		   path = ':' + G_DHCP_ClientClass[i][4];
		
		   $F(path + 'VendorClassID'   , $('INPUT_CC_Name').value);
		   $F(path + 'MinAddress'  , $('INPUT_CC_MinAddr').value);
		   $F(path + 'MaxAddress'  , $('INPUT_CC_MaxAddr').value);
		   $F(path + 'DNSServers'  , $('INPUT_CC_DnsAddr').value);
	     $('uiPostForm').submit();
	   }
	   
	}
}

function addcondoption(){
	cdopcheckboxched=0;
	$('Dhcpcondoptionclass').style.display='block';
	$('INPUT_CONDOPTEnabled').checked = false;
	$('addcondoption').disabled = true;
	$('editcondoption').disabled = true;
	$('delecondoption').disabled = true;
	$('INPUT_CONDOPT_ClientClass').value='';
	$('INPUT_CONDOPT_Tag').value='';
	$('INPUT_CONDOPT_Value').value='';

	Heigtduiqi();
}	

function  editcondoption(){
	
	cdopcheckboxched=1;
	for (var i = 0; i < G_COND_DHCP_Option.length; i++) {
		var tmpid = "choose4" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{   
		  $('Dhcpcondoptionclass').style.display='block';
		    $('addcondoption').disabled = true;
	           $('editcondoption').disabled = true;
	           $('delecondoption').disabled = true;
		    Form.Checkbox('INPUT_CONDOPTEnabled', G_COND_DHCP_Option[i][0]);
			$('INPUT_CONDOPT_ClientClass').value=G_COND_DHCP_Option[i][5];
		    $('INPUT_CONDOPT_Tag').value=G_COND_DHCP_Option[i][1];
		    $('INPUT_CONDOPT_Value').value=Base64.Decode(G_COND_DHCP_Option[i][2]);
		    Heigtduiqi();
		    return;
		}			
	}
	alert(SEcode[1009]);		
	
	
}	

function condoptionuisubmit(){
	
	if (cdopcheckboxched==0)
	{
		recondoptionaddsubmint();
	}
	else
	{
	    recondoptioneditsubmint();		
	}
}	

function recondoptionaddsubmint(){
	if(G_DHCP_ClientClass.length == 0)
	{
		alert('Please add DHCPConditionalServingPool object first!');
	}
	else
	{
		var prepath;
		for (i = 0; i < G_DHCP_ClientClass.length; i++)
		{
			if($('INPUT_CONDOPT_ClientClass').value == G_DHCP_ClientClass[i][0])
			{
				prepath = G_DHCP_ClientClass[i][4];
			}
		}
		$H({
			'add-obj'    : prepath+'DHCPOption.',
			':Tag'  : $('INPUT_CONDOPT_Tag').value,
			':Value' : Base64.Encode($('INPUT_CONDOPT_Value').value),
			':Enable'   : $('INPUT_CONDOPTEnabled').checked? 1:0,
			'obj-action' : 'add-set',
			'var:menu'   : 'setup',
			'var:page'   : 'lan',
			'getpage'    : 'html/index.html',
			'errorpage'  : 'html/index.html',
			'var:errorpage' : 'lan',
			'var:CacheLastData' : ViewState.Save()
		},true);
		$('uiPostForm').submit();	
	}
}
	
function recondoptioneditsubmint(){
	var path;
	for (var i = 0; i < G_COND_DHCP_Option.length; i++) {
		var tmpid = "choose4" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{   
		   $H({    
	          'obj-action' : 'set',
		      'var:menu'   : 'setup',
		      'var:page'   : 'lan',
		      'getpage'    : 'html/index.html',
		      'errorpage'  : 'html/index.html',
		      'var:errorpage' : 'lan',
		      'var:CacheLastData' : ViewState.Save()
	        },true);
		   if($('INPUT_CONDOPT_ClientClass').value != G_COND_DHCP_Option[i][5])
			{
               alert("The Client Class Name has been changed, plz del this node first, then add a new one!");
			}
			else
			{
			   path = ':' + G_COND_DHCP_Option[i][4];
			   $F(path + 'Enable'    , $('INPUT_CONDOPTEnabled').checked? 1:0);
			   $F(path + 'Tag'   , $('INPUT_CONDOPT_Tag').value);
			   $F(path + 'Value'  , Base64.Encode($('INPUT_CONDOPT_Value').value));
			}
	     $('uiPostForm').submit();
	   }
	   
	}
	
}
	
function delecondoption(){
	
	 if(!confirm(SEcode[1008])){
		return false;
	 }
    for (var i = 0; i < G_COND_DHCP_Option.length; i++) {
		var tmpid = "choose4" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{  
		   $H({
		      'del-obj'    : G_COND_DHCP_Option[i][4],
	            'obj-action' : 'del',
		      'var:menu'   : 'setup',
		      'var:page'   : 'lan',
		      'getpage'    : 'html/index.html',
		      'errorpage'  : 'html/index.html',
		      'var:errorpage' : 'lan',
		      'var:CacheLastData' : ViewState.Save()
	        },true);
	     $('uiPostForm').submit();
	   }			
  }	
}

function  addreseverdisp(){
	
	drcheckboxched=0;
	$('Dhcpreservation').style.display='block';
	$('INPUT_DHCPFilterEnabled').checked = false;
	$('addresever').disabled = true;
	$('editresever').disabled = true;
	$('deleresever').disabled = true;
	$('INPUT_WHITE_pcname').value='';
	$('INPUT_WHITE_IP').value='';
	$('INPUT_WHITE_MAC').value='';
	Heigtduiqi();
}
    
function  editreseverdisp(){
	
	drcheckboxched=1;
	for (var i = 0; i < G_DHCP_WhiteList.length; i++) {
		var tmpid = "choose2" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{   
		    $('Dhcpreservation').style.display='block';
			$('addresever').disabled = true;
	        $('editresever').disabled = true;
	        $('deleresever').disabled = true;
			Form.Checkbox('INPUT_DHCPFilterEnabled', G_DHCP_WhiteList[i][2]);
			$('INPUT_WHITE_pcname').value=G_DHCP_WhiteList[i][4];
			$('INPUT_WHITE_IP').value=G_DHCP_WhiteList[i][0];
			$('INPUT_WHITE_MAC').value=G_DHCP_WhiteList[i][1];
			Heigtduiqi();
			return;
		}			
	}
	alert(SEcode[1009]);		
	
	
}	
	
function reseveruisubmit(){
	
	if (drcheckboxched==0)
	{
		readdsubmint();
	}
	else
	{
	    reeditsubmint();		
	}
}


function readdsubmint(){
       var strMac_IpAd; 
       strMac_IpAd=$('INPUT_WHITE_IP').value.split("."); 
	var str_IpMinAd;
	str_IpMinAd=G_MinAddress.split("."); 
	var str_IpMaxAd;
	str_IpMaxAd=G_MaxAddress.split("."); 

	
      if((parseInt(str_IpMinAd[0]) != parseInt(strMac_IpAd[0]) ) || ( parseInt(str_IpMinAd[1]) != parseInt(strMac_IpAd[1] )) || (parseInt( str_IpMinAd[2]) != parseInt(strMac_IpAd[2]) )||(parseInt(str_IpMinAd[3]) > parseInt(strMac_IpAd[3])) ||(parseInt(strMac_IpAd[3])>parseInt(str_IpMaxAd[3])))
	  	
	 	{
	 	   alert ("The ipadress must be in the same range as the dhcp! ");
		   return false;
	 	}
      

	$H({
		'add-obj'    : 'InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.X_TWSZ-COM_DHCPFilter.WhiteList.',
		':FilterIP'  : $('INPUT_WHITE_IP').value,
		':FilterMAC' : $('INPUT_WHITE_MAC').value,
	    ':PerEnabled'   : $('INPUT_DHCPFilterEnabled').checked? 1:0,
		':FilterPCName'   :  $('INPUT_WHITE_pcname').value,
		'obj-action' : 'add-set',
		'var:menu'   : 'setup',
		'var:page'   : 'lan',
		'getpage'    : 'html/index.html',
		'errorpage'  : 'html/index.html',
		'var:errorpage' : 'lan',
		'var:CacheLastData' : ViewState.Save()
	},true);
	
	$('uiPostForm').submit();	
	
}


function reeditsubmint(){
	var path;
	for (var i = 0; i < G_DHCP_WhiteList.length; i++) {
		var tmpid = "choose2" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{   
		   $H({    
	          'obj-action' : 'set',
		      'var:menu'   : 'setup',
		      'var:page'   : 'lan',
		      'getpage'    : 'html/index.html',
		      'errorpage'  : 'html/index.html',
		      'var:errorpage' : 'lan',
		      'var:CacheLastData' : ViewState.Save()
	        },true);
		   path = ':' + G_DHCP_WhiteList[i][3];
		   $F(path + 'PerEnabled'    , $('INPUT_DHCPFilterEnabled').checked? 1:0);
		   $F(path + 'FilterIP'   , $('INPUT_WHITE_IP').value);
		   $F(path + 'FilterMAC'  , $('INPUT_WHITE_MAC').value);
		   $F(path + 'FilterPCName'  , $('INPUT_WHITE_pcname').value);
	     $('uiPostForm').submit();
	   }
	   
	}
	
}

function deleresever(){
	
	 if(!confirm(SEcode[1008])){
		return false;
	 }
    for (var i = 0; i < G_DHCP_WhiteList.length; i++) {
		var tmpid = "choose2" + i;
		if (Form.Checkbox(tmpid) == 1) 
		{  
		   $H({
		      'del-obj'    : G_DHCP_WhiteList[i][3],
	          'obj-action' : 'del',
		      'var:menu'   : 'setup',
		      'var:page'   : 'lan',
		      'getpage'    : 'html/index.html',
		      'errorpage'  : 'html/index.html',
		      'var:errorpage' : 'lan',
		      'var:CacheLastData' : ViewState.Save()
	        },true);
	     $('uiPostForm').submit();
	   }			
  }	
}


function  uien_secipadd(){
	
	var _node = $('ipaddress','submask');
	
	if($('en_secipadd').checked == true)
	{	
		$('INPUT_IPAddress2').disabled = false ;
		$('INPUT_SubnetMask2').disabled= false;	
		_node[0].style.color = '#000000';
		_node[1].style.color = '#000000';		
	}	
	else
	{				
	    $('INPUT_IPAddress2').disabled = true ;
		$('INPUT_SubnetMask2').disabled= true;	
		_node[0].style.color = '#AAAAAA';
		_node[1].style.color = '#AAAAAA';	
	}	
}



function dealWithError(){

         if (G_Error != 1){ return false; }
         var arrayHint = [];
         dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload,dealWithError);







