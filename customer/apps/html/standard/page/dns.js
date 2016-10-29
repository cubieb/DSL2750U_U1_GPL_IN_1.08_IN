var connection = [];
var m = 0;
<?if eq $var:G_IPv6Enable 1
`
	<?objget :InternetGatewayDevice.WANDevice. ""
	`
		<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
		`	
			<?if gt $11 0
			`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANIPConnection. "DNSServers Name DNSOverrideAllowed X_TELEFONICA-ES_IPv4Enabled X_TELEFONICA-ES_IPv6Enabled X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed X_TWSZ-COM_IPv6Config.UsrIPv6DNSServers X_TWSZ-COM_UsrDNSServers ConnectionType"
				`	connection[m] = new Array(); 
					connection[m][0] = m; 
					connection[m][1] = "$01"; //DNSServers
					connection[m][2] = ":InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANIPConnection.<?echo $10?>.";
					connection[m][3] = "$02"; //Name
					connection[m][4] = "$03"; //Enable
					connection[m][5] = "$04"; //X_TELEFONICA-ES_IPv4Enabled  
					connection[m][6] = "$05"; //X_TELEFONICA-ES_IPv6Enabled
					connection[m][7] = "$06"; //IPv6DNSOverrideAllowed
					connection[m][8] = "$07"; //UsrIPv6DNSServers
					connection[m][9] = "$08";//X_TWSZ-COM_UsrDNSServers
					connection[m][10] = "$09"; //ConnectionType
					++m;
				`?>
			`?>
			<?if gt $12 0
			`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANPPPConnection. "DNSServers Name DNSOverrideAllowed X_TELEFONICA-ES_IPv4Enabled X_TELEFONICA-ES_IPv6Enabled X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed X_TWSZ-COM_IPv6Config.UsrIPv6DNSServers X_TWSZ-COM_UsrDNSServers ConnectionType"
				`	connection[m] = new Array();
					connection[m][0] = m; 
					connection[m][1] = "$01"; //DNSServers
					connection[m][2] = ":InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANPPPConnection.<?echo $10?>.";
					connection[m][3] = "$02"; //Name
					connection[m][4] = "$03"; //IPv4_DNSOverrideAllowed
					connection[m][5] = "$04"; //X_TELEFONICA-ES_IPv4Enabled  
					connection[m][6] = "$05"; //X_TELEFONICA-ES_IPv6Enabled
					connection[m][7] = "$06"; //IPv6DNSOverrideAllowed
					connection[m][8] = "$07"; //UsrIPv6DNSServers
					connection[m][9] = "$08";//X_TWSZ-COM_UsrDNSServers
					connection[m][10] = "$09"; //ConnectionType
					++m;
				`?>
			`?>
		`?>
	`?>
`?>


<?if eq $var:G_IPv6Enable 0
`		
	<?objget :InternetGatewayDevice.WANDevice. ""
	`
		<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
		`		
			<?if gt $11 0
			`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANIPConnection. "DNSServers Name DNSOverrideAllowed X_TWSZ-COM_UsrDNSServers ConnectionType"
				`	connection[m] = new Array(); 
					connection[m][0] = m; 
					connection[m][1] = "$01"; //DNSServers
					connection[m][2] = ":InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANIPConnection.<?echo $10?>.";
					connection[m][3] = "$02"; //Name
					connection[m][4] = "$03"; //DNSOverrideAllowed
					connection[m][5] = "$04"; //X_TWSZ-COM_UsrDNSServers
					connection[m][6] = "$05"; //ConnectionType
					++m;
				`?>
			`?>
			<?if gt $12 0
			`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANPPPConnection. "DNSServers Name DNSOverrideAllowed X_TWSZ-COM_UsrDNSServers ConnectionType"
				`	connection[m] = new Array();
					connection[m][0] = m; 
					connection[m][1] = "$01"; //DNSServers
					connection[m][2] = ":InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.<?echo $30?>.WANPPPConnection.<?echo $10?>.";
					connection[m][3] = "$02"; //Name
					connection[m][4] = "$03"; //IPv4_DNSOverrideAllowed
					connection[m][5] = "$04"; //X_TWSZ-COM_UsrDNSServers
					connection[m][6] = "$05"; //ConnectionType
					++m;
				`?>
			`?>
		`?>
	`?>
`?>


<?setvaronce var:wanSel 0?>
var wanSel = <?echo $var:wanSel?>; //返回指定wan连接的标记

