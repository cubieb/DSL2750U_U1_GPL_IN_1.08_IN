
//获得实例
var G_PolicyRoute_Inst   = [];
var m = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_PolicyRoute. "WANDescription LANDescription WANPathName LANPathName"
`	G_PolicyRoute_Inst[m] = [];
	G_PolicyRoute_Inst[m][0] = "$01"; //WANDescription
	G_PolicyRoute_Inst[m][1] = "$02"; //LANDescription
	G_PolicyRoute_Inst[m][2] = "$03"; //WANPathName
	G_PolicyRoute_Inst[m][3] = "$04"; //LANPathName
	G_PolicyRoute_Inst[m][5] = "InternetGatewayDevice.X_TWSZ-COM_PolicyRoute.$00.";
	m++;
`?>

//获取lan config的接口名称//Don't change the order
var G_interface_config = [];
var t = 0;
<?objget :InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig. "X_TWSZ-COM_Description"
`	G_interface_config[t] = [];
	G_interface_config[t][0] = "$01";
	G_interface_config[t][1] = "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.$00";
	t++;
`?>

//获取WLAN的SSID //Don't change the order
<?objget :InternetGatewayDevice.LANDevice.1.WLANConfiguration. "X_TWSZ-COM_DeviceName"
`	G_interface_config[t] = [];
	G_interface_config[t][0] = "$01";
	G_interface_config[t][1] = "InternetGatewayDevice.LANDevice.1.WLANConfiguration.$00";
	t++;
`?>

var G_lan_interface = [];
var l = 0;
for(var i = 0; i < G_interface_config.length; i++)
{
	var j = 0;
	for(j = 0; j < G_PolicyRoute_Inst.length; j++)
	{
		if(G_interface_config[i][0] ==G_PolicyRoute_Inst[j][1] )
			break;
	}
	if(j == G_PolicyRoute_Inst.length)
	{
		G_lan_interface[l] = [];
		G_lan_interface[l][0] = G_interface_config[i][0];
		G_lan_interface[l][1] = G_interface_config[i][1];
		l++;
	}			
}

