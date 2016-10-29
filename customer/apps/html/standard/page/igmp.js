//IGMP
<?mget :InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy. "Enable WANPathName LANPathName Enable_PassThrough Enable_FastLeaving GQ_Interval GQ_Response_Interval MQ_Interval MQ_Response_Interval MQ_Count LQ_Interval LQ_Count"
`	var G_IGMPEnable  = "$01"; // Enable
	var G_WANPathName = "$02"; // WANPathName
	var G_LANPathName = "$03"; // LANPathName
	
	var G_PassThroughEnable = "$04"; 
	var G_FastLeavingEnable = "$05"; 
	var G_GQ_Interval = "$06"; 
	var G_GQR_Interval = "$07"; 
	var G_MQ_Interval = "$08"; 
	var G_MQR_Interval = "$09"; 
	var G_MQ_Count = "$0a"; 
	var G_LQ_Interval = "$0b"; 
	var G_LQ_Count = "$0c"; 
`?>

var G_Wanconns = [];
var n = 0;
//WAN Device

<?objget :InternetGatewayDevice.WANDevice. ""
`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00"
			G_Wanconns[n][2] = "$02";
			n++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00"
			G_Wanconns[n][2] = "$02";
			n++;
		`?>
	`?>
`?>
`?>
//LAN Device
<?objget :InternetGatewayDevice.Layer2Bridging.Bridge. "BridgeName X_TWSZ-COM_AssociatedLANDevice"
`	G_Wanconns[n] = [];
	G_Wanconns[n][0] = "$01";
	G_Wanconns[n][1] = "$02";
	n++;
`?>

function uiOnload(){
	Form.Checkbox('INPUT_IGMPProxyEnable',G_IGMPEnable);
	wanAndLanPath();
	$('SELECT_WanInterface').value = G_WANPathName;
	$('SELECT_LanInterface').value = G_LANPathName;

	<?if eq 1 $var:mod_IGMP_PASSTHROUGH
	`
	Form.Checkbox('INPUT_PassThroughEnable',G_PassThroughEnable);	
`?>
	Form.Checkbox('INPUT_FastLeavingEnable',G_FastLeavingEnable);	
	$('INPUT_GQ_Interval').value = G_GQ_Interval;
	$('INPUT_GQR_Interval').value = G_GQR_Interval;
	$('INPUT_MQ_Interval').value = G_MQ_Interval;
	$('INPUT_MQR_Interval').value = G_MQR_Interval;
	$('INPUT_MQ_Count').value = G_MQ_Count;
	$('INPUT_LQ_Interval').value = G_LQ_Interval;
	$('INPUT_LQ_Count').value = G_LQ_Count;
}

function wanAndLanPath(){
	var text_wan = [],value_wan = [];
	var text_lan = [],value_lan = [];
	
	for(var i = 0; i < G_Wanconns.length; i++){
		if(G_Wanconns[i][1].indexOf('WANDevice') > 0){
			if (G_Wanconns[i][0].indexOf('br') > -1||G_Wanconns[i][2]=="TR069") {
				continue;
			}
			text_wan.push(G_Wanconns[i][0]);
			value_wan.push(G_Wanconns[i][1]);
		} else if (G_Wanconns[i][1].indexOf('LANDevice') > 0){		
			text_lan.push(G_Wanconns[i][0]);
			value_lan.push(G_Wanconns[i][1]);
		}
	}
	$S('SELECT_WanInterface',text_wan,value_wan);
	$S('SELECT_LanInterface',text_lan,value_lan);


	var bwan = 0, blan = 0;
	
	for(var i = 0; i < value_wan.length; i++)
	{
		if(G_WANPathName == value_wan[i])
		{
			bwan = 1;
			$('SELECT_WanInterface').selectedIndex=i;
			break;
		}
	}

	for(var i = 0; i < value_lan.length; i++)
	{
		if(G_LANPathName == value_lan[i])
		{
			blan = 1;
			$('SELECT_LanInterface').selectedIndex=i;
			break;
		}
	}

	if(bwan == 0)
	{
		$('SELECT_WanInterface').selectedIndex=0;
	}
	if(blan == 0)
	{
		$('SELECT_LanInterface').selectedIndex=0;
	}
	
}

function uiSubmit(){
	var _node = $('SELECT_WanInterface','SELECT_LanInterface');
	//the code below is to solve bug:21131
	if (Form.Checkbox('INPUT_IGMPProxyEnable') == 0)
		path = 'InternetGatewayDevice.WANDevice.1.WANConnectionDevice.9.WANPPPConnection.9';
	else
		path = 'unknown';
	$H({
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.Enable'      : Form.Checkbox('INPUT_IGMPProxyEnable'),
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.WANPathName' : _node[0].value == '' ? path : _node[0].value,
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.LANPathName' : _node[1].value == '' ? 'unknown' : _node[1].value,
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.Enable_FastLeaving' : Form.Checkbox('INPUT_FastLeavingEnable'),
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.GQ_Interval' : $('INPUT_GQ_Interval').value,
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.GQ_Response_Interval' : $('INPUT_GQR_Interval').value,
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.MQ_Interval' : $('INPUT_MQ_Interval').value,
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.MQ_Response_Interval' : $('INPUT_MQR_Interval').value,
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.MQ_Count' : $('INPUT_MQ_Count').value,
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.LQ_Interval' : $('INPUT_LQ_Interval').value,
		':InternetGatewayDevice.X_TWSZ-COM_IGMP.Proxy.LQ_Count' : $('INPUT_LQ_Count').value,	
	<?if eq 1 $var:mod_IGMP_PASSTHROUGH
	`
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.Enable_PassThrough' : Form.Checkbox('INPUT_PassThroughEnable'),
`?>	
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:subpage'  : G_SubPage,
		'var:errorpage': G_SubPage,
		'obj-action':'set',
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:CacheLastData': ViewState.Save()
	});
	$('uiPostForm').submit();
}

function dealWithError(){
	if(G_Error != 1){
		return false;
	}
	
	var arrayHint = [];
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