function uiOnload(){
	
	$('IPv4_DNS_Setting').style.display = 'none';		//初始化 <div id="IPv4_DNS_Setting"> 为hidden
	$('IPv6_DNS_Setting').style.display = 'none';		//初始化 <div id="IPv6_DNS_Setting"> 为hidden
	
	//判断当前是否存在wan连接
	var _len = connection.length;
	if (_len == 0){ return false;}
	//动态生成下拉框：WAN连接
	var text_dns = [], value_dns = [];
	var k = 0;
	for (var i = 0; i < _len; i++){
		<?if eq $var:G_IPv6Enable 1 
		`
			if (connection[i][10].indexOf('IP_Bridged') > -1){
				continue;
			}
		`?>
		<?if eq $var:G_IPv6Enable 0
		`
			if (connection[i][6].indexOf('IP_Bridged') > -1){
				continue;
			}
		`?>
		text_dns[k] = connection[i][3];
		value_dns[k] = connection[i][0];
		k++;
	}
	$S('selectConnectionName', text_dns, value_dns);
	$('selectConnectionName').value = wanSel;

<?if eq $var:G_IPv6Enable 0
`
	$('IPv4_DNS_Setting').style.display = 'block';
	if(connection[Number(wanSel)][4] == '0') //检查 IPv4_DNSOverrideAllowed 是否有效，有效则对应DNS input元素可以输入
	{   
	$('INPUT_DNSOverrideAllowedTrue').checked = false;
		$('INPUT_DNSOverrideAllowedFalse').checked = true;
		$('INPUT_DNS1').disabled = true;
	$('INPUT_DNS2').disabled = true;
	$('FSTDNSserver').style.color = '#AAAAAA';	
	$('AltDNSserver').style.color = '#AAAAAA';	
	}
	else
	{
		$('INPUT_DNSOverrideAllowedTrue').checked = true;
		$('INPUT_DNSOverrideAllowedFalse').checked = false;
		$('INPUT_DNS1').disabled = false;
	$('INPUT_DNS2').disabled = false; 
	$('FSTDNSserver').style.color = '#000000';	
	$('AltDNSserver').style.color = '#000000';
	}
`?>

<?if eq $var:G_IPv6Enable 1
`	
	if(connection[Number(wanSel)][5] == "1")	//检查 X_TELEFONICA-ES_IPv4Enabled 是否有效，有效即该WAN支持IPv4
	{
		$('IPv4_DNS_Setting').style.display = "block";
		if(connection[Number(wanSel)][4] == '0') //检查 IPv4_DNSOverrideAllowed 是否有效，有效则对应DNS input元素可以输入
		{   
			$('INPUT_DNSOverrideAllowedTrue').checked = false;
			$('INPUT_DNSOverrideAllowedFalse').checked = true;
			$('INPUT_DNS1').disabled = true;
			$('INPUT_DNS2').disabled = true;
			$('FSTDNSserver').style.color = '#AAAAAA';	
			$('AltDNSserver').style.color = '#AAAAAA';	
		}
		else
		{
			$('INPUT_DNSOverrideAllowedTrue').checked = true;
			$('INPUT_DNSOverrideAllowedFalse').checked = false;
			$('INPUT_DNS1').disabled = false;
			$('INPUT_DNS2').disabled = false; 
			$('FSTDNSserver').style.color = '#000000';	
			$('AltDNSserver').style.color = '#000000';
		}
	}
	if(connection[Number(wanSel)][6] == "1") //检查 X_TELEFONICA-ES_IPv6Enabled 是否有效，有效即该WAN支持IPv6
	{
		$('IPv6_DNS_Setting').style.display = 'block';
		if(connection[Number(wanSel)][7] == '0') //检查 IPv6DNSOverrideAllowed 是否有效，有效则对应DNS input元素可以输入
		{   
			$('IPv6_DNSOverrideAllowedTrue').checked = false;
			$('IPv6_DNSOverrideAllowedFalse').checked = true;
			$('INPUT_IPv6_DNS1').disabled = true;
			$('INPUT_IPv6_DNS2').disabled = true;
			$('IPv6_FSTDNSserver').style.color = '#AAAAAA';	
			$('IPv6_AltDNSserver').style.color = '#AAAAAA';	
		}
		else
		{
			$('IPv6_DNSOverrideAllowedTrue').checked = true;
			$('IPv6_DNSOverrideAllowedFalse').checked = false;
			$('INPUT_IPv6_DNS1').disabled = false;
			$('INPUT_IPv6_DNS2').disabled = false; 
			$('IPv6_FSTDNSserver').style.color = '#000000';	
			$('IPv6_AltDNSserver').style.color = '#000000';
		}
	}
`?>
	//将DNS写入到文本框中
	var node_dns  = ['INPUT_DNS1','INPUT_DNS2'];
	var DnsServer = connection[Number(wanSel)][1].split(',');
	<?if eq $var:G_IPv6Enable 1 
	`
		var userDnsServer = connection[Number(wanSel)][9].split(',');
	`?>
	<?if eq $var:G_IPv6Enable 0
	`
		var userDnsServer = connection[Number(wanSel)][5].split(',');
	`?>

	for (var j = 0; j < DnsServer.length; j++){
		if(connection[Number(wanSel)][4] == '0')
		{
			$(node_dns[j]).value = userDnsServer[j];
		}
		else
		{
			$(node_dns[j]).value = DnsServer[j];
		}
	}
	
<?if eq $var:G_IPv6Enable 1 
`
	node_dns  = ['INPUT_IPv6_DNS1','INPUT_IPv6_DNS2'];
	DnsServer = connection[Number(wanSel)][8].split(',');
	
	for (var j = 0; j < DnsServer.length; j++){
		$(node_dns[j]).value = DnsServer[j];
	}
`?>

}