var G_Wanconns = [];
var n = 0;
//WAN Device
//.modify by lbw 20110715
<?objget :InternetGatewayDevice.WANDevice. ""
`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	
	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_Wanconns[n] = [];
			G_Wanconns[n][0] = "$01";
			G_Wanconns[n][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00";
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

var G_wan_interface = [];
var k = 0;
for(var i = 0; i < G_Wanconns.length; i++)
{
	//var j = 0;
	//	alert(G_Wanconns[i][0].substring(0,2));
	//if(G_Wanconns[i][0].substring(0,2) == "br")      //omit bridge 
	//	continue;

	//for(j = 0; j < G_PolicyRoute_Inst.length; j++)
	//{
	//	if(G_Wanconns[i][0] ==G_PolicyRoute_Inst[j][0] )
	//		break;
	//}
	//if(j == G_PolicyRoute_Inst.length)
	{
		G_wan_interface[k] = [];
		G_wan_interface[k][0] = G_Wanconns[i][0];
		G_wan_interface[k][1] = G_Wanconns[i][1];
		G_wan_interface[k][2] = G_Wanconns[i][2];
		G_wan_interface[k][3] = G_Wanconns[i][3];
		k++;
	}			
}


function uiOnload(){

	$('lang_wan_lan').style.display = 'none';
	wanAndLanPath();
	createTable();

}

function wanAndLanPath(){
	var text_wan = [],value_wan = [];
	var text_lan = [],value_lan = [];
	
	for(var i = 0; i < G_wan_interface.length; i++){
		if(G_wan_interface[i][1].indexOf('WANDevice') > 0){
			if(G_wan_interface[i][2]=="TR069"){
				continue;
				}
			if((G_wan_interface[i][3]=="IP_Bridged") || (G_wan_interface[i][3]=="PPPoE_Bridged"))
			{
			    continue;
			}
			text_wan.push(G_wan_interface[i][0]);
			value_wan.push(G_wan_interface[i][1]);
		} 
		//else if (G_Wanconns[i][1].indexOf('LANDevice') > 0){		
		//	text_lan.push(G_Wanconns[i][0]);
		//	value_lan.push(G_Wanconns[i][1]);
		//}
	}

	for(var j = 0; j <G_lan_interface.length; j++ ){
			text_lan.push(G_lan_interface[j][0]);
			value_lan.push(G_lan_interface[j][1]);
	}
	
	$S('SELECT_WanInterface',text_wan,value_wan);
	$S('SELECT_LanInterface',text_lan,text_lan);
}

function createTable(){	
	var array_inst = [];
	//alert(G_vlan_lan.length);
	for(var i = 0; i < G_PolicyRoute_Inst.length; i++){
		array_inst[i] = [];
		
		array_inst[i].push('<input type="checkbox" id="rule_index'+i+'" name="rule_index'+i+'" onclick="$id(' + i + ')">'); 
		//array_bridge[i].push(G_vlan_lan[i][1]);
		for(var j = 0; j <G_Wanconns.length; j++)
		{
		    if(G_PolicyRoute_Inst[i][2] == G_Wanconns[j][1])
			{
			    G_PolicyRoute_Inst[i][0] = G_Wanconns[j][0];
			    break;
			}
		}
		array_inst[i].push(G_PolicyRoute_Inst[i][0].ellipsis(68));
		array_inst[i].push(G_PolicyRoute_Inst[i][1].ellipsis(68));
	}
	$T('wan_lan_list',array_inst);

}

function uiSubmit(){

	
	$('Apply').disabled=true;
	$('cancel').disabled = true;

	
	var wandescription ='';
	var landescription = $('SELECT_LanInterface').value;

	var wanPath = $('SELECT_WanInterface').value;
	var lanPath = '';

	if(landescription =='')
	{
		alert("LAN Connection is null !");
		uiPageRefresh();
		return false;
	}

	for(var i=0; i < G_wan_interface.length; i++)
	{
		if(wanPath == G_wan_interface[i][1])
		{
			wandescription = G_wan_interface[i][0];
			break;
		}
	}

	for(var j=0; j < G_lan_interface.length; j++)
	{
		if(landescription == G_lan_interface[j][0])
		{
			lanPath = G_lan_interface[j][1];
			break;
		}
	}
	if(wanPath =='')
	{
		alert("wanPath is null !");
		uiPageRefresh();
		return false;
	}

	if(lanPath =='')
	{
		alert("lanPath is null !");
		uiPageRefresh();
		return false;
	}
//	alert(wanPath);
//	alert(lanPath);
	
//	alert($('SELECT_WanInterface').value);
//	alert($('SELECT_LanInterface').value);

	$H({
		'add-obj' 	 		: 'InternetGatewayDevice.X_TWSZ-COM_PolicyRoute.',
		':WANDescription'          : wandescription,
		':LANDescription'            : landescription,
		':WANPathName'          : wanPath,
		':LANPathName'             :lanPath,
		'obj-action' 		: 'add-set',
		'var:menu'     : G_Menu,
		'var:page'     : G_Page,
		'var:subpage'  : G_SubPage,
		'var:errorpage': G_SubPage,
		'getpage'   :'html/index.html',
		'errorpage' :'html/index.html',
		'var:CacheLastData': ViewState.Save()
	});


	$('uiPostForm').submit();

}

function uiAdd(){
	$('lang_wan_lan').style.display = 'block';
	$('add').disabled = true;
	$('delete1').disabled = true;
}

function uiDelete(){

	if(_array_id.length > 1){
		alert(SEcode[1009]);
		return false;
	}
	else if(_array_id.length == 0){
		alert(SEcode[9888]);
		return false;
	}

	for(var j = 0; j < G_PolicyRoute_Inst.length; j++){
		var DelIndexID = "rule_index"+j;
		var DelIndex = Form.Checkbox(DelIndexID);
		if(DelIndex=='1'){
			if(!confirm(SEcode[1008])){return false;}
			$H({
				'del-obj'           : G_PolicyRoute_Inst[j][5],
				'obj-action'       : 'del',
				'var:menu'        : G_Menu,
				'var:page'         : G_Page,
				'var:subpage'     : G_SubPage,
				'var:errorpage'   : G_SubPage,
				'getpage'           : 'html/index.html',
				'errorpage'        : 'html/index.html'
			});
			$('uiPostForm').submit();
		}
	}
	
	
}


function dealWithError(){
	if(G_Error != 1){
		return false;
	}
	
	var arrayHint = [];	
	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);

