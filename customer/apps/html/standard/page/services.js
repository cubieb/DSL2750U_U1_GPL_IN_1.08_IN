//LAN access control
var G_LgAccessCtrl = [];
var nn = 0;
<?setvar var:num <?get :InternetGatewayDevice.X_TWSZ-COM_ACL.LACLNumberOfEntries?>?>
<?if gt $var:num 0
`	<?mget :InternetGatewayDevice.X_TWSZ-COM_ACL.LACL.1. "DevPath ServiceNumberOfEntries"
	`	var G_DevPath	= "$01";
		var G_Number			= "$02";
	`?>
	<?objget :InternetGatewayDevice.X_TWSZ-COM_ACL.LACL.1.Service. "Name Enable SrcIP SrcMask Protocol InternalPort ExternalPort"
	`	G_LgAccessCtrl[nn] = [];
		G_LgAccessCtrl[nn][0] = "$01"; //Name
		G_LgAccessCtrl[nn][1] = "$02"; //enable
		G_LgAccessCtrl[nn][2] = "$03"; //ip
		G_LgAccessCtrl[nn][3] = "$04"; //mask
		G_LgAccessCtrl[nn][4] = "$05"; //protocol
		G_LgAccessCtrl[nn][5] = "$06"; //InternalPort
		G_LgAccessCtrl[nn][6] = "$07"; //ExternalPort
		nn++;
	`?>
`?>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Remote access control
var G_RgAccessCtrl = [];
var G_RPath = [];
var t = 0, n = 0;
<?setvar var:num <?get :InternetGatewayDevice.X_TWSZ-COM_ACL.RACLNumberOfEntries?>?>
<?if gt $var:num 0
`	<?objget :InternetGatewayDevice.X_TWSZ-COM_ACL.RACL. "DevPath ServiceNumberOfEntries"
	`	G_RPath[t] = "<?echo $11?>," + "<?echo $10?>,"+ "<?echo $12?>";
		<?objget :InternetGatewayDevice.X_TWSZ-COM_ACL.RACL.$10.Service. "Name Enable SrcIP SrcMask Protocol InternalPort ExternalPort"
		`	G_RgAccessCtrl[n] = [];
			G_RgAccessCtrl[n][0] = "$01"; //Name
			G_RgAccessCtrl[n][1] = "$02"; //enable
			G_RgAccessCtrl[n][2] = "$03"; //ip
			G_RgAccessCtrl[n][3] = "$04"; //mask
			G_RgAccessCtrl[n][4] = "$05"; //protocol
			G_RgAccessCtrl[n][5] = "$06"; //InternalPort
			G_RgAccessCtrl[n][6] = "$07"; //ExternalPort
			n++;
		`?>
		t++;
	`?>
`?>
//Wan Connection
var G_wanConnction = [];
var m = 0;