function onChangeSelected(){
	var value_dns = $('INPUT_DNS1','INPUT_DNS2','selectConnectionName');
	//清空文本框
	value_dns[0].value = '';
	value_dns[1].value = '';
	//修改wan连接的标记
	wanSel = value_dns[2].value;
	//重新加载一次
	uiOnload();
}

function uiSubmit(){
	
	if(!checkValue()){ return false;}
	//组合IPv4 DNSSEVERS的值
	var ipv4_node_dns  = $('INPUT_DNS1','INPUT_DNS2');
	var ipv4_value_dns = ipv4_node_dns[0].value.strip() + ',' + ipv4_node_dns[1].value.strip();
	$F(connection[Number(wanSel)][2] + 'DNSServers' ,ipv4_value_dns.delcomma());
	$F(connection[Number(wanSel)][2] + 'DNSOverrideAllowed' ,($('INPUT_DNSOverrideAllowedTrue').checked ? 1 : 0));

<?if eq $var:G_IPv6Enable 1 
`
	//组合IPv6 DNSSEVERS的值
	var ipv6_node_dns  = $('INPUT_IPv6_DNS1','INPUT_IPv6_DNS2');
	var ipv6_value_dns = ipv6_node_dns[0].value.strip() + ',' + ipv6_node_dns[1].value.strip();
	$F(connection[Number(wanSel)][2] + 'X_TWSZ-COM_IPv6Config.IPv6DNSOverrideAllowed' ,($('IPv6_DNSOverrideAllowedTrue').checked ? 1 : 0));
	if($('IPv6_DNSOverrideAllowedTrue').checked)
	{
		$F(connection[Number(wanSel)][2] + 'X_TWSZ-COM_IPv6Config.UsrIPv6DNSServers' ,ipv6_value_dns.delcomma());
	}
`?>
	
	$H({
		'var:wanSel':wanSel,
		'obj-action':'set',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:menu'  :'advanced',
		'var:page'  :'dns',
		'var:errorpage'    : 'dns',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}

function onDNSOverrideAllowedClick(){
	<?if eq $var:G_IPv6Enable 1 
	`
		var nodes = $('INPUT_DNS1','INPUT_DNS2','INPUT_IPv6_DNS1','INPUT_IPv6_DNS2');
	`?>
	<?if eq $var:G_IPv6Enable 0 
	`
		var nodes = $('INPUT_DNS1','INPUT_DNS2');
	`?>

	if($('INPUT_DNSOverrideAllowedFalse').checked )
	{
		nodes[0].disabled = true;
		nodes[1].disabled = true;
		$('FSTDNSserver').style.color = '#AAAAAA';	
		$('AltDNSserver').style.color = '#AAAAAA';	
	}
	else
	{
		nodes[0].disabled = false;
		nodes[1].disabled = false;	
		$('FSTDNSserver').style.color = '#000000';	
		$('AltDNSserver').style.color = '#000000';
	}
<?if eq $var:G_IPv6Enable 1
`
	if($('IPv6_DNSOverrideAllowedFalse').checked)
	{
		nodes[2].disabled = true;
		nodes[3].disabled = true;
		$('IPv6_FSTDNSserver').style.color = '#AAAAAA';	
		$('IPv6_AltDNSserver').style.color = '#AAAAAA';	
	}
	else
	{
		nodes[2].disabled = false;
		nodes[3].disabled = false;	
		$('IPv6_FSTDNSserver').style.color = '#000000';	
		$('IPv6_AltDNSserver').style.color = '#000000';
	}
`?>
}


function checkValue(){
	if ($('selectConnectionName').value == ''){
		DealWith.Wrong('selectConnectionName', SEcode[1002]);
		return false;
	}
	
	return true;
}

function dealWithError(){

	if (G_Error != 1){ return false; }
	var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}


addListeners(uiOnload, dealWithError, Form.Action);
