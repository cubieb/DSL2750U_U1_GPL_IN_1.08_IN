//wizard -> sum
var ajax;
var sntp_data, isp_data, wlan_data;
var wan_type = 0;
var G_Conns = [];
var Eth_num = 0;
<?if eq 1 $var:mod_WAN_REPLACE
`	
	var G_Wanconns_DSL_PPP = [];
	var G_Wanconns_DSL_IP = [];
	var a = 0;
	var b = 0;
	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries WANDSLLinkConfig.LinkType"
	`	<?if gt $11 0
		`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
			`	G_Wanconns_DSL_IP[a] = [];
				G_Wanconns_DSL_IP[a][0] = "$01";
				G_Wanconns_DSL_IP[a][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00."
				G_Wanconns_DSL_IP[a][2] = "$02";
				G_Wanconns_DSL_IP[a][3] = "$03";	
				G_Wanconns_DSL_IP[a][4] = "$23";	
				a++;
			`?>
		`?>
		<?if gt $12 0
		`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
			`	G_Wanconns_DSL_PPP[b] = [];
				G_Wanconns_DSL_PPP[b][0] = "$01";
				G_Wanconns_DSL_PPP[b][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00."
				G_Wanconns_DSL_PPP[b][2] = "$02";
				G_Wanconns_DSL_PPP[b][3] = "$03";	
				b++;
			`?>
		`?>
	`?>
`?>

<?if eq 1 $var:mod_ETHWAN
`	
	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
		`	
		<?if gt $11 0
		`	
			<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection. "ConnectionStatus Name Enable AddressingType"
			`	
				G_Conns[Eth_num] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection.$00."
				];
				Eth_num++;
			`?>
		`?>
		<?if gt $12 0
		`	
			<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection. "ConnectionStatus Name Enable ConnectionTrigger"
			`	
				G_Conns[Eth_num] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection.$00."
				];
				Eth_num++;
			`?>
		`?>
	`?>
`?>
function uiOnload(){
	chgHTML();
	//获取暂存数据
	isp_data = getJsonObj('isp');
	sntp_data = getJsonObj('stp');	
	wlan_data = getJsonObj('sec');
	psd_data = getJsonObj('psd');
	
	var tmp = isp_data['INPUT_Psd'];
	var show = "";
	if(tmp!=null)
	{
		for (var i = 0; i < tmp.length; i++)
		{
			show = show + '*';
		}
	}
	var tmp1 = wlan_data['INPUT_WepKey'];
	var show1 = "";
	if(tmp1!=null){
		for (var i = 0; i < tmp1.length; i++)
		{
			show1 = show1 + '*';
		}
	}
	var tmp2 = wlan_data['INPUT_WPA_Key'];
	var show2 = "";
	if(tmp2!=null){
		for (var i = 0; i < tmp2.length; i++)
		{
			show2 = show2 + '*';
		}
	}
	var tmp3 = wlan_data['INPUT_WPA2_Key'];
	var show3 = "";
	if(tmp3!=null){
		for (var i = 0; i < tmp3.length; i++)
		{
			show3 = show3 + '*';
		}
	}
	
	if(wlan_data['auth'] == "Basic")
	{
		var auth = "WEP";
	}
	else if(wlan_data['auth'] == "WPA")
	{
		var auth = "WPA-PSK";
	}
	else if(wlan_data['auth'] == "11i")
	{
		var auth = "WPA2-PSK";
	}
	//赋便
	setJSONValue({
		//sntp
		'td_time_setting'	: sntp_data['INPUT_Enable'] == 1? data_language.dynamic.enable : data_language.dynamic.disable,
		'td_ntp_server1'	: (sntp_data['INPUT_NTPServer1'] == "" ? data_language.dynamic.notset : sntp_data['INPUT_NTPServer1']) || data_language.dynamic.notset,
		'td_ntp_server2'	: (sntp_data['INPUT_NTPServer2'] == "" ? data_language.dynamic.notset : sntp_data['INPUT_NTPServer2']) || data_language.dynamic.notset,
		'td_time'			: sntp_data['CurrentLocalTime'] || data_language.dynamic.notset,
		'td_time_zone'		: sntp_data['INPUT_TimeZoneName'] || data_language.dynamic.notset,
		'td_daylight'		: sntp_data['INPUT_DaylightSavingsUsed'] == 1? data_language.dynamic.enable  :data_language.dynamic.disable,
		'td_daylight_start'	: sntp_data['DaylightSavingsStart'] || data_language.dynamic.notset,
		'td_daylight_end'	: sntp_data['DaylightSavingsEnd'] || data_language.dynamic.notset,
		//isp
		'td_wan_type'		: isp_data['INPUT_WanAcsType'] == 0? "DSL" : data_language.dynamic.Ethwan,
		'td_pvc'			: isp_data['INPUT_VPI'] + '/' + isp_data['INPUT_VCI'],
		'td_protocol'		: isp_data['SELECT_Protocol'],
		'td_conn_type'		: isp_data['SELECT_ConnType'],
		'td_username'		: isp_data['INPUT_Username'] || data_language.dynamic.notset,
		'td_password'		: isp_data['INPUT_Psd'] || data_language.dynamic.notset,
		'td_ip_address'		: isp_data['INPUT_IPAddress_static'] || data_language.dynamic.notset,
		'td_subnet_mask'	: isp_data['INPUT_SubMask_static'] || data_language.dynamic.notset,
		'td_default_gw'		: isp_data['INPUT_DefaultGy_static'] || data_language.dynamic.notset,
		'td_dns_server'		: isp_data['INPUT_PryDnsServer_static'] || data_language.dynamic.notset,
		//wlan
		'td_wlan_ssid'		: wlan_data['INPUT_SSID'],
		'td_vb_status'		: wlan_data['vb_status']== 1 ? data_language.dynamic.visible : data_language.dynamic.invisible,
		'td_encryption'		: wlan_data['auth'] == "None" ? data_language.dynamic.none : auth,
		'td_wep_key'		: wlan_data['INPUT_WepKey'] || data_language.dynamic.notset,
		'td_share_key'		: wlan_data['INPUT_WPA_Key'] ||  wlan_data['INPUT_WPA2_Key'] || data_language.dynamic.notset,
		'td_newpsd'			: psd_data['INPUT_NewPassword'] || data_language.dynamic.notset
	});
	
	if(isp_data['INPUT_WanAcsType'] == 1){   //以太网方式
		wan_type = 'Ethernet';
		$('td_pvc').parentNode.style.display = 'none';
		$('td_conn_type').parentNode.style.display = 'none';
	}
	else{
		wan_type = 'DSL';
	}
	//管理控件
	displaytime();
	displayTr();
	displaypsd();
}
function chgHTML(){
	var main_menu = document.getElementsByClassName('main_menu');
	var td_menu   = document.getElementsByClassName('td_menu');
	//隐藏主菜勿
	if(main_menu[0]){
		main_menu[0].style.display = 'none';
	}
	//隐藏二级菜单
	if(td_menu[0]){
		td_menu[0].style.display = 'none';
	}
}

