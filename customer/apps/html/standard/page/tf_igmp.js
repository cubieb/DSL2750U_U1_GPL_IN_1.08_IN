//IGMP
//IGMP Proxy
var G_ProxyTables = [];
var m = 0;
<?mget :InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy. "Enabled LANPathName Enable_PassThrough Enable_FastLeaving GQ_Interval GQ_Response_Interval MQ_Interval MQ_Response_Interval MQ_Count LQ_Interval LQ_Count IGMPTableNumberOfEntries Version"
`	var G_ProxyEnable  = "$01"; // Enabled
	var G_LANPathName = "$02"; // LANPathName
	
	var G_PassThroughEnable = "$03"; 
	var G_FastLeavingEnable = "$04"; 
	var G_GQ_Interval = "$05"; 
	var G_GQR_Interval = "$06"; 
	var G_MQ_Interval = "$07"; 
	var G_MQR_Interval = "$08"; 
	var G_MQ_Count = "$09"; 
	var G_LQ_Interval = "$0a"; 
	var G_LQ_Count = "$0b"; 
	var G_TableEntriesNum = "$0c"; // IGMPTableNumberOfEntries	
	var G_Version = "$0d";// IGMP Version
	<?if gt $1c 0
	`	<?objget :InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.IGMPTable. "GroupAddress Interface State"
		`	G_ProxyTables[m] = [];
			G_ProxyTables[m][0] = "$01";
			G_ProxyTables[m][1] = "$02";
			G_ProxyTables[m][2] = "$03";
			++m;
		`?>
	`?>
`?>

<?if neq 1 $var:mod_ETHWAN
		`
var G_ATM = [];
m = 0;
var G_dsl_uprate = "N/A";
var G_dsl_downrate = "N/A";
var G_myType = "Ethernet";
<?objget :InternetGatewayDevice.WANDevice. "WANCommonInterfaceConfig.WANAccessType"
`	<?if eq `DSL` `<?echo $21?>`
	`	G_myType = "<?echo $21?>";
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
		G_dsl_uprate = "<?get :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.UpstreamCurrRate ?>";
		G_dsl_downrate = "<?get :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.DownstreamCurrRate ?>";
	`?>
`?>
`?>

<?if eq 1 $var:mod_ETHWAN
		`
var G_ATM = [];
m = 0;
var G_dsl_uprate = "N/A";
var G_dsl_downrate = "N/A";

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
		G_dsl_uprate = "<?get :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.UpstreamCurrRate ?>";
		G_dsl_downrate = "<?get :InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.DownstreamCurrRate ?>";
`?>


var G_Wanconns = [];
var n = 0;
//WAN Device

<?objget :InternetGatewayDevice.WANDevice. ""
`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00"
			G_Wanconns[n][2] = "$02";
			G_Wanconns[n][3] = "$03";			
			n++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00"
			G_Wanconns[n][2] = "$02";
			G_Wanconns[n][3] = "$03";			
			n++;
		`?>
	`?>
`?>
`?>

<?if eq 1 $var:mod_ETHWAN
		`
var G_ATM_ETH = [];
s= 0;

		<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANEthernetLinkConfig.EthernetLinkStatus WANEthernetLinkConfig.X_TELEFONICA-ES_IGMPEnabled"
		`	G_ATM_ETH[s] = ["InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$00.",//Path
				"$01",//LinkStatus
				"$02" //X_TELEFONICA-ES_IGMPEnabled
				];
				s++;
		`?>		

var G_Wanconns_DSL = [];
var a = 0;
//WAN Device