<?objget :InternetGatewayDevice.WANDevice. ""
	`
<?objget :InternetGatewayDevice.WANDevice.$10.WANConnectionDevice. "WANIPConnectionNumberOfEntries WANPPPConnectionNumberOfEntries"
`	<?if gt $11 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_wanConnction[m] = [];
			G_wanConnction[m][0] = "$01"; // name;
			G_wanConnction[m][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANIPConnection.$00";  // Path
            	       G_wanConnction[m][2] = "$02";
			G_wanConnction[m][3] = "$03";
			++m;
		`?>
	`?>
	<?if gt $12 0
	`	<?objget :InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection. "Name X_TWSZ-COM_ServiceList ConnectionType"
		`	G_wanConnction[m] = [];
			G_wanConnction[m][0] = "$01"; // name;
			G_wanConnction[m][1] = "InternetGatewayDevice.WANDevice.$30.WANConnectionDevice.$20.WANPPPConnection.$00";  // Path;
           	       G_wanConnction[m][2] = "$02";
		       G_wanConnction[m][3] = "$03";
			++m;
		`?>
	`?>
`?>
`?>

var pathIndex = '<?echo $var:pathIndex?>' == '-' ? 0 : '<?echo $var:pathIndex?>';
//selectedIndex is the selectedIndex of the SELECT obj, while pathIndex is the sub-index of G_RPath array
var selectedIndex = '<?echo $var:selectedIndex?>' == '-' ? 0 : '<?echo $var:selectedIndex?>';
//函数替换
var DealWith = 
{
	Wrong : function(Node, Msg){
		alert(Msg);
		$(Node).focus();
	},
	Right : function(Node){
	}
}

function uiOnload()
{
	var _len = G_wanConnction.length;
	//动态生成下拉框：lan连接
	createConnOptions();
	//$('SELECT_ConnList').selectedIndex = pathIndex;
	$('SELECT_ConnList').selectedIndex = selectedIndex;
	for(var i = 0, _path; i < G_RPath.length; i++){
		_path = G_RPath[i].split(',');
		//找到G_Path的i，然后通过i来读取对应它下面所有的节点值
		if(_len != 0)
		{
			if((pathIndex == 0)&&(_path[0] == G_wanConnction[0][1])){
				pathIndex = i;
			}
		}
	}
	//动态生成表格：远程访问控制
	createTable();
}

function createConnOptions()
{
	var _len = G_wanConnction.length;
	var array_text = [], array_value = [];
	var bad_index = false;
	var to_be_index = -1;
	array_text.push("LAN");
	array_value.push("LAN");
	
	for(var i = 0; i < _len; i++){
		if (G_wanConnction[i][3]=="IP_Bridged"||G_wanConnction[i][2]=="TR069") {
		  if (pathIndex == i)
		  	bad_index = true;
		  continue;

		}

		if (to_be_index == -1)
			to_be_index = i;
		array_text.push(G_wanConnction[i][0]); //Name
		array_value.push(G_wanConnction[i][1]); //Path
	}
	$S('SELECT_ConnList',array_text,array_value);

	if (bad_index == true)
		pathIndex = to_be_index;
}

function createTable()
{
	var Num = (G_RPath[pathIndex] || ',,0').split(',')[2];
	Num = Number(Num);
	var array_remote = [];
	var array_lan = [];
	var _lenth = G_LgAccessCtrl.length;

	if($('SELECT_ConnList').value == "LAN")
	{

		for(var j = 0; j < _lenth; j++){
			if (G_LgAccessCtrl[j][0] == 'SNMP' )
				continue;
			<?if neq 1 $var:mod_IPPMNGR
			`
			if(G_LgAccessCtrl[j][0] == 'PRINTER')
				continue;
			`?>
			<?if neq 1 $var:mod_TR069
			`
			if(G_LgAccessCtrl[j][0] == 'TR069')
				continue;
			`?>
			array_lan[j] = [];
			array_lan[j].push(G_LgAccessCtrl[j][0]);
			array_lan[j].push('<input type="checkbox" id="Enable_LAN' + j + '" ' + (G_LgAccessCtrl[j][1] == '1' ? 'checked' : '') + ' style="width:100px">'); //Enable
			if(G_LgAccessCtrl[j][0] == 'ICMP' || G_LgAccessCtrl[j][0] == 'DNS' || G_LgAccessCtrl[j][0] == 'TR069'){
			    	array_lan[j].push('<input type="input" id="Enable_WANIP_' + j + '" value="'+G_LgAccessCtrl[j][2] + '" size=7 maxlength=96 style="width:80px" disabled=true>&nbsp;/&nbsp;'  +
						'<input type="input" id="Enable_WANMask_' + j + '" value="'+G_LgAccessCtrl[j][3] + '" size=7 maxlength=96 style="width:80px" disabled=true>&nbsp;&nbsp;:&nbsp;&nbsp;'  +
						'<input type="input" id="Enable_WANPort_' + j+ '" value="'+G_LgAccessCtrl[j][6] + '" size=7 maxlength=5 style="width:40px" disabled=true>'); //WAN Destination Port
			}else{
				array_lan[j].push('<input type="input" id="Enable_WANIP_' + j+ '" value="'+G_LgAccessCtrl[j][2] + '" size=7 maxlength=96 style="width:80px" disabled=true>&nbsp;/&nbsp;'  +
						'<input type="input" id="Enable_WANMask_' + j + '" value="'+G_LgAccessCtrl[j][3] + '" size=7 maxlength=96 style="width:80px" disabled=true>&nbsp;&nbsp;:&nbsp;&nbsp;'  +
					'<input type="input" id="Enable_WANPort_' + j + '" value="'+G_LgAccessCtrl[j][6] + '" size=7 maxlength=5 style="width:40px">'); //WAN Destination Port
			}
		}
	

		$T('td_vlan',array_lan);
	}
	else
	{
		for(var i = 0, k; i < Num; i++)
		{
			//lyl remove ssh and snmp becasuse ssh we do not support, snmp is always allowed
			if (G_RgAccessCtrl[i][0] == 'SNMP' )
				continue;
			<?if neq 1 $var:mod_IPPMNGR
			`
			if(G_RgAccessCtrl[i][0] == 'PRINTER')
				continue;
			`?>
			<?if neq 1 $var:mod_TR069
			`
			if(G_RgAccessCtrl[i][0] == 'TR069')
				continue;
			`?>
			if(G_RgAccessCtrl[i][0] == 'DNS')
				continue;
			k = i + pathIndex*Num; //找对应的实例,这个地方非常重要	
			array_remote[i] = [];
			array_remote[i].push(G_RgAccessCtrl[k][0]); //Name

			if(Num != 0)
			{
			    array_remote[i].push('<input type="checkbox" id="Enable_' + i + '" ' + (G_RgAccessCtrl[k][1] == '1' ? 'checked' : '') + ' style="width:100px">'); //Enable
			    if(G_RgAccessCtrl[k][0] == 'ICMP' || G_RgAccessCtrl[k][0] == 'DNS' || G_RgAccessCtrl[k][0] == 'TR069'){
			    	array_remote[i].push('<input type="input" id="Enable_WANIP_' + i + '" value="'+G_RgAccessCtrl[k][2] + '" size=7 maxlength=96 style="width:80px">&nbsp;/&nbsp;'  +
						'<input type="input" id="Enable_WANMask_' + i + '" value="'+G_RgAccessCtrl[k][3] + '" size=7 maxlength=96 style="width:80px">&nbsp;&nbsp;:&nbsp;&nbsp;'  +
						'<input type="input" id="Enable_WANPort_' + i + '" value="'+G_RgAccessCtrl[k][6] + '" size=7 maxlength=5 style="width:40px" disabled=true>'); //WAN Destination Port
			    }else{
				array_remote[i].push('<input type="input" id="Enable_WANIP_' + i + '" value="'+G_RgAccessCtrl[k][2] + '" size=7 maxlength=96 style="width:80px">&nbsp;/&nbsp;'  +
						'<input type="input" id="Enable_WANMask_' + i + '" value="'+G_RgAccessCtrl[k][3] + '" size=7 maxlength=96 style="width:80px">&nbsp;&nbsp;:&nbsp;&nbsp;'  +
					'<input type="input" id="Enable_WANPort_' + i + '" value="'+G_RgAccessCtrl[k][6] + '" size=7 maxlength=5 style="width:40px">'); //WAN Destination Port
			    }
			}
			
		}
	$T('td_vlan',array_remote);
	}
}

function onChgLanIntf(select_lan)
{
	for(var i = 0, _path; i < G_RPath.length; i++){
		_path = G_RPath[i].split(',');
		//找到G_Path的i，然后通过i来读取对应它下面所有的节点值
		if(_path[0] == select_lan.value){
			pathIndex = i;
		}
	}
	//清空表格
	Table.Clear('td_vlan');
	createTable();
}


function getSelectedWan()
{
	var i = 0;
	for (; i < G_RPath.length; ++i)
	{
		if (G_RPath[i].split(',')[0] == $('SELECT_ConnList').value) 
			return i;
	}
	
	return -1;
	 
}

function uiSubmit()
{
	var local_selectedIndex = $('SELECT_ConnList').selectedIndex;
	if (local_selectedIndex > -1)
	{
		selectedIndex = local_selectedIndex;
	}

	if(G_RPath.length > 0 && $('SELECT_ConnList').value != 'LAN')
	{
		pathIndex = getSelectedWan();
		var _split = G_RPath[pathIndex].split(',');
		var _len = Number(_split[2]);
		var localPath = ':InternetGatewayDevice.X_TWSZ-COM_ACL.RACL.' + _split[1] + '.Service.'; 
		var Accessport;
		var AccessIP;
		var AccessMask;
		for(var i = 0, _node; i < _len; i++)
		{
			if (G_RgAccessCtrl[i][0] == 'SNMP' )
				continue;
			<?if neq 1 $var:mod_IPPMNGR
			`
			if(G_RgAccessCtrl[i][0] == 'PRINTER')
				continue;
			`?>
			<?if neq 1 $var:mod_TR069
			`
			if(G_RgAccessCtrl[i][0] == 'TR069')
				continue;
			`?>
			if(G_RgAccessCtrl[i][0] == 'DNS')
				continue;
			_node = $('Enable_' + i);
			Accessport = $('Enable_WANPort_' + i);
			
			//生成表单
			$F(localPath + (i + 1) + '.Enable',_node.checked ? 1 : 0);
			$F(localPath + (i + 1) + '.ExternalPort', Accessport.value);

			AccessTemp = $('Enable_WANIP_' + i).value.split('/');
			AccessIP = $('Enable_WANIP_' + i).value;
			AccessMask = $('Enable_WANMask_' + i).value;
			
			$F(localPath + (i + 1) + '.SrcIP',AccessIP);
			$F(localPath + (i + 1) + '.SrcMask', AccessMask);
		}
		$H({
			'var:menu'  :'maintenance',
			'var:page'  :'accessctrl',
			'var:subpage':'services',
			'getpage'   :'html/index.html',
			'errorpage' :'html/index.html',
			'obj-action':'set',
			'var:pathIndex':pathIndex,
			'var:selectedIndex':selectedIndex,
			'var:errorpage':'services',
			'var:CacheLastData': ViewState.Save()
		});
		
		$('uiPostForm').submit();
	}
	
	if($('SELECT_ConnList').value == 'LAN')
	{
		var LAN_len = Number(G_Number);
		var Accessport;
		var LANlocalPath = ':InternetGatewayDevice.X_TWSZ-COM_ACL.LACL.1.Service.';
	
		for(var i = 0, LAN_node; i < LAN_len; i++)
		{
			if (G_LgAccessCtrl[i][0] == 'SNMP' )
				continue;
			<?if neq 1 $var:mod_IPPMNGR
			`
			if(G_LgAccessCtrl[i][0] == 'PRINTER')
				continue;
			`?>
			<?if neq 1 $var:mod_TR069
			`
			if(G_LgAccessCtrl[i][0] == 'TR069')
				continue;
			`?>
			LAN_node = $('Enable_LAN' + i);
			Accessport = $('Enable_WANPort_' + i);

			//生成表单
			if(G_RgAccessCtrl[i][0] == "HTTP" && LAN_node.checked == 0){
				if(!confirm("If you close HTTP service, when you restart after two minutes,you will not be able to login again, are you sure?")){
					return ;
				}	
			}
			
			$F(LANlocalPath + (i + 1) + '.Enable',LAN_node.checked ? 1 : 0);
			$F(LANlocalPath + (i + 1) + '.ExternalPort', Accessport.value);

		}
		$H({
			'var:menu'  :'maintenance',
			'var:page'  :'accessctrl',
			'var:subpage':'services',
			'getpage'   :'html/index.html',
			'errorpage' :'html/index.html',
			'obj-action':'set',
			'var:selectedIndex':selectedIndex,
			'var:errorpage':'services',
			'var:CacheLastData': ViewState.Save()
		});
		
		$('uiPostForm').submit();
	}

}

function dealWithError(){
	if (G_Error != 1){
		return false;
	}
	
	var arrayHint = [];

	dealErrorMsg(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