function displaytime(){
	var tr;
	if(sntp_data['INPUT_Enable'] == 1){
		tr = document.getElementsByClassName('Time');
		if(sntp_data['INPUT_DaylightSavingsUsed'] == 0){
			tr = [].concat(document.getElementsByClassName('Time_manual'),document.getElementsByClassName('Daylight'));
		}
	}
	else{
		tr = [].concat(document.getElementsByClassName('Daylight'),document.getElementsByClassName('Time_Zone'));
	}
	
	for(var i = 0,_len = tr.length; i < _len; i++){
		tr[i].style.display = 'none';
	}
}
function displayTr(){
	var tr;
	
	if(isp_data['SELECT_Protocol'] == 'PPPoE' || isp_data['SELECT_Protocol'] == 'PPPoA'){
		tr = document.getElementsByClassName('ip');
	} else if(isp_data['SELECT_Protocol'] == 'Static IP' || isp_data['SELECT_Protocol'] == 'IPoA'){
		tr = document.getElementsByClassName('ppp');
	} else {
		tr = [].concat(document.getElementsByClassName('ppp'),document.getElementsByClassName('ip'));
	}
	
	for(var i = 0,_len = tr.length; i < _len; i++){
		tr[i].style.display = 'none';
	}
}

function displaypsd(){
	var tr;
	if(0 == psd_data['skip']){
		tr = [].concat(document.getElementsByClassName('newpsd'));
		
		for(var i = 0,_len = tr.length; i < _len; i++){
		tr[i].style.display = 'none';
		}
	}
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

function getProtocol(){ //['PPPoE','PPPoA','Dynamic IP','Static IP','Bridge','IPOA'];
	switch(isp_data['SELECT_Protocol']){
		case 'PPPoE' : {
			return ['EoA','PPP'];
		}
		case 'PPPoA' : {
			return ['PPPoA','PPP'];
		}
		case 'Dynamic IP' : {
			return ['EoA','IP'];
		}
		case 'Static IP' : {
			return ['EoA','IP'];
		}
		case 'Bridge' : {
			return ['EoA','IP'];
		}
		case 'IPoA' : {
			return ['IPoA','IP'];
		}
	}
}
/*如果是将要建lan上行的wan连接，要先使能lan上行功能*/
function uiAjax_Add(){
	if(wan_type == 'Ethernet'){
		if (0 == Eth_num){
			$H({
			'obj-action' : 'set',
			'var:page'   : 'wizsum',
			'getpage': 'html/page/wiz_ajax.js'
			}, true);	
			$F(':InternetGatewayDevice.WANDevice.3.WANEthernetInterfaceConfig.Enable' , 1);
			var _url = "/cgi-bin/webproc";
			ajax = Ajax.getInstance(_url, "", 0, uiAjax_create);
			ajax.post($('uiPostForm'));	
		}
		else{
			uiAjax_create_start();
		}
	} 
	else {	
		
<?if eq 1 $var:mod_WAN_REPLACE
`		
	uiAjax_check_conflict();
`?>	
<?if neq 1 $var:mod_WAN_REPLACE
`		
	uiAjax_create_start();
`?>
	}
}

function uiAjax_create(_text){
	try{
		eval(_text);
	} catch (ex){
		G_Error = '1';
		return false;
	}
	if (G_Error == '0'){   
		uiAjax_create_start();    	//成功, 则开始创建实例

	} else if (G_Error == '1'){
		$('display_process').style.display = "none"; 
		$('display_process_stop').style.display = "block"; 
		dealWithError();
		document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wizard';
	}
}

<?if eq 1 $var:mod_WAN_REPLACE
`	
function getProtocolex(){ //['PPPoE','PPPoA','Dynamic IP','Static IP','Bridge','IPOA'];
	switch(isp_data['SELECT_Protocol']){
		case 'PPPoE' : {
			return ['PPP','EoA','EoA'];
		}
		case 'PPPoA' : {
			return ['PPP','EoA','PPPOA'];
		}
		case 'Dynamic IP' : {
			return ['IP','IP_Routed','EoA'];
		}
		case 'Static IP' : {
			return ['IP','IP_Routed','EoA'];
		}
		case 'Bridge' : {
			return ['IP','IP_Bridged','EoA'];
		}
		case 'IPoA' : {
			return ['IP','IP_Routed','IPoA'];
		}
	}
}

 /*DSL-2730U/DME/LN Requirements,when there is conflict with existed wan connection,delete the existed wan connection.*/
function uiAjax_check_conflict(){ 
	
	var _path;
	var _procex = getProtocolex();
	var _name = 'PVC:' + isp_data['INPUT_VPI'] + '/' + isp_data['INPUT_VCI'];
	
	if(_procex[0] == 'PPP')
	{
		for(var k = 0, _lenth = G_Wanconns_DSL_PPP.length; k < _lenth; k++)
		{
			if(G_Wanconns_DSL_PPP[k][0] == _name)
			{
				_path = G_Wanconns_DSL_PPP[k][1];
				break;
			}		  
		}
	}
	if(_procex[0] == 'IP')
	{
		for(var k = 0, _lenth = G_Wanconns_DSL_IP.length; k < _lenth; k++)
		{
			if(G_Wanconns_DSL_IP[k][0] == _name && _procex[1] == G_Wanconns_DSL_IP[k][3] && _procex[2] == G_Wanconns_DSL_IP[k][4])
			{
				_path = G_Wanconns_DSL_IP[k][1];
				break;
			}		  
		}
	}
	if(_path)
	{
		$H({
				'mid': '0430',
				'del-obj'       : _path,
				'obj-action' 	: 'del',
				'var:page'   	: 'wizsum',
				'getpage'		: 'html/page/wiz_ajax.js'
		}, true);
	}
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, uiAjax_create_start);
	ajax.post($('uiPostForm'));	
	
}
`?>
 
function uiAjax_create_start(){
	//Protocol
	var _proc = getProtocol();
	if(wan_type == 'DSL'){
		var _path = ':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WAN' + _proc[1] + 'Connection.0.';
	} 
	else{
		var _path = ':InternetGatewayDevice.WANDevice.3.WANConnectionDevice.0.WAN' + _proc[1] + 'Connection.0.';
	}
	
	$H({
		'mid': '0430',
		'obj-action' : 'set',
		'var:page'   : 'wizsum',
		'getpage': 'html/page/wiz_ajax.js'
	}, true);
	
	if(wan_type == 'DSL'){ // -> DSL
		$H({
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.DestinationAddress':'PVC:'+ isp_data['INPUT_VPI'] + '/' + isp_data['INPUT_VCI'],//PVC
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.ATMEncapsulation' : isp_data['SELECT_ConnType'], //LLC
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.LinkType' : _proc[0], //Protocol		
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.Enable' : "1",
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.ATMPeakCellRate' : "0",
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.ATMMaximumBurstSize' : "0",
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.ATMSustainableCellRate' : "0",
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.X_TWSZ-COM_VLANID' : "0",
			':InternetGatewayDevice.WANDevice.1.WANConnectionDevice.0.WANDSLLinkConfig.ATMQoS' : "UBR"
		});
	} else { // -> Ethernet
		$F(':InternetGatewayDevice.WANDevice.3.WANConnectionDevice.0.X_TWSZ-COM_VLANID' , '0');
	}
	
	//$F(_path + 'Name',isp_data['SELECT_ServiceName']);
	if(wan_type == 'DSL'){
		$F(_path + 'Name', 'PVC:' + isp_data['INPUT_VPI'] + '/' + isp_data['INPUT_VCI']);
	}
	$F(_path + 'Enable','1'); //默认启用
	switch(isp_data['SELECT_Protocol']){//PPPoA/E
		case 'PPPoE' :
			if(wan_type == 'Ethernet'){
				$F(_path + 'Name', 'pppoe_' + '0_'+ Eth_num);
			}
			$F(_path + 'MaxMRUSize', '1492');
		case 'PPPoA' :
			$F(_path + 'Username',isp_data['INPUT_Username']);
			$F(_path + 'Password',isp_data['INPUT_Psd']);
			$F(_path + 'NATEnabled','1');
			break;
		case 'Dynamic IP' : //DHCP
			if(wan_type == 'Ethernet'){
				$F(_path + 'Name', 'mer_' + '0_'+ Eth_num);
			}
			$F(_path + 'AddressingType','DHCP');
			$F(_path + 'NATEnabled','1');
			$F(_path + 'ConnectionType', 'IP_Routed');
			break;
		case 'Static IP' : //Static
			if(wan_type == 'Ethernet'){
				$F(_path + 'Name', 'mer_' + '0_'+ Eth_num);
			}
			$F(_path + 'AddressingType','Static');
			$F(_path + 'ExternalIPAddress',isp_data['INPUT_IPAddress_static']);
			$F(_path + 'SubnetMask',isp_data['INPUT_SubMask_static']);
			$F(_path + 'DefaultGateway',isp_data['INPUT_DefaultGy_static']);
			$F(_path + 'X_TWSZ-COM_UsrDNSServers',isp_data['INPUT_PryDnsServer_static']);
			$F(_path + 'DNSOverrideAllowed','1');
			$F(_path + 'NATEnabled','1');
			$F(_path + 'ConnectionType' , 'IP_Routed');
			break;
		case 'Bridge' :
			if(wan_type == 'Ethernet'){
				$F(_path + 'Name', 'br_' + '0_'+ Eth_num);
			}
			$F(_path + 'ConnectionType' , 'IP_Bridged');
			//$F(_path + 'X_TWSZ-COM_ProtocolType', 'IP');
			$F(_path + 'X_TWSZ-COM_ServiceList' , 'Internet');
			break;
		case 'IPoA' : 
			$F(_path + 'ExternalIPAddress' , isp_data['INPUT_IPAddress_static']);
			$F(_path + 'SubnetMask'        , isp_data['INPUT_SubMask_static']);
			$F(_path + 'AddressingType'    , 'Static'); //Static
			$F(_path + 'DefaultGateway'    , isp_data['INPUT_DefaultGy_static']);
			$F(_path + 'DNSServers',isp_data['INPUT_PryDnsServer_static']);
			$F(_path + 'DNSOverrideAllowed' , '1');
			$F(_path + 'ConnectionType'         , 'IP_Routed');
<?if eq $var:G_IPv6Enable 1			
`	
			$F(_path + 'X_TELEFONICA-ES_IPv6Enabled'         , '0');
			$F(_path + 'X_TELEFONICA-ES_IPv6PrefixDelegationEnabled'         , '0');
			$F(_path + 'X_TELEFONICA-ES_DHCP6cForAddress'         , '0');
`?>	
			break;
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
		return false;
	}
	
	if (G_Error == '0'){
		uiAjax_set();
	} else if (G_Error == '1'){
		if(wan_type == 'DSL'){
			$('display_process').style.display = "none"; 
			$('display_process_stop').style.display = "block"; 
			dealWithError();
			document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wizard';
		}
		else{
			ajax_disable_eth();//如果建立第一条lan上行失败，则要将lan上行功能关闭
		}
	}
}

function ajax_disable_eth(){
	 if (0 == Eth_num){	 
		$H({
		'obj-action' : 'set',
		'var:page'   : 'wizsum',
		'getpage': 'html/page/wiz_ajax.js'
		}, true);
		 $F(':InternetGatewayDevice.WANDevice.3.WANEthernetInterfaceConfig.Enable' , 0);	
		var _url = "/cgi-bin/webproc";
		ajax = Ajax.getInstance(_url, "", 0, ajax_disable_eth_end);
		ajax.post($('uiPostForm'));	
	}
	else{
		$('display_process').style.display = "none"; 
		$('display_process_stop').style.display = "block"; 
		dealWithError();
		document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wizard';
	}
}
function ajax_disable_eth_end(){
	$('display_process').style.display = "none"; 
	$('display_process_stop').style.display = "block"; 
	dealWithError();
	document.location.href = '/cgi-bin/webproc?getpage=html/index.html&var:menu=setup&var:page=wizard';
}

function uiAjax_set(){
	$H({
		//sntp
		':InternetGatewayDevice.Time.Enable': sntp_data['INPUT_Enable'],
		':InternetGatewayDevice.Time.NTPServer1'   : sntp_data['INPUT_NTPServer1'],
		':InternetGatewayDevice.Time.NTPServer2'   : sntp_data['INPUT_NTPServer2'],
		':InternetGatewayDevice.Time.LocalTimeZoneName': sntp_data['INPUT_TimeZoneName'],
		':InternetGatewayDevice.Time.CurrentLocalTime': sntp_data['CurrentLocalTime'] || undefined,
		':InternetGatewayDevice.Time.LocalTimeZone': sntp_data['INPUT_TimeZone'],
		':InternetGatewayDevice.Time.country_num': sntp_data['INPUT_country_num'],
		':InternetGatewayDevice.Time.DaylightSavingsUsed'  : sntp_data['INPUT_DaylightSavingsUsed'],
		':InternetGatewayDevice.Time.DaylightSavingsStart' : sntp_data['DaylightSavingsStart'] || undefined,
		':InternetGatewayDevice.Time.DaylightSavingsEnd'   : sntp_data['DaylightSavingsEnd'] || undefined,
		//wireless
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Enable'  : wlan_data['INPUT_WlanEnable'],		
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSID': wlan_data['INPUT_SSID'],
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSIDAdvertisementEnabled' : wlan_data['vb_status'],
		':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BeaconType'  : wlan_data['auth'],
		
		'var:page'   : 'wlsum',
		'getpage': 'html/page/wiz_ajax.js',
		'obj-action' : 'set'
	},true);
	
	switch(wlan_data['auth']){
		case 'None'  : {
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BasicEncryptionModes'   , 'None');
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BasicAuthenticationMode', 'None');
			break;
		}
		case 'Basic' : {
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BasicEncryptionModes'   , 'WEPEncryption');
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BasicAuthenticationMode', 'None');
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WEPKeyIndex', '1');			
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WEPKey.1.WEPKey', wlan_data['INPUT_WepKey']);
			break;
		}
		case 'WPA' : {
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WPAEncryptionModes'  , 'TKIPandAESEncryption');
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WPAAuthenticationMode'   , 'PSKAuthentication');
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_PSKExpression', "KeyPassphrase");
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.PreSharedKey.1.KeyPassphrase', wlan_data['INPUT_WPA_Key']);
			break;
		}
		case '11i' : {
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.IEEE11iEncryptionModes'   , 'TKIPandAESEncryption');
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.IEEE11iAuthenticationMode', 'PSKAuthentication');
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_PSKExpression', "KeyPassphrase");
			$F(':InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.PreSharedKey.1.KeyPassphrase' , wlan_data['INPUT_WPA2_Key']);
			break;
		}
	}
	if(1 == psd_data['skip']){
		$F(':InternetGatewayDevice.X_TWSZ-COM_Authentication.UserList.1.Password', psd_data['INPUT_OldPassword'] + ':' + psd_data['INPUT_NewPassword']);
	}
	var _url = "/cgi-bin/webproc";
	ajax = Ajax.getInstance(_url, "", 0, ajax_SetProc);
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
		if(wan_type == 'DSL'){
			var _path = G_Conns_DSL[DSL_num_ajax-1][0];//DSL_num_ajax和G_Conns_DSL[DSL_num_ajax-1][0]是执行wizsum_ajax.js获取到的值
		} 
		else{
			var _path = G_Conns_Eth[Eth_num_ajax-1][0];
		}
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

function ajax_SetProc(_text){
	//成功,则跳转页雿
	try{
		eval(_text);
	} catch (ex){
		G_Error == '1';
		return false;
	}
	
	if (G_Error == '0'){
		$G('/cgi-bin/webproc',{
		'var:menu'			: 'setup',
		'var:page'			: 'wizard',
		'getpage'			: 'html/index.html',
		'errorpage'			: 'html/index.html'
		});	
	} else if(G_Error == '1'){
		$('display_process').style.display = "none"; 
		$('display_process_stop').style.display = "block";
		dealWithError();
		read_object();      //如果wan连接已经设置成功，但后面的参数设置失败，为了保持整个快速配置的原子性，需要将已经建好的wan连接删除。
	}
}

function uiBack(){
	$H({
		'var:menu'	: 'setup',
		'var:subpage' : 'wizaccountpsd',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	});
	
	$('uiPostForm').submit();
}

function uiCancle(){
	if(!confirm(SEcode[1012])){
		return false;
	}
	$H({
		'var:menu' : 'setup',
		'var:page' : 'wizard',
		'getpage'  : 'html/index.html'
	});
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