<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_Wanconns_DSL[a] = [];
			G_Wanconns_DSL[a][0] = "$01";
			G_Wanconns_DSL[a][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANIPConnection.$00"
			G_Wanconns_DSL[a][2] = "$02";
			G_Wanconns_DSL[a][3] = "$03";			
			a++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_Wanconns_DSL[a] = [];
			G_Wanconns_DSL[a][0] = "$01";
			G_Wanconns_DSL[a][1] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.$20.WANPPPConnection.$00"
			G_Wanconns_DSL[a][2] = "$02";
			G_Wanconns_DSL[a][3] = "$03";			
			a++;
		`?>
	`?>
`?>

var G_Wanconns_ETH = [];
var t = 0;
//WAN Device

<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_Wanconns_ETH[t] = [];
			G_Wanconns_ETH[t][0] = "$01";
			G_Wanconns_ETH[t][1] = "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANIPConnection.$00"
			G_Wanconns_ETH[t][2] = "$02";
			G_Wanconns_ETH[t][3] = "$03";			
			t++;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_Wanconns_ETH[t] = [];
			G_Wanconns_ETH[t][0] = "$01";
			G_Wanconns_ETH[t][1] = "InternetGatewayDevice.WANDevice.3.WANConnectionDevice.$20.WANPPPConnection.$00"
			G_Wanconns_ETH[t][2] = "$02";
			G_Wanconns_ETH[t][3] = "$03";			
			t++;
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

function getAtmParam(_i){
	var path_wan = G_Wanconns[_i][1];
	
	for(var i = 0; i < G_ATM.length; i++){
		if(path_wan.indexOf(G_ATM[i][0]) > -1){
			return i;
		}
	}
	
	return -1;
}
<?if eq 1 $var:mod_ETHWAN
		`
function getAtmParam_DSL(_i){
	var path_wan = G_Wanconns_DSL[_i][1];
	
	for(var i = 0; i < G_ATM.length; i++){
		if(path_wan.indexOf(G_ATM[i][0]) > -1){
			return i;
		}
	}
	
	return -1;
}

function getAtmParam_ETH(_k){
	var path_wan_eth = G_Wanconns_ETH[_k][1];
	
	for(var k = 0; k < G_ATM_ETH.length; k++){
		if(path_wan_eth.indexOf(G_ATM_ETH[k][0]) > -1){
			return k;
		}
	}
	
	return -1;
}
`?>


function uiOnload(){
	//Form.Checkbox('INPUT_IGMPProxyEnable',G_IGMPEnable);
	//Form.Checkbox('INPUT_IGMPProxyEnable',G_ProxyEnable);	
	if(G_ProxyEnable == '1')
	{
	    $('INPUT_IGMPProxyEnable').checked = true;
	}
	else
	{
	    $('INPUT_IGMPProxyEnable').checked = false;
	}
	
	wanAndLanPath();

	createDSLTable();

	$('SELECT_Version').value = G_Version;
	
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

	$T('td_proxyTables',G_ProxyTables);	
}

//DSL
function createDSLTable(){ //生成WAN连接列表
	var value_array = [];
	var value_array_Eth = [];
	<?if eq 1 $var:mod_ETHWAN
		`
	for(var i = 0, _len = G_Wanconns_DSL.length; i < _len; i++){
		j = getAtmParam_DSL(i);
		if(G_Wanconns_DSL[i][1].indexOf('WANDevice') > 0){
			if (G_Wanconns_DSL[i][3].indexOf('Bridged') > -1||G_Wanconns_DSL[i][2]=="TR069"||G_Wanconns_DSL[i][0]=="pppo3g") {
				continue;
			}
		value_array[i] = [];
		value_array[i].push('<input type="checkbox" id="Enable_WAN' + i + '" ' + (G_ATM[j][5] == '1' ? 'checked' : '') + ' >'); //Enable
		value_array[i].push( G_Wanconns_DSL[i][0]); //Service Name
						  
			}

		}
	$T('wan_conns_list', value_array);
	for(var k = 0, _lenth = G_Wanconns_ETH.length; k < _lenth; k++){
		q = getAtmParam_ETH(k);
		if(G_Wanconns_ETH[k][1].indexOf('WANDevice') > 0){
			if (G_Wanconns_ETH[k][3].indexOf('Bridged') > -1||G_Wanconns_ETH[k][2]=="TR069"||G_Wanconns_ETH[k][0]=="pppo3g") {
				continue;
			}
		var c = i+ k;
		value_array_Eth[k] = [];
		value_array_Eth[k].push('<input type="checkbox" id="Enable_WAN' + c + '" ' + (G_ATM_ETH[q][2] == '1' ? 'checked' : '') + ' >'); //Enable
		value_array_Eth[k].push( G_Wanconns_ETH[k][0]); //Service Name
						  
			}
		}
	$T('wan_conns_list', value_array_Eth);
	`?>
	<?if neq 1 $var:mod_ETHWAN
		`
		for(var i = 0, _len = G_Wanconns.length; i < _len; i++){
		j = getAtmParam(i);
		if(G_Wanconns[i][1].indexOf('WANDevice') > 0){
			if (G_Wanconns[i][3].indexOf('Bridged') > -1||G_Wanconns[i][2]=="TR069"||G_Wanconns[i][0]=="pppo3g") {
				continue;
			}
		value_array[i] = [];
		value_array[i].push('<input type="checkbox" id="Enable_WAN' + i + '" ' + (G_ATM[j][5] == '1' ? 'checked' : '') + ' >'); //Enable
		value_array[i].push( G_Wanconns[i][0]); //Service Name
						  
			}
		}	
		$T('wan_conns_list', value_array);
	`?>	
	

}

function wanAndLanPath(){
	var text_lan = [],value_lan = [];
	
	for(var i = 0; i < G_Wanconns.length; i++){
		if (G_Wanconns[i][1].indexOf('LANDevice') > 0){		
			text_lan.push(G_Wanconns[i][0]);
			value_lan.push(G_Wanconns[i][1]);
		}
	}

	$S('SELECT_LanInterface',text_lan,value_lan);


	var blan = 0;

	for(var i = 0; i < value_lan.length; i++)
	{
		if(G_LANPathName == value_lan[i])
		{
			blan = 1;
			$('SELECT_LanInterface').selectedIndex=i;
			break;
		}
	}

	if(blan == 0)
	{
		$('SELECT_LanInterface').selectedIndex=0;
	}
	
}

function uiSubmit(){
	var _node = $('SELECT_WanInterface','SELECT_LanInterface');
	var num = 0, index = 0;
	//the code below is to solve bug:21131
	if (Form.Checkbox('INPUT_IGMPProxyEnable') == 0)
		path = 'InternetGatewayDevice.WANDevice.1.WANConnectionDevice.9.WANPPPConnection.9';
	else
		path = 'unknown';

<?if eq 1 $var:mod_ETHWAN
		`
	for(var i = 0, _len = G_Wanconns_DSL.length; i < _len; i++)
	{
		j = getAtmParam_DSL(i);

		if(G_Wanconns_DSL[i][1].indexOf('WANDevice') > 0)
		{
		    if (G_Wanconns_DSL[i][3].indexOf('Bridged') > -1||G_Wanconns_DSL[i][2]=="TR069") {
				continue;
		    }
			
			if(G_Wanconns_DSL[i][0].indexOf('pppo3g') > -1){
				continue;
			}
           var idx_id="Enable_WAN"+i;
           if(Form.Checkbox(idx_id)==1)
           {
               var Path=G_ATM[j][0]+'WANDSLLinkConfig.X_TELEFONICA-ES_IGMPEnabled';
               $F(":" + Path, 1);
			   num++;
           }
           else
           {
           	   var Path=G_ATM[j][0]+'WANDSLLinkConfig.X_TELEFONICA-ES_IGMPEnabled';
               $F(":" + Path, 0);
          			 }	
			}

		}
	for(var k = 0, _lenth = G_Wanconns_ETH.length; k < _lenth; k++)
	{
		s = getAtmParam_ETH(k);

		if(G_Wanconns_ETH[k][1].indexOf('WANDevice') > 0)
		{
		    if (G_Wanconns_ETH[k][3].indexOf('Bridged') > -1||G_Wanconns_ETH[k][2]=="TR069") {
				continue;
		    }
			
			if(G_Wanconns_ETH[k][0].indexOf('pppo3g') > -1){
				continue;
			}
	
	    var id= k+i;
           var dx_id="Enable_WAN"+id;
           if(Form.Checkbox(dx_id)==1)
           {
               var Path=G_ATM_ETH[s][0]+'WANEthernetLinkConfig.X_TELEFONICA-ES_IGMPEnabled';
               $F(":" + Path, 1);
			   index++;
           }
           else
           {
           	   var Path=G_ATM_ETH[s][0]+'WANEthernetLinkConfig.X_TELEFONICA-ES_IGMPEnabled';
               $F(":" + Path, 0);
          			 }	
			}

		}
	`?>	
<?if neq 1 $var:mod_ETHWAN
		`
		for(var i = 0, _len = G_Wanconns.length; i < _len; i++)
		{
			j = getAtmParam(i);

			if(G_Wanconns[i][1].indexOf('WANDevice') > 0)
			{
			    if (G_Wanconns[i][3].indexOf('Bridged') > -1||G_Wanconns[i][2]=="TR069") {
					continue;
			    }
				
				if(G_Wanconns[i][0].indexOf('pppo3g') > -1){
					continue;
				}
				
	           var idx_id="Enable_WAN"+i;

	           if(Form.Checkbox(idx_id)==1)
	           {
	               var Path=G_ATM[j][0]+'WANDSLLinkConfig.X_TELEFONICA-ES_IGMPEnabled';
	               $F(":" + Path, 1);
				   num++;
	           }
	           else
	           {
	           	   var Path=G_ATM[j][0]+'WANDSLLinkConfig.X_TELEFONICA-ES_IGMPEnabled';
	               $F(":" + Path, 0);
	          			 }		
				}

			}
	
	`?>	
	
	if( num+index >1)
    {
       alert("Please select only one item.");
       return false;
    }
		
	$H({
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.Enabled'      : Form.Checkbox('INPUT_IGMPProxyEnable'),
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.Version'      : $('SELECT_Version').value,			
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.LANPathName' : _node[1].value == '' ? 'unknown' : _node[1].value,
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.Enable_FastLeaving' : Form.Checkbox('INPUT_FastLeavingEnable'),
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.GQ_Interval' : $('INPUT_GQ_Interval').value,
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.GQ_Response_Interval' : $('INPUT_GQR_Interval').value,
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.MQ_Interval' : $('INPUT_MQ_Interval').value,
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.MQ_Response_Interval' : $('INPUT_MQR_Interval').value,
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.MQ_Count' : $('INPUT_MQ_Count').value,
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.LQ_Interval' : $('INPUT_LQ_Interval').value,
		':InternetGatewayDevice.X_TELEFONICA-ES_IGMPProxy.LQ_Count' : $('INPUT_LQ_Count').value,	
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
